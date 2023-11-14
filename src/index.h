#pragma once


#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <sys/stat.h>

#include "absl/container/flat_hash_map.h"
#include "glog/logging.h"

template <class T> bool ReadArrayData(std::vector<T>* data, const std::filesystem::path& path) {
    int fd = ::open(path.c_str(), O_RDONLY);
    if (fd < 0) {
        PLOG(ERROR) << "Fail to open file:" << path;
        return false;
    }

    struct stat st;
    fstat(fd, &st);
    auto size = st.st_size;

    if (size % sizeof(T) != 0) {
        LOG(ERROR) << "Size not match:" << size << " " << sizeof(T) << ":" << path;
        return false;
    }

    data->resize(size / sizeof(T));
    char* buf = (char*)data->data();
    while (size != 0) {
        auto rc = ::read(fd, buf, size);
        if (rc < 0) {
            LOG(ERROR) << "Fail to read:" << path;
            return false;
        }
        buf += rc;
        size -= rc;
    }
    return true;
}

template <> bool ReadArrayData<std::string>(std::vector<std::string>* data, const std::filesystem::path& path) {
    std::ifstream input(path.c_str());
    if (!input) {
        PLOG(ERROR) << "Fail to open file:" << path;
        return false;
    }

    std::string line;
    while (std::getline(input, line)) {
        data->push_back(std::move(line));
    }
    return true;
}

struct InvertedIndex {
    // term -> term id
    absl::flat_hash_map<std::string, uint32_t> dict;
    uint32_t next_term_id = 1;
    // term id -> doc list
    std::vector<std::vector<uint32_t>> posting_lists;
    uint32_t doc_id_base = 0;

    // std::vector<roaring::PostingList> roaring_postings;
};

class IndexTest {
public:
    bool LoadSlice(const std::filesystem::path& entry) {
        std::vector<std::string> field_names;
        if (!ReadArrayData(&field_names, entry / "index.fnm")) {
            return false;
        }

        std::vector<uint32_t> field_term_range;
        if (!ReadArrayData(&field_term_range, entry / "index.fof")) {
            return false;
        }
        CHECK(field_term_range.size() == field_names.size() + 1);

        std::vector<std::string> terms;
        if (!ReadArrayData(&terms, entry / "index.trm")) {
            return false;
        }

        std::vector<uint32_t> posting_ranges;
        if (!ReadArrayData(&posting_ranges, entry / "index.tof")) {
            return false;
        }

        std::vector<uint32_t> docs;
        if (!ReadArrayData(&docs, entry / "index.doc")) {
            return false;
        }

        for (int i = 0; i < field_names.size(); ++i) {
            const auto& fn = field_names[i];
            auto term_begin = field_term_range[i];
            auto term_end = field_term_range[i + 1];
            auto& index = fields_[fn];
            if (!index) {
                index.reset(new InvertedIndex);
            }

            int term_size = term_end - term_begin;
            LOG(INFO) << "field:" << fn << " term_size:" << term_size;
            if (index->posting_lists.size() < term_size) {
                index->posting_lists.resize(term_size);
            }

            uint32_t max_doc_id = 0;
            for (uint32_t term_index = term_begin; term_index < term_end; ++term_index) {
                const auto& term = terms[term_index];

                auto& term_id = index->dict[term];
                if (term_id == 0) {
                    term_id = index->next_term_id;
                    index->next_term_id += 1;
                }
                if (index->posting_lists.size() < term_id + 1) {
                    index->posting_lists.resize(term_id + 1);
                }
                auto& postings = index->posting_lists[term_id];

                auto doc_id_begin = posting_ranges[term_index];
                auto doc_id_end = posting_ranges[term_index + 1];
                for (uint32_t doc_id_index = doc_id_begin; doc_id_index < doc_id_end; ++doc_id_index) {
                    if (!postings.empty()) {
                        CHECK(docs[doc_id_index] + index->doc_id_base > postings.back());
                    }
                    postings.push_back(docs[doc_id_index] + index->doc_id_base);
                }
                max_doc_id = std::max(max_doc_id, postings.back());
            }
            index->doc_id_base = max_doc_id + 1;
            LOG(INFO) << "doc_id_base:" << index->doc_id_base;
        }

        return true;
    }

    bool LoadSlices() {
        for (auto slice : {"slice.0", "slice.1", "slice.2", "slice.3", "slice.4"}) {
            if (!LoadSlice(std::filesystem::path("index_data/video4_0.base") / slice)) {
                return false;
            }
        }

        std::map<int, int> length_stats;
        for (const auto& it : fields_) {
            LOG(INFO) << "field:" << it.first << " terms:" << it.second->posting_lists.size();
            for (const auto& postings : it.second->posting_lists) {
                length_stats[postings.size()] += 1;
            }
        }
        return true;
    }

    const std::vector<uint32_t>& GetPostings(const std::string& field, const std::string& term) const {
        static std::vector<uint32_t> empty;

        auto it = fields_.find(field);
        if (it == fields_.end()) {
            return empty;
        }
        const auto* index = it->second.get();
        auto term_it = index->dict.find(term);
        if (term_it == index->dict.end()) {
            return empty;
        }
        return index->posting_lists[term_it->second];
    }

private:
    // field -> [term, doc list]
    absl::flat_hash_map<std::string, std::unique_ptr<InvertedIndex>> fields_;
};

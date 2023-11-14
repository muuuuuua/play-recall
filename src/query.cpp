#include "query.h"
#include <iostream>

void QueryBase::dfs_print(std::ostream& out) {
    out << this->name() << std::endl;
    dfs(this, "", out);
}

void QueryBase::dfs(const QueryBase* cur, const std::string& prefix, std::ostream& out) {
    // 递归遍历子节点
    std::vector<const QueryBase*> child_vec;
    cur->children(child_vec);
    for (size_t i = 0; i < child_vec.size(); i++) {
        if (i == child_vec.size() - 1) {
            out << prefix << "└── " << child_vec[i]->name() << std::endl;
            dfs(child_vec[i], prefix + "    ", out);
        } else {
            out << prefix << "├── ";
            // if (i == 0 && cur->is_leftjoin_query()) {
            //     out << "*";
            // }
            out << child_vec[i]->name() << std::endl;
            dfs(child_vec[i], prefix + "│   ", out);
        }
    }
}

AndQuery::AndQuery(std::vector<std::unique_ptr<QueryBase>>&& queries) {
    queries_ = std::move(queries);
    std::sort(
        queries_.begin(), queries_.end(),
        [](const std::unique_ptr<QueryBase>& a, const std::unique_ptr<QueryBase>& b) { return a->cost() < b->cost(); });
}

DocId AndQuery::next_doc() {
    return do_next(queries_[0]->next_doc());
}

DocId AndQuery::advance(DocId target) {
    return do_next(queries_[0]->advance(target));
}

DocId AndQuery::do_next(DocId lead_doc) {
ADVANCE_HEAD:
    // LOG(INFO) << "DoNext, lead:" << lead_doc;
    while (lead_doc != NO_MORE_DOCS) {
        for (size_t i = 1; i < queries_.size(); ++i) {
            auto* other = queries_[i].get();
            // invariant: other.doc <= lead_doc
            if (other->doc() < lead_doc) {
                auto next = other->advance(lead_doc);
                // LOG(INFO) << "DoNext, a:" << lead_doc << " n:" << next;
                if (next > lead_doc) {
                    // next beyond lead_doc - break and advance lead to the new highest doc
                    lead_doc = queries_[0]->advance(next);
                    goto ADVANCE_HEAD;
                }
            }
        }

        break;
    }
    return lead_doc;
}

void OrQuery::collect_min_docs_query() {
    DocId min_doc = NO_MORE_DOCS;
    min_doc_queries_.clear();
    for (auto&& query : queries_) {
        const auto cur_doc = query->doc();
        if (min_doc > cur_doc) {
            min_doc = cur_doc;
            min_doc_queries_.clear();
            min_doc_queries_.push_back(query.get());
        } else if (min_doc == cur_doc) {
            min_doc_queries_.push_back(query.get());
        }
    }
}

OrQuery::OrQuery(std::vector<std::unique_ptr<QueryBase>>&& queries) {
    queries_ = std::move(queries);
    cost_ = 0;
    for (auto& query : queries_) {
        cost_ += query->cost();
    }
    min_doc_queries_.reserve(queries_.size());
    collect_min_docs_query();
}

DocId OrQuery::next_doc() {
    for (auto query : min_doc_queries_) {
        query->next_doc();
    }
    collect_min_docs_query();
    return doc();
}

DocId OrQuery::advance(DocId target) {
    for (auto&& query : queries_) {
        if (query->doc() < target) {
            query->advance(target);
        }
    }
    collect_min_docs_query();
    return doc();
}

TermQuery::TermQuery(const std::vector<DocId>& postings, const std::string& term, const std::string& field) :
    postings_(postings), term_(term), field_(field) {}


DocId TermQuery::next_doc() {
    index_ += 1;
    if (index_ >= postings_.size()) {
        current_doc_ = NO_MORE_DOCS;
    } else {
        current_doc_ = postings_[index_];
    }
    return current_doc_;
}

DocId TermQuery::advance(DocId target) {
    if (!advance_to(target, postings_.data(), index_, postings_.size(), current_doc_, 8)) {
        current_doc_ = NO_MORE_DOCS;
    }
    return current_doc_;
}

bool TermQuery::advance_to(DocId target, const DocId* docs, int& current, int end, DocId& doc, int skip_len) {
    current += 1;
    if (current >= end) {
        return false;
    }

    doc = docs[current];
    if (doc >= target) {
        //short-cut for already-matched
        return true;
    }

    while (skip_len > 4) {
        int try_skip = current + skip_len;
        while (try_skip < end && docs[try_skip] <= target) {
            current = try_skip;
            doc = docs[current];
            try_skip += skip_len;
        }
        if (doc == target) {
            //short-cut for matched
            return true;
        }
        skip_len >>= 2;
    }

    current += 1;
    while (current < end) {
        doc = docs[current];
        if (doc >= target) {
            return true;
        }
        ++current;
    }
    return false;
}

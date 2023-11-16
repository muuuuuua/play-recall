#include "absl/strings/str_join.h"
#include "butil/time.h"
#include "index.h"
#include "query.h"
#include "roaring.hh"

using namespace roaring;

void TestSimpleQueryVirtual(const IndexTest& index, const std::vector<std::string>& terms,
                            const std::vector<std::string>& fields) {
    std::vector<std::unique_ptr<QueryBase>> or_queries;
    for (auto const& term : terms) {
        std::vector<std::unique_ptr<QueryBase>> or_fields;
        for (auto const& field : fields) {
            or_fields.emplace_back(new TermQuery(index.GetPostings(field, term), term, field));
        }
        or_queries.emplace_back(new OrQuery(std::move(or_fields)));
    }
    AndQuery and_query(std::move(or_queries));

    int doc_count = 0;
    auto* q = &and_query;
    q->dfs_print(std::cout);

    int64_t begin = butil::cpuwide_time_us();
    while (true) {
        q->next_doc();
        auto doc = q->doc();
        if (doc == NO_MORE_DOCS) {
            break;
        }
        doc_count += 1;
    }
    int64_t end = butil::cpuwide_time_us();
    LOG(INFO) << "simple virtual and queries:" << (end - begin) << " " << doc_count;
}

void TestRoaringBitset(const IndexTest& index, const std::vector<std::string>& terms,
                       const std::vector<std::string>& fields) {
    std::vector<std::vector<Roaring>> and_queries;
    for (auto const& term : terms) {
        std::vector<Roaring> or_queries;
        for (auto const& field : fields) {
            const auto& postings = index.GetPostings(field, term);
            or_queries.emplace_back(Roaring(postings.size(), postings.data()));
        }
        and_queries.emplace_back(std::move(or_queries));
    }

    int64_t begin = butil::cpuwide_time_us();

    std::vector<Roaring> and_candidates(and_queries.size());
    for (size_t i = 0; i < and_queries.size(); i++) {
        std::vector<Roaring*> temp(and_queries[i].size());
        for (size_t k = 0; k < and_queries[i].size(); k++) {
            temp[k] = &and_queries[i][k];
        }
        and_candidates[i] = Roaring::fastunion(and_queries[0].size(), const_cast<const Roaring**>(temp.data()));
    }
    std::sort(and_candidates.begin(), and_candidates.end(),
              [](const Roaring& a, const Roaring& b) { return a.cardinality() < b.cardinality(); });

    Roaring res = and_candidates[0];
    for (size_t i = 1; i < and_candidates.size(); i++) {
        res &= and_candidates[i];
    }
    int doc_count = res.cardinality();

    int64_t end = butil::cpuwide_time_us();
    LOG(INFO) << "roaring bitset and queries:" << (end - begin) << " " << doc_count;
}

int main() {
    IndexTest test;
    test.LoadSlice("index_data/video4_0.base/slice.0");

    std::vector<std::vector<std::string>> test_terms = {
        {"大", "聪", "导", "剪", "版", "正义", "联盟", "达到", "预期", "是", "好看", "到", "飞起", "还是", "看", "个",
         "寂寞"},
        {"罗翔", "说", "刑法"},
        {"没有", "一", "个", "动作", "是", "多余", "的"},
        {"直播", "多少", "人", "在线", "看"},
        {"我的世界", "出生", "点"},
        {"小甜", "什么", "都", "不", "知道"},
    };
    std::vector<std::string> test_fields = {"title", "b_title", "uname", "staff", "tag", "description"};

    for (auto terms : test_terms) {
        LOG(INFO) << "terms:" << absl::StrJoin(terms, " ");
        // TestSimpleQuery(test, terms, test_fields);
        TestSimpleQueryVirtual(test, terms, test_fields);
        TestRoaringBitset(test, terms, test_fields);
    }

    return 0;
}

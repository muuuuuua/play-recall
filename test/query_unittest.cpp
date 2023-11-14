#include "query.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace std;
using namespace testing;

TEST(QueryTest, NormalTest) {
    std::vector<std::unique_ptr<QueryBase>> or_queries;
    or_queries.emplace_back(new TermQuery({1, 3, 5, 26}, "t0", "f"));
    or_queries.emplace_back(new TermQuery({1, 2, 4, 10, 100}, "t1", "f"));

    std::vector<std::unique_ptr<QueryBase>> and_queries;
    and_queries.emplace_back(new OrQuery(std::move(or_queries)));
    and_queries.emplace_back(new TermQuery({2, 3, 4, 5, 6, 34, 56}, "t2", "f"));
    and_queries.emplace_back(new TermQuery({1, 4, 5, 23, 70, 200}, "t3", "f"));
    and_queries.emplace_back(new TermQuery({4, 5, 14, 78}, "t4", "f"));

    AndQuery and_query(std::move(and_queries));

    int doc_count = 0;
    auto* q = &and_query;
    q->dfs_print(std::cout);

    std::vector<DocId> res;
    while (true) {
        q->next_doc();
        auto doc = q->doc();
        if (doc == NO_MORE_DOCS) {
            break;
        }
        doc_count += 1;
        res.push_back(doc);
    }

    ASSERT_EQ(doc_count, 2);
    EXPECT_THAT(res, ElementsAre(4, 5));
}

#include "roaring.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace std;
using namespace testing;
using namespace roaring;

TEST(RoaringTest, NormalTest) {
    Roaring t0 = Roaring::bitmapOfList({1, 3, 5, 26});
    Roaring t1 = Roaring::bitmapOfList({1, 2, 4, 10, 100});

    std::vector<Roaring> and_queries;
    and_queries.emplace_back(t0 | t1);
    and_queries.emplace_back(Roaring::bitmapOfList({2, 3, 4, 5, 6, 34, 56}));
    and_queries.emplace_back(Roaring::bitmapOfList({1, 4, 5, 23, 70, 200}));
    and_queries.emplace_back(Roaring::bitmapOfList({4, 5, 14, 78}));

    Roaring res = and_queries[0];
    for (size_t i = 1; i < and_queries.size(); i++) {
        res &= and_queries[i];
    }

    ASSERT_EQ(res.cardinality(), 2ul);
    vector<uint32_t> ans(res.cardinality());
    res.toUint32Array(ans.data());
    EXPECT_THAT(ans, ElementsAre(4, 5));
}

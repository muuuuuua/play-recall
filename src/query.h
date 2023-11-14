#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "absl/strings/str_cat.h"


using DocId = uint32_t;
using Cost = uint32_t;
// static const DocId INVALID_DOC = -1;
static const DocId NO_MORE_DOCS = std::numeric_limits<DocId>::max();

class QueryBase {
public:
    QueryBase() = default;
    virtual ~QueryBase() = default;
    QueryBase(const QueryBase&) = delete;
    QueryBase& operator=(const QueryBase&) = delete;

    // query名称
    virtual std::string name() const = 0;

    // 返回值: 当前的doc_id
    // INVALID_DOC: 无效的doc, 没有调用 next_doc() 或 advance(target);
    // NO_MORE_DOCS: 无效的doc, 已经遍历完所有doc或截断;
    // 其他: 当前指向的doc_id.
    virtual DocId doc() const = 0;

    // 返回下一个doc_id
    virtual DocId next_doc() = 0;

    // 从当前位置向后寻找到第一个>=target的doc_id, 用来多队列求交
    virtual DocId advance(DocId target) = 0;

    // 返回当前query的doc数量
    // 可能匹配到的doc个数上限, 不一定准确, 用来在求交的时候减少遍历代价
    virtual Cost cost() const = 0;

    // // 返回当前doc对应doc_info指针
    // // 当前的实现仅填充docid和正排指针及realtime_info指针
    // virtual const DocInfoPtr& doc_info() const = 0;
    //
    // // 返回当前doc的term bitmatrix, 描述<term, field>的命中情况
    // // NOTE: 由于某些子类实现时会修改成员变量, 所以去掉函数末尾的const修饰
    // virtual const TermBitmatrix<>& term_bitmatrix() = 0;
    //
    // // 必须在next_docs() != NO_MORE_DOCS的时候才有效
    // // 收集当前doc下的所有<<pos, pos_end>, <term, field>>信息
    // // 收集当前子树的所有叶子节点的TermPoslist
    // // 函数内部对poslist_vec不做clear, 直接push_back, 以便高效地DFS调用
    // virtual void term_poslists(TermPoslistVec& poslist_vec) const = 0;
    //
    // // 必须在next_docs() != NO_MORE_DOCS的时候才有效
    // // 收集当前doc下的所有的命中的<term_idx, field_idx> --> term weight
    // // 经由<block_query, and_query, or_query, ..., term_query>设置
    // virtual void set_term_weight_matrix(TermWeightMatrix& tw_matrix) const = 0;
    //
    // // 将内部信息格式化为json
    // virtual std::string to_string() const = 0;
    //
    // // 收集所有的<term, field>对应的doc freq
    // // 获取当前子树的所有叶子节点的term doc frequency. 一般一次请求仅调用一次.
    // // 函数内部对tdf_vec不做clear, 直接push_back, 以便高效地DFS调用
    // // NOTE: TermDocfreq 中的term* 指向叶子节点的空间, 注意生命周期
    // virtual void terms_doc_freq(std::vector<TermDocfreq>& tdf_vec) const = 0;

    // // 遍历所有命中的doc, 放入Collector中
    // virtual void collect(Collector& collector, QueryContext* ctx);
    //
    // // 每次next_doc(), advance() 返回有效doc时, +1
    // virtual int32_t hits() const {
    //     return _hits;
    // }

    // // 宽度优先遍历
    // // proc return true means stop traveling
    // void bfs_travel(std::function<bool(const Query*)> proc);
    //
    void dfs_print(std::ostream& out);

    // 子类 override 时注意先 clear child_vec 再填充
    virtual void children(std::vector<const QueryBase*>& child_vec) const {
        child_vec.clear();
    }

private:
    void dfs(const QueryBase* cur, const std::string& prefix, std::ostream& out);
};

class AndQuery : public QueryBase {
public:
    AndQuery(std::vector<std::unique_ptr<QueryBase>>&& queries);

    std::string name() const override {
        return "AND";
    }
    DocId doc() const override {
        return queries_[0]->doc();
    }
    DocId next_doc() override;
    DocId advance(DocId target) override;
    Cost cost() const override {
        return queries_[0]->cost();
    }

    void children(std::vector<const QueryBase*>& child_vec) const override {
        child_vec.clear();
        child_vec.reserve(queries_.size());
        for (const auto& q : queries_) {
            child_vec.push_back(q.get());
        }
    }

private:
    DocId do_next(DocId lead_doc);

    std::vector<std::unique_ptr<QueryBase>> queries_;
};

class OrQuery : public QueryBase {
public:
    OrQuery(std::vector<std::unique_ptr<QueryBase>>&& queries);

    std::string name() const override {
        return "OR";
    }
    DocId doc() const override {
        return min_doc_queries_.front()->doc();
    }
    DocId next_doc() override;
    DocId advance(DocId target) override;
    Cost cost() const override {
        return cost_;
    }

    void children(std::vector<const QueryBase*>& child_vec) const override {
        child_vec.clear();
        child_vec.reserve(queries_.size());
        for (const auto& q : queries_) {
            child_vec.push_back(q.get());
        }
    }

private:
    void collect_min_docs_query();

    std::vector<std::unique_ptr<QueryBase>> queries_;
    std::vector<QueryBase*> min_doc_queries_;
    Cost cost_;
};

class TermQuery : public QueryBase {
public:
    TermQuery(const std::vector<DocId>& postings, const std::string& term, const std::string& field);

    std::string name() const override {
        return absl::StrCat(field_, "#", term_, "->", cost());
    }
    DocId doc() const override {
        return current_doc_;
    }
    DocId next_doc() override;
    DocId advance(DocId target) override;
    Cost cost() const override {
        return postings_.size();
    }

private:
    bool advance_to(DocId target, const DocId* docs, int& current, int end, DocId& doc, int skip_len);

    DocId current_doc_{0};
    int index_{-1};
    std::vector<DocId> postings_;

    std::string term_;
    std::string field_;
};

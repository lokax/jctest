#include <unordered_map>
#include <vector>

#define TABLE_NUM = 10

struct JoinNode {
    JoinNode* left;
    JoinNode* right;
    size_t cost;

    JoinNode()
        : cost(100)
    {
    }

    JoinNode(JoinNode* left, JoinNode* right, size_t cost)
        : left(left)
        , right(right)
        , cost(cost)
    {
    }

    ~JoinNode()
    {
        delete left;
        delete right;
    }
};

std::vector<JoinNode*> join_nodes;

size_t Cost(JoinNode* left, JoinNode* right)
{
    return left->cost + right->cost + 100;
}

int main()
{
    std::unordered_map<RelationSet, JoinNode*> dp_table;

    // Dpsize算法
    for (size_t i = 2; i <= 10; ++i) {
        JoinNode* best_plan = nullptr;
        // 查找由i张表进行Join的最优计划
        for (size_t j = 1; j < i; ++j) {
            size_t k = i - j;
            // 查找由j张表形成的所有子集
            std::vector<RelationSet> left_sets = FindAllSubSet(j);
            std::vector<RelationSet> right_sets = FindAllSubSet(k);
            for (size_t left_set_idx = 0; left_set_idx < left_sets.size(); ++left_set_idx) {
                for (size_t right_set_idx = 0; right_set_idx < right_sets.size(); ++right_set_idx) {
                    JoinNode* left_best_plan = dp_table[left_sets[left_set_idx]];
                    JoinNode* right_best_plan = dp_table[right_sets[right_set_idx]];

                    // 检查集合Left和集合Right是否重叠
                    if (Check(left_sets[left_set_idx], right_sets[right_set_idx])) {
                        continue;
                    }

                    // 检查集合Left和集合Right是否连通
                    if (CheckConnecion(left_sets[left_set_idx], right_sets[right_set_idx])) {
                        auto new_cost = Cost(left_best_plan, right_best_plan);
                        if (best_plan == nullptr || new_cost < best_plan->cost) {
                            best_plan = new JoinNode(left_best_plan, right_best_plan, new_cost);
                            auto new_sets = Union(left_sets[left_set_idx], right_sets[right_set_idx]);
                            dp_table[new_sets] = best_plan;
                        }
                    }
                }
            }
        }
    }

    /// GOO算法
    for (size_t i = 0; i < 10; ++i) {
        join_nodes.emplace_back(new JoinNode());
    }

    while (join_nodes.size() > 1) {
        JoinNode* best_plan = nullptr;
        size_t left_idx = 0;
        size_t right_idx = 0;
        // 贪心，找到两张表Join的代价最小
        for (size_t i = 0; i < join_nodes.size(); ++i) {
            for (size_t j = i + 1; j < join_nodes.size(); ++j) {
                size_t new_join_node_cost = Cost(join_nodes[i], join_nodes[j]);
                if (best_plan == nullptr || new_join_node_cost < best_plan->cost) {
                    best_plan = new JoinNode(join_nodes[i], join_nodes[j], new_join_node_cost);
                    left_idx = i;
                    right_idx = j;
                }

                // Cost(join_nodes[j], join_nodes[i]);
            }
        }
        join_nodes.erase(join_nodes.begin() + left_idx);
        join_nodes.erase(join_nodes.begin() + right_idx);
        join_nodes.push_back(best_plan);
    }
    
}
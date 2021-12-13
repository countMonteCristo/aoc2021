#include "common.hpp"

#include <queue>
#include <unordered_set>


typedef std::unordered_set<std::string> verticies_t;
typedef std::pair<std::string, std::string> edge_t;
// typedef std::unordered_map<std::string, uint64_t> visited_t;
typedef std::unordered_map<std::string, verticies_t> graph_t;


void add_edge(const edge_t& edge, graph_t& graph) {
    graph[edge.first].insert(edge.second);
    graph[edge.second].insert(edge.first);
}

bool is_small(const std::string& s) {
    return std::all_of(s.begin(), s.end(), [](const char c) -> bool { return std::islower(c); });
}

uint64_t all_paths_1(const std::string& from, const std::string& to, const graph_t& graph, verticies_t visited_lower) {

    if (from == to)
        return 1;

    uint64_t paths_count = 0;

    for (const auto& v: graph.at(from)) {
        if (is_small(v)) {
            if (visited_lower.find(v) != visited_lower.end()) {
                // уже были в маленькой верщине
                continue;
            } else {
                auto new_visited(visited_lower);
                new_visited.insert(v);
                paths_count += all_paths_1(v, to, graph, new_visited);
            }
        } else {
            paths_count += all_paths_1(v, to, graph, visited_lower);
        }
    }
    return paths_count;
}

uint64_t all_paths_2(const std::string& from, const std::string& to, const graph_t& graph, verticies_t visited_lower, bool lower_visited_twice) {

    if (from == to)
        return 1;

    uint64_t paths_count = 0;

    for (const auto& v: graph.at(from)) {
        if (is_small(v)) {
            if (visited_lower.find(v) != visited_lower.end()) {
                // уже были в маленькой вершине
                if (lower_visited_twice) {
                    // усли уже быди в какой-то маленькой вершине и зашли ещё раз в какую-то другую или ту же
                    continue;
                } else {
                    // пока во всех маленьких вершинах были по одному разу
                    if ((v != "start") && (v != "end"))
                        paths_count += all_paths_2(v, to, graph, visited_lower, true);
                }
            } else {
                auto new_visited(visited_lower);
                new_visited.insert(v);
                paths_count += all_paths_2(v, to, graph, new_visited, lower_visited_twice);
            }
        } else {
            paths_count += all_paths_2(v, to, graph, visited_lower, lower_visited_twice);
        }
    }
    return paths_count;
}

void part_1(const aoc::input_t& input) {

    graph_t graph;
    for (const auto& line: input) {
        auto t = aoc::split(line, '-');
        add_edge(std::make_pair(t[0], t[1]), graph);
    }

    verticies_t visited_lower;
    visited_lower.insert("start");
    uint64_t ans = all_paths_1("start", "end", graph, visited_lower);

    if (ans != 3495)
        std::cerr << "Wrong answer in Day 12 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}


void part_2(const aoc::input_t& input) {

    graph_t graph;
    for (const auto& line: input) {
        auto t = aoc::split(line, '-');
        add_edge(std::make_pair(t[0], t[1]), graph);
    }

    verticies_t visited_lower;
    visited_lower.insert("start");
    uint64_t ans = all_paths_2("start", "end", graph, visited_lower, false);

    if (ans != 94849)
        std::cerr << "Wrong answer in Day 12 Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day12_1.txt");
    // const std::string day_input("./inputs/day12_test1.txt");
    // const std::string day_input("./inputs/day12_test2.txt");
    // const std::string day_input("./inputs/day12_test3.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

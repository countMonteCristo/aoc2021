#include "common.hpp"

#include <limits>
#include <unordered_set>

typedef aoc::u8v2_t table_t;
typedef aoc::i32v2_t scores_t;

typedef std::unordered_set<aoc::coord_t> points_t;

namespace std {
    template <>
    struct hash<aoc::coord_t>
    {
        std::size_t operator()(const aoc::coord_t& r) const
        {
            return std::hash<int32_t>()(r.first * 1000 + r.second);
        }
    };
}

int32_t dijkstra_heuristic(const aoc::coord_t& p1, const aoc::coord_t& p2) {
    (void) p1;
    (void) p2;
    return 0;
}

int32_t astar_heuristic(const aoc::coord_t& p1, const aoc::coord_t& p2) {
    return std::abs(p1.first - p2.first) + std::abs(p1.second - p2.second);
}

aoc::coord_t pop_best_point(const scores_t& scores, points_t& points, const aoc::coord_t& goal, int32_t (*heuristic)(const aoc::coord_t& p1, const aoc::coord_t& p2) = dijkstra_heuristic) {
    int32_t score = std::numeric_limits<int32_t>::max();
    aoc::coord_t point = {0, 0};

    bool found = false;
    for (const auto& p: points) {
        int32_t curr = scores[p.first][p.second] + heuristic(p, goal);
        if (curr < score ) {
            score = curr;
            point = p;
            found = true;
        }
    }
    if (!found) {
        std::cout << "Did not found best point!" << std::endl;
    } else {
        points.erase(point);
    }

    return point;
}

int32_t best_path(const table_t& table) {
    table_t visited(table.size(), aoc::u8v_t(table[0].size(), 0));

    scores_t scores(table.size(), aoc::i32v_t(table[0].size(), std::numeric_limits<int32_t>::max()));
    scores[0][0] = 0;

    points_t points;
    points.insert({0, 0});

    const aoc::coord_t goal = {scores.size() - 1, scores[0].size() - 1};

    while (points.size() > 0) {
        aoc::coord_t p = pop_best_point(scores, points, goal);
        visited[p.first][p.second] = 1;

        auto nbrs = aoc::neighbours(p, table[0].size(), table.size());
        for (const auto& n: nbrs) {
            if (visited[n.first][n.second])
                continue;
            points.insert(n);
            auto& score = scores[n.first][n.second];
            auto from_score = scores[p.first][p.second] + table[n.first][n.second];
            if (score > from_score) {
                score = from_score;
            }
        }
    }

    return scores.back().back();
}

void part_1(const aoc::input_t& input) {
    table_t table = aoc::convert(input, aoc::s2u8v);

    int32_t ans = best_path(table);

    if (ans != 498)
        std::cerr << "Wrong answer in Day 15 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}


void part_2(const aoc::input_t& input) {
    const size_t mult = 5;

    table_t table = aoc::convert(input, aoc::s2u8v);

    table.resize(mult * input.size());
    for (size_t r = 0; r < table.size(); r++) {
        auto br = std::div((int)r, input.size());
        table[r].resize(mult * input[br.rem].size());

        for (size_t c = 0; c < table[r].size(); c++) {
            auto bc = std::div((int)c, input[0].size());
            uint8_t v = table[br.rem][bc.rem] + br.quot + bc.quot;
            while (v > aoc::MAX_DIGIT)
                v -= aoc::MAX_DIGIT;
            table[r][c] = v;
        }
    }

    int32_t ans = best_path(table);

    if (ans != 2901)
        std::cerr << "Wrong answer in Day 15 Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day15_1.txt");
    // const std::string day_input("./inputs/day15_test.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

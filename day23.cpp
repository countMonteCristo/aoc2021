#include "common.hpp"

#include <queue>
#include <stack>
#include <limits>
#include <unordered_set>

typedef std::vector<aoc::coords_t> paths_t;
typedef std::unordered_map<char, paths_t> c2paths_t;

namespace std {
    template <>
    struct hash<aoc::coord_t>
    {
        std::size_t operator()(const aoc::coord_t& r) const
        {
            return std::hash<int32_t>()(r.first * 1000 + r.second);
        }
    };

    template <>
    struct hash<aoc::input_t>
    {
        std::size_t operator()(const aoc::input_t& i) const
        {
            std::string s;
            for (const auto& line: i)
                s += line;
            return std::hash<std::string>()(s);
        }
    };
}

typedef std::unordered_map<aoc::coord_t, aoc::coords_t> path_to_point_t;
typedef std::unordered_map<aoc::coord_t, path_to_point_t> be2path_t;
typedef std::unordered_map<aoc::coord_t, be2path_t> am2path_t;

am2path_t path_cache;
c2paths_t all;

static const std::unordered_map<char, aoc::coords_t> ends = {
    {'A', {{5, 3}, {4, 3}, {3, 3}, {2, 3}} },
    {'B', {{5, 5}, {4, 5}, {3, 5}, {2, 5}} },
    {'C', {{5, 7}, {4, 7}, {3, 7}, {2, 7}} },
    {'D', {{5, 9}, {4, 9}, {3, 9}, {2, 9}} },
};

static const std::unordered_map<char, uint64_t> cons = {
    {'A', 1}, {'B', 10}, {'C', 100}, {'D', 1000},
};

uint64_t get_energy(char c, const aoc::coords_t& path) {
    return cons.at(c) * (path.size() - 1);
}

bool is_amph(char c) {
    return (('A' <= c) && (c <= 'D'));
}

// uint8_t hsh(const aoc::coord_t& pos) {
//     return ((((int8_t)pos.first) << 4) | ((int8_t)pos.second));
// }

// uint64_t hsh(const aoc::input_t& map) {
//     std::vector<aoc::coords_t> points = {
//         {}, {}, {}, {}
//     };
//     for (size_t row = 0; row < map.size(); row++) {
//         for (size_t col = 0; col < map[row].size(); col++) {
//             char c = map[row][col];
//             if (!is_amph(c))
//                 continue;
//             points[c - 'A'].push_back({row, col});
//         }
//     }
//
//     uint64_t result = 0;
//     size_t n = 0;
//     for (auto& p: points) {
//         if ((p[0].first > p[1].first) || ((p[0].first == p[1].first) && (p[0].second > p[1].second)))
//             std::swap(p[0], p[1]);
//         n += 1;
//         result <<= 8;
//         result |= hsh(p[0]);
//         result <<= 8;
//         result |= hsh(p[1]);
//     }
//     return result;
// }

struct State {

    aoc::input_t map;
    std::vector<aoc::input_t> history;
    // uint32_t map_hash;

    uint64_t energy;
    aoc::coord_t last;

    std::vector<uint32_t> costs;

    uint64_t ap;
    size_t home;
    // std::unordered_set<uint64_t> hashes;
    // std::vector<uint64_t> h_vec;

    State copy(const aoc::input_t& new_map, const uint64_t en) const {
        State s;

        s.map = new_map;
        s.history = history;
        s.history.push_back(s.map);
        // s.map_hash = new_map_hash;
        // s.hashes = hashes;
        s.costs = costs;
        s.energy = en;
        // s.h_vec = h_vec;

        // s.hashes.insert(new_map_hash);
        // s.h_vec.push_back(new_map_hash);
        s.ap = s.approx(all);
        s.home = s.count_home();

        return s;
    }

    size_t count_home() const {
        // const auto& map = maps.back();
        size_t count = 0;
        for (size_t i = 0; i < 4; i++) {
            size_t col = (2 * i) + 3;
            char c = 'A' + i;
            for (size_t row = map.size() - 2; row > 1; row--) {
                if (map[row][col] == c)
                    count++;
                else
                    break;
            }
        }

        return count;
    }

    uint64_t approx(const c2paths_t& all) const {
        // const auto& map = maps.back();
        uint64_t val = 0;

        std::vector<uint8_t> counts(4, 0);

        for (size_t row = 0; row < map.size(); row++) {
            for (size_t col = 0; col < map[row].size(); col++) {
                char c = map[row][col];

                if (!is_amph(c))
                    continue;

                aoc::coord_t pos = {row, col};

                const auto& c_ends = ends.at(c);
                if (std::find(c_ends.begin(), c_ends.end(), pos) != c_ends.end())
                    continue;

                const auto& c_paths = all.at(c);

                for (const auto& p: c_paths) {
                    if ((p.front() == pos) && (p.back() == ends.at(c)[1])) {
                        val += get_energy(c, p);
                        counts[c-'A']++;
                        break;
                    }
                }
            }
        }

        for(uint8_t i = 0; i < counts.size(); i++) {
            if (counts[i] > 1)
                val += cons.at('A' + i) * counts[i] * (counts[i] - 1) / 2;
        }

        return val;
    }
};

bool cmp(const State& s1, const State& s2) {
    uint64_t e1 = s1.energy + s1.ap;
    uint64_t e2 = s2.energy + s2.ap;
    return e1 > e2;
}

static const std::unordered_set<aoc::coord_t> forbidden = {
    {1, 3}, {1, 5}, {1, 7}, {1, 9},
};


void show(const State& state) {
    // if (state.maps.size() == 1) {
        for (const auto& line: state.map)
            std::cout << line << std::endl;
    // } else {
    //     for (size_t i = 0; i < state.maps.back().size(); i++) {
    //         std::cout << state.maps[state.maps.size() - 2][i] << "\t" << state.maps.back()[i] << std::endl;
    //     }
    // }
    std::cout << "E=" << state.energy << " approx=" << state.ap << std::endl << std::endl;
}

void show(const State& from, const State& to) {
    for (size_t i = 0; i < from.map.size(); i++) {
        std::cout << from.map[i] << "\t" << to.map[i] << std::endl;
    }
    std::cout << "E=" << from.energy << "\t\t" << "E=" << to.energy << std::endl;
    std::cout << std::endl;
}

bool is_final(const State& state) {
    const auto& map = state.map;
    for (size_t row = 2; row < map.size()-1; row++) {
        for (char i = 0; i < 4; i++) {
            size_t col = 2 * i + 3;
            if (map[row][col] != ('A' + i))
                return false;
        }
    }
    return true;
}


bool is_valid(const aoc::coords_t& path, const aoc::input_t& map) {
    for (size_t i = 1; i < path.size(); i++) {
        const auto& [r, c] = path[i];
        if (map[r][c] != '.')
            return false;
    }
    return true;
}

void print(const aoc::input_t& map) {
    for (const auto& line: map)
        std::cout << line << std::endl;
}

std::vector<State> get_states_for(const aoc::coord_t& pos, const State& state, const c2paths_t& all, uint64_t min_energy) {

    const auto& [row, col] = pos;
    char c = state.map[row][col];

    std::vector<State> result;

    const auto& c_paths = all.at(c);
    // const auto& c_paths_from = path_cache.at(c).at(pos);

    // std::cout << "all paths for " << c << " " << pos << ":" << std::endl;
    // for (const auto& p: c_paths) {
    //     std::cout << "  " << p << std::endl;
    // }

    aoc::u32v_t valid_paths_idx;
    const auto& c_ends = ends.at(c);
    for (uint32_t i = 0; i < c_paths.size(); i++) {
        // std::cout << pos << ": check path " << c_paths[i] << std::endl;
        if ((c_paths[i].front() == pos) && is_valid(c_paths[i], state.map)) {
            // std::cout << "front is ok, path is valid" << std::endl;
            const auto& b = c_paths[i].back();
            // if ((c_ends[1] == b) && (state.map[b.first + 1][b.second] != c)) {
            if (b.second == ((c - 'A')*2 + 3))  {
                bool can = true;
                for (size_t k = state.map.size() - 2; k > b.first; k--)
                {
                    if ((state.map[k][b.second] != '.') && ((state.map[k][b.second] != c)))
                    {
                        can = false;
                        break;
                    }
                }
                if (!can)
                    continue;
                // std::cout << "------------------------------" << std::endl;
                // print(state.maps.back());
                // std::cout << std::endl;
                // std::cout << c_ends[1] << " " << b << " " << state.maps.back()[b.first][b.second] << " " << c << "stack is not ok, skip" << std::endl;
                // std::cout << "------------------------------" << std::endl;
                // continue;
            }

            const auto& f = c_paths[i].front();
            if ((c_ends[1] == f) && (state.map[f.first + 1][f.second] == c)) {
                continue;
            }
            valid_paths_idx.push_back(i);
        }
    }

    // std::cout << "valid paths for " << c << " " << pos << ":" << std::endl;
    // for (uint32_t idx: valid_paths_idx) {
    //     std::cout << "  " << c_paths[idx] << std::endl;
    // }

    for (uint32_t idx: valid_paths_idx) {
        const auto& p = c_paths[idx];
        const auto to = p.back();

        aoc::input_t map = state.map;
        map[row][col] = '.';
        map[to.first][to.second] = c;
        // uint64_t map_hash = hsh(map);

        // if (state.hashes.find(map_hash) != state.hashes.end())
        //     continue;
        if (std::find(state.history.begin(), state.history.end(), map) != state.history.end())
            continue;

        // history.push_back(map);
        uint64_t cost = get_energy(c, p);
        if ((state.energy + cost) > min_energy)
            continue;
        State ns = state.copy(map, state.energy + cost);
        result.push_back(ns);

        if (std::find(c_ends.begin(), c_ends.end(), to) != c_ends.end())
            break;
    }

    return result;
}

std::vector<State> get_best_state(const State& state, const c2paths_t& all, uint64_t min_energy) {
    std::vector<State> result_dumb;
    std::vector<State> result_smart;
    const auto& map = state.map;

    size_t s_home = state.home;

    for (size_t row = 0; row < map.size(); row++) {
        for (size_t col = 0; col < map.at(row).size(); col++) {
            aoc::coord_t pos = {row, col};
            if (is_amph(map.at(row).at(col)) && (state.last != pos)) {
                // std::cout << "try moving " << map.at(row).at(col) << " from (" << pos << "):" << std::endl;
                // const auto& states = get_states_for(pos, state, all, min_energy);

                // for (const auto& ns: states)
                    // show(state, ns);
                for (const auto& s: get_states_for(pos, state, all, min_energy)) {
                    if (s.home > s_home)
                        result_smart.push_back(s);
                    else
                        result_dumb.push_back(s);
                }
                // result.insert(result.end(), states.begin(), states.end());
            }
        }
    }

    if (result_smart.size()) {
        std::sort(result_smart.begin(), result_smart.end(), cmp);
        return result_smart;
    } else {
        std::sort(result_dumb.begin(), result_dumb.end(), cmp);
        return result_dumb;
    }
}


void clear(aoc::input_t& map) {
    for (size_t row = 0; row < map.size(); row++) {
        for (size_t col = 0; col < map[row].size(); col++)
            if (is_amph(map[row][col]))
                map[row][col] = '.';
    }
}

aoc::coords_t find_path(char c, const aoc::coord_t& from, const aoc::coord_t& to, const aoc::input_t& map) {
    aoc::coords_t path;

    std::stack<aoc::coords_t> stack;
    stack.push({from});

    while (!stack.empty()) {
        auto p = stack.top();
        stack.pop();
        auto nbrs = aoc::neighbours(p.back(), map[0].size(), map.size());
        for (const auto& n: nbrs) {
            const auto& [nr, nc] = n;
            if (map[nr][nc] != '.')
                continue;
            if (std::find(p.begin(), p.end(), n) != p.end())
                continue;
            auto next_p = p;
            next_p.push_back(n);
            if (n == to)
                return next_p;
            stack.push(next_p);
        }
    }

    // if (path.size()) {
    //     path_cache[c][from][to] = path;
    // }

    return path;
}

bool cmp_paths(const aoc::coords_t& p1, const aoc::coords_t& p2) {
    return p1.size() < p2.size();
}

paths_t get_all_paths(char c, const aoc::coord_t& pos, const aoc::input_t& map) {
    paths_t paths;

    const auto& c_ends = ends.at(c);
    if (c_ends.front() ==  pos)
        return {};

    // get paths to home
    for (const auto& to: c_ends) {
        const auto& cp = find_path(c, pos, to, map);
        if (cp.size())
            paths.push_back(cp);
    }

    paths_t tpaths;
    for (size_t col = 2; col < map[1].size()-2; col++) {
        if (map[1][col] != '.')
            continue;
        aoc::coord_t to = {1, col};
        if (pos == to)
            continue;
        if (std::find(forbidden.begin(), forbidden.end(), to) != forbidden.end())
            continue;
        const auto& cp = find_path(c, pos, to, map);
        if (cp.size())
            tpaths.push_back(cp);
    }
    std::sort(tpaths.begin(), tpaths.end(), cmp_paths);
    paths.insert(paths.end(), tpaths.begin(), tpaths.end());

    const auto& cp = find_path(c, pos, {1, 1}, map);
    if (cp.size())
        paths.push_back(cp);
    const auto& cq = find_path(c, pos, {1, 11}, map);
    if (cq.size())
        paths.push_back(cq);

    return paths;
}

c2paths_t precalc(const aoc::input_t& map, const std::string& chars) {
    c2paths_t paths;
    for (char c: chars) {
        for (size_t row = 0; row < map.size(); row++) {
            for (size_t col = 0; col < map[row].size(); col++) {
                if (map[row][col] != '.')
                    continue;
                aoc::coord_t p = {row, col};
                if (std::find(forbidden.begin(), forbidden.end(), p) != forbidden.end())
                    continue;
                auto all = get_all_paths(c, p, map);
                paths[c].insert(paths[c].end(), all.begin(), all.end());
            }
        }
    }
    return paths;
}


void part_1(const aoc::input_t& input) {

    aoc::input_t map = input;
    clear(map);
    all = precalc(map, "ABCD");

    std::cout << "precalc ok" << std::endl;

    // std::vector<std::pair<aoc::input_t, uint64_t>> cache;
    std::unordered_map<aoc::input_t, uint64_t> cache;

    std::stack<State> stack;

    State init;

    init.map = input;
    init.history = {input};
    init.energy = 0;
    init.last = {-1, -1};
    init.costs = {0};
    init.ap = init.approx(all);

    stack.push(init);
    uint64_t min_energy = std::numeric_limits<uint64_t>::max();
    // uint64_t min_energy = 13475;

    size_t n = 0;
    // size_t max_n = 40;

    while (!stack.empty()) {
    // while (!q.empty()) {
        auto state = stack.top();
        stack.pop();
        // State state = q.top();
        // q.pop();

        // show(state);
        if (state.ap + state.energy > min_energy)
            continue;
        // show(state);
        n++;
        // if (n >= max_n)
            // break;

        if (is_final(state)) {
            if (state.energy < min_energy) {
                min_energy = state.energy;
                std::cout << "found final state with energy " << min_energy << std::endl;
                for (size_t j = 0; j < state.history.size(); j++) {

                    // bool found = false;
                    // for (size_t k = 0; k < cache.size(); k++) {
                    //     if (cache[k].first == state.history[j]) {
                    //         if (cache[k].second > (state.energy - state.costs[j])) {
                    //             cache[k].second = (state.energy - state.costs[j]);
                    //         }
                    //         found = true;
                    //         break;
                    //     }
                    // }
                    // if (!found) {
                    //     cache.push_back({state.history[j], state.energy - state.costs[j]});
                    // }

                    auto it = cache.find(state.history[j]);
                    if (it != cache.end()) {
                        if (it->second > (state.energy - state.costs[j]))
                            it->second = (state.energy - state.costs[j]);
                    } else {
                        cache[state.history[j]] = state.energy - state.costs[j];
                    }
                }
            }
            continue;
        }

        if ((8 - state.count_home()) > (13 - state.history.size()))
            continue;

        // std::cout << "next: ===========================================" << std::endl;
        for(const auto& ns: get_best_state(state, all, min_energy)) {
            bool proceed = true;
            // const auto& map = ns.map;

            // auto it = cache.find(ns.map);
            // if (it != cache.end() && ((ns.energy + it->second) >= min_energy)) {
            //     proceed = false;
            // }

            // show(ns);
            if (proceed) {
                stack.push(ns);
                // q.push(ns);
            }
        }
        // std::cout << "===========================================" << std::endl;
    }

    if (min_energy != 13455)
        std::cerr << "Wrong answer in Day 23 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << min_energy << std::endl;
}


void part_2(const aoc::input_t& input) {

    aoc::input_t map = input;
    clear(map);
    all = precalc(map, "ABCD");

    std::cout << "precalc ok" << std::endl;

    // std::vector<std::pair<aoc::input_t, uint64_t>> cache;

    std::unordered_map<aoc::input_t, uint64_t> cache = {
        // {map, 0}
    };

    std::stack<State> stack;

    State init;

    init.map = input;
    init.history = {input};
    init.energy = 0;
    init.last = {-1, -1};
    init.costs = {0};
    init.ap = init.approx(all);
    init.home = init.count_home();

    stack.push(init);
    // uint64_t min_energy = std::numeric_limits<uint64_t>::max();
    uint64_t min_energy = 60000;

    size_t n = 0;
    size_t max_n = 10;

    while (!stack.empty()) {
    // while (!q.empty()) {
        auto state = stack.top();
        stack.pop();
        // State state = q.top();
        // q.pop();

        // show(state);
        if (state.ap + state.energy > min_energy)
            continue;
        // show(state);
        n++;
        // if (n >= max_n)
        //     break;

        if (is_final(state)) {
            if (state.energy < min_energy) {
                min_energy = state.energy;
                std::cout << "found final state with energy " << min_energy << std::endl;
                for (size_t j = 0; j < state.history.size(); j++) {

                    // bool found = false;
                    // for (size_t k = 0; k < cache.size(); k++) {
                    //     if (cache[k].first == state.history[j]) {
                    //         if (cache[k].second > (state.energy - state.costs[j])) {
                    //             cache[k].second = (state.energy - state.costs[j]);
                    //         }
                    //         found = true;
                    //         break;
                    //     }
                    // }
                    // if (!found) {
                    //     cache.push_back({state.history[j], state.energy - state.costs[j]});
                    // }

                    auto it = cache.find(state.history[j]);
                    if (it != cache.end()) {
                        if (it->second > (state.energy - state.costs[j]))
                            it->second = (state.energy - state.costs[j]);
                    } else {
                        cache[state.history[j]] = state.energy - state.costs[j];
                    }
                }
            }
            continue;
        }

        if ((16 - state.home) > (24 - state.history.size()))
            continue;

        // std::cout << "next: ===========================================" << std::endl;
        for(const auto& ns: get_best_state(state, all, min_energy)) {
            bool proceed = true;
            // const auto& map = ns.map;

            // auto it = cache.find(ns.map);
            // if (it != cache.end() && ((ns.energy + it->second) >= min_energy)) {
            //     proceed = false;
            // }

            // show(ns);
            if (proceed) {
                stack.push(ns);
                // q.push(ns);
            }
        }
        // std::cout << "===========================================" << std::endl;
    }


    if (min_energy != 1)
        std::cerr << "Wrong answer in Day 23 Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << min_energy << std::endl;

}

int main() {
    const std::string day_input("./inputs/day23_1.txt");
    // const std::string day_input("./inputs/day23_test.txt");

    auto input = aoc::load_input_from(day_input);

    // part_1(input);

    aoc::input_t add = {
        "  #D#C#B#A#",
        "  #D#B#A#C#",
    };
    input.insert(input.begin() + 3, add.begin(), add.end());

    part_2(input);

    return 0;
}

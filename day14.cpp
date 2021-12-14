#include "common.hpp"

#include <algorithm>


typedef std::pair<char, char> reagents_t;
typedef std::unordered_map<reagents_t, char> reactions_t;
typedef std::unordered_map<reagents_t, uint64_t> polymer_t;
typedef std::pair<char, uint64_t> count_t;
typedef std::unordered_map<count_t::first_type, count_t::second_type> counter_t;

auto count_cmp = [](const count_t& x1, const count_t& x2) { return x1.second < x2.second;};

namespace std {
    template <>
    struct hash<reagents_t>
    {
        std::size_t operator()(const reagents_t& r) const
        {
            return std::hash<int32_t>()(r.first * 1000 + r.second);
        }
    };
}

void init(const aoc::input_t& input, reactions_t& reactions) {
    for (size_t i = 2; i < input.size(); i++) {
        auto parts = aoc::split(input[i], ' ');
        reagents_t r = {parts[0][0], parts[0][1]};
        reactions[r] = parts[2][0];
    }
}

std::string bruteforce(const std::string& prev, const reactions_t& reactions) {
    std::string next(2*prev.size()-1, '*');
    for (size_t i = 0; i < prev.size() - 1; i++) {
        reagents_t r = {prev[i], prev[i+1]};
        char c = reactions.at(r);
        next[2*i] = prev[i];
        next[2*i+1] = c;
    }
    next[next.size()-1] = prev.back();
    return next;
}

polymer_t fast_step(const polymer_t& prev, const reactions_t& reactions) {
    polymer_t nxt;
    for (const auto& kv: prev) {
        const auto& [p, value] = kv;
        char c = reactions.at(p);
        nxt[{p.first, c}] += value;
        nxt[{c, p.second}] += value;
    }
    return nxt;
}

void part_1(const aoc::input_t& input) {

    const std::string& tmplt = input[0];
    reactions_t reactions;
    init(input, reactions);

    std::string nxt(tmplt);
    for (int i = 0; i < 10; i++)
        nxt = bruteforce(nxt, reactions);

    counter_t counter;
    for (char c: nxt)
        counter[c]++;
    std::vector<count_t> elems(counter.begin(), counter.end());
    std::sort(elems.begin(), elems.end(), count_cmp);

    uint64_t ans = elems.back().second - elems.front().second;

    if (ans != 2712)
        std::cerr << "Wrong answer in Day 14 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}

void part_2(const aoc::input_t& input) {

    const std::string& tmplt = input[0];
    reactions_t reactions;
    init(input, reactions);

    polymer_t polymer;
    for (size_t i = 0; i < tmplt.size() - 1; i++)
        polymer[{tmplt[i], tmplt[i+1]}]++;

    for (int i = 0; i < 40; i++)
        polymer = fast_step(polymer, reactions);

    counter_t counter;
    for (const auto& kv: polymer) {
        counter[kv.first.second] += kv.second;
    }
    counter[tmplt[0]]++;

    std::vector<count_t> elems(counter.begin(), counter.end());
    std::sort(elems.begin(), elems.end(), count_cmp);

    uint64_t ans = elems.back().second - elems.front().second;;

    if (ans != 8336623059567)
        std::cerr << "Wrong answer in Day 14 Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day14_1.txt");
    // const std::string day_input("./inputs/day14_test.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

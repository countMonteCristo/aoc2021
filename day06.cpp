#include <numeric>

#include "common.hpp"


uint64_t sim(const u64v_t& initial, size_t N, uint8_t new_default, uint8_t old_default) {
    u64v_t counts(new_default + 1);
    for (auto n: initial) {
        counts[n]++;
    }

    for (size_t i = 0; i < N; i++) {
        u64v_t next(counts.size(), 0);
        for (size_t j = 0; j < counts.size(); j++) {
            auto current = counts[j];
            if (j == 0) {
                next[new_default] += current;
                next[old_default] += current;
            } else {
                next[j-1] += current;
            }
        }
        counts = next;
    }

    return std::accumulate(counts.begin(), counts.end(), (uint64_t)0);
}

void part_1(const input_t& input) {
    u64v_t initial = split<uint64_t>(input[0], ',', s2u64);
    size_t ans = sim(initial, 80, 8, 6);

    if (ans != 365131)
        std::cerr << "Wrong answer in Day 06 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}


void part_2(const input_t& input) {
    u64v_t initial = split<uint64_t>(input[0], ',', s2u64);
    size_t ans = sim(initial, 256, 8, 6);

    if (ans != 1650309278600)
        std::cerr << "Wrong answer in Day 06 Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day06_1.txt");
    // const std::string day_input("./inputs/day06_test.txt");

    auto input = load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

#include "common.hpp"


void part_1(const aoc::input_t& input) {
    auto pos = aoc::convert(aoc::split(input[0], ','), aoc::s2u32);
    std::sort(pos.begin(), pos.end());

    uint32_t ans = 0;
    for (size_t i = 0, j = pos.size() - 1; i < j; i++, j--) {
        ans += pos[j] - pos[i];
    }

    if ( ans != 343468 )
        std::cerr << "Wrong answer in Day 07 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}

inline uint32_t cost(uint32_t n) {
    return n * (n + 1) / 2;
}

uint32_t bruteforce(const aoc::u32v_t pos) {
    uint32_t min = pos.front();
    uint32_t max = pos.back();

    aoc::u32v_t result(max - min + 1, 0);
    for (uint32_t x = min; x <= max; x++) {
        uint32_t sum = 0;
        for (auto n: pos) {
            int32_t diff = n - x;
            sum += cost(std::abs(diff));
        }
        result[x-min] = sum;
    }

    std::sort(result.begin(), result.end());
    return result[0];
}

void part_2(const aoc::input_t& input) {
    auto pos = aoc::convert(aoc::split(input[0], ','), aoc::s2u32);
    std::sort(pos.begin(), pos.end());

    uint32_t ans = bruteforce(pos);

    if ( ans != 96086265 )
        std::cerr << "Wrong answer in Day 07 Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day07_1.txt");
    // const std::string day_input("./inputs/day07_test.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

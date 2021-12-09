#include "common.hpp"


size_t get_increase_count(const aoc::i32v_t& depths, size_t window) {
    size_t count = 0;
    for (size_t i=window; i<depths.size(); i++) {
        if (depths[i] > depths[i-window]) {
            count++;
        }
    }
    return count;
}

void part_1(const aoc::input_t& input) {
    auto depths = aoc::convert(input, +aoc::s2i32);
    size_t count = get_increase_count(depths, 1);

    if (count != 1301)
        std::cerr << "Wrong answer in day 01, part 1" << std::endl;

    std::cout << "[Part 1] Increases count: " << count << std::endl;
}

void part_2(const aoc::input_t& input) {
    auto depths = aoc::convert(input, +aoc::s2i32);
    size_t count = get_increase_count(depths, 3);

    if (count != 1346)
        std::cerr << "Wrong answer in day 01, part 2" << std::endl;

    std::cout << "[Part 2] Increases count: " << count << std::endl;
}

int main() {
    const std::string day_input("./inputs/day01_1.txt");
    aoc::input_t input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

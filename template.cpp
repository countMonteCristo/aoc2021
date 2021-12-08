#include "common.hpp"


void part_1(const aoc::input_t& input) {

    uint32_t ans = 0;

    if (ans != 1)
        std::cerr << "Wrong answer in Day <%DAY> Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}


void part_2(const aoc::input_t& input) {

    uint32_t ans = 0;

    if (ans != 1)
        std::cerr << "Wrong answer in Day <%DAY> Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day<%DAY>_1.txt");
    // const std::string day_input("./inputs/day<%DAY>_test.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

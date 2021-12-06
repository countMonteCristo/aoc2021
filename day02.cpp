#include "common.hpp"

typedef std::pair<std::string, uint32_t> inst_t;
typedef std::vector<inst_t> course_t;

auto s2inst = [](const std::string& s) -> inst_t {
    input_t parts = split<std::string>(s, ' ');
    return std::make_pair(parts[0], std::stoul(parts[1]));
};

void part_1(const input_t& input) {
    course_t course = convert<inst_t>(input, s2inst);

    int hpos = 0;
    int depth = 0;

    for (const auto& [command, X]: course) {
        if (command == "forward") {
            hpos += X;
        }
        if (command == "up") {
            depth -= X;
        }
        if (command == "down") {
            depth += X;
        }
    }

    int ans = hpos * depth;

    if (ans != 2091984)
        std::cerr << "Wrong answer in day 02, part 1" << std::endl;

    std::cout << "[Task 1] hpos = " << hpos << " depth = " << depth << " hpos*depth = " << ans << std::endl;
}

void part_2(const input_t& input) {
    course_t course = convert<inst_t>(input, s2inst);

    int hpos = 0;
    int depth = 0;
    int aim = 0;

    for (const auto& [command, X]: course) {
        if (command == "forward") {
            hpos += X;
            depth += aim * X;
        }
        if (command == "up") {
            aim -= X;
        }
        if (command == "down") {
            aim += X;
        }
    }

    int ans = hpos * depth;

    if (ans != 2086261056)
        std::cerr << "Wrong answer in day 02, part 2" << std::endl;

    std::cout << "[Task 2] hpos = " << hpos << " depth = " << depth << " aim = " << aim << " hpos*depth = " << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day02_1.txt");

    auto input = load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

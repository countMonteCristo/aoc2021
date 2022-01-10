#include "common.hpp"


void debug(const aoc::input_t& map) {
    for (const auto& line: map) {
        std::cout << line << std::endl;
    }
}

uint32_t step(aoc::input_t& map) {
    // std::cout << "before > moves:"  << std::endl;
    // debug(map);

    uint32_t moves_count = 0;
    aoc::input_t next = map;
    for (size_t row = 0; row < map.size(); row++) {
        for (size_t col = 0; col < map[row].size(); col++) {
            if (map[row][col] == '>') {
                size_t adj = (col + 1) % map[row].size();
                if (map[row][adj] == '.') {
                    next[row][adj] = '>';
                    next[row][col] = '.';
                    moves_count++;
                }
            }
        }
    }
    map = next;

    // std::cout << "all > moves:"  << std::endl;
    // debug(map);

    for (size_t row = 0; row < map.size(); row++) {
        size_t adj = (row + 1) % map.size();
        for (size_t col = 0; col < map[row].size(); col++) {
            if (map[row][col] == 'v') {
                if (map[adj][col] == '.') {
                    next[adj][col] = 'v';
                    next[row][col] = '.';
                    moves_count++;
                }
            }
        }
    }

    map = next;
    return moves_count;
}



void part_1(const aoc::input_t& input) {

    uint32_t ans = 0;
    aoc::input_t map = input;

    // std::cout << "initial:" << std::endl;
    // debug(map);

    while (true) {
        uint32_t moves_count = step(map);
        ans++;
        // std::cout << "After day " << ans << ":" << std::endl;
        // debug(map);
        // if (ans >= 5)
        //     break;
        if (moves_count == 0)
            break;
    }

    if (ans != 1)
        std::cerr << "Wrong answer in Day 25 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}


void part_2(const aoc::input_t& input) {

    uint32_t ans = 0;

    if (ans != 1)
        std::cerr << "Wrong answer in Day 25 Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day25_1.txt");
    // const std::string day_input("./inputs/day25_test.txt");
    // const std::string day_input("./inputs/day25_t1.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

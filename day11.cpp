#include "common.hpp"

#include <stack>

typedef aoc::u8v2_t table_t;

void simulate_step(table_t& energy, uint32_t& n_flashes) {
    const size_t width = energy[0].size();
    const size_t height = energy.size();

    for (size_t r = 0; r < height; r++) {
        for (size_t c = 0; c < energy[r].size(); c++) {
            energy[r][c]++;
        }
    }

    std::stack<aoc::coord_t> flashes;
    for (size_t r = 0; r < height; r++) {
        for (size_t c = 0; c < width; c++) {
            if (energy[r][c] > aoc::MAX_DIGIT) {
                flashes.push({r,c});
            }
        }
    }

    while (flashes.size()) {
        auto point = flashes.top();
        flashes.pop();
        n_flashes++;

        auto nbrs = aoc::neighbours(point, width, height, aoc::Nbh::Moore);
        for (auto& p: nbrs) {
            auto [r, c] = p;
            if (energy[r][c] <= aoc::MAX_DIGIT) {
                energy[r][c]++;
                if (energy[r][c] > aoc::MAX_DIGIT)
                    flashes.push(p);
            }
        }
    }

    for (size_t r = 0; r < height; r++) {
        for (size_t c = 0; c < width; c++) {
            if (energy[r][c] > aoc::MAX_DIGIT) {
                energy[r][c] = 0;
            }
        }
    }
}

void part_1(const aoc::input_t& input) {

    table_t energy(input.size(), aoc::u8v_t(input[0].size(), 0));
    for (size_t r = 0; r < input.size(); r++) {
        for (size_t c = 0; c < input[r].size(); c++) {
            energy[r][c] = static_cast<uint8_t>(input[r][c] - '0');
        }
    }

    uint32_t ans = 0;
    for (size_t i = 0; i < 100; ++i) {
        simulate_step(energy, ans);
    }

    if (ans != 1673)
        std::cerr << "Wrong answer in Day 11 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}


void part_2(const aoc::input_t& input) {

    table_t energy(input.size(), aoc::u8v_t(input[0].size(), 0));
    for (size_t r = 0; r < input.size(); r++) {
        for (size_t c = 0; c < input[r].size(); c++) {
            energy[r][c] = static_cast<uint8_t>(input[r][c] - '0');
        }
    }

    uint32_t total = energy.size() * energy[0].size();
    uint32_t ans = 0;
    while (true) {
        uint32_t n_flashes = 0;
        simulate_step(energy, n_flashes);
        ans++;
        if (n_flashes == total )
            break;
    }

    if (ans != 279)
        std::cerr << "Wrong answer in Day 11 Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day11_1.txt");
    // const std::string day_input("./inputs/day11_test.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

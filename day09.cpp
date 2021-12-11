#include "common.hpp"

typedef aoc::u32v_t basin_t;
typedef aoc::u8v2_t table_t;


void fill_basin(table_t& heights, const aoc::coord_t& point, basin_t& basins) {
    aoc::coords_t basin;
    basin.push_back(point);
    auto& [pc, pr] = point;
    heights[pr][pc] = aoc::MAX_DIGIT;
    while (true) {
        aoc::coords_t candidates;
        for (const auto& p: basin) {
            auto all_nbrs = aoc::neighbours(p, heights[0].size(), heights.size(), aoc::Nbh::VonNeumann);
            for (const auto& n: all_nbrs) {
                auto& [c, r] = n;
                if (heights[r][c] != aoc::MAX_DIGIT) {
                    candidates.push_back(n);
                    heights[r][c] = aoc::MAX_DIGIT;
                }
            }
        }
        basin.insert(basin.end(), candidates.begin(), candidates.end());

        if (!candidates.size())
            break;
    }
    basins.push_back(basin.size());
}

void part_1(const aoc::input_t& input) {

    table_t heights(input.size(), aoc::u8v_t(input[0].size(), 0));
    for (size_t r = 0; r < input.size(); r++) {
        for (size_t c = 0; c < input[r].size(); c++) {
            heights[r][c] = static_cast<uint8_t>(input[r][c] - '0');
        }
    }

    uint32_t ans = 0;
    for (size_t r = 0; r < heights.size(); r++) {
        for (size_t c = 0; c < heights[r].size(); c++) {
            const auto nbrs = aoc::neighbours({c, r}, heights[0].size(), heights.size(), aoc::Nbh::VonNeumann);

            size_t count = 0;
            for (const auto& n: nbrs) {
                auto& [nc, nr] = n;
                if (heights[r][c] < heights[nr][nc])
                    count++;
            }

            if (count == nbrs.size())
                ans += (heights[r][c] + 1);
        }
    }

    if (ans != 508)
        std::cerr << "Wrong answer in Day 09 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}


void part_2(const aoc::input_t& input) {

    table_t heights(input.size(), aoc::u8v_t(input[0].size(), 0));
    for (size_t r = 0; r < input.size(); r++) {
        for (size_t c = 0; c < input[r].size(); c++) {
            heights[r][c] = static_cast<uint8_t>(input[r][c] - '0');
        }
    }

    basin_t basins;
    for (size_t r = 0; r < heights.size(); r++) {
        for (size_t c = 0; c < heights[r].size(); c++) {
            if (heights[r][c] != aoc::MAX_DIGIT) {
                fill_basin(heights, {c, r}, basins);
            }
        }
    }

    std::sort(basins.begin(), basins.end(), std::greater<>{});

    uint32_t ans = 1;
    for (size_t i=0; i < 3; i++) {
        ans *= basins[i];
    }

    if (ans != 1564640)
        std::cerr << "Wrong answer in Day 09 Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day09_1.txt");
    // const std::string day_input("./inputs/day09_test.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

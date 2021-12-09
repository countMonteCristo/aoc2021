#include "common.hpp"

constexpr uint8_t MAX_HEIGHT = 9;


typedef std::pair<int, int> coord_t;
typedef std::vector<coord_t> coords_t;
typedef aoc::u32v_t basin_t;

coords_t neighbours(const coord_t& point, int width, int height) {
    coords_t result;
    auto& [x, y] = point;
    coords_t all = {{x-1, y}, {x+1, y}, {x, y-1}, {x, y+1}};
    for (const auto& n: all) {
        auto& [nx, ny] = n;
        if ((nx < 0) || (nx == width) || (ny < 0) || (ny == height))
            continue;
        result.push_back(n);
    }
    return result;
}

void fill_basin(std::vector<aoc::u8v_t>& heights, const coord_t& point, basin_t& basins) {
    coords_t basin;
    basin.push_back(point);
    auto& [pc, pr] = point;
    heights[pr][pc] = MAX_HEIGHT;
    while (true) {
        coords_t candidates;
        for (const auto& p: basin) {
            auto all_nbrs = neighbours(p, heights[0].size(), heights.size());
            for (const auto& n: all_nbrs) {
                auto& [c, r] = n;
                if (heights[r][c] != MAX_HEIGHT) {
                    candidates.push_back(n);
                    heights[r][c] = MAX_HEIGHT;
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

    std::vector<aoc::u8v_t> heights(input.size(), aoc::u8v_t(input[0].size(), 0));
    for (size_t r = 0; r < input.size(); r++) {
        for (size_t c = 0; c < input[r].size(); c++) {
            heights[r][c] = static_cast<uint8_t>(input[r][c] - '0');
        }
    }

    uint32_t ans = 0;
    for (size_t r = 0; r < heights.size(); r++) {
        for (size_t c = 0; c < heights[r].size(); c++) {
            const auto nbrs = neighbours({c, r}, heights[0].size(), heights.size());

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

    std::vector<aoc::u8v_t> heights(input.size(), aoc::u8v_t(input[0].size(), 0));
    for (size_t r = 0; r < input.size(); r++) {
        for (size_t c = 0; c < input[r].size(); c++) {
            heights[r][c] = static_cast<uint8_t>(input[r][c] - '0');
        }
    }

    basin_t basins;
    for (size_t r = 0; r < heights.size(); r++) {
        for (size_t c = 0; c < heights[r].size(); c++) {
            if (heights[r][c] != MAX_HEIGHT) {
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

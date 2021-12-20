#include "common.hpp"

#include <cmath>

aoc::coord_t fire(int32_t v0x, int32_t v0y, int32_t k) {
    int32_t x = (v0x >= k)? ( k * v0x - k*(k-1)/2 ): v0x*(v0x+1)/2;
    int32_t y = k*v0y - k*(k-1)/2;
    return {x, y};
}

int32_t brute_p1(const aoc::i32v_t& xr, aoc::i32v_t& yr) {
    int32_t vx_min = std::ceil((std::sqrt(1 + 8.0 * xr[0]) - 1) / 2);
    int32_t vx_max = xr[1];
    int32_t vy_min = 0;
    int32_t vy_max = -yr[0];

    int32_t max_h = 0;
    for (int32_t vx = vx_min; vx <= vx_max; ++vx) {
        for (int32_t vy = vy_min; vy <= vy_max; ++vy) {
            int32_t k = 1;
            while (true) {
                auto [x, y] = fire(vx, vy, k);
                if ((xr[0] <= x) && (x <= xr[1]) && (yr[0] <= y) && (y <= yr[1])) {
                    max_h = std::max(max_h, vy*(vy+1)/2);
                }
                if ((x > xr[1]) || (y < yr[0]))
                    break;
                k++;
            }

        }
    }
    return max_h;
}

int32_t brute_p2(const aoc::i32v_t& xr, aoc::i32v_t& yr) {
    int32_t vx_min = std::ceil((std::sqrt(1 + 8.0 * xr[0]) - 1) / 2);
    int32_t vx_max = xr[1];
    int32_t vy_min = yr[0];
    int32_t vy_max = -yr[0];

    int32_t ans = 0;
    for (int32_t vx = vx_min; vx <= vx_max; ++vx) {
        for (int32_t vy = vy_min; vy <= vy_max; ++vy) {
            int32_t k = 1;
            while (true) {
                auto [x, y] = fire(vx, vy, k);
                if ((xr[0] <= x) && (x <= xr[1]) && (yr[0] <= y) && (y <= yr[1])) {
                    ans++;
                    break;
                }
                if ((x > xr[1]) || (y < yr[0]))
                    break;
                k++;
            }
        }
    }
    return ans;
}

void part_1(const aoc::input_t& input) {

    aoc::input_t words = aoc::split(input[0], '=');
    aoc::i32v_t x_range = aoc::convert(aoc::split(aoc::split(words[1], ',')[0], '.'), aoc::s2i32);
    aoc::i32v_t y_range = aoc::convert(aoc::split(words[2], '.'), aoc::s2i32);

    int32_t vx_min = std::ceil((std::sqrt(1 + 8.0 * x_range[0]) - 1) / 2);
    int32_t vx_max = x_range[1];

    int32_t ans = 0;
    // int32_t qvx = 0;
    // int32_t qvy = 0;
    for (int32_t vx = vx_min; vx <= vx_max; ++vx) {
        int32_t k = 1;
        int32_t d_max = vx*(vx+1)/2;
        while (true) {
            int32_t x = (vx >= k)? ( k * vx - k*(k-1)/2 ): d_max;
            if (x > x_range[1])
                break;
            if (x < x_range[0]) {
                k++;
                continue;
            }
            int32_t vy_min = std::ceil((1.0*y_range[0])/k + (k-1.0)/2);
            int32_t vy_max = std::floor((1.0*y_range[1])/k + (k-1.0)/2);

            if (vy_min > vy_max) {
                if (k + 2 * y_range[0] <= 0) {
                    k++;
                    continue;
                } else {
                    break;
                }
            }

            for (int32_t vy = std::max(1, vy_min); vy <= vy_max; vy++) {
                if (vy <= k) {
                    int32_t h = vy*(vy+1)/2;
                    if (h > ans) {
                        ans = h;
                        qvx = vx;
                        qvy = vy;
                    }
                }
            }
            k++;
        }
    }

    if (ans != 2701)
        std::cerr << "Wrong answer in Day 17 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << " brute_p1: " << brute_p1(x_range, y_range) << std::endl;
}

void part_2(const aoc::input_t& input) {

    aoc::input_t words = aoc::split(input[0], '=');
    aoc::i32v_t x_range = aoc::convert(aoc::split(aoc::split(words[1], ',')[0], '.'), aoc::s2i32);
    aoc::i32v_t y_range = aoc::convert(aoc::split(words[2], '.'), aoc::s2i32);

    int32_t vx_min = std::ceil((std::sqrt(1 + 8.0 * x_range[0]) - 1) / 2);
    int32_t vx_max = x_range[1];

    aoc::coords_t vels;

    uint32_t ans = 0;
    for (int32_t vx = vx_min; vx <= vx_max; ++vx) {
        int32_t k = 1;
        int32_t d_max = vx*(vx+1)/2;
        while (true) {
            int32_t x = (vx >= k)? ( k * vx - k*(k-1)/2 ): d_max;
            if (x > x_range[1])
                break;
            if (x < x_range[0]) {
                k++;
                continue;
            }
            int32_t vy_min = std::ceil((1.0*y_range[0])/k + (k-1.0)/2);
            int32_t vy_max = std::floor((1.0*y_range[1])/k + (k-1.0)/2);

            if (vy_min > vy_max) {
                if ( k + 2 * y_range[0] <= 0 ) {
                    k++;
                    continue;
                } else {
                    break;
                }
            }

            for (int32_t vy = vy_min; vy <= vy_max; vy++) {
                auto [xk, yk] = fire(vx, vy, k);
                if ((x_range[0] <= xk) && (xk <= x_range[1]) && (y_range[0] <= yk) && (yk <= y_range[1])) {
                    auto p = std::make_pair(vx, vy);
                    if (std::find(vels.begin(), vels.end(), p) == vels.end()) {
                        ans++;
                        vels.push_back(p);
                    }
                }
            }
            k++;
        }
    }

    if (ans != 1070)
        std::cerr << "Wrong answer in Day 17 Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << " brute_p2: " << brute_p2(x_range, y_range)<< std::endl;
}

int main() {
    const std::string day_input("./inputs/day17_1.txt");
    // const std::string day_input("./inputs/day17_test.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

#include "common.hpp"

#include <limits>
#include <unordered_map>


constexpr int16_t WINDOW_WIDTH = 3;
constexpr int16_t WINDOW_HEIGHT = 3;
constexpr int16_t WINDOW_POINTS_COUNT = WINDOW_WIDTH * WINDOW_HEIGHT;

typedef aoc::u8v_t algo_t;
typedef std::pair<int16_t, int16_t> pos_t;

uint8_t c2d(char c) {
    return (c == '.')? 0: 1;
}

aoc::u8v_t s2v(const std::string& line) {
    aoc::u8v_t result(line.size(), 0);
    for (size_t i = 0; i < result.size(); ++i)
        result[i] = c2d(line[i]);
    return result;
}

typedef std::unordered_map<int16_t, uint8_t> col2el_t;
typedef std::unordered_map<int16_t, col2el_t> row2col_t;

size_t get_pixel(const row2col_t& prev, const algo_t& algo, const pos_t& pos, uint8_t outer) {
    size_t value = 0;
    for (size_t i = 0; i < WINDOW_POINTS_COUNT; i++) {
        int16_t nrow = pos.first - WINDOW_HEIGHT / 2 + i / WINDOW_WIDTH;
        int16_t ncol = pos.second - WINDOW_WIDTH / 2 + i % WINDOW_WIDTH;
        uint8_t d;
        if ((prev.find(nrow) == prev.end()) || (prev.at(nrow).find(ncol) == prev.at(nrow).end())) {
            d = outer;
        } else {
            d = prev.at(nrow).at(ncol);
        }
        value = (value << 1) | d;
    }
    return algo[value];
}

row2col_t step(const row2col_t& prev, const algo_t& algo, uint8_t outer, const pos_t& tl, const pos_t& br) {
    row2col_t next;

    for (const auto& [row, column_map]: prev) {
        for (const auto& item: column_map) {
            int16_t col = item.first;
            next[row][col] = get_pixel(prev, algo, {row, col}, outer);
        }
    }

    for  (int16_t c = tl.second - 1; c <= br.second + 1; c++ ) {
        next[tl.first - 1][c] = get_pixel(prev, algo, {tl.first - 1, c}, outer);
        next[br.first + 1][c] = get_pixel(prev, algo, {br.first + 1, c}, outer);
    }

    for  (int16_t r = tl.first; r <= br.first; r++ ) {
        next[r][tl.second - 1] = get_pixel(prev, algo, {r, tl.second - 1}, outer);
        next[r][br.second + 1] = get_pixel(prev, algo, {r, br.second + 1}, outer);
    }

    return next;
}

void draw(const row2col_t& image, const pos_t& tl, const pos_t& br) {
    for (int16_t r = tl.first; r <= br.first; r++) {
        for (int16_t c = tl.second; c <= br.second; c++) {
            char d = image.at(r).at(c)? '#': '.';
            std::cout << d;
        }
        std::cout << std::endl;
    }
}

void part_1(const aoc::input_t& input) {
    aoc::u8v_t algo = s2v(input[0]);

    row2col_t image;
    for (size_t i = 2; i < input.size(); i++) {
        for (size_t j = 0; j < input[i].size(); j++) {
            image[(int16_t)i - 2][(int16_t)j] = c2d(input[i][j]);
        }
    }

    uint8_t outer = 0;
    for (int16_t i = 0; i < 2; i++) {
        image = step(image, algo, outer, {0-i, 0-i, }, {input.size() - 3 + i, input.back().size() - 1 + i});
        size_t outer_index = (outer == 0)? 0: (algo.size() - 1);
        outer = algo[outer_index];
    }

    uint32_t ans = 0;
    for (const auto& it_col: image) {
        for (const auto& p: it_col.second) {
            ans += p.second;
        }
    }

    if (ans != 5301)
        std::cerr << "Wrong answer in Day 20 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}


void part_2(const aoc::input_t& input) {

    aoc::u8v_t algo = s2v(input[0]);

    row2col_t image;
    for (size_t i = 2; i < input.size(); i++) {
        for (size_t j = 0; j < input[i].size(); j++) {
            image[((int16_t)i) - 2][(int16_t)j] = c2d(input[i][j]);
        }
    }

    uint8_t outer = 0;
    for (int16_t i = 0; i < 50; i++) {
        image = step(image, algo, outer, {0-i, 0-i, }, {input.size() - 3 + i, input.back().size() - 1 + i});
        size_t outer_index = (outer == 0)? 0: (algo.size() - 1);
        outer = algo[outer_index];
    }

    uint32_t ans = 0;
    for (const auto& it_col: image) {
        for (const auto& p: it_col.second) {
            ans += p.second;
        }
    }

    if (ans != 19492)
        std::cerr << "Wrong answer in Day 20 Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day20_1.txt");
    // const std::string day_input("./inputs/day20_test.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

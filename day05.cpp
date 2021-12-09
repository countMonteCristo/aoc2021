#include "common.hpp"


struct Point {
    Point() : x(0), y(0) {}
    Point(uint32_t x_, uint32_t y_) : x(x_), y(y_) {}
    uint32_t x;
    uint32_t y;
};

struct Line {
    Line(const Point& p1_, const Point& p2_) : p1(p1_), p2(p2_) {}
    Point p1;
    Point p2;
};

auto s2point = [](const std::string& s) -> Point {
    auto sub = aoc::split(s, ',');
    return Point(aoc::s2u32(sub[0]), aoc::s2u32(sub[1]));
};

auto s2line = [](const std::string& s) -> Line {
    auto sub = aoc::split(s, ' ');
    return Line(s2point(sub[0]), s2point(sub[2]));
};

typedef std::vector<Line> lines_t;

struct Table {
    Table(const aoc::input_t& input) : width(0), height(0) {
        lines = aoc::convert(input, s2line);
        _init_size();

        field.resize(height);
        for (size_t r = 0; r < field.size(); r++) {
            field[r].resize(width);
        }
    }

    lines_t lines;
    aoc::u32v2_t field;
    uint32_t width;
    uint32_t height;

    void _init_size() {
        for (const auto& line: lines) {
            width = std::max(width, line.p1.x);
            width = std::max(width, line.p2.x);
            height = std::max(height, line.p1.y);
            height = std::max(height, line.p2.y);
        }
        width++;
        height++;
    }

    void process(bool all=false) {
        uint32_t start, finish;
        uint32_t r_start, c_start, r_finish, c_finish;

        for (const auto& line: lines) {
            if (line.p1.x == line.p2.x) {           // vertical
                aoc::minmax(line.p1.y, line.p2.y, start, finish);
                for (size_t r = start; r <= finish; r++) {
                    field[r][line.p1.x]++;
                }
            } else if (line.p1.y == line.p2.y) {    // horizontal
                aoc::minmax(line.p1.x, line.p2.x, start, finish);
                for (size_t c = start; c <= finish; c++) {
                    field[line.p1.y][c]++;
                }
            } else if (all) {                       // 45 degrees

                int dx = line.p2.x - line.p1.x;
                int dy = line.p2.y - line.p1.y;

                aoc::minmax(line.p1.y, line.p2.y, r_start, r_finish);
                aoc::minmax(line.p1.x, line.p2.x, c_start, c_finish);

                if (dx == dy) {
                    aoc::minmax(line.p1.x, line.p2.x, c_start, c_finish);
                    for (size_t r=r_start, c=c_start; (r <=r_finish) && (c <= c_finish); r++, c++) {
                        field[r][c]++;
                    }
                } else if (dx == -dy) {
                    std::swap(c_start, c_finish);
                    for (size_t r=r_start, c=c_start; (r <=r_finish) && (c >= c_finish); r++, c--) {
                        field[r][c]++;
                    }
                } else {
                    std::cerr << "WTF?" << std::endl;
                }

            }
        }
    }

    uint32_t ans() {
        uint32_t result = 0;
        for (size_t r = 0; r < field.size(); r++) {
            for (size_t c = 0; c < field[r].size(); c++) {
                if ( field[r][c] > 1) {
                    result++;
                }
            }
        }
        return result;
    }
};

void part_1(const aoc::input_t& input) {
    Table table(input);
    table.process();
    size_t ans = table.ans();

    if (ans != 7436)
        std::cerr << "Wrong answer in day 05, part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}


void part_2(const aoc::input_t& input) {
    Table table(input);
    table.process(true);
    size_t ans = table.ans();

    if (ans != 21104)
        std::cerr << "Wrong answer in day 05, part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day05_1.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

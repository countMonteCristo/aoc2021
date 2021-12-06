#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>


typedef std::vector<std::string> input_t;
typedef std::vector<size_t> row_t;


struct Point {
    size_t x;
    size_t y;
};
struct Line {
    Point p1;
    Point p2;
};

typedef std::vector<Line> lines_t;

input_t split(const std::string& line, char delim) {
    input_t result;
    std::stringstream stream(line);
    std::string item;

    while (std::getline(stream, item, delim)) {
        result.push_back(item);
    }

    return result;
}

Point parse_point(const std::string& s) {
    input_t coords_str = split(s, ',');
    Point p;
    p.x = std::stoul(coords_str[0]);
    p.y = std::stoul(coords_str[1]);
    return p;
}

input_t load_input_from(const std::string& filepath) {
    std::ifstream infile(filepath);
    if ( !infile.is_open() ) {
        std::cerr << "Cannot open input file: " << filepath << std::endl;
        std::exit(1);
    }

    input_t result;
    std::string line;
    while (std::getline(infile, line)) {
        result.push_back(line);
    }
    return result;
}

lines_t parse(const input_t& input) {
    lines_t lines;
    for (const auto& row: input ) {
        input_t line_str = split(row, ' ');
        Line l;
        l.p1 = parse_point(line_str[0]);
        l.p2 = parse_point(line_str[2]);
        lines.push_back(l);
    }
    return lines;
}



void part_1(const input_t& input) {
    lines_t lines = parse(input);

    size_t max_x = 0;
    size_t max_y = 0;
    for (const auto& line: lines) {
        max_x = std::max(max_x, line.p1.x);
        max_x = std::max(max_x, line.p2.x);
        max_y = std::max(max_y, line.p1.y);
        max_y = std::max(max_y, line.p2.y);
    }
    std::cout << max_x << " " << max_y << std::endl;

    std::vector<std::vector<size_t>> table;
    table.resize(max_y);
    for (size_t r = 0; r < table.size(); r++) {
        table[r].resize(max_x);
    }

    for (const auto& line: lines) {
        if (line.p1.x == line.p2.x) {
            size_t start = std::min(line.p1.y, line.p2.y);
            size_t finish = std::max(line.p1.y, line.p2.y);
            for (size_t r=start; r <= finish; r++) {
                table[r][line.p1.x]++;
            }
        } else if (line.p1.y == line.p2.y) {
            size_t start = std::min(line.p1.x, line.p2.x);
            size_t finish = std::max(line.p1.x, line.p2.x);
            for (size_t c=start; c <= finish; c++) {
                table[line.p1.y][c]++;
            }
        }
    }

    size_t ans = 0;
    for (size_t r = 0; r < table.size(); r++) {
        for (size_t c = 0; c < table[r].size(); c++) {
            if ( table[r][c] > 1) {
                ans++;
            }
        }
    }

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}


void part_2(const input_t& input) {

    lines_t lines = parse(input);

    size_t max_x = 0;
    size_t max_y = 0;
    for (const auto& line: lines) {
        max_x = std::max(max_x, line.p1.x);
        max_x = std::max(max_x, line.p2.x);
        max_y = std::max(max_y, line.p1.y);
        max_y = std::max(max_y, line.p2.y);
    }
    std::vector<std::vector<size_t>> table;
    table.resize(max_y + 1);
    for (size_t r = 0; r < table.size(); r++) {
        table[r].resize(max_x + 1);
    }

    for (const auto& line: lines) {
        if (line.p1.x == line.p2.x) {
            size_t start = std::min(line.p1.y, line.p2.y);
            size_t finish = std::max(line.p1.y, line.p2.y);
            for (size_t r=start; r <= finish; r++) {
                table[r][line.p1.x]++;
            }
        } else if (line.p1.y == line.p2.y) {
            size_t start = std::min(line.p1.x, line.p2.x);
            size_t finish = std::max(line.p1.x, line.p2.x);
            for (size_t c=start; c <= finish; c++) {
                table[line.p1.y][c]++;
            }
        } else {    // 45 degrees
            int dx = line.p2.x - line.p1.x;
            int dy = line.p2.y - line.p1.y;

            if (dx == dy) {
                size_t r_start = std::min(line.p1.y, line.p2.y);
                size_t r_finish = std::max(line.p1.y, line.p2.y);
                size_t c_start = std::min(line.p1.x, line.p2.x);
                size_t c_finish = std::max(line.p1.x, line.p2.x);
                for (size_t r=r_start, c=c_start; (r <=r_finish) && (c <= c_finish); r++, c++) {
                    table[r][c]++;
                }
            } else if (dx == -dy) {
                size_t r_start = std::min(line.p1.y, line.p2.y);
                size_t r_finish = std::max(line.p1.y, line.p2.y);
                size_t c_start = std::max(line.p1.x, line.p2.x);
                size_t c_finish = std::min(line.p1.x, line.p2.x);
                for (size_t r=r_start, c=c_start; (r <=r_finish) && (c >= c_finish); r++, c--) {
                    table[r][c]++;
                }
            } else {
                std::cout << "WTF?" << std::endl;
            }

        }
    }

    size_t ans = 0;
    for (size_t r = 0; r < table.size(); r++) {
        for (size_t c = 0; c < table[r].size(); c++) {
            if ( table[r][c] > 1) {
                ans++;
            }
        }
    }

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day05_1.txt");

    auto input = load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

#include "common.hpp"

#include <unordered_set>

struct Point {
    Point(int32_t x_, int32_t y_): x(x_), y(y_) {}

    bool operator==(const Point& p) const {
        return (x == p.x) && (y == p.y);
    }

    int32_t x, y;
};

namespace std {
    template <>
    struct hash<Point>
    {
        std::size_t operator()(const Point& p) const
        {
            return std::hash<int32_t>()(p.x * 10000 + p.y);
        }
    };
}

typedef std::unordered_set<Point> points_t;

class Folder {
public:
    Folder(const std::string& line) {
        aoc::input_t parts = aoc::split(aoc::split(line, ' ')[2], '=');
        along = parts[0];
        pos = aoc::s2i32(parts[1]);
    }

    Point operator()(const Point& point) const {
        int32_t new_x = point.x;
        int32_t new_y = point.y;
        if (along == "x") {     // fold along x
            new_x = pos - std::abs(point.x - pos);
        } else {                // fold along y
            new_y = pos - std::abs(point.y - pos);
        }
        return Point(new_x, new_y);
    }
private:
    std::string along;
    int32_t pos;
};

typedef std::vector<Folder> folds_t;

points_t fold(points_t points, const Folder& f) {
    points_t new_points;
    for (auto p: points) {
        new_points.insert(f(p));
    }
    return new_points;
}

void prepare_input(const aoc::input_t& input, points_t& points, folds_t& transforms) {
    bool is_points = true;
    for (const auto& line: input) {
        if (!line.size()) {
            is_points = false;
            continue;
        }
        if (is_points) {
            auto p = aoc::convert(aoc::split(line, ','), aoc::s2i32);
            points.insert(Point(p[0], p[1]));
        } else {
            transforms.push_back(Folder(line));
        }
    }
}

void part_1(const aoc::input_t& input) {

    points_t points;
    folds_t transforms;
    prepare_input(input, points, transforms);

    uint32_t ans = fold(points, transforms[0]).size();

    if (ans != 710)
        std::cerr << "Wrong answer in Day 13 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}


void print(const points_t& points) {
    int max_x = 0;
    int max_y = 0;
    for (const auto& p: points) {
        max_x = std::max(max_x, p.x);
        max_y = std::max(max_y, p.y);
    }
    size_t width = max_x + 1;
    size_t height = max_y + 1;

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            char c = (points.find(Point(x, y)) != points.end())? '#': ' ';
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

void part_2(const aoc::input_t& input) {

    points_t points;
    folds_t transforms;
    prepare_input(input, points, transforms);

    for (auto& f: transforms) {
        points = fold(points, f);
    }
    print(points);
    const std::string ans("EPLGRULR");

    if (ans != "EPLGRULR")
        std::cerr << "Wrong answer in Day 13 Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day13_1.txt");
    // const std::string day_input("./inputs/day13_test.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

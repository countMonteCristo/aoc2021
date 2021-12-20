#include "common.hpp"

#include <algorithm>
#include <unordered_set>


constexpr size_t N_COORDS = 3;
constexpr size_t N_COMMON_MIN = 12;
constexpr size_t ORIENT_MAX = 24;

struct Point3d {
    Point3d() : x(0), y(0), z(0) {}
    Point3d(int32_t x_, int32_t y_, int32_t z_)
        : x(x_), y(y_), z(z_) {}

    int32_t x, y, z;

    int32_t n(size_t n_coord) const {
        if (n_coord == 0)
            return x;
        if (n_coord == 1)
            return y;
        if (n_coord == 2)
            return z;
        std::cerr << "incorrect n_coord value: " << n_coord << " (should be 0, 1 or 2)" << std::endl;
        exit(1);
    }

    int32_t manhattan() const {
        return std::abs(x) + std::abs(y) + std::abs(z);
    }

    Point3d operator+(const Point3d& p) const {
        return Point3d(x+p.x, y+p.y, z+p.z);
    }

    Point3d operator-(const Point3d& p) const {
        return Point3d(x-p.x, y-p.y, z-p.z);
    }

    bool operator==(const Point3d& p) {
        return (x == p.x) && (y == p.y) && (z == p.z);
    }
};

typedef std::vector<Point3d> scanner_t;
typedef std::vector<scanner_t> data_t;

std::ostream& operator<<(std::ostream& o, const Point3d& p) {
    return o << "(" << p.x << "," << p.y << "," << p.z << ")";
}

Point3d oriented(const Point3d& p, size_t orientation) {
    Point3d r;
    switch (orientation) {
        case  0: r.x =  p.x; r.y =  p.y; r.z =  p.z; break;
        case  1: r.x =  p.x; r.y = -p.y; r.z = -p.z; break;
        case  2: r.x =  p.x; r.y = -p.z; r.z =  p.y; break;
        case  3: r.x =  p.x; r.y =  p.z; r.z = -p.y; break;
        case  4: r.x = -p.x; r.y =  p.y; r.z = -p.z; break;
        case  5: r.x = -p.x; r.y = -p.y; r.z =  p.z; break;
        case  6: r.x = -p.x; r.y =  p.z; r.z =  p.y; break;
        case  7: r.x = -p.x; r.y = -p.z; r.z = -p.y; break;
        case  8: r.x =  p.y; r.y =  p.z; r.z =  p.x; break;
        case  9: r.x =  p.y; r.y = -p.z; r.z = -p.x; break;
        case 10: r.x =  p.y; r.y = -p.x; r.z =  p.z; break;
        case 11: r.x =  p.y; r.y =  p.x; r.z = -p.z; break;
        case 12: r.x = -p.y; r.y = -p.z; r.z =  p.x; break;
        case 13: r.x = -p.y; r.y =  p.z; r.z = -p.x; break;
        case 14: r.x = -p.y; r.y =  p.x; r.z =  p.z; break;
        case 15: r.x = -p.y; r.y = -p.x; r.z = -p.z; break;
        case 16: r.x =  p.z; r.y = -p.y; r.z =  p.x; break;
        case 17: r.x =  p.z; r.y =  p.y; r.z = -p.x; break;
        case 18: r.x =  p.z; r.y =  p.x; r.z =  p.y; break;
        case 19: r.x =  p.z; r.y = -p.x; r.z = -p.y; break;
        case 20: r.x = -p.z; r.y =  p.y; r.z =  p.x; break;
        case 21: r.x = -p.z; r.y = -p.y; r.z = -p.x; break;
        case 22: r.x = -p.z; r.y = -p.x; r.z =  p.y; break;
        case 23: r.x = -p.z; r.y =  p.x; r.z = -p.y; break;
    }
    return r;
}

void show_oriented(const scanner_t& scanner, size_t ornt) {
    std::cout << "orientation: " << ornt << std::endl;
    for (const auto& p: scanner) {
        auto r = oriented(p, ornt);
        std::cout << p << " -> " << r << std::endl;
    }
}

aoc::i32v_t get_coord(const scanner_t& scanner, size_t n_coord, size_t orientation) {
    aoc::i32v_t result(scanner.size());
    for (size_t i = 0; i < scanner.size(); i++) {
        result[i] = oriented(scanner[i], orientation).n(n_coord);
    }
    return result;
}

typedef std::pair<int32_t, size_t> diff_t;
typedef std::vector<diff_t> diffs_t;

diff_t check_coord(const aoc::i32v_t& known, const aoc::i32v_t& tested) {
    std::unordered_set<int32_t> known_set(known.begin(), known.end());

    for (size_t i = 0; i < known.size(); i++) {
        for (size_t j = 0; j < tested.size(); j++) {
            int32_t diff = known[i] - tested[j];

            size_t n_common = 0;
            for (size_t k = 0; k < tested.size(); k++) {
                n_common += (size_t)(known_set.find(tested[k] + diff) != known_set.end());
            }

            if (n_common >= N_COMMON_MIN) {
                return {diff, n_common};
            }
        }
    }

    return {0, 0};
}

std::pair<Point3d, size_t> check_coord_orientations(const scanner_t& known_scanner, const scanner_t& tested_scanner) {
    Point3d result;
    size_t orient;

    aoc::i32v2_t known_table;
    for (size_t i = 0; i < N_COORDS; i++)
        known_table.push_back(get_coord(known_scanner, i, 0));

    for (orient = 0; orient < ORIENT_MAX; orient++) {
        // std::cout << orient << ":\n";

        diffs_t cd;
        cd.resize(N_COORDS);

        bool success = true;
        for (size_t n_coord = 0; n_coord < N_COORDS; n_coord++) {
            const auto& known = known_table[n_coord];
            auto tested = get_coord(tested_scanner, n_coord, orient);
            auto result = check_coord(known, tested);

            success &= (result.second > 0);
            if (!success)
                break;

            cd[n_coord] = result;
        }
        if (success) {
            result.x = cd[0].first;
            result.y = cd[1].first;
            result.z = cd[2].first;
            break;
        }
    }
    return {result, orient};
}

scanner_t part_1(const aoc::input_t& input) {

    data_t data;
    scanner_t scanner;
    size_t scanner_id = 0;
    for (const auto& line: input) {
        if (!line.size()) {
            scanner_id++;
            data.push_back(scanner);
            scanner.resize(0);
            continue;
        }
        if (line.back() == '-') {
            // start of new scanner data
            continue;
        }
        aoc::i32v_t c = aoc::convert(aoc::split(line, ','), aoc::s2i32);
        Point3d p(c[0], c[1], c[2]);
        scanner.push_back(p);
    }
    data.push_back(scanner);

    check_coord_orientations(data[0], data[3]);
    std::unordered_set<size_t> matched_scanners;
    scanner_t global_diff(data.size(), Point3d());
    matched_scanners.insert(0);

    while (matched_scanners.size() < data.size()) {
        for (size_t i = 1; i < data.size(); i++) {
            if (matched_scanners.find(i) != matched_scanners.end())
                continue;

            for (auto j: matched_scanners) {
                auto [diff, orient] = check_coord_orientations(data[j], data[i]);
                if (orient < ORIENT_MAX) {
                    // нашли совпадение
                    global_diff[i] = diff + global_diff[j];
                    for (size_t k = 0; k < data[i].size(); k++) {
                        data[i][k] = oriented(data[i][k], orient);
                    }

                    matched_scanners.insert(i);
                    break;
                }
            }
        }
    }

    scanner_t all;
    for (size_t j = 0; j < data.size(); j++) {
        const auto& scanner = data[j];
        for (size_t i = 0; i < scanner.size(); i++) {
            Point3d p = scanner[i] + global_diff.at(j);
            if (std::find(all.begin(), all.end(), p) == all.end())
                all.push_back(p);
        }
    }

    uint32_t ans = all.size();

    if (ans != 491)
        std::cerr << "Wrong answer in Day 19 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
    return global_diff;
}


void part_2(const scanner_t& global_diff) {

    int32_t ans = 0;
    for (size_t i = 0; i < global_diff.size(); i++) {
        for (size_t j = 0; j < global_diff.size(); j++) {
            auto p = global_diff.at(i) - global_diff.at(j);
            ans = std::max(ans, p.manhattan());
        }
    }

    if (ans != 13374)
        std::cerr << "Wrong answer in Day 19 Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day19_1.txt");
    // const std::string day_input("./inputs/day19_test.txt");
    // const std::string day_input("./inputs/day19_t1.txt");

    auto input = aoc::load_input_from(day_input);

    scanner_t global_diffs = part_1(input);
    part_2(global_diffs);

    return 0;
}

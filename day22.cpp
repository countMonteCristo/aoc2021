#include "common.hpp"

#include <limits>
#include <queue>

constexpr int64_t MIN_C = -50;
constexpr int64_t MAX_C = 50;

typedef std::vector<int64_t> range_t;
typedef std::pair<int64_t, int64_t> seg_t;

typedef std::unordered_map<int64_t, bool> z2state_t;
typedef std::unordered_map<int64_t, z2state_t> y2z_t;
typedef std::unordered_map<int64_t, y2z_t> xyz_t;


bool seg_int(const seg_t& s1, const seg_t& s2) {
    return ((s1.second >= s2.first) && (s1.first <= s2.second));
}

std::vector<seg_t> cut_seg(const seg_t& s, const seg_t& other) {
    if (!seg_int(s, other)) {
        return {s};
    }
    if (other.first <= s.first){
        if (other.second < s.second) {
            return {{s.first, other.second}, {other.second+1, s.second}};
        } else {
            return {s};
        }
    } else {
        if (other.second < s.second) {
            return {
                {s.first, other.first-1},
                {other.first, other.second},
                {other.second+1, s.second}
            };
        } else {
            return {
                {s.first, other.first-1},
                {other.first, s.second}
            };
        }
    }
}

int64_t len(const seg_t& s) {
    return (s.second - s.first + 1);
}

struct Cube {
    Cube()
        : xs({0, 0}), ys({0, 0}), zs({0, 0}) {}
    Cube(const seg_t& xs_,  const seg_t& ys_, const seg_t& zs_)
        :xs(xs_), ys(ys_), zs(zs_)
    {}
    Cube(const range_t& xr,  const range_t& yr, const range_t& zr)
        :xs({xr[0], xr[1]}), ys({yr[0], yr[1]}), zs({zr[0], zr[1]})
    {}
    seg_t xs, ys, zs;

    int64_t volume() const {
        return len(xs) * len(ys) * len(zs);
    }

    bool intersects(const Cube& c) const {
        return seg_int(xs, c.xs) && seg_int(ys, c.ys) && seg_int(zs, c.zs);
    }

    std::vector<Cube> cut_by(const Cube& c) const;
};

std::ostream& operator<<(std::ostream& o, const seg_t& s) {
    return o << s.first << ".." << s.second;
}

std::ostream& operator<<(std::ostream& o, const Cube& c) {
    return o << "(" << c.xs << ", " << c.ys << ", " << c.zs << ")";
}

std::vector<Cube> Cube::cut_by(const Cube& c) const {
    std::vector<Cube> result;

    if (!intersects(c))
        return {*this};

    auto x_seg = cut_seg(xs, c.xs);
    auto y_seg = cut_seg(ys, c.ys);
    auto z_seg = cut_seg(zs, c.zs);
    for (const auto& txs: x_seg) {
        for (const auto& tys: y_seg) {
            for (const auto& tzs: z_seg) {
                Cube q(txs, tys, tzs);
                if ( !c.intersects(q) ) {
                    result.push_back(q);
                }
            }
        }
    }

    return result;
}

void part_1(const aoc::input_t& input) {
    xyz_t cubes;

    for (const auto& line: input) {
        auto parts = aoc::split(line, ' ');
        bool state = (parts[0] == "on");
        auto ranges = aoc::split(parts[1], ',');
        auto xr = aoc::convert(aoc::split(aoc::split(ranges[0], '=')[1], '.'), aoc::s2i64);
        auto yr = aoc::convert(aoc::split(aoc::split(ranges[1], '=')[1], '.'), aoc::s2i64);
        auto zr = aoc::convert(aoc::split(aoc::split(ranges[2], '=')[1], '.'), aoc::s2i64);

        if (!seg_int({xr[0], xr[1]}, {-50, 50}) || !seg_int({yr[0], yr[1]}, {-50, 50}) || !seg_int({zr[0], zr[1]}, {-50, 50}))
            continue;

        for (int64_t x = std::max(xr[0], MIN_C); x <= std::min(xr[1], MAX_C); x++) {
            for (int64_t y = std::max(yr[0], MIN_C); y <= std::min(yr[1], MAX_C); y++) {
                for (int64_t z = std::max(zr[0], MIN_C); z <= std::min(zr[1], MAX_C); z++) {
                    cubes[x][y][z] = state;
                }
            }
        }
    }

    uint64_t ans = 0;
    for (int64_t x = MIN_C; x <= MAX_C; x++) {
        for (int64_t y = MIN_C; y <= MAX_C; y++) {
            for (int64_t z = MIN_C; z <= MAX_C; z++) {
                ans += (uint64_t)cubes[x][y][z];
            }
        }
    }

    if (ans != 615700)
        std::cerr << "Wrong answer in Day 22 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}


void part_2(const aoc::input_t& input) {

    std::vector<Cube> on_cubes;
    for (const auto& line: input) {
        auto parts = aoc::split(line, ' ');
        bool state = (parts[0] == "on");
        auto ranges = aoc::split(parts[1], ',');
        auto xr = aoc::convert(aoc::split(aoc::split(ranges[0], '=')[1], '.'), aoc::s2i64);
        auto yr = aoc::convert(aoc::split(aoc::split(ranges[1], '=')[1], '.'), aoc::s2i64);
        auto zr = aoc::convert(aoc::split(aoc::split(ranges[2], '=')[1], '.'), aoc::s2i64);

        Cube current(xr, yr, zr);

        if (state) { // insert 'on'-cube
            std::vector<Cube> parts = {current};
            for (const auto& other: on_cubes) {
                std::vector<Cube> new_parts;
                for (const auto& c: parts) {
                    auto cubes = c.cut_by(other);
                    new_parts.insert(new_parts.end(), cubes.begin(), cubes.end());
                }
                parts = new_parts;
            }
            on_cubes.insert(on_cubes.end(), parts.begin(), parts.end());
        } else {    // insert 'off' cube
            std::vector<Cube> new_on;
            for (const auto& on: on_cubes) {
                auto parts = on.cut_by(current);
                new_on.insert(new_on.end(), parts.begin(), parts.end());
            }
            on_cubes = new_on;
        }
    }

    uint64_t ans = 0;
    for (const auto& c: on_cubes) {
        ans += static_cast<uint64_t>(c.volume());
    }

    if (ans != 1236463892941356)
        std::cerr << "Wrong answer in Day 22 Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day22_1.txt");
    // const std::string day_input("./inputs/day22_test.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

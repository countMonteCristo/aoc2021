#include "common.hpp"

#include <unordered_set>

bool is_reg(const std::string& arg) {
    return ('w' <= arg[0]) && (arg[0] <= 'z');
}

int64_t v2dec(const aoc::i64v_t& v) {
    int64_t res = 0;
    for (int64_t x: v) {
        res = (res * 10) + x;
    }
    return res;
}

// 0:  inp w
// 4:  div z D;
// 5:  add x A;
// 15: add y B;
struct Block {
    Block() : D(0), A(0), B(0) {}

    int64_t eval(int64_t w, int64_t z) const {
        if ((D == 1) || (D == 26)) {
            if (((z % 26) + A) == w) {
                return z / D;
            } else {
                return (z / D) * 26 + (w + B);
            }
        } else {
            std::cerr << "Wrong value of D: " << D << " (should be only 1 or 26)" << std::endl;
            exit(1);
        }
    }

    // block.eval(w, z) = res -> zs = block.solve_z(w, res)
    aoc::i64v_t solve_z(int64_t w, int64_t res) const {
        aoc::i64v_t ans;

        int64_t start_z = res * D;
        int64_t finish_z = start_z + D - 1;
        for (int64_t z = start_z; z <= finish_z; z++) {
            if ((z % 26) + A == w)
                ans.push_back(z);
        }

        if ((res - w - B) % 26 == 0) {
            start_z = (res - w - B) / 26;
            finish_z = start_z + D - 1;
            for (int64_t z = start_z; z <= finish_z; z++) {
                if ((z % 26) + A != w)
                    ans.push_back(z);
            }
        }
        return ans;
    }

    int64_t D, A, B;
};

std::ostream& operator<<(std::ostream& o, const Block& b) {
    return o << "div z " << b.D << " add x " << b.A << " add y " << b.B;
}

struct Programm {

    int64_t exec(const aoc::input_t& input, const aoc::i64v_t& w) {

        std::unordered_map<char, int64_t> d = {
            {'w', 0}, {'x', 0}, {'y', 0}, {'z', 0},
        };

        size_t i = 0;
        for (const auto& line: input) {
            auto parts = aoc::split(line, ' ');

            char reg = parts[1][0];
            if (parts[0] == "inp") {
                d[reg] = w[i];
                i++;
                continue;
            }
            int64_t v = (is_reg(parts[2]))? d[parts[2][0]]: aoc::s2i64(parts[2]);

            if (parts[0] == "mul") {
                d[reg] *= v;
            }
            if (parts[0] == "add") {
                d[reg] += v;
            }
            if (parts[0] == "div") {
                d[reg] /= v;
            }
            if (parts[0] == "mod") {
                d[reg] %= v;
            }
            if (parts[0] == "eql") {
                d[reg] = (d[reg] == v)? 1: 0;
            }
        }
        return d['z'];
    }

    std::vector<Block> get_blocks(const aoc::input_t& input) {

        size_t n = 0;
        std::vector<Block> blocks;

        for (const auto& line: input) {
            n++;
            auto parts = aoc::split(line, ' ');

            if (parts[0] == "inp") {
                n = 0;
                blocks.push_back(Block());
                continue;
            }
            if (n == 4) {
                blocks.back().D = aoc::s2i64(parts[2]);
            }
            if (n == 5) {
                blocks.back().A = aoc::s2i64(parts[2]);
            }
            if (n == 15) {
                blocks.back().B = aoc::s2i64(parts[2]);
            }
        }
        return blocks;
    }

    int64_t find(const std::vector<Block>& blocks, bool find_max = true) {
        int64_t w_start, w_finish, dw;
        if (find_max) {
            w_start = 1;
            w_finish = 10;
            dw = 1;
        } else {
            w_start = 9;
            w_finish = 0;
            dw = -1;
        }

        std::unordered_set<int64_t> current_zs = {0};
        std::unordered_map<int64_t, aoc::i64v_t> res;
        for (size_t ri = 0; ri < 14; ri++ ) {
            size_t i = blocks.size() - 1 - ri;

            std::unordered_set<int64_t> new_zs;
            std::unordered_map<int64_t, aoc::i64v_t> res_copy;
            int64_t w = w_start;
            while(w != w_finish) {
                for (int64_t z: current_zs) {
                    auto prev_zs = blocks[i].solve_z(w, z);

                    for (auto prev_z: prev_zs) {
                        new_zs.insert(prev_z);
                        aoc::i64v_t v = (res.find(z) == res.end())? aoc::i64v_t(): res[z];
                        v.insert(v.begin(), w);
                        res_copy[prev_z] = v;
                    }
                }
                w += dw;
            }
            current_zs = new_zs;
            res = res_copy;
        }

        return v2dec(res[0]);
    }

};

void part_1(const aoc::input_t& input) {

    Programm p;
    int64_t ans = p.find(p.get_blocks(input), true);

    if (ans != 99919765949498)
        std::cerr << "Wrong answer in Day 24 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}


void part_2(const aoc::input_t& input) {

    Programm p;
    int64_t ans = p.find(p.get_blocks(input), false);

    if (ans != 24913111616151)
        std::cerr << "Wrong answer in Day 24 Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day24_1.txt");
    // const std::string day_input("./inputs/day24_test.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

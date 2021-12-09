#include "common.hpp"

#include <unordered_map>


typedef std::pair<aoc::input_t, aoc::input_t> task_t;
typedef std::vector<task_t> result_t;

void part_1(const aoc::input_t& input) {
    uint32_t ans = 0;

    for (const auto& line: input) {
        auto parts = aoc::split(line, ' ');
        auto delim = std::find(parts.begin(), parts.end(), "|");
        ++delim;
        for (; delim != parts.end(); ++delim) {
            size_t size = delim->size();
            if ((size == 2) || (size == 3) || (size == 4) || (size == 7))
                ans++;
        }
    }

    if (ans != 239)
        std::cerr << "Wrong answer in Day 08 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}

uint32_t decode(const task_t& task) {
    std::unordered_map<char, char> wires;
    std::unordered_map<uint8_t, std::string> digits = {
        {0, ""}, {1, ""}, {2, ""}, {3, ""}, {4, ""},
        {5, ""}, {6, ""}, {7, ""}, {8, ""}, {9, ""},
    };

    // determine 1, 4, 7, 8
    aoc::input_t six_segment_digits, five_segment_digits;
    for (const auto& digit: task.first) {
        size_t size = digit.size();
        switch (size) {
            case 2: digits[1] = digit; break;
            case 3: digits[7] = digit; break;
            case 4: digits[4] = digit; break;
            case 5: five_segment_digits.push_back(digit); break;
            case 7: digits[8] = digit; break;
            case 6: six_segment_digits.push_back(digit); break;
            default: continue;
        }
    }
    const std::string& one = digits[1];
    const std::string& eight = digits[8];

    // determine digit '6'
    for (const std::string& six_candidate: six_segment_digits) {
        if (!aoc::contains(six_candidate, one[0]) || !aoc::contains(six_candidate, one[1])) {
            digits[6] = six_candidate;
            break;
        }
    }
    const std::string& six = digits[6];

    // get wire for 'c'
    for (char c: eight) {
        if (!aoc::contains(six, c)) {
            wires['c'] = c;
            break;
        }
    }

    // get wire for 'f'
    wires['f'] = (one[0] == wires['c'])? one[1]: one[0];

    // get '5' and wire for 'e'
    for (const auto& five_candidate: five_segment_digits) {
        std::vector<char> not_found_chars;
        for (char c: six) {
            if (!aoc::contains(five_candidate, c)) not_found_chars.push_back(c);
        }
        if (not_found_chars.size() == 1) {
            digits[5] = five_candidate;
            wires['e'] = not_found_chars[0];
            break;
        }
    }
    const std::string& five = digits[5];

    // get digits '0' and '9'
    for (const auto& zero_or_nine: six_segment_digits) {
        if (zero_or_nine == six) continue;
        if (!aoc::contains(zero_or_nine, wires['e'])) {
            digits[9] = zero_or_nine;
        } else {
            digits[0] = zero_or_nine;
        }
    }

    // get digits '2' and '3'
    for (const auto& two_or_three: five_segment_digits) {
        if (two_or_three == five) continue;
        if (!aoc::contains(two_or_three,  wires['f'])) {
            digits[2] = two_or_three;
        } else {
            digits[3] = two_or_three;
        }
    }

    std::unordered_map<std::string, uint8_t> s_to_digit;
    for (const auto& p: digits) {
        s_to_digit[p.second] = p.first;
    }

    std::vector<uint8_t> ans;
    for (const auto& d: task.second) {
        ans.push_back(s_to_digit.at(d));
    }

    size_t res = ans[0] * 1000 + ans[1] * 100 + ans[2] * 10 + ans[3];

    return res;
}

void part_2(const aoc::input_t& input) {
    uint32_t ans = 0;

    result_t result;

    for (const auto& line: input) {
        auto parts = aoc::split(line, ' ');
        task_t task;
        auto delim = std::find(parts.begin(), parts.end(), "|");
        for (auto it = parts.begin(); it != delim; ++it) {
            std::string cur = *it;
            std::sort(cur.begin(), cur.end());
            task.first.push_back(cur);
        }
        ++delim;
        for (auto it = delim; it != parts.end(); ++it) {
            std::string cur = *it;
            std::sort(cur.begin(), cur.end());
            task.second.push_back(cur);
        }
        result.push_back(task);
    }

    for (const auto& t: result) {
        ans += decode(t);
    }

    if (ans != 946346)
        std::cerr << "Wrong answer in Day 08 Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day08_1.txt");
    // const std::string day_input("./inputs/day08_test.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

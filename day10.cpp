#include "common.hpp"

#include <stack>


const std::unordered_map<char, char> brackets_map = {
    {'(',')'},
    {'[',']'},
    {'{','}'},
    {'<','>'},
};

enum class Status {
    Corrupted = 0,
    Incomplete,
};

typedef std::pair<Status, char> status_t;
typedef std::stack<char> stack_t;
typedef std::unordered_map<char, uint64_t> points_t;

status_t process_line(const std::string& line, stack_t& brackets) {

    for (const auto& c: line) {
        auto it = brackets_map.find(c);

        if (it != brackets_map.end()) {     // current bracket is an opening one
            brackets.push(c);
        } else {                            // current bracket is a closing one
            // lines are either Corrupted or Incomplete, so there should not be
            // cases when we found a closing bracket when stack is empty
            char close_for_top = brackets_map.at(brackets.top());
            brackets.pop();

            if (close_for_top != c) {       // expected bracket is not the one we found
                // std::cerr << line << ":\tExpected '" << close_for_top << "' but found '" << c << "' instead" << std::endl;
                return std::make_pair(Status::Corrupted, c);
            }
        }
    }

    // if line is not corrupted, then is is incomplete
    return std::make_pair(Status::Incomplete, '-');
}

void part_1(const aoc::input_t& input) {

    const points_t points = {
        {')', 3},
        {']', 57},
        {'}', 1197},
        {'>', 25137},
    };

    uint64_t ans = 0;
    for (const auto& line: input) {
        stack_t brackets;
        auto [status, c] = process_line(line, brackets);

        if (status == Status::Corrupted)
            ans += points.at(c);
    }

    if (ans != 469755)
        std::cerr << "Wrong answer in Day 10 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}


void part_2(const aoc::input_t& input) {

    const points_t points = {
        {')', 1},
        {']', 2},
        {'}', 3},
        {'>', 4},
    };

    aoc::u64v_t scores;
    scores.reserve(input.size());

    for (const auto& line: input) {
        uint64_t score = 0;

        stack_t brackets;
        auto [status, c] = process_line(line, brackets);
        (void) c;

        if (status == Status::Incomplete) {
            while (!brackets.empty()) {
                char close_for_top = brackets_map.at(brackets.top());
                brackets.pop();
                score = 5 * score + points.at(close_for_top);
            }
            scores.push_back(score);
        }
    }

    std::sort(scores.begin(), scores.end());
    uint64_t ans = scores[scores.size() / 2];

    if (ans != 2762335572)
        std::cerr << "Wrong answer in Day 10 Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day10_1.txt");
    // const std::string day_input("./inputs/day10_test.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

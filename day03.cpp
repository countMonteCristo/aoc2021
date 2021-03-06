#include "common.hpp"

#include <unordered_map>
#include <cassert>


typedef std::unordered_map<char, size_t> counter_t;


counter_t counter(const aoc::input_t& input, size_t idx) {
    counter_t result;
    for (const auto& line: input) {
        result[line[idx]]++;
    }
    return result;
}

size_t filter(const aoc::input_t& input, char (*func)(const counter_t& cnt)) {
    aoc::input_t current(input);
    size_t idx = 0;
    while (current.size() > 1) {
        const counter_t counts = counter(current, idx);
        char criteria = func(counts);
        aoc::input_t next;
        for (const auto& line: current) {
            if (line[idx] == criteria) {
                next.push_back(line);
            }
        }
        current = next;
        idx++;
    }

    assert(current.size() == 1);
    size_t result = std::stoul(current[0], nullptr, 2);

    return result;
}

void part_1(const aoc::input_t& input) {
    std::string gamma_str;
    std::string epsilon_str;

    for (size_t idx = 0; idx < input[0].size(); idx++) {
        const counter_t cnt = counter(input, idx);

        char g_char = '1';
        char e_char = '0';
        if (cnt.at('0') > cnt.at('1'))
            std::swap(g_char, e_char);

        gamma_str.push_back(g_char);
        epsilon_str.push_back(e_char);
    }

    size_t gamma = std::stoul(gamma_str, nullptr, 2);
    size_t epsilon = std::stoul(epsilon_str, nullptr, 2);

    int ans = gamma * epsilon;

    if (ans != 3813416)
        std::cerr << "Wrong answer in day 03, part 1" << std::endl;

    std::cout << "[Task 1]" << " gamma=" << gamma << " epsilon=" << epsilon << " answer=" << ans << std::endl;
}

void part_2(const aoc::input_t& input) {

    auto oxy_crit = [](const counter_t& cnt) { return (cnt.at('0') > cnt.at('1'))? '0': '1'; };
    auto co2_crit = [](const counter_t& cnt) { return (cnt.at('0') <= cnt.at('1'))? '0': '1'; };
    size_t oxy = filter(input, oxy_crit);
    size_t co2 = filter(input, co2_crit);

    int ans = oxy * co2;

    if (ans != 2990784)
        std::cerr << "Wrong answer in day 03, part 2" << std::endl;

    std::cout << "[Task 2]" << " oxy=" << oxy << " co2=" << co2 << " answer=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day03_1.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

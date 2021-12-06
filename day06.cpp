#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <numeric>


typedef std::vector<std::string> input_t;
typedef std::vector<uint64_t> row_t;

std::ostream& operator<<(std::ostream& out, const row_t& row) {
    for (size_t i = 0; i < row.size(); i++) {
        out << (size_t)row[i] << " ";
    }
    return out;
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

row_t split(const std::string& line, char delim) {
    row_t result;
    std::stringstream stream(line);
    std::string item;

    while (std::getline(stream, item, delim)) {
        result.push_back(std::stoull(item));
    }

    return result;
}

uint64_t sim(const row_t& initial, size_t N, uint8_t new_default, uint8_t old_default) {
    row_t counts;
    counts.resize(new_default + 1);
    for (auto n: initial) {
        counts[n]++;
    }

    for (size_t i=0; i<N; i++) {
        row_t next;
        next.resize(counts.size());
        for (size_t j=0; j<counts.size(); j++) {
            if ( j == 0 ) {
                next[new_default] += counts[j];
                next[old_default] += counts[j];
            } else {
                next[j-1] += counts[j];
            }

        }
        counts = next;
    }

    return std::accumulate(counts.begin(), counts.end(), (uint64_t)0);
}

void part_1(const input_t& input) {
    row_t initial = split(input[0], ',');
    size_t ans = sim(initial, 80, 8, 6);
    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}


void part_2(const input_t& input) {
    row_t initial = split(input[0], ',');
    size_t ans = sim(initial, 256, 8, 6);
    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day06_1.txt");
    // const std::string day_input("./inputs/day06_test.txt");

    auto input = load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

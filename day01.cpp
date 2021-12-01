#include <iostream>
#include <fstream>
#include <vector>

typedef int meas_t;
typedef std::vector<meas_t> measurements_t;


measurements_t load_ints_from(const std::string& filepath) {
    std::ifstream infile(filepath);
    if ( !infile.is_open() ) {
        std::cerr << "Cannot open input file: " << filepath << std::endl;
        std::exit(1);
    }
    std::string line;
    measurements_t result;
    while (std::getline(infile, line)) {
        result.push_back(std::stoi(line));
    }
    return result;
}

size_t get_increase_count(const measurements_t& depths, size_t window) {
    size_t count = 0;
    for (size_t i=window; i<depths.size(); i++) {
        if (depths[i] > depths[i-window]) {
            count++;
        }
    }
    return count;
}

void part_1(const measurements_t& depths) {
    size_t count = get_increase_count(depths, 1);
    std::cout << "[Part 1] Increases count: " << count << std::endl;
}

void part_2(const measurements_t& depths) {
    size_t count = get_increase_count(depths, 3);
    std::cout << "[Part 2] Increases count: " << count << std::endl;
}

int main() {
    const std::string day_input("./inputs/day01_1.txt");
    measurements_t depths = load_ints_from(day_input);

    part_1(depths);
    part_2(depths);

    return 0;
}

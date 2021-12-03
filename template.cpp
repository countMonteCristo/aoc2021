#include <iostream>
#include <fstream>
#include <vector>


typedef std::vector<std::string> input_t;


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


void part_1(const input_t& input) {

    std::cout << "[Task 1]" << std::endl;
}


void part_2(const input_t& input) {

    std::cout << "[Task 2]" << std::endl;
}

int main() {
    const std::string day_input("./inputs/day<%DAY>_1.txt");

    auto input = load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

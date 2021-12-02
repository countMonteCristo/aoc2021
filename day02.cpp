#include <iostream>
#include <fstream>
#include <vector>


typedef std::pair<std::string, int> inst_t;
typedef std::vector<inst_t> course_t;

course_t load_course_from(const std::string& filepath) {
    std::ifstream infile(filepath);
    if ( !infile.is_open() ) {
        std::cerr << "Cannot open input file: " << filepath << std::endl;
        std::exit(1);
    }

    course_t result;
    std::string line;
    while (std::getline(infile, line)) {
        size_t delim_idx = line.find(" ");
        std::string dir = line.substr(0, delim_idx);
        int delta = std::stoi(line.substr(delim_idx));

        result.push_back(std::make_pair(dir, delta));
    }
    return result;
}

void part_1(const course_t& course) {
    int hpos = 0;
    int depth = 0;

    for (const auto& [command, X]: course) {
        if (command == "forward") {
            hpos += X;
        }
        if (command == "up") {
            depth -= X;
        }
        if (command == "down") {
            depth += X;
        }
    }

    std::cout << "[Task 1] hpos = " << hpos << " depth = " << depth << " hpos*depth = " << hpos*depth << std::endl;
}

void part_2(const course_t& course) {
    int hpos = 0;
    int depth = 0;
    int aim = 0;

    for (const auto& [command, X]: course) {
        if (command == "forward") {
            hpos += X;
            depth += aim * X;
        }
        if (command == "up") {
            aim -= X;
        }
        if (command == "down") {
            aim += X;
        }
    }

    std::cout << "[Task 2] hpos = " << hpos << " depth = " << depth << " aim = " << aim << " hpos*depth = " << hpos*depth << std::endl;
}

int main() {
    const std::string day_input("./inputs/day02_1.txt");

    auto course = load_course_from(day_input);

    part_1(course);
    part_2(course);

    return 0;
}

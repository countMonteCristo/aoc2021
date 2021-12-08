#ifndef COMMON_HPP_
#define COMMON_HPP_

#include <vector>
#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>

namespace aoc {
    typedef std::vector<std::string> input_t;
    typedef std::vector<uint64_t> u64v_t;
    typedef std::vector<uint32_t> u32v_t;
    typedef std::vector<int64_t> i64v_t;
    typedef std::vector<int32_t> i32v_t;

    template <typename T>
    void minmax(T v1, T v2, T& out_min, T& out_max) {
        out_min = v1;
        out_max = v2;
        if (out_min > out_max)
            std::swap(out_min, out_max);
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

    bool contains(const std::string& s, char c) {
        return (s.find(c) != std::string::npos);
    }

    auto identical = [](const std::string& s) -> std::string { return s; };
    auto s2u64 = [](const std::string& s) -> uint64_t { return std::stoull(s); };
    auto s2u32 = [](const std::string& s) -> uint32_t { return std::stoul(s); };
    auto s2i32 = [](const std::string& s) -> int32_t { return std::stoi(s); };

    input_t split(const std::string& line, char delim, bool keep_empty=false) {
        input_t result;
        std::stringstream stream(line);
        std::string item;

        while (std::getline(stream, item, delim)) {
            if (!item.size() && !keep_empty)
                continue;

            result.push_back(item);
        }

        return result;
    }

    template <typename T>
    std::vector<T> convert(const input_t& input, T (*s2t)(const std::string&)) {
        std::vector<T> result;
        for (const auto& s: input) {
            result.push_back(s2t(s));
        }
        return result;
    }
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& row) {
    for (size_t i = 0; i < row.size(); i++) {
        out << row[i] << " ";
    }
    return out;
}

#endif // COMMON_HPP_

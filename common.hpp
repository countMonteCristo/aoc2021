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
    typedef std::vector<uint16_t> u16v_t;
    typedef std::vector<uint8_t> u8v_t;
    typedef std::vector<int64_t> i64v_t;
    typedef std::vector<int32_t> i32v_t;
    typedef std::vector<int16_t> i16v_t;
    typedef std::vector<int8_t> i8v_t;

    typedef std::vector<u64v_t> u64v2_t;
    typedef std::vector<u32v_t> u32v2_t;
    typedef std::vector<u16v_t> u16v2_t;
    typedef std::vector<u8v_t> u8v2_t;
    typedef std::vector<i64v_t> i64v2_t;
    typedef std::vector<i32v_t> i32v2_t;
    typedef std::vector<i16v_t> i16v2_t;

    typedef std::pair<int, int> coord_t;
    typedef std::vector<coord_t> coords_t;

    enum class Nbh {
        VonNeumann = 4,
        Moore = 8,
    };

    constexpr uint8_t MAX_DIGIT = 9;

    coords_t neighbours(const coord_t& point, int width, int height, Nbh ntype = Nbh::VonNeumann) {
        coords_t result;
        auto& [row, col] = point;
        coords_t all;
        switch (ntype) {
            case Nbh::VonNeumann: {
                all = {
                                    {row-1, col},
                    {row,   col-1},               {row,   col+1},
                                    {row+1, col},
                };
                break;
            }
            case Nbh::Moore: {
                all = {
                    {row-1, col-1}, {row-1, col}, {row-1, col+1},
                    {row,   col-1},               {row,   col+1},
                    {row+1, col-1}, {row+1, col}, {row+1, col+1},
                };
                break;
            }
            default:
                std::cerr << "Unhandled neighbour enum value in neightbours()" << std::endl;
                exit(1);
        }
        for (const auto& n: all) {
            auto& [nrow, ncol] = n;
            if ((ncol < 0) || (ncol == width) || (nrow < 0) || (nrow == height))
                continue;
            result.push_back(n);
        }
        return result;
    }

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
    auto s2u8v = [](const std::string& s) -> u8v_t {
        u8v_t result(s.size(), 0);
        for (size_t i = 0; i < s.size(); i++) result[i] = s[i] - '0';
        return result;
    };

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

    template <typename C>
    auto convert(const input_t& input, C s2t) {
        using T = std::decay_t<decltype(s2t(input[0]))>;

        std::vector<T> result;
        for (const auto& s: input) {
            result.push_back(s2t(s));
        }
        return result;
    }
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& row) {
    for (const auto& item: row) {
        out << item << " ";
    }
    return out;
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::pair<T,U>& pair) {
    out << pair.first << "," << pair.second;
    return out;
}

#endif // COMMON_HPP_

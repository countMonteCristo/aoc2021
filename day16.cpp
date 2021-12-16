#include "common.hpp"

#include <bitset>
#include <limits>

constexpr size_t WORD_BITS = 32;
constexpr size_t HEX_CHAR_BITS = 4;
constexpr size_t WORD_CHARS = WORD_BITS / HEX_CHAR_BITS;

constexpr bool DEBUG = false;


struct Packet {
    size_t bit_offset;
    size_t bit_size;
    size_t data_bit_offset;             // absolute value, so as bit_offset
    std::vector<uint32_t> digits;       // only for literals
    std::vector<Packet> sub_packets;    // only operational packets should have subpackets
    size_t version;
    size_t type;

    enum FieldSize {
        VERSION_ID = 3,
        TYPE_ID = 3,
        LEN_SIZED = 15,
        LEN_COUNTED = 11,
        LEN_ID = 1,
        DIGIT = 5,
    };

    enum LenIdBit {
        SIZED = 0,
        COUNTED = 1,
    };

    enum Types {
        LITERAL = 4,
        SUM = 0,
        PRODUCT = 1,
        MIN = 2,
        MAX = 3,
        GT = 5,
        LT = 6,
        EQ = 7,
    };
};

uint32_t c2dec(char c) {
    if (('0' <= c) && (c <= '9'))
        return c - '0';
    if (('A' <= c) && (c <= 'F'))
        return c - 'A' + 10;
    std::cerr << "Unreacheble!" << std::endl;
    exit(1);
}

uint32_t extract(const aoc::u32v_t& row, size_t& bit_pos, size_t bit_count) {
    auto d = std::div((int)bit_pos, (int)WORD_BITS);
    uint32_t word_pos = d.quot;
    uint32_t bit_offset = d.rem;
    uint32_t result;
    if (bit_offset + bit_count > WORD_BITS) {
        // affect two words
        size_t first_bit_mask_size = WORD_BITS - bit_offset;
        size_t second_bit_mask_size = bit_count - first_bit_mask_size;
        size_t first_bit_mask = (1 << first_bit_mask_size) - 1;
        size_t second_bit_mask = (1 << second_bit_mask_size) - 1;
        result = ((row[word_pos] & first_bit_mask) << second_bit_mask_size) | ((row[word_pos+1] >> (WORD_BITS - second_bit_mask_size)) & second_bit_mask);
    } else {
        // affect only one word
        size_t bit_mask_size = bit_count;
        size_t bit_mask = (1 << bit_count) - 1;
        result = (row[word_pos] >> (WORD_BITS - bit_offset - bit_mask_size)) & bit_mask;
    }
    bit_pos += bit_count;
    return result;
}

Packet extract_packet(const aoc::u32v_t& row, size_t& bit_pos) {
    Packet packet;
    packet.bit_offset = bit_pos;
    packet.version = extract(row, bit_pos, Packet::VERSION_ID);
    packet.type = extract(row, bit_pos, Packet::TYPE_ID);

    if (packet.type == Packet::LITERAL) {
        size_t packet_bits = Packet::VERSION_ID + Packet::TYPE_ID;
        packet.data_bit_offset = bit_pos;
        while (true) {
            uint32_t group = extract(row, bit_pos, Packet::DIGIT);
            packet.digits.push_back(group & ((1 << (Packet::DIGIT - 1)) - 1));
            packet_bits += Packet::DIGIT;
            if ((group & (1 << (Packet::DIGIT - 1))) == 0) {    // last group should be of form 0XXXX
                break;
            }
        }
        packet.bit_size = bit_pos - packet.bit_offset;
    } else {
        uint32_t length_id = extract(row, bit_pos, Packet::LEN_ID);
        switch (length_id) {
            case Packet::SIZED: {
                uint32_t length = extract(row, bit_pos, Packet::LEN_SIZED);
                packet.data_bit_offset = bit_pos;
                packet.bit_size = bit_pos - packet.bit_offset + length;

                uint32_t subpackets_size = 0;
                while (subpackets_size < length) {
                    Packet p = extract_packet(row, bit_pos);
                    subpackets_size += p.bit_size;
                    packet.sub_packets.push_back(p);
                }
                break;
            }
            case Packet::COUNTED: {
                uint32_t sub_packets_count = extract(row, bit_pos, Packet::LEN_COUNTED);
                packet.bit_size = bit_pos - packet.bit_offset;
                packet.data_bit_offset = bit_pos;
                for (uint32_t i = 0; i < sub_packets_count; i++) {
                    Packet p = extract_packet(row, bit_pos);
                    packet.bit_size += p.bit_size;
                    packet.sub_packets.push_back(p);
                }
                break;
            }
            default: {
                std::cerr << "Unhandled length_id value: " << length_id << std::endl;
                exit(1);
            }
        }
    }
    return packet;
}

std::ostream& operator<<(std::ostream& out, const Packet& p) {
    out << "(off=" << p.bit_offset << " sz=" << p.bit_size << " v=" << p.version << " t=" << p.type << " subs_n=" << p.sub_packets.size() << ")";
    return out;
}

std::string indent(size_t level) {
    return std::string(level, ' ');
}

uint32_t sum_versions(const Packet& packet) {
    uint32_t result = packet.version;
    for (const auto& p: packet.sub_packets) {
        result += sum_versions(p);
    }
    return result;
}

void log(std::ostream& out, bool debug, size_t level, const char* msg, const uint64_t *val_ptr = nullptr) {
    if (debug) {
        out << indent(level) << msg;
        if (val_ptr != nullptr)
            out << *val_ptr;
        out << std::endl;
    }
}

uint64_t eval(const Packet& p, size_t level=0) {
    switch (p.type) {
        case Packet::LITERAL: {
            uint64_t value = 0;
            log(std::cout, DEBUG, level, "Get Literal...");
            for (size_t i = 0; i < p.digits.size(); i++) {
                value = (value << HEX_CHAR_BITS) | (uint64_t)p.digits[i];
            }
            log(std::cout, DEBUG, level, "Literal: ", &value);
            return value;
        }
        case Packet::SUM: {
            uint64_t value = 0;
            log(std::cout, DEBUG, level, "Calc Sum...");
            for (const auto& sub: p.sub_packets) {
                value += eval(sub, level+1);
            }
            log(std::cout, DEBUG, level, "Sum: ", &value);
            return value;
        }
        case Packet::PRODUCT: {
            uint64_t value = 1;
            log(std::cout, DEBUG, level, "Calc Product...");
            for (const auto& sub: p.sub_packets) {
                value *= eval(sub, level+1);
            }
            log(std::cout, DEBUG, level, "Product: ", &value);
            return value;
        }
        case Packet::MIN: {
            log(std::cout, DEBUG, level, "Calc Min...");
            uint64_t value = std::numeric_limits<uint64_t>::max();
            for (const auto& sub: p.sub_packets) {
                value = std::min(value, eval(sub, level+1));
            }
            log(std::cout, DEBUG, level, "Min: ", &value);
            return value;
        }
        case Packet::MAX: {
            log(std::cout, DEBUG, level, "Calc Max...");
            uint64_t value = 0;
            for (const auto& sub: p.sub_packets) {
                value = std::max(value, eval(sub, level+1));
            }
            log(std::cout, DEBUG, level, "Max: ", &value);
            return value;
        }
        case Packet::GT: {
            if (p.sub_packets.size() != 2) {
                std::cerr << "Wrong number of subpackets for greater_than packet!" << std::endl;
                exit(1);
            }
            log(std::cout, DEBUG, level, "Calc GT...");
            uint64_t first = eval(p.sub_packets[0], level+1);
            uint64_t second = eval(p.sub_packets[1], level+1);
            uint64_t value = (first > second)? 1: 0;
            log(std::cout, DEBUG, level, "GT: ", &value);
            return value;
        }
        case Packet::LT: {
            if (p.sub_packets.size() != 2) {
                std::cerr << "Wrong number of subpackets for less_than packet!" << std::endl;
                exit(1);
            }
            log(std::cout, DEBUG, level, "Calc LT...");
            uint64_t first = eval(p.sub_packets[0], level+1);
            uint64_t second = eval(p.sub_packets[1], level+1);
            uint64_t value = (first < second)? 1: 0;
            log(std::cout, DEBUG, level, "LT: ", &value);
            return value;
        }
        case Packet::EQ: {
            if (p.sub_packets.size() != 2) {
                std::cerr << "Wrong number of subpackets for equal_to packet!" << std::endl;
                exit(1);
            }
            log(std::cout, DEBUG, level, "Calc EQ...");
            uint64_t first = eval(p.sub_packets[0], level+1);
            uint64_t second = eval(p.sub_packets[1], level+1);
            uint64_t value = (first == second)? 1: 0;
            log(std::cout, DEBUG, level, "EQ: ", &value);
            return value;
        }
        default: {
            std::cerr << "Unknown type id in packet " << p << std::endl;
            exit(1);
        }
    }
}

void part_1(const aoc::input_t& input) {

    auto dm = std::div((int)input[0].size(), WORD_CHARS);
    uint32_t row_size = dm.quot + static_cast<uint32_t>(dm.rem> 0);

    aoc::u32v_t row(row_size, 0);
    for (size_t i = 0, t = 0; i < input[0].size(); i++, t = (t + HEX_CHAR_BITS) % WORD_BITS) {
        uint32_t value = c2dec(input[0][i]);
        size_t shift = (WORD_BITS - HEX_CHAR_BITS) - t;
        size_t idx = i / WORD_CHARS;
        row[idx] |= (value << shift);
    }

    size_t bit_pos = 0;
    Packet main = extract_packet(row, bit_pos);
    uint32_t ans = sum_versions(main);

    if (ans != 974)
        std::cerr << "Wrong answer in Day 16 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}

void part_2(const aoc::input_t& input) {

    auto dm = std::div((int)input[0].size(), WORD_CHARS);
    uint32_t row_size = dm.quot + static_cast<uint32_t>(dm.rem> 0);

    aoc::u32v_t row(row_size, 0);
    for (size_t i = 0, t = 0; i < input[0].size(); i++, t = (t+4) % WORD_BITS) {
        uint32_t value = c2dec(input[0][i]);
        size_t shift = (WORD_BITS - HEX_CHAR_BITS) - t;
        size_t idx = i / WORD_CHARS;
        row[idx] |= (value << shift);
    }

    size_t bit_pos = 0;
    Packet main = extract_packet(row, bit_pos);
    uint64_t ans = eval(main);

    if (ans != 180616437720)
        std::cerr << "Wrong answer in Day 16 Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day16_1.txt");
    // const std::string day_input("./inputs/day16_t1.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

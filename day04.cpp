#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <numeric>


#define BINGO_SIZE 5

typedef std::vector<std::string> input_t;
typedef std::vector<size_t> row_t;
typedef std::vector<row_t> table_t;


std::ostream& operator<<(std::ostream& out, const row_t& row) {
    for (size_t i = 0; i < row.size(); i++) {
        out << row[i] << " ";
    }
    return out;
}

struct Board {
    void prepare() {
        is_over = false;

        marked.resize(table.size());
        for (size_t r=0; r<marked.size(); r++) {
            marked[r].resize(BINGO_SIZE);
            for (size_t c=0; c<marked[r].size(); c++) {
                marked[r][c] = false;
            }
        }
    }

    table_t table;
    std::vector<std::vector<bool>> marked;
    bool is_over;

    void consume(size_t num) {
        for (size_t r=0; r<table.size(); r++) {
            for (size_t c=0; c<table[r].size(); c++) {
                if (table[r][c] == num) {
                    marked[r][c] = true;
                }
            }
        }
    }

    size_t win_sum() {
        size_t sum = 0;
        for (size_t r = 0; r < table.size(); r++) {
            for (size_t c = 0; c < table[r].size(); c++) {
                if ( !marked[r][c] ) {
                    sum += table[r][c];
                }
            }
        }
        return sum;
    }

    bool wins() {
        bool is_win = false;

        //  check rows
        for (size_t r = 0; r < marked.size(); r++) {
            size_t row_sum = std::accumulate(marked[r].begin(), marked[r].end(), 0);
            if ( row_sum == marked[r].size() ) {
                is_win = true;
                break;
            }
        }

        // check columns
        for (size_t c = 0; c < marked[0].size(); c++) {
            size_t col_sum = 0;
            for (size_t r = 0; r < marked.size(); r++) {
                col_sum += static_cast<size_t>(marked[r][c]);
            }
            if ( col_sum == marked.size() ) {
                is_win = true;
                break;
            }
        }

        if (is_win) {
            is_over = true;
        }

        return is_win;
    }
};

typedef std::pair<row_t, std::vector<Board>> game_t;
typedef std::pair<Board, size_t> winner_t;

std::ostream& operator<<(std::ostream& out, const Board& b) {
    for (size_t i = 0; i < b.table.size(); i++) {
        out << b.table[i] << std::endl;
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
        if (!item.size())
            continue;
        result.push_back(std::stoul(item));
    }

    return result;
}


game_t init_bingo(const input_t& input) {
    row_t nums = split(input[0], ',');

    std::vector<Board> boards;
    Board b;
    for (size_t i=1; i<input.size(); i++) {
        if (input[i].size() == 0)
            continue;

        row_t row = split(input[i], ' ');
        if ( b.table.size() == BINGO_SIZE ) {
            boards.push_back(b);
            b = Board();
        }
        b.table.push_back(row);
    }
    boards.push_back(b);
    return std::make_pair(nums, boards);
}

winner_t play_1(game_t& game) {
    for (size_t number: game.first) {
        for ( auto& b: game.second ) {
            b.consume(number);
            if (b.wins()) {
                return std::make_pair(b, number);
            }
        }
    }
    return std::make_pair(Board(), 9999999);
}

winner_t play_2(game_t& game) {
    size_t count_wins = 0;
    for (size_t number: game.first) {
        for ( auto& b: game.second ) {
            if (b.is_over)
                continue;
            b.consume(number);
            if (b.wins()) {
                count_wins++;
                if (count_wins == game.second.size()) {
                    return std::make_pair(b, number);
                }
            }
        }
    }
    return std::make_pair(Board(), 9999999);
}


void part_1(const input_t& input) {
    auto game = init_bingo(input);

    auto winner = play_1(game);
    size_t win_sum = winner.first.win_sum();

    std::cout << "[Task 1]" << " win_sum=" << win_sum << " last_num="<< winner.second << " ans=" << win_sum*winner.second << std::endl;
}


void part_2(const input_t& input) {
    auto game = init_bingo(input);

    auto winner = play_2(game);
    size_t win_sum = winner.first.win_sum();

    std::cout << "[Task 2]" << " win_sum=" << win_sum << " last_num="<< winner.second << " ans=" << win_sum*winner.second << std::endl;
}

int main() {
    const std::string day_input("./inputs/day04_1.txt");
    // const std::string day_input("./inputs/day04_test.txt");

    auto input = load_input_from(day_input);
    auto game = init_bingo(input);

    part_1(input);
    part_2(input);

    return 0;
}

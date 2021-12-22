#include "common.hpp"

typedef uint64_t count_t;

constexpr count_t DICE_MIN = 1;

constexpr count_t P1_WIN_SCORE = 1000;
constexpr count_t P1_DICE_N_MAX = 100;
constexpr count_t DICE_ROLLS = 3;

constexpr count_t MAX_POS = 10;
constexpr count_t P2_WIN_SCORE = 21;
constexpr count_t P2_SCORE_MAX = P2_WIN_SCORE + MAX_POS;
constexpr count_t P2_DICE_N_MAX = 3;

typedef std::pair<count_t, count_t> player_t;
typedef std::vector<count_t> counts_t;

count_t mod(count_t x, count_t m) {
    return ((x - 1) % m) + 1;
}

count_t roll(count_t dice) {
    return mod(DICE_ROLLS*dice  + DICE_ROLLS*(DICE_ROLLS - 1)/2, P1_DICE_N_MAX);
}

void part_1(const aoc::input_t& input) {

    player_t p[2];

    p[0].first = aoc::s2u32(aoc::split(input[0], ' ')[4]);
    p[1].first = aoc::s2u64(aoc::split(input[1], ' ')[4]);

    count_t dice = DICE_MIN;
    size_t player_id = 0;
    while ((p[0].second < P1_WIN_SCORE) && (p[1].second < P1_WIN_SCORE)) {
        p[player_id].first = mod(p[player_id].first + roll(dice), MAX_POS);
        p[player_id].second += p[player_id].first;

        dice += DICE_ROLLS;
        player_id = (player_id + 1) % 2;
    }

    count_t ans = (dice - DICE_MIN) * std::min(p[0].second, p[1].second);

    if (ans != 711480)
        std::cerr << "Wrong answer in Day 21 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}



count_t pos2row(const counts_t& pos) {
    return (pos[0] - 1) * MAX_POS + (pos[1] - 1);
}
count_t score2col(const counts_t& score) {
    return score[0] * P2_SCORE_MAX + score[1];
}
void row2pos(count_t row, counts_t& pos) {
    pos[0] = row / MAX_POS + 1;
    pos[1] = (row % MAX_POS) + 1;
}
void col2score(count_t col, counts_t& score) {
    score[0] = col / P2_SCORE_MAX;
    score[1] = col % P2_SCORE_MAX;
}

size_t win_id(const counts_t& score) {
    if ((score[0] >= P2_WIN_SCORE) && (score[1] < P2_WIN_SCORE))
        return 0;
    if ((score[0] < P2_WIN_SCORE) && (score[1] >= P2_WIN_SCORE))
        return 1;
    std::cout << "Unknown winner for state "<< score << "! WTF?" << std::endl;
    exit(1);
}

void show(const std::vector<counts_t>& un) {
    std::cout << "Universe state:" << std::endl;
    counts_t pos = {0, 0};
    counts_t score = {0, 0};
    for (size_t r = 0; r < un.size(); r++) {
        for(size_t c = 0; c < un[r].size(); c++) {
            if (un[r][c] > 0) {
                row2pos(r, pos);
                col2score(c, score);
                std::cout << "  Pos: " << pos << " Score: " << score << " #un=" << un[r][c] << std::endl;
            }
        }
    }
}

void part_2(const aoc::input_t& input) {

    counts_t pos = {
        aoc::s2u32(aoc::split(input[0], ' ')[4]),
        aoc::s2u64(aoc::split(input[1], ' ')[4])
    };
    counts_t score = { 0, 0 };

    std::vector<counts_t> un(
        MAX_POS * MAX_POS,
        counts_t(P2_SCORE_MAX * P2_SCORE_MAX, 0)
    );
    un[pos2row(pos)][0] = 1;

    size_t pid = 0;
    while (true) {
        // 1st player: pos = (r / 10) + 1, score = c / 31
        // 2nd player: pos = (r % 10) + 1, score = c % 31
        std::vector<counts_t> nxt(
            MAX_POS * MAX_POS,
            counts_t(P2_SCORE_MAX * P2_SCORE_MAX, 0)
        );

        count_t processed = 0;
        for (count_t r = 0; r < nxt.size(); r++) {
            for (count_t c = 0; c < nxt[r].size(); c++) {
                if (un[r][c] == 0)
                    continue;

                col2score(c, score);
                if ((score[0] >= P2_WIN_SCORE) || (score[1] >= P2_WIN_SCORE)) {
                    nxt[r][c] += un[r][c];
                    continue;
                }

                processed++;
                for (size_t d1 = DICE_MIN; d1 <= P2_DICE_N_MAX; d1++){
                    for (size_t d2 = DICE_MIN; d2 <= P2_DICE_N_MAX; d2++){
                        for (size_t d3 = DICE_MIN; d3 <= P2_DICE_N_MAX; d3++){
                            size_t sum = d1 + d2 + d3;

                            row2pos(r, pos);
                            col2score(c, score);

                            pos[pid] = mod(pos[pid] + sum, MAX_POS);
                            score[pid] += pos[pid];

                            count_t new_r = pos2row(pos);
                            count_t new_c = score2col(score);

                            nxt[new_r][new_c] += un[r][c];
                        }
                    }
                }
            }
        }

        if (processed == 0)
            break;

        un = nxt;
        pid = (pid + 1) % 2;
    }

    counts_t results = {0, 0};
    for (count_t r = 0; r < un.size(); r++) {
        for (count_t c = 0; c < un[r].size(); c++) {
            if (un[r][c] == 0)
                continue;
            col2score(c, score);
            results[win_id(score)] += un[r][c];
        }
    }

    count_t ans = std::max(results[0], results[1]);

    if (ans != 265845890886828)
        std::cerr << "Wrong answer in Day P2_WIN_SCORE Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day21_1.txt");
    // const std::string day_input("./inputs/day21_test.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

#include "common.hpp"

#include <string>


constexpr size_t MIN_EXPLODE_LEVEL = 4;

class Node {
    enum ChildType{
        Left = 0,
        Right = 1,
    };

    Node* common_parent(Node::ChildType type) {
        Node* cur = this;
        while (cur) {
            if (cur->parent) {
                Node* p = cur->parent;
                if (p->child(type) == cur) {
                    cur = p;
                    break;
                }
            }
            cur = cur->parent;
        }
        return cur;
    }

    void update_closest(Node::ChildType type, uint64_t value) {
        Node* cur = another(type);
        while (!cur->is_regular()) {
            cur = cur->child(type);
        }
        cur->val += value;
    }

    void explode_closest(Node::ChildType other_closest_type) {
        Node* cur = common_parent(other_closest_type);
        if (cur) {
            cur->update_closest(other_closest_type, another(other_closest_type)->val);
        }
    }

    Node* child(Node::ChildType type) {
        switch (type) {
            case Node::Left: return left;
            case Node::Right: return right;
            default:
                std::cerr << "Unreadcheble" << std::endl;
                exit(1);
        }
    }

    Node* another(Node::ChildType type) {
        switch (type) {
            case Node::Left: return right;
            case Node::Right: return left;
            default:
                std::cerr << "Unreadcheble" << std::endl;
                exit(1);
        }
    }

    void clear() {
        delete left;
        delete right;
        left = nullptr;
        right = nullptr;
        val = 0;
    }

public:
    Node(uint64_t value = 0, Node* par = nullptr, Node* l=nullptr, Node* r=nullptr): left(l), right(r), parent(par), val(value) {
        if (left)
            left->parent = this;
        if (right)
            right->parent = this;
    }
    Node(const std::string& line) {
        if (!line.size())
            return;
        Node* current = this;

        std::string val_str;
        for (char c: line) {
            switch (c) {
                case '[': {
                    current->left = new Node(0, current);
                    current = current->left;
                    break;
                }
                case ']': {
                    if (val_str.size()) {
                        current->val = aoc::s2u64(val_str);
                        val_str.clear();
                    }
                    current = current->parent;
                    break;
                }
                case ',': {
                    if (val_str.size()) {
                        current->val = aoc::s2u64(val_str);
                        val_str.clear();
                    }
                    current->parent->right = new Node(0, current->parent);
                    current = current->parent->right;
                    break;
                }
                default: {  // 0-9
                    val_str.push_back(c);
                    break;
                }
            }
        }
    }
    ~Node() {
        if (left)
            delete left;
        if (right)
            delete right;
        left = nullptr;
        right = nullptr;
    }

    bool is_regular() const {
        return (!left && !right);
    }

    bool is_pair() const {
        return !is_regular() && left->is_regular() && right->is_regular();
    }

    static Node* combine(Node* left, Node* right) {
        return new Node(0, nullptr, left, right);
    }

    void explode(size_t level, bool& exploded) {
        if (exploded || is_regular()) {
            return;
        }

        if ((level < MIN_EXPLODE_LEVEL) || !is_pair()) {
            left->explode(level+1, exploded);
            if (!exploded) {
                right->explode(level+1, exploded);
            }
            if (exploded)
                return;
        }

        if (is_pair() && (level >= MIN_EXPLODE_LEVEL)) {
            // find left closest regular
            explode_closest(Node::Right);

            // find right closest regular
            explode_closest(Node::Left);

            clear();
        }
    }

    void split(bool& splitted) {
        if (splitted) {
            return;
        }

        if (left && right) {
            left->split(splitted);
            if (!splitted) {
                right->split(splitted);
            }
            if (splitted) {
                return;
            }
        }

        if (is_regular() && (val >= 10)) {
            auto q = std::div(val, 2);
            left = new Node(q.quot, this);
            right = new Node(q.quot + q.rem, this);
            val = 0;
            splitted = true;
        }
    }

    bool reduce() {
        bool exploded = false;
        bool splitted = false;

        // 1. try explode
        explode(0, exploded);

        // 2. try split
        if (!exploded) {
            split(splitted);
            return splitted;
        }
        return true;
    }

    uint64_t magnitude() const {
        if (is_regular()) {
            return val;
        }
        return 3 * left->magnitude() + 2 * right->magnitude();
    }

    std::string to_string() {
        if (is_regular())
            return std::to_string(val);
        return "[" + left->to_string() + "," + right->to_string() + "]";
    }

    Node* left;
    Node* right;
    Node* parent;
    uint64_t val;
};

Node* add(Node* a, Node* b) {
    Node* temp = Node::combine(a, b);
    while (temp->reduce()) {}
    return temp;
}


void part_1(const aoc::input_t& input) {
    std::vector<Node *> roots(input.size(), nullptr);
    for (size_t i = 0; i < input.size(); ++i) {
        roots[i] = new Node(input[i]);
    }

    Node* result = roots[0];
    for (size_t i = 1; i < input.size(); ++i) {
        result = add(result, roots[i]);
    }
    uint64_t ans =result->magnitude();

    if (ans != 4132)
        std::cerr << "Wrong answer in Day 18 Part 1" << std::endl;

    std::cout << "[Task 1]" << " ans=" << ans << std::endl;
}


void part_2(const aoc::input_t& input) {

    uint64_t ans = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        for (size_t j = i; j < input.size(); ++j) {
            Node *first, *second, *result;
            first = new Node(input[i]);
            second = new Node(input[j]);
            result = add(first, second);
            ans = std::max(ans, result->magnitude());

            delete first;
            delete second;

            first = new Node(input[i]);
            second = new Node(input[j]);
            result = add(second, first);
            ans = std::max(ans, result->magnitude());
        }
    }

    if (ans != 4685)
        std::cerr << "Wrong answer in Day 18 Part 2" << std::endl;

    std::cout << "[Task 2]" << " ans=" << ans << std::endl;
}

int main() {
    const std::string day_input("./inputs/day18_1.txt");
    // const std::string day_input("./inputs/day18_test.txt");

    auto input = aoc::load_input_from(day_input);

    part_1(input);
    part_2(input);

    return 0;
}

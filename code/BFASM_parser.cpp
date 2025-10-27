#include "BFASM_core.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <algorithm>
#include <cstring>

template <typename T>
using vect = std::vector<T>;

std::string trim(const std::string& str) {
    auto first = std::find_if_not(str.begin(), str.end(), [](unsigned char ch) { return std::isspace(ch); });
    if (first == str.end()) return "";
    auto last = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char ch) { return std::isspace(ch); }).base();
    return std::string(first, last);
}

bool is_number(str s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

void size_check(int lsize, int size, int line) {
    if (lsize != size) {
        std::cerr << "Too many or too little tokens on line " << line << "\n";
        std::exit(1);
    }
}

void var_check(str var, int line) {
    if (var[0] != '%') {
        std::cerr << "Variables must start with '%' (found '" << var << "' on line " << line << ")\n";
        std::exit(1);
    }
    if (var == "%_") {
        std::cerr << "'%_' is a reserved name (line " << line << ")\n";
        std::exit(1);
    }
}

void num_check(str string, int line) {
    if (!is_number(string)) {
        std::cerr << "Expected integer, got '" << string << "' on line " << line << "\n";
        std::exit(1);
    }
}

void parse_line(str &line, int n) {
    auto cmt_idx = std::find(line.begin(), line.end(), '!');
    if (cmt_idx != line.end()) {
        line = line.substr(0, cmt_idx - line.begin());
    }
    vect<str> tokens;
    line = trim(line);
    std::istringstream ss(line);
    str token;
    while (std::getline(ss, token, ' ')) {
        tokens.push_back(token);
    }
    if (!tokens.empty()) {
        if (tokens[0] == "set") {
            size_check(tokens.size(), 3, n);
            var_check(tokens[1], n);
            num_check(tokens[2], n);
            if (variables.size() > 10000) {
                std::cerr << "Too many variables (max 10,000, line " << n << ")\n";
                std::exit(1);
            }
            int val = std::stoi(tokens[2]);
            if (val > 255 || val < 0) {
                std::cerr << "Value given to 'set' command must be between 0 and 255 (found " << tokens[2] << ", line " << n << ")\n";
                std::exit(1);
            }
            set(tokens[1].substr(1), val);
        } else if (tokens[0] == "add") {
            size_check(tokens.size(), 4, n);
            var_check(tokens[1], n);
            var_check(tokens[2], n);
            var_check(tokens[3], n);
            add(tokens[1].substr(1), tokens[2].substr(1), tokens[3].substr(1));
        } else if (tokens[0] == "sub") {
            size_check(tokens.size(), 4, n);
            var_check(tokens[1], n);
            var_check(tokens[2], n);
            var_check(tokens[3], n);
            sub(tokens[1].substr(1), tokens[2].substr(1), tokens[3].substr(1));
        } else if (tokens[0] == "mul") {
            size_check(tokens.size(), 4, n);
            var_check(tokens[1], n);
            var_check(tokens[2], n);
            var_check(tokens[3], n);
            mul(tokens[1].substr(1), tokens[2].substr(1), tokens[3].substr(1));
        } else if (tokens[0] == "div") {
            size_check(tokens.size(), 4, n);
            var_check(tokens[1], n);
            var_check(tokens[2], n);
            var_check(tokens[3], n);
            div(tokens[1].substr(1), tokens[2].substr(1), tokens[3].substr(1));
        } else if (tokens[0] == "mod") {
            size_check(tokens.size(), 4, n);
            var_check(tokens[1], n);
            var_check(tokens[2], n);
            var_check(tokens[3], n);
            mod(tokens[1].substr(1), tokens[2].substr(1), tokens[3].substr(1));
        } else if (tokens[0] == "out") {
            size_check(tokens.size(), 2, n);
            var_check(tokens[1], n);
            out(tokens[1].substr(1));
        } else if (tokens[0] == "inp") {
            size_check(tokens.size(), 2, n);
            var_check(tokens[1], n);
            inp(tokens[1].substr(1));
        } else if (tokens[0] == "cmp") {
            size_check(tokens.size(), 6, n);
            var_check(tokens[1], n);
            var_check(tokens[2], n);
            var_check(tokens[3], n);
            var_check(tokens[4], n);
            var_check(tokens[5], n);
            cmp(tokens[1].substr(1), tokens[2].substr(1), tokens[3].substr(1), tokens[4].substr(1), tokens[5].substr(1));
        } else if (tokens[0] == "if") {
            size_check(tokens.size(), 2, n);
            var_check(tokens[1], n);
            bif(tokens[1].substr(1), n);
        } else if (tokens[0] == "while") {
            size_check(tokens.size(), 2, n);
            var_check(tokens[1], n);
            bwhile(tokens[1].substr(1), n);
        } else if (tokens[0] == "end") {
            size_check(tokens.size(), 1, n);
            if (callstack.empty()) {
                std::cerr << "Found 'end' statement with no matching 'if'/'while' statement (line " << n << ")\n";
                std::exit(1);
            }
            if (callstack.top().type == BlockType::IF) {
                end_if();
            } else {
                end_while();
            }
        } else {
            std::cerr << "Unknown statement '" << tokens[0] << "' on line " << n << "\n";
            std::exit(1);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Too many or too little arguments\n";
        std::exit(1);
    }

    int number = 0;
    std::ifstream file(argv[1]);

    if (!file.is_open()) {
        std::cerr << "Failed to open file " << argv[1] << "\n";
        std::exit(1);
    }

    set("_", 0);

    str line;
    while (std::getline(file, line)) {
        number++;
        parse_line(line, number);
    }

    file.close();

    if (!callstack.empty()) {
        std::cerr << "Found 'if'/'while' statement with no matching 'end' statement (line " << callstack.top().line << ")\n";
        std::exit(1);
    }

    str filename = argv[1] + (str)".bf";
    std::ofstream f(filename);
    f << brainfuck;

    std::cout << "Program compiled successfully.\n";

    return 0;
}

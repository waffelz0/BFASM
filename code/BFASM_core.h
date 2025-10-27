#pragma once
#include <string>
#include <unordered_map>
#include <stack>

typedef std::string str;

extern std::unordered_map<std::string, std::pair<int, unsigned char>> variables;
extern int var_count;
extern str brainfuck;
extern int ptr;
enum class BlockType { IF, WHILE };
struct CallstackElement {
    int ptr_pos;
    BlockType type;
    str var_name;
    int line;
};
extern std::stack<CallstackElement> callstack;

void mov(int target);
void set(str name, unsigned char val);
void out(str name);
void inp(str name);
void add(str a, str b, str c);
void sub(str a, str b, str c);
void mul(str a, str b, str c);
void div(str a, str b, str c);
void mod(str a, str b, str c);
void bif(str a, int n);
void end_if();
void bwhile(str a, int n);
void end_while();
void cmp(str a, str b, str c, str d, str e);

#pragma once
#include <string>
#include <unordered_map>

typedef std::string str;

extern std::unordered_map<std::string, std::pair<int, unsigned char>> variables;
extern int var_count;
extern str brainfuck;
extern int ptr;

void mov(int target);
void set(str name, unsigned char val);
void out(str name);
void inp(str name);
void add(str a, str b, str c);
void sub(str a, str b, str c);
void mul(str a, str b, str c);
void div(str a, str b, str c);
void begin_if(str a);
void end_if();
void begin_while(str a);
void end_while(str a);
void cmp(str a, str b, str c, str d, str e);

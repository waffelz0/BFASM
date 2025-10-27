#include "BFASM_core.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <utility>
#include <algorithm>
#include <iterator>
#include <stack>

typedef std::string str;

std::unordered_map<std::string, std::pair<int, unsigned char>> variables;
int var_count = 0;
str brainfuck = "";
int ptr = 0;

std::stack<CallstackElement> callstack;

// smart pointer movement
void mov(int target) {
    int offset = target - ptr;
    // add >'s or <'s depending on if it needs to go forwards or backwards
    if (offset > 0) {
        for (int i = 0; i < offset; i++) {
            brainfuck+= ">";
            ptr++;
        }
    } else {
        for (int i = 0; i < -offset; i++) {
            brainfuck+= "<";
            ptr--;
        }
    }
}

void set(str name, unsigned char val) {
    if (variables.find(name) == variables.end()) {
        // the variable does not exist
        variables[name] = std::make_pair(var_count, val);
        if (val != 0) {
            mov(var_count);
            for (int i = 0; i < val; i++) {
                brainfuck += "+";
            }
        }
        var_count += 1;
    } else {
        // the variable exists
        variables[name] = std::make_pair(variables[name].first, val);
        mov(variables[name].first);
        brainfuck+= "[-]";
        for (int i = 0; i < val; i++) {
            brainfuck += "+";
        }
    }
}

void out(str name) {
    mov(variables[name].first);
    brainfuck += ".";
}

void inp(str name) {
    mov(variables[name].first);
    brainfuck += ",";
}

void add(str a, str b, str c) {
    // update the internal representation of %c
    variables[c].second = variables[a].second + variables[b].second;
    // variables for uhhhhhhhhh 
    int pa = variables[a].first;
    int pb = variables[b].first;
    int pc = variables[c].first;
    // move to %a
    mov(pa);
    // clone to free space
    brainfuck+= "[-";
    mov(var_count);
    brainfuck+= "+>+";
    mov(pa);
    brainfuck+= "<]";
    // move it back to %a
    mov(var_count+1);
    brainfuck+= "[-";
    mov(pa);
    brainfuck+= "+";
    mov(var_count+1);
    brainfuck+= "]";
    // move to %b
    mov(pb);
    // clone to free space
    brainfuck+= "[-";
    mov(var_count+1);
    brainfuck+= "+>+";
    mov(pb);
    brainfuck+= "<]";
    // move it back to %b
    mov(var_count+2);
    brainfuck+= "[-";
    mov(pb);
    brainfuck+= "+";
    mov(var_count + 2);
    brainfuck+= "]";
    // move to the clone of %b
    mov(var_count+1);
    // perform the addition
    brainfuck+= "[-<+>]";
    // clear %c
    mov(pc);
    brainfuck += "[-]";
    // move the sum to %c
    mov(var_count);
    brainfuck+= "[-";
    mov(pc);
    brainfuck+= "+";
    mov(var_count);
    brainfuck+= "]";
}

void sub(str a, str b, str c) {
    // update the internal representation of %c
    variables[c].second = variables[a].second - variables[b].second;
    // variables for uhhhhhhhhh 
    int pa = variables[a].first;
    int pb = variables[b].first;
    int pc = variables[c].first;
    // move to %a
    mov(pa);
    // clone to free space
    brainfuck+= "[-";
    mov(var_count);
    brainfuck+= "+>+";
    mov(pa);
    brainfuck+= "<]";
    // move it back to %a
    mov(var_count + 1);
    brainfuck+= "[-";
    mov(pa);
    brainfuck+= "+";
    mov(var_count + 1);
    brainfuck+= "]";
    // move to %b
    mov(pb);
    // clone to free space
    brainfuck+= "[-";
    mov(var_count + 1);
    brainfuck+= "+>+";
    mov(pb);
    brainfuck+= "<]";
    // move it back to %b
    mov(var_count + 2);
    brainfuck+= "[-";
    mov(pb);
    brainfuck+= "+";
    mov(var_count + 2);
    brainfuck+= "]";
    // move to the clone of %b
    mov(var_count + 1);
    // perform the subtraction
    brainfuck+= "[-<->]";
    // clear %c
    mov(pc);
    brainfuck += "[-]";
    // move the difference to %c
    mov(var_count);
    brainfuck+= "[-";
    mov(pc);
    brainfuck+= "+";
    mov(var_count);
    brainfuck+= "]";
}

void mul(str a, str b, str c) {
    // update the internal representation of %c
    variables[c].second = variables[a].second * variables[b].second;;
    // variables for uhhhhhhhhh 
    int pa = variables[a].first;
    int pb = variables[b].first;
    int pc = variables[c].first;
    // move to %a
    mov(pa);
    // clone to free space
    brainfuck+= "[-";
    mov(var_count);
    brainfuck+= "+>+";
    mov(pa);
    brainfuck+= "<]";
    // move it back to %a
    mov(var_count + 1);
    brainfuck+= "[-";
    mov(pa);
    brainfuck+= "+";
    mov(var_count + 1);
    brainfuck+= "]";
    // move to %b
    mov(pb);
    // clone to free space
    brainfuck+= "[-";
    mov(var_count + 1);
    brainfuck+= "+>+";
    mov(pb);
    brainfuck+= "<]";
    // move it back to %b
    mov(var_count + 2);
    brainfuck+= "[-";
    mov(pb);
    brainfuck+= "+";
    mov(var_count + 2);
    brainfuck+= "]";
    // move to the clone of %a
    mov(var_count);
    // perform the multiplication
    brainfuck+= "[>[->+>+<<]>>[<<+>>-]<<<-]>[-]<";
    // clear %c
    mov(pc);
    brainfuck+= "[-]";
    // move the product back to %c
    mov(var_count + 2);
    brainfuck+= "[-";
    mov(pc);
    brainfuck+= "+";
    mov(var_count + 2);
    brainfuck+= "]";
}

void div(str a, str b, str c) {
    // update the internal representation of %c
    variables[c].second = variables[a].second / variables[b].second;;
    // variables for uhhhhhhhhh 
    int pa = variables[a].first;
    int pb = variables[b].first;
    int pc = variables[c].first;
    // move to %a
    mov(pa);
    // clone to free space
    brainfuck+= "[-";
    mov(var_count);
    brainfuck+= "+>+";
    mov(pa);
    brainfuck+= "<]";
    // move it back to %a
    mov(var_count + 1);
    brainfuck+= "[-";
    mov(pa);
    brainfuck+= "+";
    mov(var_count + 1);
    brainfuck+= "]";
    // move to %b
    mov(pb);
    // clone to free space
    brainfuck+= "[-";
    mov(var_count + 1);
    brainfuck+= "+>+";
    mov(pb);
    brainfuck+= "<]";
    // move it back to %b
    // (leaving one space inbetween the clones of %a and %b because the division algorithm needs one)
    mov(var_count + 1);
    brainfuck+= "[-";
    mov(pb);
    brainfuck+= "+";
    mov(var_count + 1);
    brainfuck+= "]";
    // move to the clone of %a
    mov(var_count);
    // perform the division
    brainfuck+= "[->+>-[>+>>]>[+[-<+>]>+>>]<<<<<<]";
    // clear the leftover values
    brainfuck+= ">[-]>[-]>[-]";
    ptr+= 3;
    // clear %c
    mov(pc);
    brainfuck += "[-]";
    // move the quotient to %c
    mov(var_count + 4);
    brainfuck += "[-";
    mov(pc);
    brainfuck += "+";
    mov(var_count + 4);
    brainfuck += "]";
}

void mod(str a, str b, str c) {
    // update the internal representation of %c
    variables[c].second = variables[a].second % variables[b].second;;
    // variables for uhhhhhhhhh 
    int pa = variables[a].first;
    int pb = variables[b].first;
    int pc = variables[c].first;
    // move to %a
    mov(pa);
    // clone to free space
    brainfuck += "[-";
    mov(var_count);
    brainfuck += "+>+";
    mov(pa);
    brainfuck += "<]";
    // move it back to %a
    mov(var_count + 1);
    brainfuck += "[-";
    mov(pa);
    brainfuck += "+";
    mov(var_count + 1);
    brainfuck += "]";
    // move to %b
    mov(pb);
    // clone to free space
    brainfuck += "[-";
    mov(var_count + 1);
    brainfuck += "+>+";
    mov(pb);
    brainfuck += "<]";
    // move it back to %b
    // (leaving one space inbetween the clones of %a and %b because the division algorithm needs one)
    mov(var_count + 1);
    brainfuck += "[-";
    mov(pb);
    brainfuck += "+";
    mov(var_count + 1);
    brainfuck += "]";
    // move to the clone of %a
    mov(var_count);
    // perform the division
    brainfuck += "[->+>-[>+>>]>[+[-<+>]>+>>]<<<<<<]";
    // clear the leftover values
    brainfuck += ">[-]>[-]>>[-]<";
    ptr += 3;
    // clear %c
    mov(pc);
    brainfuck += "[-]";
    // move the quotient to %c
    mov(var_count + 3);
    brainfuck += "[-";
    mov(pc);
    brainfuck += "+";
    mov(var_count + 3);
    brainfuck += "]";
}

void bif(str a, int n) {
    callstack.push(CallstackElement{ ptr, BlockType::IF, "_", n });
    set("_", 0);
    add(a, "_", "_");
    mov(0);
    brainfuck += "[";
}

void end_if() {
    int pa = variables[callstack.top().var_name].first;
    int saved_ptr = callstack.top().ptr_pos;
    callstack.pop();
    mov(0);
    brainfuck += "[-]]";
}

void bwhile(str a, int n) {
    int pa = variables.find(a)->second.first;
    callstack.push(CallstackElement{ ptr, BlockType::WHILE, a, n });
    mov(pa);
    brainfuck += "[";
}

void end_while() {
    int pa = variables[callstack.top().var_name].first;
    mov(pa);
    brainfuck += "]";
    mov(callstack.top().ptr_pos);
    callstack.pop();
}

void cmp(str a, str b, str c, str d, str e) {
    // update the internal representation of %c
    if (variables[a].second == variables[b].second) {
        variables[c].second = 1;
        variables[d].second = 0;
        variables[e].second = 0;
    } else if (variables[a].second > variables[b].second) {
        variables[c].second = 0;
        variables[d].second = 1;
        variables[e].second = 0;
    } else if (variables[a].second < variables[b].second) {
        variables[c].second = 0;
        variables[d].second = 0;
        variables[e].second = 1;
    }
    // variables for uhhhhhhhhh 
    int pa = variables[a].first;
    int pb = variables[b].first;
    int pc = variables[c].first;
    int pd = variables[d].first;
    int pe = variables[e].first;
    // move to %a
    mov(pa);
    // clone to free space
    brainfuck+= "[-";
    mov(var_count + 3);
    brainfuck+= "+>+";
    mov(pa);
    brainfuck+= "<]";
    // move it back to %a
    mov(var_count + 4);
    brainfuck+= "[-";
    mov(pa);
    brainfuck+= "+";
    mov(var_count + 4);
    brainfuck+= "]";
    // move to %b
    mov(pb);
    // clone to free space
    brainfuck+= "[-";
    mov(var_count + 4);
    brainfuck+= "+>+";
    mov(pb);
    brainfuck+= "<]";
    // move it back to %b
    mov(var_count + 5);
    brainfuck+= "[-";
    mov(pb);
    brainfuck+= "+";
    mov(var_count + 5);
    brainfuck+= "]";
    // set up the algorithm
    mov(var_count);
    // perform the comparison
    brainfuck+= "+[>>>>[-<]<<]<[->>+<]>>[<<<->+>>[-]]>[-]<<<<";
    // clear %c
    mov(pc);
    brainfuck+= "[-]";
    // move the equalty value to %c
    mov(var_count);
    brainfuck+= "[-";
    mov(pc);
    brainfuck+= "+";
    mov(var_count);
    brainfuck += "]";
    // clear %d
    mov(pd);
    brainfuck += "[-]";
    // move the grt value to %d
    mov(var_count + 1);
    brainfuck += "[-";
    mov(pd);
    brainfuck += "+";
    mov(var_count + 1);
    brainfuck += "]";
    // clear %e
    mov(pe);
    brainfuck += "[-]";
    // move the lss value to %e
    mov(var_count + 2);
    brainfuck += "[-";
    mov(pe);
    brainfuck += "+";
    mov(var_count + 2);
    brainfuck += "]";
}
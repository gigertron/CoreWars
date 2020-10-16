//
//  CPUClasses.hpp
//  CoreWar01
//
//  Created by Tim Cowley on 9/26/20.
//  Copyright © 2020 Tim Cowley. All rights reserved.
//

#ifndef CPUClasses_hpp
#define CPUClasses_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
enum class Operation{
    noop = 0,
    mov,
    cmp,
    set,
    get,
    add,
    sub,
    mult,
    fork,
    branch,
    branch0,
    rand
};

class Data{
public:
    int value;
    bool immediate; // treat as data and not a pointer
};

// Instructions
class Instruction{
    public:
    Operation operation; // a single operation, like mov but no arguments
    Data argument, second; // Probably one or two
    
    Instruction();
    Instruction(Operation a);
    Instruction(Operation a, Data argument);
    Instruction(Operation a, Data argument, Data second);
};

// Greetings, program
class Program{
public:
    std::vector<Instruction> text;
    void add(Instruction a);
    void reset();
    Instruction run(); // one at a time, return operations to give to the CPU. CPU performs the thing or doesn't, the Program doesn't get to decide.
    Program(std::vector<Instruction> a);
    Program();
};

// Processes are running programs
class Process{
public:
    Program program;
    int size; // in instructions only for now
    int programCounter;
    bool os; // is this a privileged process or just some rando
    int location; // where in memory are we? Changing this must change program text.
    Process(Program a);
};

class Memory{
public:
const static int memorySize = 256;
    // Here there be abstractions
    Instruction core[Memory::memorySize];
    void set(int address, Instruction);
    void set(int address, Data, Data);
    void set(int address, Data);
    std::string* display(std::vector<Process*>);
};

class System{
public:
    
    std::vector<Process*> processes;
    std::vector<size_t> threads; //
    size_t load(Program *a, Memory *m);
    std::string* display(int width, int height);
    std::string* status();
};


class CPU{
public:
    bool comparisonResult;//-1,0,1
    int x; // should be replaced with a vector of size # of processes
    void execute(Memory* mem, System* os);
};




#endif /* CPUClasses_hpp */

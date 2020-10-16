//
//  CPUClasses.cpp
//  CoreWar01
//
//  Created by Tim Cowley on 9/26/20.
//  Copyright © 2020 Tim Cowley. All rights reserved.
//
#include "CPUClasses.hpp"

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

Instruction::Instruction(Operation a, Data arg, Data mYsecond)
{
    operation = a;
    argument = arg;
    second = mYsecond;
}

Instruction::Instruction(Operation a, Data arg)
{
    operation = a;
    argument = arg;
    second = arg;
    // specifically not init'ing second
}

Instruction::Instruction(Operation a)
{
    operation = a;
    argument = Data();
}

Instruction::Instruction()
{
    operation = Operation::noop;
    argument = Data();
}

Program::Program()
{
    text.clear();
    text.resize(1);
    text.emplace_back(Instruction(Operation::noop));
}

Program::Program(std::vector<Instruction> a)
{
    text = a;
}

void Program::add(Instruction a)
{
    text.emplace_back(a);
}

void Program::reset()
{
    text.clear();
    text.resize(1);
    text.emplace_back(Instruction(Operation::noop));// whenever I use this I should really be making a new object
}

Process::Process(Program a)
{
    this->program = a;
    size = (int)program.text.size();
    programCounter = 0;
    os = false;
}

void Memory::set(int addr, Instruction i)
{
    core[addr].operation = i.operation;
}
void Memory::set(int addr, Data a)
{
    core[addr].argument = a;
    core[addr].second = {0,false};
}
void Memory::set(int addr, Data a, Data b)
{
    core[addr].argument = a;
    core[addr].second = b;
}

std::string* Memory::display(std::vector<Process*> programCounters)
{
    std::string *out = new std::string();
    for(int i=0; i<Memory::memorySize; i++)
    {
        Instruction instruction = core[i];
        for(Process *pc : programCounters)
        {
            if(i == pc->location + pc->programCounter)
            {
                out->append("PC");
                continue;
            }
        }
        if(instruction.operation == Operation::noop)out->append(".");
        else if(instruction.operation == Operation::mov)out->append("M");
        else if(instruction.operation == Operation::cmp)out->append("c");
        else if(instruction.operation == Operation::set)out->append("e");
        else if(instruction.operation == Operation::add)out->append("a");
        else if(instruction.operation == Operation::sub)out->append("s");
        else if(instruction.operation == Operation::mult)out->append("m");
        else if(instruction.operation == Operation::branch)out->append("b");
        else if(instruction.operation == Operation::branch0)out->append("0");
        else if(instruction.operation == Operation::rand)out->append("r");
        else if(instruction.operation == Operation::fork)out->append("f");
        else out->append(".");
    }
    return out;
}

std::string* System::status()
{
    std::string *out = new std::string();
    int ID=0;
    for(Process *a : processes)
    {
        out->append("ID[" + std::to_string(ID) + "] ");
        out->append("Size[" + std::to_string(a->size) + "]");
//        out->append("Location["+ std::to_string(a->size) + "]");
        out->append("PC["+ std::to_string(a->programCounter) + "]");
        
        out->append("/r/n");
    }
    return out;
}

// loads you somewhere in memory, returns -1 if can't fit
size_t System::load(Program *a, Memory *m)
{
    // Convert this program to a running process
    Process* proc = new Process(*a);
    proc->size = (int)a->text.size(); // two noops
    processes.emplace_back(proc); // welcome to the process table
    int loadLocation = 0;
    // now, place you somewhere near the bottom of memory
    loadLocation = Memory::memorySize;
    for(Process *p : processes)
    {
        loadLocation -= p->size;
    }
    loadLocation -= proc->size;
    if(loadLocation <= 0) return -1; // if it doesn't fit, it doesn't fit.
    
    processes.back()->location = loadLocation;
    processes.back()->size = proc->size;
    
    //Now, copy the program into its reserved space
    for(int i=0; i<proc->size; i++)
    {
        m->set(loadLocation + i, proc->program.text[i]);
        m->set(loadLocation + i, proc->program.text[i].argument, proc->program.text[i].second);
    }
    
    return proc->location;
}

void CPU::execute(Memory* m, System* os)
{
    for(Process* p : os->processes)
    {
        //Instruction i = m->core[p->location + p->programCounter];
        
        Operation o = m->core[p->location + p->programCounter].operation;
        Data a = m->core[p->location + p->programCounter].argument;
        Data b = m->core[p->location + p->programCounter].second;
        //we've loaded the reference and the reality: any differences mean damage!
        //if (i.operation != o || i.arguments != a)
        {
            // someting ... maybe
            //std::cout << "Damage detected to process /r/n" << std::endl;
        }
        
        // The big switch statement you knew was coming. This could be dynamically constructed...
        switch (o)
        {
            case Operation::fork:{
                //std::cout << "fork operation" << std::endl;
                os->load(&p->program, m);
                //bwaha now we are LEGION
                break;
            }
            case Operation::noop:{
                //std::cout << "noop" << std::endl;
                //Actually lots of stuff happens here!
                break;
            }
            case Operation::mov:{
                // we're copying some data from one location to CPU.x
                // or, we're not in immediate mode and we're moving from x to core
                // all relative values based off of the program location
                if(a.immediate){
                    m->set(p->location + a.value, {x,false});
                } else {
                   // m->set(b.value, {m->core[p->location + a.value].argument.value, false});// ? needed
                    m->core[p->location + p->programCounter + b.value] = m->core[p->location + p->programCounter + a.value];
                }
            }
                break;
            case Operation::set:{
                if(a.immediate)
                    x = a.value;
                else
                    m->core[a.value].argument.value = x;
                break;}
            case Operation::get:{
                    x = m->core[a.value].argument.value;
                break;}
            case Operation::branch:{
                p->programCounter += a.value; // argument data is how far back to loop
                break;}
            case Operation::branch0:{ // conditionals
                if( x == 0)
                    p->programCounter += a.value;
                break;}
            case Operation::mult:{
                x *= a.value;
                break;
            }
            case Operation::rand:{
                x = Memory::memorySize & std::rand(); // random address on this system
                break;
            }
            case Operation::add:{
                x += a.value;
                break;
            }
            case Operation::cmp:{
                if (x > a.value) x = 1;
                if (x == a.value) x = 0;
                if (x < a.value) x = -1;
                break;
            }
            case Operation::sub:{
                x -= a.value;
                break;
            }
                
        }
        // Phew, okay, I lied, it wasn't that big
        
        p->programCounter++;
        if(p->programCounter > 0 && p->programCounter > p->program.text.size()){p->programCounter = 0;}
    }
}




//
//  main.cpp
//  CoreWar01
//
//  Created by Tim Cowley on 9/26/20.
//  Copyright © 2020 Tim Cowley. All rights reserved.
//

#include <iostream>
#include "CPUClasses.hpp"


int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, Core!\n";
    
    // This first version will play out a battle between two programs.
    // First: hard-coded programs, then loaded from file
    // Battle: step by step, then on a timer (like 1 iter per second)
    // Graphics: just text for now: a real graphics layer will wait
    
    Program *defender = new Program();
    Program *attacker = new Program();
    
    defender->add(Instruction(Operation::noop));
   
    
    attacker->add(Instruction(Operation::set, Data({2,true})));
    attacker->add(Instruction(Operation::set, Data({2,false})));
    attacker->add(Instruction(Operation::set, Data({2,false})));
    attacker->add(Instruction(Operation::set, Data({2,false})));
    attacker->add(Instruction(Operation::set, Data({2,false})));
    attacker->add(Instruction(Operation::set, Data({2,false})));
    attacker->add(Instruction(Operation::set, Data({2,false})));
    attacker->add(Instruction(Operation::set, Data({2,false})));
    attacker->add(Instruction(Operation::set, Data({2,false})));

    attacker->add(Instruction(Operation::mov, Data({2,false}), Data({-10,false})));
    attacker->add(Instruction(Operation::branch, Data({-15,false})));
    
    System os;
    Memory m;
    CPU cpu;
    os.load(defender, &m);
    os.load(attacker, &m);

    while(true)
    {
        cpu.execute(&m, &os);
        std::cout << "Memory Core Status" << std::endl;
        std::cout << *(os.status()) << std::endl;
        std::vector<Process*> programCounters;
        for(Process *p : os.processes)
            programCounters.emplace_back(p);
        std::cout << *(m.display(programCounters)) << std::endl;
    }
    
    return 0;
}

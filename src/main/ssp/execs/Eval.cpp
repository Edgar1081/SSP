#pragma once
#include "../Io.h"
#include "../GSA.h"
#include <memory>
#include <iomanip>
#include <random>

int main(int argc, char *argv[]) {
    std::cout << std::setprecision(16);
    std::unique_ptr<Io> input = std::make_unique<Io>(argv[1]);
    int size = input->get_size();
    std::shared_ptr<int[]> orig = input->get_array();
    int target = input->get_target();

    int om = 0;

    for(int i = 0; i< size; i++){
        if(orig[i] > 0)
            om += orig[i];
    }

    std::shared_ptr<Instance> instance =
        std::make_shared<Instance>(size, orig, om);

    std::shared_ptr<Agent> a =
        std::make_shared<Agent>(instance, size, target);

    std::cout << "COST: " << a-> get_cost() << std::endl;
    std::cout << "MASS: " << a-> get_mass() << std::endl;

    std::cout << target << std::endl;

    std::bitset<1000> sol = a->get_sol_bits();
    for(int i = 0; i < size; i++){
        if(sol[i])
            std::cout << orig[i];
        else
            continue;
        if(i != size-1)
            std::cout << ",";
    }

    return 0;
}

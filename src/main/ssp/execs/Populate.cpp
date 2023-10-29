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

    std::cout << std::setprecision(16);

    std::unique_ptr<GSA> gsa = std::make_unique<GSA>(0, 10, size, orig, target);
    gsa->print_masses();

    for(int i = 0; i <  10; i++)
        for(int j = i+1; j < 10; j++)
            std::cout << "(" << i << "," << j << ") " << gsa->get_distance(i,j) << std::endl;

    std::cout << gsa->get_massive() << std::endl;
    std::cout << gsa->get_massive_index() << std::endl;

    std::cout << gsa->get_fastest_index() << std::endl;
    std::cout << gsa->get_fastest() << std::endl;

    std::cout << gsa->get_closest() << std::endl;
    return 0;
}

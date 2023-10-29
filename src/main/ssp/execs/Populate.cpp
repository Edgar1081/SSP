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

    std::cout << std::fixed << std::setprecision(16);

    std::unique_ptr<GSA> gsa = std::make_unique<GSA>(12345, 100, size, orig, target);


    gsa->print_distances();


    for(int i = 0; i< 1000; i++){
        gsa->iterate();
        gsa->print_masses();
        std::cout << "ITER: " << i << std::endl;
        std::cout << gsa->get_massive_index() <<
        ": " << gsa->get_massive() << std::endl;

        std::cout << gsa->get_fastest_index() <<
        ": " << gsa->get_fastest() << std::endl;
    }

    std::shared_ptr<int []> best = gsa->get_sol();
    int subset_size = gsa->get_sol_size();
    gsa->print_cost();

    for(int i = 0; i < subset_size; i++)
        std::cout<< best[i] << ",";



    std::cout<< std::endl;
    std::cout<< subset_size;
    return 0;
}

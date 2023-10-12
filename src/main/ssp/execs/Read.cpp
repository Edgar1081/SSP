#include "../Io.h"
#include <memory>
#include <random>
#include <sqlite3.h>

int main(int argc, char *argv[]) {
    std::cout << std::setprecision(16);
    std::unique_ptr<Io> input = std::make_unique<Io>(argv[1]);
    int size = input->get_size();
    int * ins = input->get_array();
    // std::shared_ptr<Instance> instance = std::make_shared<Instance>(ins, bdd, size, 0);


    for(int i = 0; i<size; i++){
        std::cout << ins[i];
        if(i != size-1)
            std::cout << ",";
    }
    return 0;
}

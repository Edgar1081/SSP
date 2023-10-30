#include "../Io.h"
#include "../GSA.h"
#include <memory>
#include <iomanip>
#include <random>
#include <cstdlib>

int main(int argc, char *argv[]) {
    std::cout << std::setprecision(16);
    std::unique_ptr<Io> input = std::make_unique<Io>(argv[1]);
    int size = input->get_size();
    std::shared_ptr<int[]> orig = input->get_array();
    int target = input->get_target();

    int seed = 0;
    int rosas = 2;

    for (int i = 2; i < argc; i++) {
        if (std::string(argv[i]) == "-s" && i + 1 < argc) {
            seed = std::atoi(argv[i + 1]);
        } else if (std::string(argv[i]) == "-a" && i + 1 < argc) {
            rosas = std::atoi(argv[i + 1]);
        }
    }
    std::cout << std::fixed << std::setprecision(16);

    std::unique_ptr<GSA> gsa =
        std::make_unique<GSA>(seed, rosas, size, orig, target);
    gsa->solve_coords();
    return 0;
}

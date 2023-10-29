#pragma once
#include <cstdlib>
#include <memory>
#include <bitset>
#include <random>

class Instance {
  private :
    int size;
    std::shared_ptr<int[]> orig;
    int Omega;

  public :

    Instance (int _size, std::shared_ptr<int []> _orig, int _Omega):
      size(_size), orig(_orig), Omega(_Omega){

    }

    std::bitset<1000> get_init(int index) {
      std::bitset<1000> randomBits;

      std::mt19937 gen;
      std::uniform_int_distribution<int> distrib(0, 1);
      gen.seed(index);

      for (size_t i = 0; i < 1000; i++) {
        randomBits[i] = distrib(gen);
      }

      return randomBits;
    }

    int get_Omega(){
      return Omega;
    }

    std::shared_ptr<int[]> get_neigh(const std::shared_ptr<int[]>& m, int size) {
      std::shared_ptr<int[]> n(new int[size]);

      std::copy(m.get(), m.get() + size, n.get());

      return n;
    }

    std::shared_ptr<int[]> get_orig(){
      return orig;
    }

    // double* get_costs() {
    //   return actuals_cost;
    // }

    // double* get_lasts_cost() {
    //   return lasts_cost;
    // }
};

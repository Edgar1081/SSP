#pragma once
#include <cstdlib>
#include <memory>
#include <bitset>
#include <random>

class Instance {
  private :
    int size;
    std::shared_ptr<int[]> orig;

  public :

    Instance (int _size, std::shared_ptr<int []> _orig):
      size(_size),orig(_orig){

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

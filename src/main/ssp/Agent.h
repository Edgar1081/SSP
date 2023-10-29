#pragma once
#include <bitset>
#include "Instance.h"

class Agent {
  private:
    std::shared_ptr<Instance> ins;
    std::shared_ptr<int[]> orig;
    int size;
    int index;
    int target;
    std::bitset<1000> actual;
    int Omega;
    double v;
    double mass;
    int cost;
    int sum;
    double force;

    void first_cost() {
      sum = 0;
      for (int i = 0; i < size; i++)
        if (actual[i] == 1)
          sum += orig[i];

      cost = abs(sum - target);
    }

    void recal_cost() {
      cost = abs(sum - target);
    }

    void calc_mass() {
      double rest = static_cast<double>(cost) / static_cast<double>(target);
      mass = Omega - rest;
    }

  public:
    Agent(std::shared_ptr<Instance> _ins, int _size,
          int _index, int _target) : ins(_ins), size(_size), index(_index), target(_target) {
      actual = ins->get_init(index);
      Omega = ins->get_Omega();
      orig = ins->get_orig();
      first_cost();
      calc_mass();
      v = 0;
    }

    Agent(std::shared_ptr<Instance> _ins, int _size,
          int _target, std::bitset<1000> _actual)
      : ins(_ins), size(_size), target(_target), actual(_actual) {
      Omega = ins->get_Omega();
      orig = ins->get_orig();
      first_cost();
      calc_mass();
      v = 0;
    }

    std::bitset<1000> get_sol() {
      return actual;
    }

    int get_cost() {
      return cost;
    }

    int get_sum() {
      return sum;
    }

    double get_mass() {
      return mass;
    }

    int get_index() {
      return index;
    }

    double get_force(){
      return force;
    }

    void calc_force(std::shared_ptr<Agent> massive, double G, int r){
      if(r == 0)
        force = 0;
      else{
        double f = G*(massive->get_mass()*mass)/r;
        force = f;
      }
    }

    void flip(int x) {
      actual.flip(x);
      if (actual[x])
        sum += orig[x];
      else
        sum -= orig[x];
      recal_cost();
      calc_mass();
    }

    int get_v(){
      return v;
    }

    void set_v(int _v){
      v = _v;
    }

};

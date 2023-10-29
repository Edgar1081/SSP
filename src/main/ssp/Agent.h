#pragma once
#include <bitset>
#include <iomanip>
#include <string>
#include <algorithm>
#include "Instance.h"

class Agent {
  private:
    std::shared_ptr<Instance> ins;
    std::shared_ptr<int[]> orig;
    int size;
    int index;
    int target;
    int prob;
    std::bitset<1000> actual;
    int Omega;
    double v;
    double mass;
    int cost;
    int64_t sum;
    double force;
    int subset_size;
    std::mt19937 gen;

    void first_cost() {
      int64_t f_sum = 0;
      for (int i = 0; i < size; i++)
        if (actual[i])
          f_sum += orig[i];

      sum = f_sum;
      cost = std::abs(sum - target);
    }

    void recal_cost() {
      cost = std::abs(sum - target);
    }

    void calc_mass() {
      double rest = static_cast<double>(cost) / static_cast<double>(target);
      mass = Omega - rest;
    }

    void subset_count(){
      subset_size = actual.count();
    }

  public:
    Agent(std::shared_ptr<Instance> _ins, int _size,
          int _index, int _target, int _prob) :
      ins(_ins), size(_size), index(_index),
      target(_target), prob(_prob) {
      gen.seed(index+prob);
      get_init(index);
      Omega = ins->get_Omega();
      orig = ins->get_orig();
      first_cost();
      subset_count();
      //calc_mass();
      v = 0;
    }

    Agent(std::shared_ptr<Instance> _ins, int _size,
          int _target)
      : ins(_ins), size(_size), target(_target) {
      Omega = ins->get_Omega();
      orig = ins->get_orig();
      get_init(0);
      first_cost();
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

    void set_mass(double b, double w){
      mass = std::abs((cost - w)/(b-w));
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
      if (actual[x]){
        sum += orig[x];
        subset_size++;
      }else{
        sum -= orig[x];
        subset_size--;
      }
      recal_cost();
    }

    int get_v(){
      return v;
    }

    void set_v(int _v){
      v = _v;
    }

    std::shared_ptr<int[]> get_subset(){
      int s = 0;
      std::shared_ptr<int[]> subset(new int[actual.count()]);
      int j = 0;
      for(int i = 0; i < size; i++)
        if(actual[i]){
          s += subset[j] = orig[i];
          j++;
        }
      std::cout << "SUM: " << s << std::endl;
      return subset;
    }

    int get_subset_size(){
      return subset_size;
    }

    int get_target(){
      return target;
    }

    void get_init(int index) {
      std::bitset<1000> random_bits;
      std::uniform_int_distribution<int> distrib(0, prob);

      for (size_t i = 0; i < 1000; i++) {
        if(distrib(gen) == 0)
          random_bits.set(i);
      }
      std::string bitsetString = random_bits.to_string();

      std::shuffle(bitsetString.begin(), bitsetString.end(), gen);
      std::bitset<1000> shuffled(bitsetString);
      actual = shuffled;
    }

    std::bitset<1000> get_sol_bits(){
      return actual;
    }
};

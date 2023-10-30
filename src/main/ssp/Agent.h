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
    double v;
    double mass;
    int cost;
    int64_t sum;
    double force;
    int subset_size;
    std::mt19937 gen;
    unsigned __int128 x;
    unsigned __int128 y;

    int hamming(std::shared_ptr<Agent> b){
      std::bitset<1000> h = actual ^ b->get_sol();
      return h.count();
    }

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

    void subset_count(){
      subset_size = actual.count();
    }

  public:
    Agent(std::shared_ptr<Instance> _ins, int _size,
          int _index, int _target, int _prob) :
      ins(_ins), size(_size), index(_index),
      target(_target), prob(_prob) {
      gen.seed(index);
      get_init();
      orig = ins->get_orig();
      first_cost();
      subset_count();
      set_coords();
      v = 0;
    }

    Agent(std::shared_ptr<Instance> _ins, int _size,
          int _target)
      : ins(_ins), size(_size), target(_target) {
      orig = ins->get_orig();
      get_init();
      first_cost();
      set_coords();
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

    int distance(std::shared_ptr<Agent> b){
      return hamming(b);
    }

    void close(std::shared_ptr<Agent> b,
               std::uniform_int_distribution<int> distrib,
               std::mt19937 gen){
      std::bitset<1000> B = b->get_sol();
      while(true){
        int x = distrib(gen);
        if(actual[x] != B[x]){
          flip(x);
          break;
        }
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
      set_coords();
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
      return subset;
    }

    int get_subset_size(){
      return subset_size;
    }

    int get_target(){
      return target;
    }

    void random_move(std::uniform_int_distribution<int> distrib){
      int x = distrib(gen);
      int y = distrib(gen);
      flip(x);
      flip(y);
    }

    void get_init() {
      std::bitset<1000> random_bits;
      std::uniform_int_distribution<int> distrib(0, prob);

      for (size_t i = 0; i < 1000; i++) {
        if(distrib(gen) == 0)
          random_bits.set(i);
      }
      std::string bitsetString = random_bits.to_string();

      //std::shuffle(bitsetString.begin(), bitsetString.end(), gen);
      std::bitset<1000> shuffled(bitsetString);
      actual = shuffled;
    }

    std::bitset<1000> get_sol_bits(){
      return actual;
    }

    unsigned __int128 get_x(){
      return static_cast<__int128>(x);
    }

    unsigned __int128 get_y(){
      return static_cast<__int128>(y);
    }

    unsigned __int128 bit_to_int(std::bitset<500> bits) {
      unsigned __int128 result = 0;
      for (size_t i = 0; i < bits.size(); ++i) {
        result = (result << 1) | static_cast<__int128>(bits[i]);
      }
      return result;
    }
    void set_coords(){
      std::string actualString = actual.to_string();
      size_t halfSize = actualString.size() / 2;

      std::string xbinS = actualString.substr(0, halfSize);
      std::string ybinS = actualString.substr(halfSize);

      std::bitset<500> xbin(xbinS);
      std::bitset<500> ybin(ybinS);

      x = bit_to_int(xbin);
      y = bit_to_int(ybin);
    }
};

#pragma once
#include <cstdlib>
#include <memory>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <cstdint>
#include <tuple>
#include "Agent.h"
#include "Instance.h"

class GSA {
  private :
    int seed;
    int rosas;
    int size;
    int64_t Omega;
    std::shared_ptr<int[]> orig;
    int target;
    std::shared_ptr<Agent>* agents;
    int bigger_mass_index;
    int smaller_mass_index;
    double bigger_mass;
    int fastest_index;
    int best_fit;
    int worst_fit;
    std::mt19937 gen;

    void set_Omega(){
      int64_t sum = 0;
      for(int i = 0; i < size; i++)
        if(orig[i] > 0)
          sum += orig[i];
      Omega = sum;
    }

    void update_masses(){
      int b = std::numeric_limits<int>::max();
      int w = -1;

      for(int i = 0; i < rosas; i++){
        std::shared_ptr<Agent> agent = agents[i];
        if(agent->get_cost() > w)
          w = agent->get_cost();
        if(agent->get_cost() < b)
          b = agent->get_cost();
      }

      best_fit = b;
      worst_fit = w;
      for(int i = 0; i < rosas; i++){
        std::shared_ptr<Agent> agent = agents[i];
        agent->set_mass(best_fit, worst_fit);
      }
    }

    void fill_agents(std::shared_ptr<Instance> ins){
      std::uniform_int_distribution<int> M(0, 4);
      agents = new std::shared_ptr<Agent>[rosas];
      int prob = 0;
      int probofprob = 0;
      for(int i = 0; i < rosas; i++){
        probofprob = M(gen);
        std::uniform_int_distribution<int> dist(0, probofprob);
        prob = dist(gen);
        std::shared_ptr<Agent> agent =
          std::make_shared<Agent>(ins, size, i, target, prob);
        agents[i] = agent;
      }
    }

    void update_massive(){
      bigger_mass_index = -1;
      bigger_mass = -1;
      for(int i = 0; i < rosas; i++){
        std::shared_ptr<Agent> agent = agents[i];
        if(agent->get_mass() > bigger_mass){
          bigger_mass = agent->get_mass();
          bigger_mass_index = agent->get_index();
        }
      }
      if(bigger_mass_index == -1){
        bigger_mass_index = 0;
        bigger_mass = agents[bigger_mass_index]->get_mass();
      }


    }

    void update_forces(){
      fastest_index = -1;
      double f = -1;
      for(int i = 0; i < rosas; i++){
        if(i == bigger_mass_index){
          agents[i]->calc_force(agents[bigger_mass_index], 1000, 0);
          continue;
        }
        double d = hamming1(i, bigger_mass_index);
        agents[i]->calc_force(agents[bigger_mass_index], 1000, d);
        if(agents[i]->get_force() > f){
          f = agents[i] -> get_force();
          fastest_index = i;
        }
      }
    }

    void calc_velocities(){
      std::tuple<int, double>* s= new std::tuple<int, double>[rosas];
      for(int i = 0; i < rosas; i++)
        s[i] = std::make_tuple(i, agents[i]->get_force());

      std::sort(s, s + rosas, compare_tuples);

      for(int i = 0; i < rosas; i++){
        int index = std::get<0>(s[i]);
        agents[index]->set_v(std::ceil(std::get<1>(s[i])));
        if(std::get<1>(s[i]) == 0)
          agents[index]->set_v(0);
      }
      delete[] s;
    }

    int hamming1(int i, int j){
      std::shared_ptr<Agent> a =agents[i];
      std::shared_ptr<Agent> b =agents[j];
      return hamming(a,b);
    }

    int hamming(std::shared_ptr<Agent> a, std::shared_ptr<Agent> b){
      std::bitset<1000> h = a->get_sol() ^ b->get_sol();
      return h.count();
    }

    static bool compare_tuples(std::tuple<int, double> A,
                        std::tuple<int, double> B){
      return std::get<1>(A) < std::get<1>(B);
    }


    void close_n(int a, int b, int n){
      for(int i = 0; i < n; i++){
        close(a,b);
      }
    }

    void update_sys(){
      update_massive();
      update_forces();
      calc_velocities();
    }

  public :

    GSA (int _seed,int _rosas, int _size,
         std::shared_ptr<int[]> _orig, int _target):
      seed(_seed),rosas(_rosas), size(_size),
      orig(_orig), target(_target){
      gen.seed(seed);
      set_Omega();
      std::shared_ptr<Instance> instance =
        std::make_shared<Instance>(size, orig, Omega);
      orig = instance->get_orig();
      fill_agents(instance);
      update_masses();
      update_sys();
    }

    std::shared_ptr<int []> get_set() {
      return orig;
    }

    void print_masses() {
      for (int i = 0; i < rosas; i++) {
        std::cout << "Index: " << agents[i]->get_index() <<
          " MASS: " << agents[i]->get_mass() <<
          "  COST: " << agents[i]->get_cost() <<
          "  VEL : " << agents[i]->get_v() <<
          "  FORCE: " << agents[i]->get_force() <<
          std::endl;
      }
    }

    int get_distance(int i, int j){
      return hamming(agents[i],agents[j]);
    }

    std::bitset<1000> get_sol(int i){
      return agents[i]->get_sol();
    }

    void close(int i, int j){
      int h = hamming1(i,j);
      if(h == 0)
        return;
      std::bitset<1000> A = agents[i]->get_sol();
      std::bitset<1000> B = agents[j]->get_sol();
      std::uniform_int_distribution<int> distrib(0, size);

      while(true){
        int x = distrib(gen);
        if(A[x] != B[x]){
          agents[i]->flip(x);
          break;
        }
      }
    }

    void print_distances(){
      for(int i = 0; i < rosas; i++){
        for(int j= i+1; j < rosas; j++)
          std::cout <<
            "(" << i << "," << j << "): " <<
            hamming1(i,j) << std::endl;
      }
    }

    double get_massive(){
      return bigger_mass;
    }

    int get_massive_index(){
      return bigger_mass_index;
    }

    int get_fastest_index(){
      return fastest_index;
    }

    int get_fastest(){
      return agents[fastest_index]->get_v();
    }


    int get_closest(){
      int d = std::numeric_limits<int>::max();
      for(int i = 0; i <  10; i++)
        for(int j = i+1; j < 10; j++){
          int h = hamming(agents[i], agents[j]);
          if(h < d)
            d = h;
        }
      return d;
    }

    int get_farest(){
      int d = -1;
      for(int i = 0; i <  10; i++)
        for(int j = i+1; j < 10; j++){
          int h = hamming(agents[i], agents[j]);
          if(h > d)
            d = h;
        }
      return d;
    }


    bool all_together(){
      int total_d = 0;
      for(int i = 0; i <  10; i++)
        for(int j = i+1; j < 10; j++)
          total_d += hamming1(i,j);
      return total_d <= 0;
    }

    void solve(){
      while(!all_together()){
        iterate();
        std::cout << agents[bigger_mass_index]->get_cost() << std::endl;
      }
    }

    void iterate(){
      for(int i = 0; i< rosas; i++){
        if(i == bigger_mass_index)
          continue;
        int v = agents[i]->get_v();
        for(int j = 0; j<v; j++){
          close(i, bigger_mass_index);
          update_masses();
          update_sys();
        }
      }
      // move_sys();
    }

    void update_bigger(){
      for(int i = 0; i < rosas; i++){
        std::shared_ptr<Agent> agent = agents[i];
        int h = hamming1(bigger_mass_index, i);
        agents[bigger_mass_index]->calc_force(agent, 1000, h);
        int center_v = agents[bigger_mass_index]->get_force();
        close_n(bigger_mass_index, i, std::ceil(center_v));
      }
    }

    void flip_random(int i){
      std::uniform_int_distribution<int> distrib(0, size);

    }

    void print_cost(){
      std::cout << agents[bigger_mass_index]->get_cost() <<std::endl;
      std::cout << agents[bigger_mass_index]->get_sum() <<std::endl;
      std::cout << agents[bigger_mass_index]->get_target() <<std::endl;
    }

    std::shared_ptr<int[]> get_sol(){
      return agents[bigger_mass_index]->get_subset();
    }

    int get_sol_size(){
      return agents[bigger_mass_index]->get_subset_size();
    }

};

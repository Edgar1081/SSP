#pragma once
#include <cstdlib>
#include <memory>
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
    double bigger_mass;
    int fastest_index;

    void set_Omega(){
      int64_t sum = 0;
      for(int i = 0; i < size; i++)
        if(orig[i] > 0)
          sum += orig[i];
      Omega = sum;
    }

    void fill_agents(std::shared_ptr<Instance> ins){
      agents = new std::shared_ptr<Agent>[rosas];
      for(int i = 0; i < rosas; i++){
        std::shared_ptr<Agent> agent =
          std::make_shared<Agent>(ins, size, i, target);
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
    }

    void update_forces(){
      fastest_index = -1;
      double f = -1;
      for(int i = 0; i < rosas; i++){
        double d = hamming(agents[i], agents[bigger_mass_index]);
        agents[i]->calc_force(agents[bigger_mass_index], 1, d);
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
        agents[index]->set_v(i);
      }

      for (int i = 0; i < rosas; i++) {
        std::cout << "Index: " << agents[i]->get_index() <<
          " VELOCITY " << agents[i]->get_v() << std::endl;
      }

      delete[] s;
    }


    int hamming(std::shared_ptr<Agent> a, std::shared_ptr<Agent> b){
      std::bitset<1000> h = a->get_sol() ^ b->get_sol();
      return h.count();
    }

    static bool compare_tuples(std::tuple<int, double> A,
                        std::tuple<int, double> B){
      return std::get<1>(A) < std::get<1>(B);
    }


    void close_n(int i, int j, int n){
      for(int i = 0; i < n; i++){
        close(i,j);
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
      set_Omega();
      std::shared_ptr<Instance> instance =
        std::make_shared<Instance>(size, orig, Omega);
      orig = instance->get_orig();
      fill_agents(instance);
      update_sys();
    }

    std::shared_ptr<int []> get_set() {
      return orig;
    }

    void print_masses() {
      for (int i = 0; i < rosas; i++) {
        std::cout << "Index: " << agents[i]->get_index() <<
          " MASS: " << agents[i]->get_mass() <<
          " COST: " << agents[i]->get_cost() << std::endl;
      }
    }

    int get_distance(int i, int j){
      return hamming(agents[i],agents[j]);
    }

    std::bitset<1000> get_sol(int i){
      return agents[i]->get_sol();
    }

    void close(int i, int j){
      std::bitset<1000> A = agents[i]->get_sol();
      std::bitset<1000> B = agents[j]->get_sol();
      std::mt19937 gen;
      std::uniform_int_distribution<int> distrib(0, size);
      gen.seed(seed);

      while(true){
        int x = distrib(gen);
        if(A[x] != B[x]){
          agents[i]->flip(x);
          break;
        }
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

    void iterate(){

    }
};

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
    int sweep_index;

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
      std::uniform_int_distribution<int> M(1, 99);
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
      smaller_mass_index = -1;
      bigger_mass = -1;
      for(int i = 0; i < rosas; i++){
        std::shared_ptr<Agent> agent = agents[i];
        if(agent->get_mass() > bigger_mass){
          bigger_mass = agent->get_mass();
          bigger_mass_index = agent->get_index();
        }
        if(agent->get_mass() == 0){
          smaller_mass_index = agent->get_index();
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
          //agents[i]->calc_force(agents[bigger_mass_index], 1000, 0);
          continue;
        }
        double d = distance(i, bigger_mass_index);
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

    int distance(int i, int j){
      std::shared_ptr<Agent> a =agents[i];
      std::shared_ptr<Agent> b =agents[j];
      return a->distance(b);
    }

    static bool compare_tuples(std::tuple<int, double> A,
                        std::tuple<int, double> B){
      return std::get<1>(A) > std::get<1>(B);
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
      std::shared_ptr<Instance> instance =
        std::make_shared<Instance>(size, orig);
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
      return distance(i,j);
    }

    std::bitset<1000> get_sol(int i){
      return agents[i]->get_sol();
    }

    void close(int i, int j){
      std::shared_ptr<Agent> A = agents[i];
      std::shared_ptr<Agent> B = agents[j];
      int h = A->distance(B);
      if(h == 0)
        return;
      std::uniform_int_distribution<int> distrib(0,size);
      A->close(B, distrib, gen);
    }

    void print_distances(){
      for(int i = 0; i < rosas; i++){
        for(int j= i+1; j < rosas; j++)
          std::cout <<
            "(" << i << "," << j << "): " <<
            distance(i,j) << std::endl;
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
          int h = distance(i, j);
          if(h < d)
            d = h;
        }
      return d;
    }

    int get_farest(){
      int d = -1;
      for(int i = 0; i <  10; i++)
        for(int j = i+1; j < 10; j++){
          int h = distance(i,j);
          if(h > d)
            d = h;
        }
      return d;
    }

    bool all_together(){
      int total_d = 0;
      for(int i = 0; i <  rosas; i++)
        if(agents[i]->get_mass() >= 1.0)
          total_d++;
      return total_d >= rosas-1;
    }

    void solve(){
      solve_gen(false);
    }

    void solve_coords(){
      solve_gen(true);
    }

    void solve_gen(bool c){
      int i = 0;
      while(!all_together()){
        // if(agents[bigger_mass_index]->get_cost() > 0)
        //   random_move(bigger_mass_index);
        random_move(smaller_mass_index);
        update_masses();
        update_sys();

        iterate(c);
        if(!c)
          std::cout << i++ << ": " <<
            agents[bigger_mass_index]->get_cost() << std::endl;
      }
      sweep_index = bigger_mass_index;
    }


    void iterate(bool c){
      std::tuple<int, double>* s= new std::tuple<int, double>[rosas];
      for(int i = 0; i < rosas; i++)
        s[i] = std::make_tuple(i, agents[i]->get_v());
      std::sort(s, s + rosas, compare_tuples);

      if(c)
        print_coords();

      int actual_bigger = bigger_mass_index;

      for(int i = 0; i< rosas; i++){
        std::shared_ptr<Agent> agent =
          agents[std::get<0>(s[i])];
        int index = agent->get_index();
        int v = agent->get_v();
        if(v == 0)
          continue;

        for(int j = 0; j < v; j++){
          close(index, bigger_mass_index);
          update_masses();
          update_sys();
          if(actual_bigger != bigger_mass_index)
            return;
        }
      }
      delete [] s;
    }


    bool sweep(int index){
      for(int i = 0; i < size; i++){
        for(int j = i+1; j < size; j++){
          int actual_cost = agents[index]->get_cost();
          if(i == j)
            continue;
          if(actual_cost == 0)
            return false;
          agents[index]->flip(i);
          agents[index]->flip(j);
          int after_cost = agents[index]->get_cost();
          if (after_cost < actual_cost){
            std::cout << "Sweep :" <<
              agents[index]->get_cost() << std::endl;
            actual_cost = after_cost;
            return true;
          }else{
            agents[index]->flip(i);
            agents[index]->flip(j);
          }
        }
      }
      return false;
    }

    void print_coords(){
      for(int i = 0; i< rosas; i++){
        std::shared_ptr<Agent> a = agents[i];
        std::cout << res(a->get_x()) << "," <<
          res(a->get_y()) << "," << a->get_mass() << std::endl;
      }
      std::cout << std::endl;
    }

    std::string res(unsigned __int128 n) {
      n = static_cast<__int128>(n);
      if (n == 0) {
        return "0";
      }
      std::string result;
      bool isNegative = false;
      if (n < 0) {
        isNegative = true;
        n = -n;
      }
      while (n > 0) {
        char digit = '0' + static_cast<char>(n % 10);
        result = digit + result;
        n /= 10;
      }
      if (isNegative) {
        result = '-' + result;
      }
      return result;
    }

    void random_move(int i){
      std::uniform_int_distribution<int> distrib(0, size-1);
      agents[i]->random_move(distrib);
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


    void sweep_all(){
      while(sweep(sweep_index)){}
      bigger_mass_index = sweep_index;
    }



};

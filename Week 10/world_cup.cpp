// 1. Store all the data
// 2. Do a triangulation with the center of the circles and update the revenues given that
// 3. Formulate the problem with Linear Programming (Gmpq is necessary here)

#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Gmpq IT; //Here we use Gmpq because some entries are not integer
typedef CGAL::Gmpq ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(){
  //INPUT
  int n, m, c; cin >> n >> m >> c;
  vector<K::Point_2> pos;
  vector<int> supplies;
  vector<int> alcool;
  
  for(int i=0; i<n; i++){
    int x, y; cin >> x >> y;
    K::Point_2 p(x,y);
    pos.push_back(p);
    int s, a; cin >> s >> a;
    supplies.push_back(s);
    alcool.push_back(a);
  }
  
  vector<int> demands;
  vector<int> upper;
  for(int j=0; j<m; j++){
    int x, y; cin >> x >> y;
    K::Point_2 p(x,y);
    pos.push_back(p);
    int d, u; cin >> d >> u;
    demands.push_back(d);
    upper.push_back(u);
  }
  
  vector<vector<double>> revenues(n);
  for(int i=0; i<n; i++){
    for(int j=0; j<m; j++){
      double r; cin >> r;
      revenues[i].push_back(r);
    }
  }
  
  //Triangulation
  Triangulation t;
  t.insert(pos.begin(), pos.end());
  
  //Update of the revenues given the geography of the problem
  for(int k=0; k<c; k++){
    int x, y; cin >> x >> y;
    K::Point_2 p(x,y);
    long r; cin >> r;
    
    //The idea is that most circles contain nothing at all so that reduces the calculations
    Triangulation::Vertex_handle v = t.nearest_vertex(p);
    if(CGAL::squared_distance(v->point(),p)<r*r){ //The circle contains something, then we explore more
      for(int i=0; i<n; i++){ //For each warehouse
        for(int j=0; j<m; j++){ //For each stadium
          bool b1 = (CGAL::squared_distance(p, pos[i])<r*r);  //Does the circle contain this warehouse ?
          bool b2 = (CGAL::squared_distance(p, pos[n+j])<r*r);  //Or this statium ?
          if(b1!=b2){ //If one is inside and the other outside, then the path has to go through the circle
            revenues[i][j] -= 0.01; //And we reduce the revenue
          }
        }
      }
    }
  }
  //LP
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  
  int id = 0;
  for(int j=0; j<m; j++){
    for(int i=0; i<n; i++){
      lp.set_a(i*m+j, id, 1); //Because we have a strict equality, we make 2 inequalities, one here
      lp.set_a(i*m+j, id+1, -1);  //and the other here
      lp.set_a(i*m+j, id+2, alcool[i]);
    }
    lp.set_b(id, demands[j]);
    lp.set_b(id+1, -demands[j]);
    lp.set_b(id+2, 100*upper[j]);
    id += 3;
  }
  
  for(int i=0; i<n; i++){
    for(int j=0; j<m; j++){
      lp.set_a(i*m+j, id, 1);
    }
    lp.set_b(id, supplies[i]);
    id++;
  }
  
  for(int i=0; i<n; i++){
    for(int j=0; j<m; j++){
      lp.set_c(i*m+j,-revenues[i][j]); //Negative as the algorithm wants to minimize the cost function
    }
  }
  
  //OUTPUT
  Solution s = CGAL::solve_linear_program(lp, ET());
  if(s.is_infeasible()){
    cout << "RIOT!" << endl;
  }else{
    double res = CGAL::to_double(s.objective_value());
    if(res == 0){
      cout << 0 << endl;
    }else{
      cout << floor(-res) << endl;
    }
  }
  
}

int main(){
  cout << fixed << setprecision(0);
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}
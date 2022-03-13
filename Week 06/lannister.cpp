// 1. First LP problem, separating the nobles and commons and trying to minimize the total cost
// 2. If not feasible: "Yuck" If min cost is too high: "Banckrupt"
// 3. Else a solution exist, and we now do a second problem:
// The constraints are again the separation of houses, but now also the cost not exceeding the limit (we know it's possible)
// And we try to minimize the max length of water pipe (introduce a variable for the max, every length is smaller than it, and we want to minimize it)

#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef long IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(){
  //INPUT
  int n, m; cin >> n >> m;
  long s; cin >> s;
  vector<pair<int,int>> nobles;
  vector<pair<int,int>> commons;
  for(int i=0; i<n; i++){
    int x, y; cin >> x >> y;
    nobles.push_back(make_pair(x,y));
  }
  for(int i=0; i<m; i++){
    int x, y; cin >> x >> y;
    commons.push_back(make_pair(x,y));
  }
  
  //First LP problem
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  
  //The main sewage pipe is given by: -x+by+c=0 
  //a!=0 for non-horizontality and because a=-1, the nobles are on positive side and commons negative
  const int b = 0;
  const int c = 1;
  
  //Nobles on the positive side
  pair<long,long> sum_noble = make_pair(0,0); 
  for(int i=0; i<n; i++){
    lp.set_a(b, i, -nobles[i].second);
    lp.set_a(c, i, -1);
    lp.set_b(i, -nobles[i].first);
    sum_noble.first += nobles[i].first; //This quantity and the others will be used to calculate total length of small sewage pipe
    sum_noble.second += nobles[i].second;
  }
  
  //Commons on the negative side
  pair<long,long> sum_common = make_pair(0,0); 
  for(int j=0; j<m; j++){
    lp.set_a(b, n+j, commons[j].second);
    lp.set_a(c, n+j, 1);
    lp.set_b(n+j, commons[j].first);
    sum_common.first += commons[j].first; //Same
    sum_common.second += commons[j].second;
  }
  
  //Just convince yourself that this is the condition on the total length of sewage pipe (that we want to minimize here)
  //Hint: it's easy to calculate the distance between each house and the main pipe because we know exactly which are on the right (commons) or on the left (nobles)
  lp.set_c(b, sum_noble.second-sum_common.second);
  lp.set_c(c, n-m);
  lp.set_c0(-sum_noble.first+sum_common.first);
  
  Solution sol = CGAL::solve_linear_program(lp, ET());
  
  //If unfeasible it's because of the separation (only constraint)
  if(sol.is_infeasible()){
    cout << "Yuck!" << endl;
  }else{
    //If the min cost is still too high, we cannot have a solution either
    if(s!=-1 and CGAL::to_double(sol.objective_value())>s){
      cout << "Bankrupt!" << endl;
    }else{
      //Else, a solution exists, now we integrate the cost condition in the problem, and try to minimize the max length of water pipes
      Program lp2 (CGAL::SMALLER, false, 0, false, 0);
      
      //Main water pipe follows equation: -bx-y+d=0 (a=-1 and b are given because of the orthogonality with sewage pipe)
      const int d = 2;
      const int e = 3; //e is the max water pipe length
      
      //As before
      int id = 0;
      for(int i=0; i<n; i++){
        lp2.set_a(b, id, -nobles[i].second);
        lp2.set_a(c, id, -1);
        lp2.set_b(id, -nobles[i].first);
        id++;
      }
      
      //As before
      for(int j=0; j<m; j++){
        lp2.set_a(b, id, commons[j].second);
        lp2.set_a(c, id, 1);
        lp2.set_b(id, commons[j].first);
        id++;
      }
      
      //Ensures that every water pipe has a length smaller than e
      //We can calculate the length of each them but we don't know the sign of it (we get either l or -l)
      //So the condition is that this value must be between -e and e
      
      for(int i=0; i<n; i++){
        // +/- l smaller than e
        lp2.set_a(b, id, nobles[i].first);
        lp2.set_a(d, id, 1);
        lp2.set_a(e, id, -1);
        lp2.set_b(id, -nobles[i].second);
        id++;
        // +/- l higher than -e
        lp2.set_a(b, id, -nobles[i].first);
        lp2.set_a(d, id, -1);
        lp2.set_a(e, id, -1);
        lp2.set_b(id, nobles[i].second);
        id++;
      }
      //Same for commons
      for(int j=0; j<m; j++){
        lp2.set_a(b, id, commons[j].first);
        lp2.set_a(d, id, 1);
        lp2.set_a(e, id, -1);
        lp2.set_b(id, -commons[j].second);
        id++;
        lp2.set_a(b, id, -commons[j].first);
        lp2.set_a(d, id, -1);
        lp2.set_a(e, id, -1);
        lp2.set_b(id, commons[j].second);
        id++;
      }
      
      //Here again the total sum of sewage pipe, but in a condition now (smaller than s)
      if(s!=-1){
        lp2.set_a(b, id, sum_noble.second-sum_common.second);
        lp2.set_a(c, id, n-m);
        lp2.set_b(id, s+sum_noble.first-sum_common.first);
        id++;
      }
      
      //Minimize the max length of water pipe e
      lp2.set_c(e, 1);
      
      Solution s2 = CGAL::solve_linear_program(lp2, ET());
      
      //OUTPUT
      cout << ceil(CGAL::to_double(s2.objective_value())) << endl;
    }
  }
}

int main(){
  cout << fixed << setprecision(0);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}
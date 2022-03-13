// 1. LP Problem
// First variables are the coordinates (if every dimension) of the center of the ball
// Last variable is the radius (to maximize)
// That is, the center must be inside the shape (some inequalities)
// And the ball also (center + radius * normalized wall normal <= wall limit)

#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

void testcase(int n){
  //INPUT
  int d; cin >> d;
  vector<vector<int>> ineq(n);
  for(int i=0; i<n; i++){
    for(int j=0; j<d+1; j++){
      int x; cin >> x;
      ineq[i].push_back(x);
    }
  }
  
  //LP Problem
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  
  vector<int> norm;
  for(int i=0; i<n; i++){
    int y = 0;
    for(int j=0; j<d; j++){ //Center is inside the shape: as many inequalities as walls
      lp.set_a(j, i, ineq[i][j]);
      y += ineq[i][j]*ineq[i][j];
    }
    lp.set_b(i, ineq[i][d]);
    norm.push_back(sqrt(y));  //Calculate the norm of ai for later
  }
  
  
  for(int i=0; i<n; i++){ //The ball is inside the shape:
    for(int j=0; j<d; j++){
      lp.set_a(j, n+i, ineq[i][j]); //Center
    }
    lp.set_a(d, n+i, norm[i]); // + radius * normalized normal to the wall
    lp.set_b(n+i, ineq[i][d]); // doesn't cross the wall
  }
  
  lp.set_c(d, -1); //Maximize the radius
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  //OUTPUT
  if(s.is_infeasible()){
    cout << "none" << endl;
  }else if(s.is_unbounded()){
    cout << "inf" << endl;
  }else{
    cout << floor(-CGAL::to_double(s.objective_value())) << endl;
  }
}

int main(){
  ios_base::sync_with_stdio(false);
  int n; cin >> n;
  while(n!=0){
    testcase(n);
    cin >> n;
  }
}
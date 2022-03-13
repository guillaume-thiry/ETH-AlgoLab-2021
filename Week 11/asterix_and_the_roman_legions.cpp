// 1. Solve the problem with Linear Programming (use the formula given to write the inequalities)
// The first two variables (X,Y) represent the position of Asterix
// The last variable (Z) is the minimal distance with the legions, that we want to maximize

#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef long IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

void testcase(){
  //INPUT
  long xs, ys; cin >> xs >> ys;
  int n; cin >> n;
  
  //LP
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  const int X = 0;
  const int Y = 1;
  const int Z = 2;
  
  for(int i=0; i<n; i++){//for each legion 
    long a, b, c; cin >> a >> b >> c;
    long v; cin >> v;
    
    //The sign of this quantity can change but we want the absolute value
    if(a*xs+b*ys+c>0){
      lp.set_a(X,2*i,-a); lp.set_a(Y,2*i,-b); lp.set_b(2*i,c);  //Checks that the position of Asterix is on the correct side of the legion
      lp.set_a(X,2*i+1,-a); lp.set_a(Y,2*i+1,-b); lp.set_a(Z,2*i+1,v*sqrt(a*a+b*b)); lp.set_b(2*i+1,c); //Checks that the distance to the legion is smaller than Z
    }else{
      lp.set_a(X,2*i,a); lp.set_a(Y,2*i,b); lp.set_b(2*i,-c);
      lp.set_a(X,2*i+1,a); lp.set_a(Y,2*i+1,b); lp.set_a(Z,2*i+1,v*sqrt(a*a+b*b)); lp.set_b(2*i+1,-c);
    }
  }
  
  lp.set_c(Z,-1); //Distance with the legions to maximize
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  //OUTPUT
  cout << floor(-CGAL::to_double(s.objective_value())) << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}
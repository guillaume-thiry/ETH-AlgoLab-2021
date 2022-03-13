// 1. LP problem

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

int main(){
  //INPUT
  ios_base::sync_with_stdio(false);
  int n, m; cin >> n >> m;
  while(n!=0){
    
    vector<int> min_val;
    vector<int> max_val;
    
    for(int i=0; i<n; i++){
      int a, b; cin >> a >> b;
      min_val.push_back(a);
      max_val.push_back(b);
    }
    
    vector<int> prices;
    vector<vector<int>> contains(m);
    
    for(int j=0; j<m; j++){
      int x; cin >> x;
      prices.push_back(x);
      for(int k=0; k<n; k++){
        int y; cin >> y;
        contains[j].push_back(y);
      }
    }
    
    //LP Problem
    Program lp (CGAL::SMALLER, true, 0, false, 0);
    
    for(int i=0; i<n; i++){
      for(int j=0; j<m; j++){
        lp.set_a(j,2*i,contains[j][i]); 
        lp.set_a(j,2*i+1,-contains[j][i]); //For 'larger than' inequality, just put everything with negative signs
      }
      lp.set_b(2*i,max_val[i]);
      lp.set_b(2*i+1,-min_val[i]);
    }
    
    for(int j=0; j<m; j++){
      lp.set_c(j, prices[j]);
    }
    
    //OUTPUT
    Solution s = CGAL::solve_linear_program(lp, ET());
    if(s.is_infeasible()){
      cout << "No such diet." << endl;
    }else{
      cout << floor(CGAL::to_double(s.objective_value())) << endl;
    }
    cin >> n >> m;
    
  }
}
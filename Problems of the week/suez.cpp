// 1. For each new poster, determines the closest old poster in term of scaling:
// To do that, calculate Delta_x/w and Delta_y/h and take the max of the two values
// Do that for every old poster and pick the one with the smallest value
// These values represent how much we can scale up the current poster until it touches the old one (convince yourself with a drawing)
// 2. Create a Linear Programming problem where the variables are the scalings of each new poster
// 3. A new poster have constraints with the closest old poster (determined early) and all the other new posters

#include <iostream>
#include <vector>
#include <iomanip>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

void testcase(){
  //INPUT
  int n, m, h, w;
  cin >> n >> m >> h >> w;
  vector<pair<double, double>> news;
  vector<pair<double, double>> olds;
  for(int i=0; i<n; i++){
    double x, y; cin >> x >> y;
    news.push_back(make_pair(x,y));
  }
  for(int j=0; j<m; j++){
    double x, y; cin >> x >> y;
    olds.push_back(make_pair(x,y));
  }
  
  //LP problem
  Program lp (CGAL::SMALLER, true, 1, false, 0);
  int id=0;
  
  //Find the closest old poster for each new poster
  if(m!=0){
    vector<int> comp;
    for(int i=0; i<n; i++){
      vector<double> ratios;
      for(int j=0; j<m; j++){
        double r1 = abs(news[i].first-olds[j].first)/w; //Delta_x/w
        double r2 = abs(news[i].second-olds[j].second)/h; //Delta_y/h
        ratios.push_back(max(r1,r2)); //Max
      }
      //Find minimum ratio
      double c = ratios[0];
      int k = 0;
      for(int j=0; j<m; j++){
        if(ratios[j]<c){
          c = ratios[j];
          k = j;
        }
      }
      //And associate the corresponding old poster to the new one
      comp.push_back(k);
    }
    //Then set up the constraint between the poster and its corresponding old one
    for(int i=0; i<n; i++){
      int j = comp[i];
        //We have to recalculate that because now we need to know if the maximum is attained along x or along y
        double r1 = abs(news[i].first-olds[j].first)/w;
        double r2 = abs(news[i].second-olds[j].second)/h;
        
        if(r1>r2){
          lp.set_a(i, id, w);
          lp.set_b(id, 2*int(abs(news[i].first-olds[j].first))-w);  //Hit the old poster along x (which means right and left sides of posters are touching)
          id++;
        }else{
          lp.set_a(i, id, h);
          lp.set_b(id, 2*int(abs(news[i].second-olds[j].second))-h); //Hit the old poster along y
          id++;
        }
    }
  }
  //Now setup the rest of the constraints between all new ones
  for(int i=0; i<n; i++){
    for(int k=i+1; k<n; k++){ //We do that for all pairs of new posters
      //Same as before
      double r1 = abs(news[i].first-news[k].first)/w;
      double r2 = abs(news[i].second-news[k].second)/h;
      //Same
      if(r1>r2){
        lp.set_a(i, id, w);
        lp.set_a(k, id, w);
        lp.set_b(id, 2*int(abs(news[i].first-news[k].first)));
        id++;
      }else{
        lp.set_a(i, id, h);
        lp.set_a(k, id, h);
        lp.set_b(id, 2*int(abs(news[i].second-news[k].second)));
        id++;
      }
    }
  }
  //Objective function
  for(int i=0; i<n; i++){
    lp.set_c(i, -2*(h+w));
  }
  
  //OUTPUT
  Solution s = CGAL::solve_linear_program(lp, ET());
  cout << ceil(-CGAL::to_double(s.objective_value())) << endl;
  
}

int main(){
  cout << fixed << setprecision(0);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}
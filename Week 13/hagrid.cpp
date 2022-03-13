// 1. DFS on the tree: for every child, recursively determines:
//  - The total number of coins possible to obtain if starting this subtree at time 0
//  - The total number of nodes in this subtree
//  - The time it takes to explore this subtree (back and forth)
// 2. Calculate the same values for the current tree using the values of the children
//  - Choose the order of exploration of the children greedily for the ratio time_taken/number_of_nodes
// The quickest a subtree is to explore, the soonest it should be chosen (first test cases with balanced trees)
// But also important to prioritize a subtree with a lot of nodes (because there is a lot to lose if chosen too late)
// To convince yourself of this, image a order of children and two indices i and j (i<j) where ti/ni > tj/nj
// Show that it's better if you invert j and i (so take j first) while keeping all the others in the same order


#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iomanip>

using namespace std;

typedef tuple<long, long, int> elt;

elt best(int i, vector<vector<pair<int,int>>>& child, vector<long>& coins){
  //Values to calculate for this tree
  long tot_coin = coins[i];
  long tot_t = 0;
  int tot_n = 1;
  
  //Call the function on each child
  vector<pair<double, pair<long, int>>> coefs;
  int n = child[i].size();
  for(int k=0; k<n; k++){
    int j = child[i][k].first;
    elt next = best(j, child, coins);
    tot_coin += get<0>(next); //Add the coins obtained with each child
    tot_coin -= child[i][k].second*get<2>(next); //But take into account the time to reach the child first
    tot_n += get<2>(next); //Add the nodes together
    tot_t += 2*child[i][k].second; //For total time, don't forget the time to reach each child (two times)
    tot_t += get<1>(next);
    double c = double(2*child[i][k].second+get<1>(next))/get<2>(next);  //coefficients t/n for the greedy selection
    coefs.push_back(make_pair(c, make_pair(2*child[i][k].second+get<1>(next), get<2>(next))));
  }
  //Sort the coefficients for greedy approach
  sort(coefs.begin(), coefs.end());
  //Now take the children in the greedy order
  //Keep the count of the time taken and substract this to the next children coins (with number of nodes)
  long cum_t = 0;
  for(int k=0; k<n; k++){
    tot_coin -= cum_t*coefs[k].second.second; //what has been lost for this child while exploring the previous ones
    cum_t += coefs[k].second.first; //update of the cumulative time
  }
  
  elt e(tot_coin, tot_t, tot_n);
  return e;
}

void testcase(){
  //INPUT
  int n; cin >> n;
  vector<long> coins = {0};
  for(int i=0; i<n; i++){
    long x; cin >> x;
    coins.push_back(x);
  }
  vector<vector<pair<int, int>>> child(n+1);
  for(int i=0; i<n; i++){
    int u, v, l; cin >> u >> v >> l;
    child[u].push_back(make_pair(v,l));
  }
  
  //Greedy solution
  elt res = best(0, child, coins);
  
  //OUTPUT
  cout << fixed << setprecision(0) << get<0>(res) << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}
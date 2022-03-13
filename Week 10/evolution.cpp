// 1. Store all the data in different datastructures
// 2. Find the root (LUCA)
// 3. Explore the tree with a DFS, storing the current path, and answer all the queries for a given member at once


#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

//Binary Search on the ancestors path to find the answering corresponding to age a
int bs(vector<int>& vec, vector<int>&ages, int a){
  int j = vec.size()-1;
  int i = 0;
  while(i!=j){
    int m = (i+j)/2;
    if(ages[vec[m]]>a){
      i = m+1;
    }else{
      j = m;
    }
  }
  return vec[i];
}

void answer(vector<int>& path, int i, vector<int>& res, vector<vector<int>>& child, vector<int>& ages, vector<vector<pair<int,int>>>& queries){
  //First answer all the queries for species i
  int nq = queries[i].size();
  for(int k=0; k<nq; k++){
    int a = queries[i][k].first;
    int j = queries[i][k].second;
    res[j] = bs(path, ages, a);
  }
  //Then propagate to children (DFS)
  int nc = child[i].size();
  for(int c=0; c<nc; c++){
    path.push_back(child[i][c]); //Update the path for this child by adding it
    answer(path, child[i][c], res, child, ages, queries); //And recursively call the function 'answer' on it
    path.pop_back(); //And take it out of the path (backtracking)
  }
}

void testcase(){
  //INPUT 
  int n, q; cin >> n >> q;
  unordered_map<string, int> species;
  vector<string> reverse; //Datastructure to retrieve the name of the species (given an index) at the end
  vector<int> ages;
  
  for(int i=0; i<n; i++){
    string s; cin >> s;
    int a; cin >> a;
    species.insert(make_pair(s,i));
    reverse.push_back(s);
    ages.push_back(a);
  }
  vector<vector<int>> child(n);
  vector<int> parents(n,0); //This vector is useful to find the root (only one without parent)
  for(int i=0; i<n-1; i++){
    string x, y; cin >> x >> y;
    child[species[y]].push_back(species[x]);
    parents[species[x]] = 1;
  }
  //Find LUCA
  int root = 0;
  for(int i=0; i<n; i++){
    if(parents[i] == 0){
      root = i;
    }
  }
  
  vector<vector<pair<int,int>>> queries(n); //Store the queries by member of the tree (to answer all of them at the same time)
  for(int j=0; j<q; j++){
    string s; cin >> s;
    int b; cin >> b;
    queries[species[s]].push_back(make_pair(b, j));
  }
  //Result vector
  vector<int> res(q,0);
  
  vector<int> path = {root}; //Store the current path in the DFS
  answer(path, root, res, child, ages, queries); //DFS to answer the queries
  
  //OUTPUT
  for(int j=0; j<q; j++){
    cout << reverse[res[j]] << " ";
  }
  cout << endl;
}

int main(){
  int t; cin >> t;
  while(t--){
    testcase();
  }
}
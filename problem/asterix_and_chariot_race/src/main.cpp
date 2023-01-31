#include <iostream>
#include <vector>
#include <limits>

struct state {
  int marked;  // minimum cost for this subtree, given that the root itself is marked
  int covered; // minimum cost for this subtree, given that the root is covered for sure (i.e. marked or covered by child)
  int uncovered;     // minimum cost for this subtree, given that every child except the root must be covered, the root may or may not be covered
  // invariant:  uncovered <= covered <= marked
};

void dfs(int u, 
      std::vector<struct state> & T,
      std::vector<int> const& c,
      std::vector<std::vector<int>> const& children) {

  struct state sum = {0, 0, 0};
  
  // what is the cost for at least one child of v to be marked?
  int cover_cost = std::numeric_limits<int>::max();
  
  for (int v: children[u]) {
    dfs(v, T, c, children);
    
    cover_cost = std::min(cover_cost, T[v].marked - T[v].covered);

    sum.marked += T[v].marked;
    sum.covered += T[v].covered;
    sum.uncovered += T[v].uncovered;
  }
  
  T[u].marked = c[u] + sum.uncovered;
  T[u].covered = std::min(T[u].marked, sum.covered + cover_cost);
  T[u].uncovered = std::min(T[u].covered, sum.covered);
}

void testcase() {
  int n;
  std::cin >> n;
  
  std::vector<std::vector<int>> children(n);
  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    std::cin >> u >> v;
    children[u].push_back(v);
  }
  
  std::vector<int> c(n);
  for (int i = 0; i < n; ++i)
    std::cin >> c[i];
    
  std::vector<struct state> T(n);
  dfs(0, T, c, children);
  
  std::cout << T[0].covered << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}

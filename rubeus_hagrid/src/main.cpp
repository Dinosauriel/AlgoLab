#include <iostream>
#include <vector>
#include <climits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> Graph;
typedef boost::graph_traits<Graph>::edge_iterator edge_it;
typedef boost::graph_traits<Graph>::out_edge_iterator out_edge_it;


// struct vert_profile {
//   int id;
//   int tree_length;
//   int tree_weight;
//   int g;
// };

int tree_weight(int v, std::vector<int> & TW, Graph const& graph) {
  int w = 1;
  auto neighbours = boost::adjacent_vertices(v, graph);
  for (auto n_it = neighbours.first; n_it != neighbours.second; ++n_it) {
    w += tree_weight(*n_it, TW, graph);
  }
  TW[v] = w;
  return w;
}

int tree_length(int v, std::vector<int> const& L, std::vector<int> & TL, Graph const& graph) {
  int length = L[v];
  auto neighbours = boost::adjacent_vertices(v, graph);
  for (auto n_it = neighbours.first; n_it != neighbours.second; ++n_it) {
    length += tree_length(*n_it, L, TL, graph);
  }
  TL[v] = length;
  return length;
}


long value(int v, int delay, std::vector<int> const& L, std::vector<int> const& TL, std::vector<int> const& TW, std::vector<int> const& G, Graph const& graph) {
  long val = G[v] - delay;
  
  auto neighbours = boost::adjacent_vertices(v, graph);
  std::set<int> candidates;
  for (auto n_it = neighbours.first; n_it != neighbours.second; ++n_it)
    candidates.insert(*n_it);
    
    
  int w = TW[v] - 1;
  while (candidates.size() > 0) {

    // find the neighbour that will incur the least cost
    long best_cost = LONG_MAX;
    int u = 0;
    for (auto c_it = candidates.begin(); c_it != candidates.end(); ++c_it) {
      long cost = (w - TW[*c_it]) * 2 * TL[*c_it];
      if (cost < best_cost) {
        best_cost = cost;
        u = *c_it;
      }
    }
    
    // std::cout << "value(" << v << ", " << delay << "): u = " 
    // << u << " with weight " << TW[u] << " and length " 
    // << TL[u] << " has best cost " << best_cost << std::endl;

    candidates.erase(u);
    w -= TW[u];
    
    // calculate the value of the selected subtree
    long val_u = value(u, delay + L[u], L, TL, TW, G, graph);
    val += val_u;
    delay += 2 * TL[u];
  }
  
  return val;
}

void testcase() {
  int n;
  std::cin >> n;

  std::vector<int> G(n + 1, 0);
  
  for (int i = 0; i < n; ++i) {
    std::cin >> G[i + 1];
  }
  
  std::vector<int> L(n + 1, 0);
  Graph graph(n + 1);

  for (int i = 0; i < n; ++i) {
    int u, v, l;
    std::cin >> u >> v >> l;
    boost::add_edge(u, v, graph);
    L[v] = l;
  }
  
  std::vector<int> TL(n + 1);
  tree_length(0, L, TL, graph);
  
  std::vector<int> TW(n + 1);
  tree_weight(0, TW, graph);
  
  std::cout << value(0, 0, L, TL, TW, G, graph) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}
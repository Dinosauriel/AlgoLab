#include <iostream>
#include <vector>
#include <climits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> Graph;
typedef boost::graph_traits<Graph>::edge_iterator edge_it;
typedef boost::graph_traits<Graph>::out_edge_iterator out_edge_it;


struct vert_profile {
  unsigned long id;
  int tree_length;
  int tree_weight;
  int g;
};

bool vert_profile_less_than(struct vert_profile const& a, struct vert_profile const& b) {
  if (a.tree_weight == b.tree_weight) return a.tree_length < b.tree_length;
  return (a.tree_length * b.tree_weight) < (b.tree_length * a.tree_weight);
}

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
  
  auto adj = boost::adjacent_vertices(v, graph);
  std::vector<struct vert_profile> N;
  for (auto n_it = adj.first; n_it != adj.second; ++n_it) {
    struct vert_profile profile {
      *n_it,
      TL[*n_it],
      TW[*n_it],
      G[*n_it]
    };
    N.push_back(profile);
  }

  std::sort(N.begin(), N.end(), vert_profile_less_than);

  int w = TW[v] - 1;
  for (auto n_it = N.begin(); n_it != N.end(); ++n_it) {
    int u = (*n_it).id;

    // std::cout << "value(" << v << ", " << delay << "): u = " 
    // << u << " with weight " << TW[u] << " and length " 
    // << TL[u] << " has best cost " << best_cost << std::endl;

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
  
  // std::cout << "calculating length" << std::endl;

  std::vector<int> TL(n + 1);
  tree_length(0, L, TL, graph);

  // std::cout << "calculating weight" << std::endl;
  
  std::vector<int> TW(n + 1);
  tree_weight(0, TW, graph);

  // std::cout << "calculating value" << std::endl;
  
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
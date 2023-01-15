#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

#include <iostream>
#include <vector>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

void add_edge(graph &G, int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
}

void testcase() {
  int n, m;
  std::cin >> n >> m;
  
  std::vector<int> b(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> b[i];
  }
  
  graph G(n + 2);
  
  int source = n;
  int sink = n + 1;
  
  int threshhold = 0;
  
  for (int i = 0; i < n; ++i) {
    if (b[i] > 0) {
      add_edge(G, source, i, b[i]);
      threshhold += b[i];
    } else {
      add_edge(G, i, sink, -b[i]);
    }
  }
  
  for (int k = 0; k < m; ++k) {
    int i, j, d;
    std::cin >> i >> j >> d;
    add_edge(G, i, j, d);
  }
  
  long flow = boost::push_relabel_max_flow(G, source, sink);
  
  if (threshhold > flow) {
    std::cout << "yes" << std::endl;
    return;
  }
  std::cout << "no" << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}

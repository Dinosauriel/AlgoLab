#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

struct road {
  int a;
  int b;
  int c;
  int d;
};


void testcase() {
  int n, m, k, x, y;
  std::cin >> n >> m >> k >> x >> y;
  
  // std::cout << n << " " << m << " " << x << " " << k << std::endl;
  
  std::vector<struct road> R(m);
  
  for (int i = 0; i < m; ++i)
    std::cin >> R[i].a >> R[i].b >> R[i].c >> R[i].d;
  
  weighted_graph G((k + 1) * n);
  weight_map weights = boost::get(boost::edge_weight, G);
  
  int source = x;
  int target = k * n + y;

  for (int i = 0; i < m; ++i) {
    struct road r = R[i];
    
    for (int j = 0; j < k + 1; ++j) {
      int offset = n * j;
      edge_desc e;
      e = boost::add_edge(offset + r.a, offset + r.b, G).first;
      weights[e] = r.c;
      e = boost::add_edge(offset + r.b, offset + r.a, G).first;
      weights[e] = r.c;
      
      if (r.d == 1 && j < k) {
        int nxt_offset = n * (j + 1);
        e = boost::add_edge(offset + r.a, nxt_offset + r.b, G).first;
        weights[e] = r.c;
        e = boost::add_edge(offset + r.b, nxt_offset + r.a, G).first;
        weights[e] = r.c;
      }
    }
  }
  
  std::vector<int> dist((k + 1) * n);
  boost::dijkstra_shortest_paths(G, source,
  boost::distance_map(boost::make_iterator_property_map(
    dist.begin(), boost::get(boost::vertex_index, G))));

  std::cout << dist[target] << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}

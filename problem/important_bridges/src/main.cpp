#include <iostream>
#include <vector>
#include <algorithm>

// BGL includes 
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>


namespace boost
{
struct edge_component_t
{
    enum
    {
        num = 555
    };
    typedef edge_property_tag kind;
} edge_component;
}
// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_component_t, std::size_t>> graph;
typedef boost::graph_traits<graph>::vertex_descriptor    vertex_desc;    // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  
typedef boost::graph_traits<graph>::edge_iterator    edge_it;    // to iterate over all edges


void testcase() {
  int n, m;
  std::cin >> n >> m;
  
  std::vector<std::pair<int, int>> B(m);
  
  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    B[i] = std::make_pair(u, v);
  }

  graph G(n);
  for (int i = 0; i < m; ++i) {
    boost::add_edge(B[i].first, B[i].second, G);
  }
  
  boost::property_map<graph, boost::edge_component_t>::type component = boost::get(boost::edge_component, G);
  int nc = boost::biconnected_components(G, component);
  
  std::vector<int> comp_size(nc);
  boost::graph_traits<graph>::edge_iterator ei, ei_end;
  for (boost::tie(ei, ei_end) = boost::edges(G); ei != ei_end; ++ei) {
    comp_size[component[*ei]] += 1;
  }
    
    
  // #critical bridges = #biconnected components of size 1
  int ncrit = 0;
  for (int i = 0; i < nc; ++i) {
    if (comp_size[i] == 1)
      ++ncrit;
  }
  
  
  std::vector<std::pair<int, int>> critical;
  critical.reserve(ncrit);

  for (boost::tie(ei, ei_end) = boost::edges(G); ei != ei_end; ++ei) {
    if (comp_size[component[*ei]] == 1) {
      int u = boost::source(*ei, G);
      int v = boost::target(*ei, G);
      if (u > v)
        std::swap(u, v);
      critical.push_back(std::make_pair(u, v));
    }
  }
  
  std::sort(critical.begin(), critical.end());
  
  std::cout << ncrit << std::endl;
  for (auto b: critical)
    std::cout << b.first << " " << b.second << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}

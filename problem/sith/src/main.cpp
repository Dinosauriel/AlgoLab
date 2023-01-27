#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>    Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb>           Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                 Triangulation;
typedef Triangulation::Edge_iterator                            Edge_iterator;
typedef Triangulation::Vertex_handle                            Vertex_handle;

int dfs(int v, std::vector<std::vector<int>> const& G, std::vector<bool> & visited) {
  if (visited[v])
    return 0;
  visited[v] = true;
  int size = 1;
  
  for (size_t i = 0; i < G[v].size(); ++i) {
    size += dfs(G[v][i], G, visited);
  }

  return size;
}

void testcase() {
  int n, r;
  std::cin >> n >> r;
  K::FT r_sq = K::FT(r) * K::FT(r);
  
  std::vector<std::pair<K::Point_2, int>> p(n);
  for (int i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    p[i] = std::make_pair(K::Point_2(x, y), i);
  }
  
  // binary search for k
  int lo = 0;
  int hi = n - 1;
  
  while (lo != hi) {
    int k = (lo + hi) / 2 + 1;
    // std::cout << "(" << lo << ", " << hi << ") -> " << k << std::endl;
    // m = size of graph on day k
    int m = n - k;
    
    Triangulation t;
    t.insert(p.begin() + k, p.end());
    
    // construct graph from triangulation
    std::vector<std::vector<int>> G(m);
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
      Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
      Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
      
      if (CGAL::squared_distance(v1->point(), v2->point()) <= r_sq) {
        
        int i1 = v1->info() - k;
        int i2 = v2->info() - k;
        // std::cout << "edge " << i1 << " - " << i2 << std::endl;
        G[i1].push_back(i2);
        G[i2].push_back(i1);
      }
    }
    
    // std::cout << "searching components" << std::endl;
    
    // search for largest connected component
    int max_component_size = 0;
    std::vector<bool> visited(m, false);
    for (int i = 0; i < m; ++i) {
      if (visited[i])
        continue;
      int component_size = dfs(i, G, visited);
      max_component_size = std::max(max_component_size, component_size);
    }
    
    // std::cout << "largest component has size " << sz << std::endl;
    
    if (max_component_size < k) {
      hi = k - 1;
    } else {
      lo = k;
    }
  }
  
  std::cout << lo << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}

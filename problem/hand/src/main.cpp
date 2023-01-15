#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/number_utils.h>


typedef int Color;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<Color, K> Vb; // add color attribute to vertices
typedef CGAL::Triangulation_face_base_2<K> Fb; // unchanged face base
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds; // merge into data structure

typedef CGAL::Delaunay_triangulation_2<K, Tds>     Triangulation; // with custom datastructure
typedef Triangulation::Finite_faces_iterator  Face_iterator;
typedef Triangulation::Finite_edges_iterator  Edge_iterator;
typedef Triangulation::Finite_vertices_iterator  Vertex_iterator;

struct edge {
  Triangulation::Edge inst;
  K::FT squared_length;
};

bool edg_less_than(struct edge const& a, struct edge const& b) {
  return a.squared_length < b.squared_length;
}

int tent_groups_possible(std::vector<int> const& connected_subgraphs, int k) {
  if (k == 1) {
    return connected_subgraphs[1];
  }
  
  if (k == 2) {
    return connected_subgraphs[2] + (connected_subgraphs[1] / 2);
  }
  
  std::vector<int> S = connected_subgraphs;
  if (k == 3) {
    int res = S[3];
    
    int m = std::min(S[1], S[2]);
    res += m;
    
    S[1] -= m;
    S[2] -= m;
    
    // either S[1] == 0 or S[2] == 0
    res += S[1] / 3;
    res += S[2] / 2;
    return res;
  }
  
  if (k == 4) {
    int res = S[4];

    int m = std::min(S[3], S[1]);
    res += m;
    
    S[3] -= m;
    S[1] -= m;
    
    // either S[1] == 0 or S[3] == 0
    if (S[3] == 0) {
      res += (S[2] + S[1]) / 4;
      return res;
    }
    // S[1] == 0
    m = std::min(S[3], S[2]);
    res += m;
    
    S[3] -= m;
    S[2] -= m;
    
    res += S[3] / 2;
    res += S[2] / 2;
    return res;
  }
  
  return 0;
}

void test_case() {
  int n, k, f0;
  K::FT s0;

  std::cin >> n;
  std::cin >> k;
  std::cin >> f0;
  std::cin >> s0;

  std::vector<K::Point_2> pts;
  pts.reserve(n);
  
  for (int i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x;
    std::cin >> y;
    pts.push_back(K::Point_2(x, y));
  }
  
  Triangulation t;
  t.insert(pts.begin(), pts.end());
  
  // color vertices
  int color = 0;
  for (auto v = t.vertices_begin(); v != t.vertices_end(); ++v) {
    v->info() = color;
    ++color;
  }
  
  std::vector<struct edge> edges;
  edges.reserve(t.finite_edges().size());
  
  for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    edges.push_back({*e, t.segment(e).squared_length()});
  }

  std::sort(edges.begin(), edges.end(), edg_less_than);
  
  std::vector<int> connected_subgraphs(k + 1);
  connected_subgraphs[1] = n;
  
  long res_s = 0;
  int res_f = 1;

  for (std::size_t i = 0; i < edges.size(); ++i) {
    int f_possible = tent_groups_possible(connected_subgraphs, k);
    // f_possible will monotonically sink
    // edges[i].squared_length will monotonically rise
      
    if (f_possible >= f0) {
      res_s = edges[i].squared_length;
    }
      
    if (edges[i].squared_length >= s0) {
      res_f = std::max(f_possible, res_f);
    }

    // std::cout << "edge " << i << ", squared_length = " << edges[i].squared_length << std::endl;
    Triangulation::Edge e = edges[i].inst;
    Triangulation::Vertex_handle v1 = e.first->vertex((e.second + 1) % 3);  
    Triangulation::Vertex_handle v2 = e.first->vertex((e.second + 2) % 3);
    
    // std::cout << ", color(v1) = " << v1->info() << ", color(v2) = " << v2->info() << std::endl;
    // std::cout << "families possible before merge: " << f_possible << std::endl;
    
    if (v1->info() != v2->info()) {
      //merge the two subgraphs
      Color c1 = v1->info();
      Color c2 = v2->info();
      Color new_color = std::min(c1, c2);
      
      int s1_size = 0;
      int s2_size = 0;
      
      //update vertex colors
      for (Vertex_iterator vit = t.finite_vertices_begin(); vit != t.finite_vertices_end(); ++vit) {
        if (vit->info() == c1) {
          ++s1_size;
          vit->info() = new_color;
        }
        if (vit->info() == c2) {
          ++s2_size;
          vit->info() = new_color;
        }
      }
      
      //update connected_subgraph structure
      //remove previous subgraphs
      connected_subgraphs[std::min(k, s1_size)] -= 1;
      connected_subgraphs[std::min(k, s2_size)] -= 1;
      //add newly merged subgraph
      connected_subgraphs[std::min(k, s1_size + s2_size)] += 1;
    }
    // std::cout << *v1 << " info: " << v1->info() << " " << *v2 << " info: " << std::endl;
  }
  // std::cout << "-----" << std::endl;
  std::cout << res_s << " " << res_f << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    test_case();
  }
}

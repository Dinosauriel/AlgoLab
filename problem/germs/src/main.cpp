#include <iostream>
#include <vector>
#include <algorithm>

// #include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/nearest_neighbor_delaunay_2.h>

// typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;

typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_iterator  Vertex_iterator;

void testcase(int n) {
  K::FT l, b, r, t;
  std::cin >> l >> b >> r >> t;
  
  std::vector<std::pair<K::Point_2, int>> G(n);
  for (int i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    G[i] = std::make_pair(K::Point_2(x, y), i);
  }

  std::vector<double> crit_time(n);
  
  for (int i = 0; i < n; ++i) {
    K::FT d1 = G[i].first.x() - l;
    K::FT d2 = r - G[i].first.x();
    K::FT d3 = G[i].first.y() - b;
    K::FT d4 = t - G[i].first.y();
    K::FT d = std::min(d1, std::min(d2, std::min(d3, d4)));
    
    crit_time[i] = CGAL::to_double(CGAL::sqrt(d - 0.5));
  }
  
  // construct triangulation
  Triangulation triang;
  triang.insert(G.begin(), G.end());

  for (Vertex_iterator v_i = triang.finite_vertices_begin(); v_i != triang.finite_vertices_end(); ++v_i) {

    auto v = v_i->handle();
    auto nearest_v = CGAL::nearest_neighbor(triang, v);
    
    if (nearest_v != nullptr) {
      // converting squared distance to double before taking the square root
      // we could also use the CGAL::sqrt function for exact results, but that is much,
      // much slower (test 3 and 4 will fail).
      double d_sq = CGAL::to_double(CGAL::squared_distance(v->point(), nearest_v->point()));
      double d = std::sqrt(d_sq) / 2;
      double t = std::sqrt(d - 0.5);
      
      crit_time[v->info()] = std::min(crit_time[v->info()], t);
    }
  }
  
  std::sort(crit_time.begin(), crit_time.end());
  
  long f = std::ceil(crit_time[0]);
  long m = std::ceil(crit_time[n / 2]);
  long ll = std::ceil(crit_time[n - 1]);
  
  std::cout << f << " " << m << " " << ll << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n;
  std::cin >> n;
  while (n != 0) {
    testcase(n);
    std::cin >> n;
  }
}

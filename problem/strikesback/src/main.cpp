#include <iostream>
#include <vector>
#include <climits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;

typedef CGAL::Gmpq InputType;
typedef CGAL::Gmpq ExactType;

struct asteroid {
  K::Point_2 coords;
  int density;
};

void test_case() {
  int a, s, b, e;
  std::cin >> a >> s >> b >> e;
  
  std::vector<struct asteroid> A;
  std::vector<K::Point_2> S;
  std::vector<K::Point_2> B;
  
  for (int i = 0; i < a; ++i) {
    int x, y, d;
    std::cin >> x >> y >> d;
    A.push_back({K::Point_2(x, y), d});
  }
  
  for (int i = 0; i < s; ++i) {
    int x, y;
    std::cin >> x >> y;
    S.push_back(K::Point_2(x, y));
  }
  
  for (int i = 0; i < b; ++i) {
    int x, y;
    std::cin >> x >> y;
    B.push_back(K::Point_2(x, y));
  }
  
  // -1 means infinite radius
  std::vector<K::FT> R(s, -1);
  if (b > 0) {
    Triangulation bt;
    bt.insert(B.begin(), B.end());
  
    for (int i = 0; i < s; ++i) {
      K::Point_2 p = (bt.nearest_vertex(S[i]))->point();
      R[i] = CGAL::squared_distance<K>(p, S[i]);
    }
  }
  
  // std::cout << "R: ";
  // for (int i = 0; i < s; ++i) {
  //   std::cout << R[i] << ", ";
  // }
  // std::cout << std::endl;
  
  std::vector<std::vector<K::FT>> impact(s, std::vector<K::FT>(a, 0));
  
  for (int i = 0; i < s; ++i) {
    for (int j = 0; j < a; ++j) {
      K::FT dist = CGAL::squared_distance<K>(S[i], A[j].coords);
      if (dist < R[i] || R[i] == -1) {
        //asteroid is within radius
        //calculate explosion impact factor
        impact[i][j] = K::FT(1) / CGAL::max(K::FT(1), dist);
      }
    }
  }
  
  // create linear program
  CGAL::Quadratic_program<InputType> lp(CGAL::LARGER, true, 0, false, 0); 
  
  // no objective function
  // variables: e_0, ..., e_s-1
  for (int i = 0; i < s; ++i) {
    lp.set_c(i, 1);
  }

  // e_0 + ... + e_s-1 <= e
  for (int i = 0; i < s; ++i) {
    lp.set_a(i, 0, -1);
  }
  lp.set_b(0, -e);
  
  std::cout.precision(10);
  for (int i = 0; i < a; ++i) {
  
    // sum(e_j * impact[j][i]) >= density[i]
    for (int j = 0; j < s; ++j) {
      if (impact[j][i] > 0) {
        lp.set_a(j, i + 1, impact[j][i]);
      }
      // std::cout << (int) (impact[j][i] * e) << " ";
    }
    
    lp.set_b(i + 1, A[i].density);
    
    // std::cout << " >= " << A[i].density << std::endl;
  }

  CGAL::Quadratic_program_solution<ExactType> solution = CGAL::solve_linear_program(lp, ExactType());

  if (solution.is_infeasible()) {
    std::cout << "n" << std::endl;
    return;
  }

  std::cout << "y" << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    test_case();
  }
}

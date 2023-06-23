#include "core.hpp"

std::vector<std::vector<int>> read_graph() {
  int n;
  std::cout << "Input number of vertices:\n";
  std::cin >> n;
  std::cout << "Input adjacency matrix with zeros on the diagonal, -1 if there is no edge:\n";
  std::vector<std::vector<int>> res(n, std::vector<int>(n));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cin >> res[i][j];
      if (res[i][j] < 0) {
        res[i][j] = INF;
      }
    }
  }
  return res;
}

std::vector<std::vector<std::pair<int, int>>> read_components() {
  int c;
  std::cout << "Input components count:\n";
  std::cin >> c;
  std::vector<std::vector<std::pair<int, int>>> res(c);
  for (int i = 0; i < c; ++i) {
    int sz;
    std::cout << "Input number of edges in component:\n";
    std::cin >> sz;
    std::cout << "Input edges as pairs of space-separated vertices:\n";
    res[i].resize(sz);
    for (int j = 0; j < sz; ++j) {
      std::cin >> res[i][j].first >> res[i][j].second;
    }
  }
  return res;
}

int main() {
  // read data
  auto gr = read_graph();
  auto components = read_components();
  // Floyd algorithm to find dist between every pair
  floyd(gr);
  // count vertex degrees
  auto degrees = get_degrees(gr.size(), components);
  // find (c-1)
  auto components_vertex = get_components_vertex(components);
  int ans = INF;
  std::vector<std::pair<int, int>> res(components_vertex.size());
  choose(components_vertex, gr, 1, res, degrees, ans);
  // add initial
  for (const auto& c : components) {
    for (auto [u, v] : c) {
      ans += gr[u][v];
    }
  }
  std::cout << "Min weight cycle: "  << ans << '\n';
  return 0;
}

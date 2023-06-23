#include <iostream>
#include <vector>
#include <unordered_set>
#define INF 100000007

std::vector<std::vector<int>> read_graph() {
  int n;
  std::cin >> n;
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
  std::cin >> c;
  std::vector<std::vector<std::pair<int, int>>> res(c);
  for (int i = 0; i < c; ++i) {
    int sz;
    std::cin >> sz;
    res[i].resize(sz);
    for (int j = 0; j < sz; ++j) {
      std::cin >> res[i][j].first >> res[i][j].second;
    }
  }
  return res;
}


std::vector<std::vector<int>> make_graph(int n, int p) {
  int max_dist = 10000;
  std::vector<std::vector<int>> res(n, std::vector<int>(n, 0));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < i; ++j) {
      int r = rand() % max_dist;
      res[i][j] = (r < max_dist * p / 100) ? INF : r;
      res[j][i] = res[i][j];
    }
  }
  return res;
}

std::vector<std::vector<std::pair<int, int>>> make_components(int c, std::vector<std::vector<int>>& gr) {
  std::vector<std::vector<std::pair<int, int>>> res(c);
  std::vector<int> vertex_to_comp(gr.size(), -1);
  int count = 0;
  for (int i = 0; i < gr.size() * 100; ++i) {
    int v = rand() % gr.size();
    int u = rand() % gr.size();
    if (u != v && gr[u][v] < INF && vertex_to_comp[u] == -1 && vertex_to_comp[v] == -1) {
      res[count].emplace_back(u, v);
      vertex_to_comp[u] = count;
      vertex_to_comp[v] = count;
      ++count;
      if (count == c) break;
    }
  }

  for (int j = 0; j < gr.size() * 100; ++j) {
    int v = rand() % gr.size();
    int u = rand() % gr.size();
    if (u != v && gr[u][v] < INF && ((vertex_to_comp[u] == -1) ^ (vertex_to_comp[v] == -1))) {
      int comp = std::max(vertex_to_comp[u], vertex_to_comp[v]);
      res[comp].emplace_back(u, v);
      vertex_to_comp[u] = comp;
      vertex_to_comp[v] = comp;
    }
  }

  return res;
}

std::vector<int> get_degrees(int n, const std::vector<std::vector<std::pair<int, int>>>& components) {
std::vector<int> res(n, 0);
for (const auto& c : components) {
for (auto [u, v] : c) {
++res[u];
++res[v];
}
}
return res;
}

std::vector<std::vector<int>> get_components_vertex(const std::vector<std::vector<std::pair<int, int>>>& components) {
std::vector<std::vector<int>> res(components.size());
for (int i = 0; i < components.size(); ++i) {
std::unordered_set<int> set;
for (auto [u, v] : components[i]) {
set.insert(u);
set.insert(v);
}
for (int v : set) {
res[i].push_back(v);
}
}
return res;
}

void floyd(std::vector<std::vector<int>>& gr) {
  size_t n = gr.size();
  for (int k=0; k<n; ++k)
    for (int i=0; i<n; ++i)
      for (int j=0; j<n; ++j)
        gr[i][j] = std::min(gr[i][j], gr[i][k] + gr[k][j]);
}

int kun(const std::vector<std::vector<int>>& gr, const std::vector<int>& degrees) {
  std::vector<std::vector<int>> a(gr.size() + 1, std::vector<int>(gr.size() + 1));
  for (int i = 1; i < a.size(); ++i) {
    for (int j = 1; j < a.size(); ++j) {
      if (i == j) a[i][j] = INF;
      else a[i][j] = ((((degrees[i-1] % 2) == 0) || ((degrees[j-1] % 2) == 0)) ? INF : gr[i-1][j-1]);
    }
  }
  int n, m;
  n = m = a.size();
  std::vector<int> u(n), v(m), p(m), way(m);
  for (int i = 1; i < n; ++i) {
    p[0] = i;
    int j0 = 0;
    std::vector<int> minv(m, INF + 1);
    std::vector<char> used(m, false);
    do {
      used[j0] = true;
      int i0 = p[j0], delta = INF + 1, j1;
      for (int j = 1; j < m; ++j)
        if (!used[j]) {
          int cur = a[i0][j] - u[i0] - v[j];
          if (cur < minv[j])
            minv[j] = cur, way[j] = j0;
          if (minv[j] < delta)
            delta = minv[j], j1 = j;
        }
      for (int j = 0; j < m; ++j)
        if (used[j])
          u[p[j]] += delta, v[j] -= delta;
        else
          minv[j] -= delta;
      j0 = j1;
    } while (p[j0] != 0);
    do {
      int j1 = way[j0];
      p[j0] = p[j1];
      j0 = j1;
    } while (j0);
  }
  int ans = 0;
  for (int j=1; j<m; ++j)
    ans += (a[p[j]][j] == INF ? 0 : a[p[j]][j]);
  return ans / 2;
}

void choose(const std::vector<std::vector<int>>& cv, const std::vector<std::vector<int>>& gr, int i, std::vector<std::pair<int, int>> res, std::vector<int>& degrees, int& ans) {
  if (i == cv.size()) {
    int cur = 0;
    for (auto [u, v] : res)
      cur += gr[u][v];
    cur += kun(gr, degrees);
    ans = std::min(ans, cur);
    return;
  }
  for (int u = 0; u < cv[i].size(); ++u) {
    for (int j = 0; j < i; ++j) {
      for (int v = 0; v < cv[j].size(); ++v) {
        res[i] = {u, v};
        ++degrees[u];
        ++degrees[v];
        choose(cv, gr, i + 1, res, degrees, ans);
        --degrees[u];
        --degrees[v];
      }
    }
  }
}
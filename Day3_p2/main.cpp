#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <assert.h>
#include <map>
#include <tuple>
#include <ranges>
#include <algorithm>
#include "../demangle.h"

using fmt::print, fmt::format;
using std::ifstream, std::getline;
using std::stringstream, std::string;
using std::vector, std::map, std::tuple;

char what(char c) {
  // 0 na, 1 num, 2 pn flag
  if ('.' == c) return 0;
  if ((c >= 0x30) && (c <= 0x39)) return 1;
  if ('*' == c) return 2;
  return 3;
}

int keep_id(vector<int> a) {
  for (size_t i = 0; i < a.size(); i++)
    if (a[i] > 0) return a[i];
  return 0;
}

string b2s(vector<bool> v) {
  auto len = v.size();
  string s(len, ' ');
  for (size_t i = 0; i < len; i++) s[i] = v[i] ? 'X' : '_';
  return s;
}

string i2s(vector<int> v) {
  auto len = v.size();
  string s(len, ' ');
  for (size_t i = 0; i < len; i++) s[i] = (v[i] == 0) ? '_' : v[i] + 0x30;
  return s;
}

int main() {
  // ifstream infile{"input_test_3"};
  // input3_test_3 = 467835
  ifstream infile{"input3"};
  // input3 = 84584891

  // load and make PN an is_num arrays
  vector<string> schem{};
  vector<vector<bool>> is_pn{};
  vector<vector<int>> is_pn_2{};
  vector<vector<int>> is_pn_3{};
  vector<vector<int>> is_num{};
  vector<vector<int>> is_num_2{};

  for (string line; getline(infile, line);) {
    auto len = line.size();

    schem.push_back(line);

    vector<bool> pn(len);
    vector<int> pn_2(len);
    vector<int> pn_3(len);
    vector<int> num(len);
    vector<int> num_2(len);

    for (size_t i = 0; i < len; i++) {
      auto s = line[i];
      num[i] = (what(s) == 1) ? 1 : 0;
      pn[i] = (what(s) == 2);
    }

    is_num.push_back(num);
    is_num_2.push_back(num_2);
    is_pn.push_back(pn);
    is_pn_2.push_back(pn_2);
    is_pn_3.push_back(pn_3);
  }

  // is_num_2 ← increment number shadow for each number
  for (size_t si = 0; si < is_num.size(); si++) {
    auto line = is_num[si];
    auto len = line.size();
    for (size_t i = 0; i < len; i++) {
      auto x = line[i];
      if (x == 1) {
        auto inc_num_2 = [len](auto &arr, size_t si, size_t i, auto x) {
          arr[si][i] += x;
          if (i > 0) arr[si][i - 1] += x;
          if (i < (len - 1)) arr[si][i + 1] += x;
        };
        // left right
        inc_num_2(is_num_2, si, i, x);
        // one level above
        if (si > 0) inc_num_2(is_num_2, si - 1, i, x);
        // one leve below
        if (si < (is_pn.size() - 1)) inc_num_2(is_num_2, si + 1, i, x);
      }
    }
  }

  // is_pn ← set pni for each * if number shadow is > 1
  // is_pn_2 ← shadow equal to the pni
  int pni = 1;
  for (size_t si = 0; si < is_pn.size(); si++) {
    auto line = is_pn[si];
    auto len = line.size();
    for (size_t i = 0; i < len; i++) {
      // x is true if '*' and there num shadow > 1
      auto x = line[i] && (is_num_2[si][i] > 1);

      // update the is_pn
      is_pn[si][i] = x;

      if (x) {
        is_pn_3[si][i] = pni;
        is_pn[si][i] = pni;

        auto set_pn_2 = [len](auto &arr, size_t si, size_t i, auto x) {
          arr[si][i] = x;
          if (i > 0) arr[si][i - 1] = x;
          if (i < (len - 1)) arr[si][i + 1] = x;
        };

        // left right
        set_pn_2(is_pn_2, si, i, pni);
        // one level above
        if (si > 0) set_pn_2(is_pn_2, si - 1, i, pni);
        // one leve below
        if (si < (is_pn.size() - 1)) set_pn_2(is_pn_2, si + 1, i, pni);

        pni++;
      }
    }
  }

  // keep the nums
  map<int, vector<int>> keep{};

  stringstream ss{};
  vector<int> vb{};

  for (size_t si = 0; si < schem.size(); si++) {
    for (size_t i = 0; i < schem[si].size(); i++) {
      auto c = schem[si][i];
      auto b = is_pn_2[si][i];

      if ((c >= 0x30) && (c <= 0x39)) {
        ss << c;
        vb.push_back(b);
      } else {
        auto s = ss.str();

        // clear stream and vec bool
        if (s.size() != 0) {
          // keep_id is is_pn_2 > 1
          int k_id = keep_id(vb);
          if (k_id > 0) keep[k_id].push_back(std::stoi(s));
          ss.clear(), ss.str("");
          vb.clear();
        }
      }
    }
  }

  // keep is a map: id → [v1 v2] in the shadow of each '*'
  int sum = 0;
  for (auto [k, v] : keep) {
    int v1 = v[0];
    int v2 = v[1];
    sum += (v1 * v2);
  }

  print("{1} {0}\n", sum, sum == 84584891);
}
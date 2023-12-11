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
  return 2;
}

bool or_reduce(vector<bool> a) {
  for (size_t i = 0; i < a.size(); i++)
    if (a[i]) return true;
  return false;
}

string b2s(vector<bool> v) {
  auto len = v.size();
  string s(len, ' ');
  for (size_t i = 0; i < len; i++) s[i] = v[i] ? 'X' : '.';
  return s;
}

int main() {
  // ifstream infile{"input_test_3"};
  // input3_test_3 = 4361
  ifstream infile{"input3"};
  // input3 = 540025

  // load and make PN an is_num arrays
  vector<string> schem{};
  vector<vector<bool>> is_pn{};
  vector<vector<bool>> is_pn_2{};
  vector<vector<bool>> is_num{};

  for (string line; getline(infile, line);) {
    auto len = line.size();

    schem.push_back(line);

    vector<bool> pn(len);
    vector<bool> pn_2(len);
    vector<bool> num(len);

    for (size_t i = 0; i < len; i++) {
      auto s = line[i];
      num[i] = (what(s) == 1);
      pn[i] = (what(s) == 2);
    }

    is_num.push_back(num);
    is_pn.push_back(pn);
    is_pn_2.push_back(pn_2);
  }

  // fill the PN + shadow
  for (size_t si = 0; si < is_pn.size(); si++) {
    auto line = is_pn[si];
    auto len = line.size();
    for (size_t i = 0; i < len; i++) {
      auto x = line[i];
      if (x) {
        // left right
        is_pn_2[si][i] = x;
        if (i > 0) is_pn_2[si][i - 1] = x;
        if (i < (len - 1)) is_pn_2[si][i + 1] = x;

        // one level above
        if (si > 0) {
          is_pn_2[si - 1][i] = x;
          if (i > 0) is_pn_2[si - 1][i - 1] = x;
          if (i < (len - 1)) is_pn_2[si - 1][i + 1] = x;
        }

        // one leve below
        if (si < (is_pn.size() - 1)) {
          is_pn_2[si + 1][i] = x;
          if (i > 0) is_pn_2[si + 1][i - 1] = x;
          if (i < (len - 1)) is_pn_2[si + 1][i + 1] = x;
        }
      }
    }
  }

  // keep the nums
  stringstream ss{};
  vector<bool> vb{};
  vector<int> keep_nums{};

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
          bool keep = or_reduce(vb);
          if (keep) keep_nums.push_back(std::stoi(s));
          ss.clear();
          ss.str("");
          vb.clear();
        }
      }
    }
  }

  // final sum
  int sum_nums = 0;
  for (auto n : keep_nums) sum_nums += n;
  print("{}\n", sum_nums);
}
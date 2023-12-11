#include <fmt/format.h>
#include <fmt/ranges.h>
#include <iostream>
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
#include <cmath>

using fmt::print, fmt::format;
using std::ifstream, std::getline;
using std::stringstream, std::string, std::stoi, std::erase;
using std::vector, std::map, std::tuple;

auto is_dig = [](char c) { return (c >= 0x30) && (c <= 0x39); };

int main() {
  // ifstream infile{"input_test_4"};
  // input_test_4 = 30
  ifstream infile{"input4"};
  // input4 = 5422730

  stringstream ss{};
  auto clear_ss = [&ss]() { ss.clear(), ss.str(""); };
  // map: card id → [win nums, my nums, count, score]
  map<int, tuple<vector<int>, vector<int>, int, int>> cards{};

  for (string line; getline(infile, line);) {
    int card_id = 0;
    auto len = line.size();
    vector<int> wins{};
    vector<int> nums{};
    bool into_wins = true;

    // string into numbers
    for (size_t i = 0; i < len; i++) {
      char c = line[i];

      // collect digits
      if (is_dig(c)) ss << c;

      // card id, set wins = T
      if (':' == c) {
        card_id = stoi(ss.str());
        clear_ss();
        into_wins = true;
      }

      // get str, keep it
      if ((' ' == c) || (len == (i + 1))) {
        string s = ss.str();
        erase(s, ' ');
        if (s.size() > 0) {
          auto n = stoi(s);
          if (into_wins)
            wins.push_back(n);
          else
            nums.push_back(n);
          clear_ss();
        }
      }

      // flip to my nums
      if ('|' == c) into_wins = false;
    }

    int n_wins = 0;
    for (auto n : nums) {
      for (auto w : wins) {
        if (n == w) n_wins++;
      }
    }

    cards[card_id] = tuple<vector<int>, vector<int>, int, int>(
        wins, nums, n_wins, pow(2, n_wins - 1));
  }

  vector<int> card_counts(cards.size() + 1);

  // set initial counts
  for (auto [k, v] : cards) card_counts[k] = 1;

  // increment
  for (auto [k, v] : cards) {
    auto m = std::get<2>(v);
    auto inc = card_counts[k];
    for (int i = k + 1; i <= k + m; i++) card_counts[i] += inc;
  }

  int sum = 0;
  for (auto x : card_counts) sum += x;

  print("N cards: `{}`\n", sum);
}
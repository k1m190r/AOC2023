#include <fmt/format.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <assert.h>
#include <map>
#include <tuple>
#include "../demangle.h"

using fmt::print, fmt::format;
using std::ifstream, std::getline;
using std::stringstream, std::string;
using std::vector, std::map, std::tuple;

// split on token → vector
vector<string> split(string text, char delim) {
  stringstream ss{text};
  vector<string> tokens{};

  for (string token{}; getline(ss, token, delim);)
    if (!token.empty()) tokens.push_back(token);
  return tokens;
}

// split into int and color, strip all whitespaces
tuple<int, int> get_color_score(string text) {
  stringstream num{};
  stringstream color{};
  static map<string, int> color_map{{"red", 0}, {"green", 1}, {"blue", 2}};

  for (auto c : text) {
    // 0x30 .. 0x39 = 0 .. 9
    if ((c >= 0x30) && (c <= 0x39)) num << c;
    // 0x61 .. 0x7a = a .. z
    if ((c >= 0x61) && (c <= 0x7a)) color << c;
  }
  return tuple<int, int>{std::stoi(num.str()), color_map[color.str()]};
}

int main() {
  // ifstream infile{"input_test_2"};
  // "input_test_2" = 2286
  ifstream infile{"input2"};
  // "input2" = 79315

  size_t sum_power{};
  vector<vector<int>> all_games{};

  for (string line; getline(infile, line);) {
    auto line_split = split(line, ':');
    assert(line_split.size() == 2);

    auto game_id{std::stoi(split(line_split[0], ' ')[1])};

    auto games{split(line_split[1], ';')};
    vector<int> stats{0, 0, 0};

    for (auto game : games) {
      auto g_scores{split(game, ',')};
      for (auto score : g_scores) {
        auto [i, c] = get_color_score(score);
        auto oi = stats[c];            // old i
        stats[c] = (oi < i) ? i : oi;  // keep old i if it's greater
      }
    }

    auto g_power = stats[0] * stats[1] * stats[2];
    sum_power += g_power;

    all_games.push_back(stats);
  }
  print("sum power: `{}`\n", sum_power);
}

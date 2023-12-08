#include <fmt/format.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <assert.h>
#include <map>
#include <tuple>

using fmt::print, fmt::format;
using std::ifstream, std::getline;
using std::stringstream, std::string;
using std::vector, std::map, std::tuple;

vector<string> split(string text, char delim) {
  stringstream ss{text};
  vector<string> tokens{};

  for (string token{}; getline(ss, token, delim);)
    if (!token.empty()) tokens.push_back(token);
  return tokens;
}

tuple<int, string> get_color_score(string text) {
  stringstream num{};
  stringstream color{};
  for (auto c : text) {
    // 0x30 .. 0x39 = 0 .. 9
    if ((c >= 0x30) && (c <= 0x39)) num << c;
    // 0x61 .. 0x7a = a .. z
    if ((c >= 0x61) && (c <= 0x7a)) color << c;
  }
  return tuple<int, string>{std::stoi(num.str()), color.str()};
}

int main() {
  ifstream infile{"input2"};
  // "input_test_2" = 8

  int sum_id{};
  map<int, map<string, int>> all_games{};

  for (string line; getline(infile, line);) {
    auto line_split = split(line, ':');
    assert(line_split.size() == 2);

    auto game_id{std::stoi(split(line_split[0], ' ')[1])};

    auto games{split(line_split[1], ';')};
    map<string, int> stats{{"red", 0}, {"green", 0}, {"blue", 0}};

    for (auto game : games) {
      auto g_scores{split(game, ',')};
      for (auto score : g_scores) {
        auto [i, c] = get_color_score(score);
        auto oi = stats[c];            // old i
        stats[c] = (oi < i) ? i : oi;  // if old_i < i then i else keep old_i
      }
    }

    // 12 red, 13 green, 14 blue
    auto red_ok = stats["red"] <= 12;
    auto green_ok = stats["green"] <= 13;
    auto blue_ok = stats["blue"] <= 14;
    if (red_ok && green_ok && blue_ok) sum_id += game_id;

    all_games[game_id] = stats;
  }
  print("sum_id: '{}'\n", sum_id);

}
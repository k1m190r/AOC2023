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
#include <climits>

using fmt::print, fmt::format;
using std::ifstream, std::getline;
using std::stringstream, std::string, std::stoll, std::erase;
using std::vector, std::map, std::tuple;

auto is_dig = [](char c) { return (c >= 0x30) && (c <= 0x39); };
auto is_letter = [](char c) { return (c >= 0x61) && (c <= 0x7a); };
// 61 .. 7a

int main() {
  // ifstream infile{"input_test_5"};
  // input_test_5 = 35
  ifstream infile{"input5"};
  // input5 = 88151870

  stringstream ss{};
  auto clear_ss = [&ss]() { ss.clear(), ss.str(""); };
  bool in_phrase = false;  // start with is_sym(c) ends with ':'
  string context{};

  vector<long long> seeds{};
  map<string, vector<vector<long long>>> maps{};

  // read in data
  for (string line; getline(infile, line);) {
    auto len = line.size();
    vector<long long> nums{};

    for (size_t i = 0; i < line.size(); i++) {
      char c = line[i];

      // collect letters
      if (is_letter(c)) {
        ss << c;
        in_phrase = true;
        continue;
      }

      // stop phrase, make context
      if (':' == c) {
        auto s = ss.str();
        clear_ss();
        in_phrase = false;
        context = s;
        continue;
      }

      // convert '-',' ' to '_'
      if (((' ' == c) || ('-' == c)) && in_phrase) {
        ss << "_";
        continue;
      }

      // make a number on space
      if ((' ' == c) && (!in_phrase)) {
        auto s = ss.str();
        clear_ss();
        if (s.size() > 0) {
          auto n = stoll(s);
          if (context == "seeds")
            seeds.push_back(n);
          else
            nums.push_back(n);
        }

        continue;
      }

      if (is_dig(c)) ss << c;

      // end of line with number
      if (len == (i + 1)) {
        auto s = ss.str();
        clear_ss();
        if (s.size() > 0) {
          auto n = stoll(s);
          if (context == "seeds")
            seeds.push_back(n);
          else {
            nums.push_back(n);
            maps[context].push_back(nums);
            nums.clear();
          }
        }
      }
    }
  }

  auto maps_order = {
      "seed_to_soil_map",         "soil_to_fertilizer_map",
      "fertilizer_to_water_map",  "water_to_light_map",
      "light_to_temperature_map", "temperature_to_humidity_map",
      "humidity_to_location_map",
  };

  auto map_func = [&maps](string map_n, long long in) {
    auto map = maps[map_n];
    for (auto v : map) {
      long long dst_st = v[0];
      long long src_st = v[1];
      long long range = v[2];
      if ((in >= src_st) && (in <= (src_st + range))) {
        return dst_st + (in - src_st);
      }
    }
    return in;
  };

  long long min_loc = LONG_LONG_MAX;
  vector<long long> locations{};
  for (auto s : seeds) {
    long long in = s;
    for (auto map_n : maps_order) {
      in = map_func(map_n, in);
    }
    locations.push_back(in);
    min_loc = (min_loc > in) ? in : min_loc;
  }
  print("min location: `{}`\n", min_loc);
}
#include <fmt/format.h>
#include <fstream>
#include <string>
#include <vector>
#include <assert.h>

using fmt::print, fmt::format;
using std::ifstream, std::getline, std::string, std::vector;

// o one
// t two three
// f four five
// s six seven
// e eight
// n nine

bool is_digi_start(char c) {
  for (char x : "otfsen")
    if (x == c) return true;
  return false;
}

bool is_digi(const char* s, string dig) {
  auto len = dig.size();
  return (0 == strncmp(s, dig.c_str(), len));
}

char maybe_digi(string s) {
  auto cs = s.c_str();

  if (!is_digi_start(cs[0])) return ' ';

  switch (cs[0]) {
    case 'o':  // one
      if (is_digi(cs, "one")) return '1';
      break;

    case 't':  // two three
      if (is_digi(cs, "two")) return '2';
      if (is_digi(cs, "three")) return '3';
      break;

    case 'f':  // four five
      if (is_digi(cs, "four")) return '4';
      if (is_digi(cs, "five")) return '5';
      break;

    case 's':  // six seven
      if (is_digi(cs, "six")) return '6';
      if (is_digi(cs, "seven")) return '7';
      break;

    case 'e':  // eight
      if (is_digi(cs, "eight")) return '8';
      break;

    case 'n':  // nine
      if (is_digi(cs, "nine")) return '9';
      break;
  }

  return ' ';
}

int main() {
  ifstream infile{"input1"};
  // "input_test_1_p2"
  // input_test_1_p2 == 281

  int sum{0};

  for (string line; getline(infile, line);) {
    vector<char> nums{};

    // 0x30 .. 0x39 = 0 .. 9
    for (int i = 0; const char c : line) {
      if ((c >= 0x30) && (c <= 0x39)) nums.push_back(c);

      auto digi = maybe_digi(line.substr(i));
      if (digi != ' ') nums.push_back(digi);
      i++;
    }

    if (nums.size() <= 0) continue;

    int v1 = nums[0] - 0x30;
    int v2 = nums.back() - 0x30;
    auto v = v1 * 10 + v2;
    sum += v;
  }
  print("sum: '{}'\n", sum);
}
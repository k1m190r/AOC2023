#include <fmt/format.h>
#include <fstream>
#include <string>
#include <vector>

using fmt::print, std::ifstream, std::getline, std::string, std::vector;

int main() {
  ifstream infile{"input1"};

  int sum{0};

  for (string line; getline(infile, line);) {
    vector<char> nums{};

    // 0x30 .. 0x39 = 0 .. 9
    for (const char c : line)
      if ((c >= 0x30) && (c <= 0x39)) nums.push_back(c);

    int v1 = nums[0] - 0x30;
    int v2 = nums.back() - 0x30;
    int v = v1 * 10 + v2;
    sum += v;
  }
  print("sum: {}\n", sum);
}
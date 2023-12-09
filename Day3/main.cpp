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

int main() {
  ifstream infile{"input3_test_3"};
  // input3_test_3 = 4361

  for (string line; getline(infile, line);) {
    print("{}\n", line);
  }
}
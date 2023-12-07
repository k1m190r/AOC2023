#include <fmt/format.h>
#include <fstream>
#include <string>

using std::string, fmt::print, std::istringstream;
using std::ifstream, std::ios_base, std::getline;

int main(){
    ifstream infile{"input1"};

    for( string line; getline(infile, line); ){
        // print("{}:{}\n", line, line.size() == line.length());

        char const *c = line.c_str();
        size_t len = line.length();

        print("{} ", len);

    }
}
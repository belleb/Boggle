#include <iostream>
#include <string>
#include <set>
#include <fstream>

using namespace std;
 
 
struct Dictionary {
 Dictionary() {
   // load _words, here's one possible implementation:
   ifstream input ("/usr/share/dict/words");
   for (string line; getline(input, line);) {
     _words.insert(line);
   }
 }
 bool contains(string const& word) const { return _words.count(word); }

 set<string> _words;
};


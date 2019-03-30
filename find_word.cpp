#include <iostream>
#include <set>
#include <fstream>
#include <locale>
#include <codecvt>
#include "simplify_word.cpp"
  
using namespace std;

struct Dictionary_en {
 Dictionary_en() {
   ifstream input ("/usr/share/dict/american-english");
   for (string line; getline(input, line);) {
     _words.insert(line);
   }
 }

 bool contains(string const& word) const { return _words.count(word); }

 set<string> _words;
};

struct Dictionary_pt {
 Dictionary_pt() {
   ifstream input ("/usr/share/dict/brazilian");
   for (string line; getline(input, line);) {
     //remove accents
     string simple = simplify(line);
     _words.insert(simple);
   }
 }

 bool contains(string const& word) const { return _words.count(word); }

 set<string> _words;
};


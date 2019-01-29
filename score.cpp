#include <iostream>
#include <string>

int score(std::string word){
  int l = word.length();
  if (l >= 26){
    throw std::invalid_argument("Word is too long");
  }
  int values[26] = {0, 0, 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657};
  return values[l];
}
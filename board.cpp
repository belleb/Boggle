#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>

using namespace std;

// frequencies of letters (English)
char frequencies_en[205] = {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 
'E', 'E', 'E', 'E', 'E', 'T', 'T', 'T', 'T', 'T', 'T', 'T', 'T', 'T', 'T', 'T', 'T', 'T', 'T', 'T', 'T', 'T', 'T', 'A', 'A', 
'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 
'O', 'O', 'O', 'O', 'O', 'I', 'I', 'I', 'I', 'I', 'I', 'I', 'I', 'I', 'I', 'I', 'I', 'I', 'I', 'I', 'N', 'N', 'N', 'N', 'N', 
'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'R', 'R', 'R', 
'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'D', 'D', 'D', 'D', 
'D', 'D', 'D', 'D', 'D', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'U', 'U', 'U', 'U', 'U', 'U', 'C', 'C', 'C', 'C', 'C', 'M', 
'M', 'M', 'M', 'M', 'F', 'F', 'F', 'F', 'F', 'Y', 'Y', 'Y', 'Y', 'W', 'W', 'W', 'W', 'G', 'G', 'G', 'G', 'P', 'P', 'P', 'P', 
'B', 'B', 'B', 'V', 'V', 'K', 'K', 'X', 'Q', 'J', 'Z'};

// frequencies of letters (Portuguese) 
char frequencies_pt[106] = {'A','A','A','A','A','A','A','A','A','A','A','A','A','A','A','E','E','E','E','E','E','E','E',
'E','E','E','E','E','O','O','O','O','O','O','O','O','O','O','O','S','S','S','S','S','S',
'S','S','R','R','R','R','R','R','R','I','I','I','I','I','I','D','D','D','D','D','N','N','N','N','N',
'M','M','M','M','M', 'U','U','U','U','U', 'T','T','T','T','C','C','C','C','L','L','L',
'P','P','P','V','V','G','G','H','H','F','B','J','Q','X','Z'};

// gets a random letter (probability weighted depending on the language) 
char getLetter(string language) {
  int letter_index;
  if (language == "en") {
    letter_index = rand() % 205; //get index
    return frequencies_en[letter_index];
  } else if (language == "pt") {
    letter_index = rand() % 106; //get index
    return frequencies_pt[letter_index];
  }
}

// produces random board
string getBoard(string language) {
  string b;
  b = "";
  int i, j;
  for (i = 0; i < 5; i++) {
    for (j = 0; j < 5; j++) {
      b += getLetter(language);
    }
  }
  return b;
}





#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>

using namespace std;

// frequencies of letters (English)
char frequencies_en[153] = {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E',
'T','T','T','T','T','T','T','T','T','T','T','T','T','A','A','A','A','A','A','A','A','A','A','A','A',
'R','R','R','R','R','R','R','R','R','R','R','R','I','I','I','I','I','I','I','I','I','I','I','N','N','N',
'N','N','N','N','N','N','N','N','O','O','O','O','O','O','O','O','O','O','O','S','S','S','S','S','S',
'S','S','S','S','S','S','D','D','D','D','D','D','C','C','C','C','C','H','H','H','H','H','L','L','L','L','L',
'F','F','F','F','M','M','M','M','P','P','P','P','U','U','U','U','G','G','G','Y','Y','Y','W','W',
'B','J','K','Q','V','X','Z'};

// frequencies of letters (Portuguese) 
char frequencies_pt[106] = {'A','A','A','A','A','A','A','A','A','A','A','A','A','A','A','E','E','E','E','E','E','E','E',
'E','E','E','E','E','O','O','O','O','O','O','O','O','O','O','O','S','S','S','S','S','S',
'S','S','R','R','R','R','R','R','R','I','I','I','I','I','I','D','D','D','D','D','N','N','N','N','N',
'M','M','M','M','M', 'U','U','U','U','U', 'T','T','T','T','C','C','C','C','L','L','L',
'P','P','P','V','V','G','G','H','H','F','B','J','Q','X','Z'};

// gets a random letter (probability weighted depending on the language) 
char getLetter(string language){
    int letter_index;
    if (language == "en"){
        letter_index = rand() % 153; //get index
        return frequencies_en[letter_index];
    }else if (language == "pt"){
        letter_index = rand() % 106; //get index
        return frequencies_pt[letter_index];
    }
}

// produces random board
string getBoard(string language){
    string b;
    b = "";
    int i, j;
    for (i = 0; i < 5; i++){
        for (j = 0; j < 5; j++){
            b += getLetter(language);
        }
    }
       
    return b;
}





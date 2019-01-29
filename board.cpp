#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>

using namespace std;

// frequencies of letters
char frequencies[153] = {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E',
'T','T','T','T','T','T','T','T','T','T','T','T','T','A','A','A','A','A','A','A','A','A','A','A','A',
'R','R','R','R','R','R','R','R','R','R','R','R','I','I','I','I','I','I','I','I','I','I','I','N','N','N',
'N','N','N','N','N','N','N','N','O','O','O','O','O','O','O','O','O','O','O','S','S','S','S','S','S',
'S','S','S','S','S','S','D','D','D','D','D','D','C','C','C','C','C','H','H','H','H','H','L','L','L','L','L',
'F','F','F','F','M','M','M','M','P','P','P','P','U','U','U','U','G','G','G','Y','Y','Y','W','W',
'B','J','K','Q','V','X','Z'};

// gets a random letter (probability weighted by the structure of the English language) 
char getLetter(){
    
    int letter_index = rand() % 153; //get index
    
    return frequencies[letter_index];
}

// produces random board
string getBoard(){

    static string b;
    
    int i, j;
    for (i = 0; i < 5; i++){
        for (j = 0; j < 5; j++){
            b += getLetter();
        }
    }    
    return b;
}





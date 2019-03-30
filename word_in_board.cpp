#include <iostream>
#include <string>
using namespace std;

// The minimum acceptable length of a word
int const MIN_WORD = 2;

// Helper function checks if current letter on the string is found at current position of the board, and continues recursively until the end of the string is reached
bool helper(char grid[5][5], bool visited[5][5], int i, int j, int k, string word) {
  // Return true if the end of the string has been reached
  if (k == word.length()) {
    return true;
  }
  // Return false if either grid[i][j] has already been visited or the character is not the same as word[k]
  else if ((visited[i][j]) || (grid[i][j] != word[k])) {
    return false; 
  }
  // Continue recursively otherwise
  else {
    visited[i][j] = true;
    bool temp[5][5];
    k++;
    bool output = false;
    int step_h, step_v;
    for (step_h = -1; step_h <= 1; step_h++) {
      for (step_v = -1; step_v <= 1; step_v++) {
        int row, col;
        row = step_v + i;
        col = step_h + j;
        if ((step_v != 0 or step_h != 0) and (0 <= row) and (row < 5) and (0 <= col) and (col < 5)) {
          // Copy visited
          int a, b, temp_k;
          for (a = 0; a < 5; a++) {
            for (b = 0; b < 5; b++) {
              temp[a][b] = visited[a][b];
            }
          }
          temp_k = k;
          output = output || helper(grid, temp, row, col, temp_k, word);
        }
      }
    }
  return output;
  }
}

// Checks whether the word s is present in the board
bool find(string word, string board) {
  if (word.length() < MIN_WORD) {
    return false;
  }    
  else {
    // Creates 5 by 5 grid using letters from the string board
    char grid[5][5];
    int i,j,k;
    k = 0;
    for (i = 0; i < 5; i++) {
      for (j = 0; j < 5; j++) {
        grid[i][j] = board[k];
        k++;
      }
    }

    // Helper is called for every position in the grid
    bool output = false; 
    for (i = 0; i < 5; i++) {
      for (j = 0; j < 5; j++) {     
        k = 0;           
        bool visited[5][5] = {{false}};
        output = (output || helper(grid, visited, i, j, k, word));
      }
    }
  return output;
  }
}
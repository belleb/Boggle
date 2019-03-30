#include <bits/stdc++.h>
#include "score.cpp"
#include "add_entry.cpp"
#include "word_in_board.cpp"

string submit(string word, string id, string language, string board, Dictionary_en d_en, Dictionary_pt d_pt) {
  // Tests if word is too long or empty. If yes, returns current score
  if ((word.length() == 0) or (word.length() > 25)) {
    return get_score_from_id(id);
  }

  // Removes accents
  string simple = simplify(word);

  // Makes a lowercase version of the word
  string sl;
  sl = simple;
  transform(sl.begin(), sl.end(), sl.begin(), ::tolower);

  // Checks if word exists in the dictionary, if yes proceeds to check on the board
  if ( ((language == "en") && d_en.contains(sl)) || ((language == "pt") && d_pt.contains(sl)) ) {
    // Makes uppercase version of the word
    string su;
    su = simple;
    transform(su.begin(), su.end(), su.begin(), ::toupper);

    // Looks for word in the board. If word exists in the board, checks
    // if it has not been entered yet 
    if (find(su, board) and word_not_entered(su, id)) {
      // Computes new score
      int points;
      points = score(word);

      // Updates the database with new score, and returns new score
      return update_score(points, id);
    }
  }

  // In case the word was not valid or had been entered already,
  // returns old score without updating
  return get_score_from_id(id);
}
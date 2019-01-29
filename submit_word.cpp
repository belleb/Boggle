#include "find_word.cpp"
#include "score.cpp"
#include "add_entry.cpp"
#include "word_in_board.cpp"
#include <bits/stdc++.h>

void submit(string word, string id){
    Dictionary d;
    string sl;
    sl = word;
    transform(sl.begin(),sl.end(),sl.begin(),::tolower);
    if (d.contains(sl)){
        cout <<"FOUND IN DICT!";
        string board = get_board_from_id(id);
        if (find(word,board) and word_not_entered(word, id)){
            cout << "FOUND IN GRAPH, AND NOT ENTERED";
            int points;
            points = score(word);
            update_score(points, id);
            
        }
    }

}
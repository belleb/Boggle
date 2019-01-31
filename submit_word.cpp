#include "find_word.cpp"
#include "score.cpp"
#include "add_entry.cpp"
#include "word_in_board.cpp"
#include <bits/stdc++.h>

string submit(string word, string id){
    Dictionary d;
    string sl;
    sl = word;
    transform(sl.begin(),sl.end(),sl.begin(),::tolower);
    if (d.contains(sl)){
        string board = get_board_from_id(id);
        string su;
        su = word;
        transform(su.begin(),su.end(),su.begin(),::toupper);
        if (find(su,board) and word_not_entered(su, id)){
            int points;
            points = score(word);
            return update_score(points, id);
            
        }
    }
    return get_score_from_id(id);

}
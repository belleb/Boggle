#include "score.cpp"
#include "add_entry.cpp"
#include "word_in_board.cpp"
#include <bits/stdc++.h>

string submit(string word, string id,string language,string board, Dictionary_en d_en, Dictionary_pt d_pt){
    //tests if word is too long or empty. If yes, returns current score
    if ((word.length() == 0) or (word.length()>25)){
        return get_score_from_id(id);
    }
    
    //remove accents
    string simple = simplify(word);
    
    //makes a lowercase version of the word
    string sl;
    sl = simple;
    transform(sl.begin(),sl.end(),sl.begin(),::tolower);
   
    //checks if word exists in the dictionary, if yes proceeds to check on the board
    if ( ((language == "en") && d_en.contains(sl)) || ((language == "pt") && d_pt.contains(sl)) ){
        //makes uppercase version of the word
        string su;
        su = simple;
        transform(su.begin(),su.end(),su.begin(),::toupper);
        
        //looks for word in the board. If word exists in the board, checks
        //if it has not been entered yet 
        if (find(su,board) and word_not_entered(su, id)){
            //computes new score
            int points;
            points = score(word);
            
            //updates the database with new score, and returns new score
            return update_score(points, id);
            
        }
    }
    
    //in case the word was not valid or had been entered already,
    //returns old score without updating
    return get_score_from_id(id);

}
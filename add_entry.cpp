#include <stdio.h>
#include <postgresql/libpq-fe.h>
#include <string>
#include "board.cpp"
#include <chrono>
using Clock = std::chrono::high_resolution_clock;

string new_id() {
    PGconn          *conn;
    PGresult        *res;

    conn = PQconnectdb("dbname=rex user=postgres");
    
    //tests connection
    if (PQstatus(conn) == CONNECTION_BAD) {
                 puts("We were unable to connect to the database");
                 exit(0);
    }
    //finds the time of creation of the board
    auto time = Clock::now();
    long int time_lg =  std::chrono::duration_cast<std::chrono::nanoseconds>(time.time_since_epoch()).count();
    string time_str = std::to_string(time_lg);
    
    //finds letters for the board
    string b = getBoard(); 
    
    //SQL query
    string query = "INSERT INTO boards (letters, time) VALUES ( '" + b + "','" + time_str + "') RETURNING board_id;";
    
    const char * c = query.c_str();
    res = PQexec(conn,  c);
    
    string id(PQgetvalue(res, 0, 0));
    
    PQclear(res);
    PQfinish(conn);
    return id;
 }

string get_board_from_id(string id){
    PGconn          *conn;
    PGresult        *res;

    if (id ==  ""){
        return "Board doesn't exist";
    }
    
    conn = PQconnectdb("dbname=rex user=postgres");
    if (PQstatus(conn) == CONNECTION_BAD) {
                 puts("We were unable to connect to the database");
                 exit(0);
    }
    
    string query = "SELECT letters FROM boards WHERE board_id= $1";
    const char * c = query.c_str();
    
    
    const char * values[1];
    values[0] = id.c_str();

    res = PQexecParams(conn,c,1, NULL, values, NULL, 0, 0);
    
    if (PQntuples(res)==0) {
        PQclear(res);
        PQfinish(conn);
        return "Board doesn't exist";
    }else{
        const char * letters = PQgetvalue(res, 0, 0);
        string output(letters);
 
        PQclear(res);
        PQfinish(conn);
        return output;
    }
}

string get_time_from_id(string id){
    PGconn          *conn;
    PGresult        *res;

    conn = PQconnectdb("dbname=rex user=postgres");
 
    if (PQstatus(conn) == CONNECTION_BAD) {
                 puts("We were unable to connect to the database");
                 exit(0);
    }
    
    
    
    string query = "SELECT time FROM boards WHERE board_id= $1";
    const char * c = query.c_str();
    
    
    const char * values[1];
    values[0] = id.c_str();

    res = PQexecParams(conn,c,1, NULL, values, NULL, 0, 0);
    
    if (PQntuples(res)==0) {
        PQclear(res);
        PQfinish(conn);
        return "Board doesn't exist";
    }else{
        const char * time = PQgetvalue(res, 0, 0);
        string output(time);
     
        PQclear(res);
        PQfinish(conn);
        return output;
    }

}

string get_score_from_id(string id){
    PGconn          *conn;
    PGresult        *res;

    conn = PQconnectdb("dbname=rex user=postgres");
 
    if (PQstatus(conn) == CONNECTION_BAD) {
                 puts("We were unable to connect to the database");
                 exit(0);
    }
    
    
    string query = "SELECT score FROM boards WHERE board_id= $1";
    const char * c = query.c_str();
    
    
    const char * values[1];
    values[0] = id.c_str();

    res = PQexecParams(conn,c,1, NULL, values, NULL, 0, 0);
    
    
    if (PQntuples(res)==0)  {
        PQclear(res);
        PQfinish(conn);
        return "Board doesn't exist";
    }else{
        const char * score = PQgetvalue(res, 0, 0);
        string output(score);
     
        PQclear(res);
        PQfinish(conn);
        return output;
    }

}

bool word_not_entered(string s, string id){
    PGconn          *conn;
    PGresult        *res;

    conn = PQconnectdb("dbname=rex user=postgres");
 
    if (PQstatus(conn) == CONNECTION_BAD) {
                 puts("We were unable to connect to the database");
                 exit(0);
    }
    
    string query = "SELECT word FROM words WHERE (board_id= $1 AND word = $2);";
    const char * c = query.c_str();
    
    
    const char * values[2];
    values[0] = id.c_str();
    values[1] = s.c_str();

    res = PQexecParams(conn,c,2, NULL, values, NULL, 0, 0);
    /*
    string query = "SELECT word FROM words WHERE (board_id='";
    query +=id;
    query+="' AND word='";
    query+= s;
    query += "');";
    
    const char * c;
    c = query.c_str();
    
    
    res = PQexec(conn,c);*/
    
    
    if (PQntuples(res)==0){//(PQgetisnull(res,0,0)) {
        PQclear(res);
        string new_query;
        PGresult        *new_res;
       
        const char * new_values[2];
        new_values[0] = id.c_str();
        new_values[1] = s.c_str();

            
        new_query = "INSERT INTO words (board_id, word) VALUES ($1, $2);";
        const char * d = new_query.c_str();
        

        new_res = PQexecParams(conn,d,2, NULL, new_values, NULL, 0, 0);
        
        
        PQclear(new_res);
        PQfinish(conn);
        return true;
    }
    else{
        PQclear(res);
        PQfinish(conn);
        return false;
    }
    
 
    

}

string update_score(int points, string id){
    PGconn          *conn;
    PGresult        *res;

    conn = PQconnectdb("dbname=rex user=postgres");
 
    if (PQstatus(conn) == CONNECTION_BAD) {
                 puts("We were unable to connect to the database");
                 exit(0);
    }
    //get current score
    string query = "SELECT score FROM boards WHERE board_id= $1";
    const char * c = query.c_str();
    
    
    const char * values[1];
    values[0] = id.c_str();

    res = PQexecParams(conn,c,1, NULL, values, NULL, 0, 0);
   
    
    const char * current_score = PQgetvalue(res, 0, 0);
    PQclear(res);
    string c_score(current_score);
    int score = std::stoi(c_score);
    
    //update score
    score += points;
    string S = std::to_string(score);
    
    
    query = "UPDATE boards SET score='" +S+ "' WHERE board_id=$1;";
    const char * d = query.c_str();
    

    res = PQexecParams(conn,d,1, NULL, values, NULL, 0, 0);
    
    
    PQclear(res);
    PQfinish(conn);
    
    return S;
   
    
}


#include <stdio.h>
#include <postgresql/libpq-fe.h>
#include <string>
#include "board.cpp"

string new_id() {
    PGconn          *conn;
    PGresult        *res;

    conn = PQconnectdb("dbname=boggle user=postgres");
 
    if (PQstatus(conn) == CONNECTION_BAD) {
                 puts("We were unable to connect to the database");
                 exit(0);
    }
    string b = getBoard();
    string query = "INSERT INTO boards (letters) VALUES ( '";
    query += b;
    query += "')";
    query += " RETURNING board_id;";
    
    const char * c = query.c_str();
    
    res = PQexec(conn,  c);
    
    string a = PQgetvalue(res, 0, 0);
    
    PQclear(res);
    PQfinish(conn);
    return a;
 }

string get_board_from_id(string id){
    PGconn          *conn;
    PGresult        *res;

    conn = PQconnectdb("dbname=boggle user=postgres");
 
    if (PQstatus(conn) == CONNECTION_BAD) {
                 puts("We were unable to connect to the database");
                 exit(0);
    }
    
    string query = "SELECT letters FROM boards WHERE board_id='";
    query +=id;
    query+="';";
    
    const char * c;
    c = query.c_str();
    
    
    res = PQexec(conn,c);
    if (PQgetisnull(res,0,0)) {
        puts("Board doesn't exist");
        exit(0);
    }
    const char * letters = PQgetvalue(res, 0, 0);
    string output(letters);

 
    PQclear(res);
    PQfinish(conn);
    return output;

}

string get_time_from_id(string id){
    PGconn          *conn;
    PGresult        *res;

    conn = PQconnectdb("dbname=boggle user=postgres");
 
    if (PQstatus(conn) == CONNECTION_BAD) {
                 puts("We were unable to connect to the database");
                 exit(0);
    }
    
    string query = "SELECT time FROM boards WHERE board_id='";
    query +=id;
    query+="';";
    
    const char * c;
    c = query.c_str();
    
    
    res = PQexec(conn,c);
    if (PQgetisnull(res,0,0)) {
        puts("Board doesn't exist");
        exit(0);
    }
    const char * time = PQgetvalue(res, 0, 0);
    string output(time);

 
    PQclear(res);
    PQfinish(conn);
    return output;

}

bool word_not_entered(string s, string id){
    PGconn          *conn;
    PGresult        *res;

    conn = PQconnectdb("dbname=boggle user=postgres");
 
    if (PQstatus(conn) == CONNECTION_BAD) {
                 puts("We were unable to connect to the database");
                 exit(0);
    }
    
    string query = "SELECT word FROM words WHERE (board_id='";
    query +=id;
    query+="' AND word='";
    query+= s;
    query += "');";
    
    const char * c;
    c = query.c_str();
    
    
    res = PQexec(conn,c);
    
    
    if (PQntuples(res)==0){//(PQgetisnull(res,0,0)) {
        string new_query;
        PGresult        *new_res;
        new_query = "INSERT INTO words (board_id, word) VALUES ('"+ id + "','"+ s;
        new_query += "');";
        const char * d;
        d = new_query.c_str();
    
        new_res = PQexec(conn,d);
        
        PQclear(res);
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

void update_score(int points, string id){
    PGconn          *conn;
    PGresult        *res;

    conn = PQconnectdb("dbname=boggle user=postgres");
 
    if (PQstatus(conn) == CONNECTION_BAD) {
                 puts("We were unable to connect to the database");
                 exit(0);
    }
    //get current score
    string query = "SELECT score FROM boards WHERE board_id='"+ id+"';";
    const char * c;
    c = query.c_str();
    
    res = PQexec(conn,c);
    
    const char * current_score = PQgetvalue(res, 0, 0);
    string c_score(current_score);
    int score = std::stoi(c_score);
    
    //update score
    score += points;
    string S = std::to_string(score);
    
    query = "UPDATE boards SET score='" +S+ "' WHERE board_id='"+id+"';";
    c = query.c_str();
    res = PQexec(conn,c);
    
    PQclear(res);
    PQfinish(conn);
   
    
}


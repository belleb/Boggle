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

bool word_already_entered(string s, string id){
    PGconn          *conn;
    PGresult        *res;

    conn = PQconnectdb("dbname=boggle user=postgres");
 
    if (PQstatus(conn) == CONNECTION_BAD) {
                 puts("We were unable to connect to the database");
                 exit(0);
    }
    
    string query = "SELECT word FROM words WHERE board_id='";
    query +=id;
    query+="' WHERE word='";
    query+= s;
    query += "';";
    
    const char * c;
    c = query.c_str();
    
    
    res = PQexec(conn,c);
    
    if (PQgetisnull(res,0,0)) {
        query = "INSERT INTO words (board_id, word) VALUES ('";
        query += id;
        query += "','";
        query += s;
        query += "');";
        const char * d;
        d = query.c_str();
    
    
        res = PQexec(conn,d);
        
        PQclear(res);
        PQfinish(conn);
        return false;
    }
    else{
        PQclear(res);
        PQfinish(conn);
        return true;
    }
    
 
    
    return output;

}


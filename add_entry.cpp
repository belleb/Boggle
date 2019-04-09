#include <stdio.h>
#include <string>
#include <chrono>
#include <postgresql/libpq-fe.h>
#include "board.cpp"

using Clock = std::chrono::high_resolution_clock;

string BOARD_MISSING = "Board doesn't exist";

// Establishes a connection or print error message
void connect(PGconn* conn) {
  conn = PQconnectdb("dbname=rex user=postgres");

  // Tests connection
  if (PQstatus(conn) == CONNECTION_BAD) {
    puts("We were unable to connect to the database");
    exit(0);
  }
}

// Creates new board and returns its board_id
string new_id(string language) {
  // Gets database connection
  PGconn* conn;
  connect(conn);

  // Gets the time the board was created
  auto time = Clock::now();
  long int time_lg =  std::chrono::duration_cast<std::chrono::nanoseconds>(time.time_since_epoch()).count();
  string time_str = std::to_string(time_lg);

  // Creates letters for the board
  string b = getBoard(language); 

  // Constructs parametrized SQL query to insert new board
  string query = "INSERT INTO boards (letters, time, language) VALUES"
                 "( '" + b + "','" + time_str + "',$1) RETURNING board_id;";
  const char* c = query.c_str();
 
  // Language is the parameter 
  const char* values[1];
  values[0] = language.c_str();

  // Runs the query
  PGresult* res;
  res = PQexecParams(conn, c, 1, NULL, values, NULL, 0, 0);

  // Gets the id 
  string id(PQgetvalue(res, 0, 0));

  // Close database connection and return desired output
  PQclear(res);
  PQfinish(conn);
  return id;
}

// Given a board_id, gets the board letters
string get_board_from_id(string id) {
  // Gets database connection
  PGconn* conn;
  connect(conn);

  if (id ==  "") {
    return BOARD_MISSING;
  }

  // Constructs parametrized SQL query to get board from id
  string query = "SELECT letters FROM boards WHERE board_id = $1;";
  const char* c = query.c_str();

  // id is the parameter
  const char* values[1];
  values[0] = id.c_str();

  // Runs the query
  PGresult* res;
  res = PQexecParams(conn, c, 1, NULL, values, NULL, 0, 0);

  // If the database doesn't find a result, produce an error message
  string output;
  if (PQntuples(res)==0) {
    output = BOARD_MISSING;
  } else {
    const char* letters = PQgetvalue(res, 0, 0);
    output = letters;
  }

  // Close database connection and return desired output
  PQclear(res);
  PQfinish(conn);
  return output;
}

// Given a board_id, gets the time
string get_time_from_id(string id) {
  // Gets database connection
  PGconn* conn;
  connect(conn);

  // Constructs parametrized SQL query to get time from id
  string query = "SELECT time FROM boards WHERE board_id = $1";
  const char* c = query.c_str();

  // id is the parameter
  const char* values[1];
  values[0] = id.c_str();

  // Runs the query
  PGresult* res;
  res = PQexecParams(conn, c, 1, NULL, values, NULL, 0, 0);

  // If the database doesn't find a result, produce an error message
  string output;
  if (PQntuples(res)==0) {
    output = BOARD_MISSING;
  } else {
    const char* time = PQgetvalue(res, 0, 0);
    output = time;
  }

  // Close database connection and return desired output
  PQclear(res);
  PQfinish(conn);
  return output;
}

// Given a board_id, gets the score
string get_score_from_id(string id) {
  // Gets database connection
  PGconn* conn;
  connect(conn);

  // Constructs parametrized SQL query to get score from id
  string query = "SELECT score FROM boards WHERE board_id = $1;";
  const char* c = query.c_str();

  // id is the parameter
  const char* values[1];
  values[0] = id.c_str();

  // Runs the query
  PGresult* res;
  res = PQexecParams(conn, c, 1, NULL, values, NULL, 0, 0);

  // If the database doesn't find a result, produce an error message
  string output;
  if (PQntuples(res) == 0) {
    output = BOARD_MISSING;
  } else {
    const char* score = PQgetvalue(res, 0, 0);
    output = score;
  }

  // Close database connection and return desired output
  PQclear(res);
  PQfinish(conn);
  return output;
}

// Given a board_id, gets its ranking out of all boards of the same language
string get_ranking_from_id(string id, string language) {
  // Gets database connection
  PGconn* conn;
  connect(conn);

  // Constructs parametrized SQL query to get ranking of board
  string query = "SELECT ranking FROM (SELECT ROW_NUMBER() OVER (ORDER BY score DESC) "
                 "AS ranking, board_id FROM boards WHERE language = $2) sub WHERE board_id = $1;";
  const char* c = query.c_str();

  // id and language are the parameters
  const char* values[2];
  values[0] = id.c_str();
  values[1] = language.c_str();

  // Runs the query
  PGresult* res;
  res = PQexecParams(conn, c, 2, NULL, values, NULL, 0, 0);

  // If the database doesn't find a result, produce an error message
  string output;
  if (PQntuples(res) == 0) {
    output = BOARD_MISSING;
  } else {
    const char* ranking = PQgetvalue(res, 0, 0);
    output = ranking;
  }

  // Close database connection and return desired output
  PQclear(res);
  PQfinish(conn);
  return output;
}

// Given a board_id, tests whether word has already been played on that board,
// and updates table of played words if not
bool word_not_entered(string word, string id) {
  // Gets database connection
  PGconn* conn;
  connect(conn);

  // Constructs parametrized SQL query to see if word has been played on the given board
  string query = "SELECT word FROM words WHERE (board_id = $1 AND word = $2);";
  const char* c = query.c_str();

  // id and word are the parameters
  const char* values[2];
  values[0] = id.c_str();
  values[1] = word.c_str();

  // Runs the query
  PGresult* res;
  res = PQexecParams(conn, c, 2, NULL, values, NULL, 0, 0);

  // If the word hasn't been played, do the necessary update 
  bool output;
  if (PQntuples(res) == 0) {
    // Clear the current result
    PQclear(res);

    // Constructs parametrized SQL query to add word to table of played words
    query = "INSERT INTO words (board_id, word) VALUES ($1, $2);";
    const char* d = query.c_str();

    // Runs the query
    res = PQexecParams(conn, d, 2, NULL, values, NULL, 0, 0);
    
    // The word was not entered before
    output = true;
  } else {
    // The word was entered before
    output = false;
  }
  
  // Close database connection and return desired output
  PQclear(res);
  PQfinish(conn);
  return output;
}

// Given a board_id, updates the score of that board by given number of points
string update_score(int points, string id) {
  // Gets database connection
  PGconn* conn;
  connect(conn);
  
  // Constructs parametrized SQL query to get score of board
  string query = "SELECT score FROM boards WHERE board_id = $1;";
  const char* c = query.c_str();

  // id is the parameter
  const char* values[1];
  values[0] = id.c_str();

  // Runs the query
  PGresult* res;
  res = PQexecParams(conn, c, 1, NULL, values, NULL, 0, 0);

  // If the database doesn't find a result, produce an error message
  string output;
  if (PQntuples(res) == 0) {
    output = BOARD_MISSING;
  } else {
    // Get the current score and clear the current result
    const char* current_score = PQgetvalue(res, 0, 0);
    PQclear(res);
    
    // Update score and set output to new score
    string c_score(current_score);
    int score = std::stoi(c_score);
    score += points;
    output = std::to_string(score);

    // Constructs parametrized SQL query to update score of board
    query = "UPDATE boards SET score = '" + output + "' WHERE board_id = $1;";
    const char* d = query.c_str();

    // Runs the query
    PGresult* res;
    res = PQexecParams(conn, d, 1, NULL, values, NULL, 0, 0);
  }

  // Close database connection and return desired output
  PQclear(res);
  PQfinish(conn);
  return output;
}


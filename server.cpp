#include "crow_all.h"
#include "find_word.cpp"
#include "submit_word.cpp"

using Clock = std::chrono::high_resolution_clock;

long int const max_time = 300;
long int const BI = 1000000000;

int main()
{
  // When testing locally, replace with "http://localhost:18080"
  static string address = "http://thesaurus-rex.org";

  // Creates English and Portuguese dictionaries once when the server is started
  static Dictionary_en d_en;
  static Dictionary_pt d_pt;

  // Simple 404 error message
  static string error404 = "404 NOT FOUND";

  // Creates the Crow application
  crow::SimpleApp app;
  crow::mustache::set_base(".");

  // Redirects http://thesaurus-rex.org to http://thesaurus-rex.org/en
  CROW_ROUTE(app, "/")
  ([](){
    crow::response res = crow::response(302);
    res.set_header("Location",address+"/en");
    return res;
  });

  // Home page in English and Portuguese
  CROW_ROUTE(app, "/<string>")
  ([](string language){
    // Validate input, return 404 on failure
    if ((language != "pt") and (language != "en")) {
        return error404;
    }
    // Return home page template
    crow::mustache::context x;
    x["language"] = language;
    auto page = crow::mustache::load("/templates/home.html");
    return page.render(x);
  });

  // Game board
  CROW_ROUTE(app, "/<string>/game/<string>")
  ([](string language, string id) {
    // Perform lookup from database
    string board = get_board_from_id(id);

    // Validate input, return 404 on failure
    if ((board == "Board doesn't exist") or ((language != "en") and (language != "pt"))){
      return error404;
    }

    // Current time
    auto time = Clock::now();
    long int time_now = std::chrono::duration_cast<std::chrono::nanoseconds>(time.time_since_epoch()).count();
    // Time of creation of the board
    string time_creation = get_time_from_id(id);

    // Calculates time elapsed and remaining time
    long int difftime = time_now - stol(time_creation);
    long int time_diff = ((max_time+1)*BI-difftime)/1000000; // Remaining time in milliseconds

    // Set parameters for loading page
    crow::mustache::context x;
    x["id"] = id;
    x["letters"] = board;
    x["time_diff"] = time_diff;
    x["score"] = get_score_from_id(id);
    x["language"] = language;

    // If the game has not ended yet, display active game board
    if (difftime < max_time*BI){ 
      auto page = crow::mustache::load("/templates/game.html");
    } else {
    // If the game has ended, display final score and ranking information
      x["ranking"] = get_ranking_from_id(id, language);
      auto page = crow::mustache::load("/templates/score.html");
    }
    return page.render(x);
  });

  // API for submitting a word
  CROW_ROUTE(app, "/<string>/game/<string>/<string>")
  ([](string language, string id, string word){
    // Perform lookup from database
    string board = get_board_from_id(id);

    // Validate input, redirect to home on failure
    if ((board == "Board doesn't exist") or ((language != "en") and (language != "pt"))){
      crow::response res = crow::response(302);
      res.set_header("Location", address);
      return res;
    }

    // Current time
    auto time = Clock::now();
    long int time_now = std::chrono::duration_cast<std::chrono::nanoseconds>(time.time_since_epoch()).count();
    // Time of creation of the board
    string time_creation = get_time_from_id(id);

    // Calculates time elapsed
    long int difftime = time_now - stol(time_creation);

    // If the game has not ended yet, return the new score of the player as a string
    if (difftime < max_time*BI){
      string score = submit(word, id,language,board,d_en,d_pt);
      return crow::response(score);
    } else {
    // If the game has ended, redirect to final score page
      crow::response res = crow::response(302);
      res.set_header("Location", address + "/" + language + "/game/" + id);
      return res;
    }
  });


  // API for creating a game
  CROW_ROUTE(app, "/<string>/game")
  ([](string language){
    // Validate input, redirect to home on failure
    if ((language != "en") and ((language) != "pt")){
      crow::response res = crow::response(302);
      res.set_header("Location", address);
      return res;
    }

    // Generate a new board, redirect to game page
    srand(time(NULL));
    string id = new_id(language);
    crow::response res = crow::response(302);
    res.set_header("Location", address + "/" + language + "/game/" + id);
    return res;
  });

  // Run the server
  // When testing, use app.port(18080).multithreaded().run()
  app.port(80).multithreaded().run();
}

// The command to compile this code:
// g++ server.cpp -o server -I /usr/include/postgresql/ -L /usr/lib/ -lm -lpq -pthread -lboost_system -std=c++11

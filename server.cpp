#include "crow_all.h"
#include "add_entry.cpp"

int main()
{
    crow::SimpleApp app;
    crow::mustache::set_base(".");

    CROW_ROUTE(app, "/")([](){
        pair <string, string> board;
        board = new_board();
        return board.first + " " + board.second;
    });
    
    

    CROW_ROUTE(app, "/game")([](){
        srand (time(NULL));
        pair <string, string> board;
        board = new_board();
        crow::mustache::context x;
         
        x["id"] = board.first;
        x["letters"] = board.second;
        
        auto page = crow::mustache::load("/templates/index.html");
        return page.render(x);
    });
    
    
    app.port(18080).multithreaded().run();
}

// g++ server.cpp -o server -I /usr/include/postgresql/ -L /usr/lib/ -lm -lpq -pthread -lboost_system -std=c++11
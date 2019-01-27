#include "crow_all.h"
#include "add_entry.cpp"

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        pair <string, string> board;
        board = new_board();
        return board.first + " " + board.second;
    });
    
    
    app.port(18080).multithreaded().run();
}
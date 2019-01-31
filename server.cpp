#include "crow_all.h"
#include "submit_word.cpp"
long int const max_time = 20;
long int const BI = 1000000000;

using Clock = std::chrono::high_resolution_clock;
int main()
{
    crow::SimpleApp app;
    crow::mustache::set_base(".");

    CROW_ROUTE(app, "/")([](){
        auto page = crow::mustache::load("/templates/home.html");
        return page.render();
    });
    
    
    CROW_ROUTE(app, "/game/<string>")
      ([](string id){
            
            string board = get_board_from_id(id);
            if (board == "Board doesn't exist"){
                auto page = crow::mustache::load("/templates/home.html");
                return page.render();  
            }
            //gets current time
            auto time = Clock::now();
            long int time_now = std::chrono::duration_cast<std::chrono::nanoseconds>(time.time_since_epoch()).count();
            //get time of creation of the board
            string time_creation = get_time_from_id(id);
            
            //calculates time elapsed and remaining time
            long int difftime = time_now-stol(time_creation);
            long int time_diff = ((max_time+1)*BI-difftime)/1000000;
            
            //decides if the game has ended or not, and behaves accordingly
            if (difftime < max_time*BI){ //300000000000){
                crow::mustache::context x;
             
                x["id"] = id;
                x["letters"] = board;
                x["time_diff"] = time_diff;
                x["score"] = get_score_from_id(id);
            
                auto page = crow::mustache::load("/templates/game.html");
                return page.render(x);
            }else{
                crow::mustache::context x;
                x["id"] = id;
                x["letters"] = board;
                x["score"] = get_score_from_id(id);            
                auto page = crow::mustache::load("/templates/score.html");
                return page.render(x);
            }
        }
     );
     
    CROW_ROUTE(app, "/game/<string>/<string>")
      ([](string id, string word){
            string board = get_board_from_id(id);
            if (board == "Board doesn't exist"){
                return crow::response(""); 
            }
            
            auto time = Clock::now();
            long int time_now = std::chrono::duration_cast<std::chrono::nanoseconds>(time.time_since_epoch()).count();
            string time_creation = get_time_from_id(id);
            
            long int difftime = time_now-stol(time_creation);
            
            if (difftime < max_time*BI){
                string score = submit(word, id);
                return crow::response(score);
            }else{
                //return crow::response(get_score_from_id(id));
                crow::response res = crow::response(302);
                res.set_header("Location","http://localhost:18080/game/"+id);
                return res;
            }
        }
        
    
        
    );
    
  

    CROW_ROUTE(app, "/game")([](){
        srand (time(NULL));
        string id = new_id();
        crow::response res = crow::response(302);
        res.set_header("Location","http://localhost:18080/game/"+id);
        return res;
    });
    
    
    
    app.port(18080).multithreaded().run();
}

// g++ server.cpp -o server -I /usr/include/postgresql/ -L /usr/lib/ -lm -lpq -pthread -lboost_system -std=c++11
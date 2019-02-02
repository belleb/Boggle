#include "crow_all.h"
#include "find_word.cpp"
#include "submit_word.cpp"
long int const max_time = 300;
long int const BI = 1000000000;


using Clock = std::chrono::high_resolution_clock;
int main()
{
    static string address = "http://thesaurus-rex.org"; //"http://localhost:18080";
    //Creates dictionaries
    static Dictionary_en d_en;
    static Dictionary_pt d_pt;
    
    
    crow::SimpleApp app;
    crow::mustache::set_base(".");

    CROW_ROUTE(app, "/")([](){
        crow::response res = crow::response(302);
        res.set_header("Location",address+"/en");
        return res;
    });
    
    CROW_ROUTE(app, "/<string>")([](string language){
        if ((language != "pt") and (language != "en") ){
            string error = "404 NOT FOUND";
            return error;
        }
        crow::mustache::context x;
        x["language"] = language;
        auto page = crow::mustache::load("/templates/home.html");
        return page.render(x);
        
    });
    
    CROW_ROUTE(app, "/<string>/game/<string>")
      ([](string language, string id){
            string board = get_board_from_id(id);
            
            //tests if the user entered valid id and valid language. If not, shows "404 NOT FOUND"
            if ((board == "Board doesn't exist") or ((language != "en") and (language != "pt"))){
                string error = "404 NOT FOUND";
                return error;
            }
            
            
            
            //gets current time
            auto time = Clock::now();
            long int time_now = std::chrono::duration_cast<std::chrono::nanoseconds>(time.time_since_epoch()).count();
            //get time of creation of the board
            string time_creation = get_time_from_id(id);
            
            //calculates time elapsed and remaining time
            long int difftime = time_now-stol(time_creation);
            long int time_diff = ((max_time+1)*BI-difftime)/1000000; //remaining time in milliseconds
            
            //parameters for loading page
            crow::mustache::context x;
                x["id"] = id;
                x["letters"] = board;
                x["time_diff"] = time_diff;
                x["score"] = get_score_from_id(id);
                x["language"] = language;
            
            //decides if the game has ended or not, and behaves accordingly
            if (difftime < max_time*BI){ 
                auto page = crow::mustache::load("/templates/game.html");
                return page.render(x);
            }else{ //game ended         
                x["ranking"] = get_ranking_from_id(id,language);
                auto page = crow::mustache::load("/templates/score.html");
                return page.render(x);
            }
        }
     );
     
    CROW_ROUTE(app, "/<string>/game/<string>/<string>")
      ([](string language,string id, string word){
            string board = get_board_from_id(id);
            //tests if the user entered valid id and valid language. If not, redirects to home
            if ((board == "Board doesn't exist") or ((language != "en") and (language != "pt"))){
                crow::response res = crow::response(302);
                res.set_header("Location",address);
                return res;
            }
            
            //gets current time and time of creation
            auto time = Clock::now();
            long int time_now = std::chrono::duration_cast<std::chrono::nanoseconds>(time.time_since_epoch()).count();
            string time_creation = get_time_from_id(id);
            
            //calculates total time elapsed
            long int difftime = time_now-stol(time_creation);
            
            //decides if game has ended and behaves accordingly
            if (difftime < max_time*BI){
                string score = submit(word, id,language,board,d_en,d_pt);
                return crow::response(score);
            }else{
                crow::response res = crow::response(302);
                res.set_header("Location",address+"/"+language+"/game/"+id);
                return res;
            }
        }
        
    
        
    );
    
  

    CROW_ROUTE(app, "/<string>/game")([](string language){
        //tests if the language is valid. If not, goes to home  
        if (((language != "en") and ((language) != "pt"))){
            crow::response res = crow::response(302);
            res.set_header("Location",address);
            return res;
        }
        srand (time(NULL));
        string id = new_id(language);
        crow::response res = crow::response(302);
        res.set_header("Location",address+"/"+language+"/game/"+id);
        return res;
    });
    
    
    app.port(80).multithreaded().run();//    app.port(18080).multithreaded().run();
}

// g++ server.cpp -o server -I /usr/include/postgresql/ -L /usr/lib/ -lm -lpq -pthread -lboost_system -std=c++11
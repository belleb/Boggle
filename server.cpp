#include "crow_all.h"
#include "submit_word.cpp"
int main()
{
    crow::SimpleApp app;
    crow::mustache::set_base(".");

  /*  CROW_ROUTE(app, "/")([](){
        re
    });
    */
    
    CROW_ROUTE(app, "/game/<string>")
      ([](string id){
            string board = get_board_from_id(id);
            string time = get_time_from_id(id);
            crow::mustache::context x;
         
            x["id"] = id;
            x["letters"] = board;
        
            auto page = crow::mustache::load("/templates/index.html");
            return page.render(x);
        }
     );
     
    CROW_ROUTE(app, "/game/<string>/<string>")
      ([](string id, string word){
            submit(word, id);
            return word+id;
        }
        
    
        
        
        
        //calculates time difference between game creation and now
        /*struct tm t, t1;
        double seconds;
        time_t ts;
        time( &ts );
        t = * localtime( &ts );
        t1 = t;
        */
        
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
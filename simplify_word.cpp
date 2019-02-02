#include <iostream>
#include <string>


using namespace std;
//removes accents
string simplify(string str){
    string output; 
    int i;
    string tr = "AAAAAAECEEEEIIIIDNOOOOOx0UUUUYPsaaaaaaeceeeeiiiiOnooooo/0uuuuypy";
    for (i=0; i< str.length(); i++){
        unsigned char ch = str[i];
        if (ch >= 192){
            output += tr[ch-192];
        }else{
            output += str[i];
        }
    }
    
    return output;
    
}
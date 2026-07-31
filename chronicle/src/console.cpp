#include <iostream>
#include <string>
#include "../include/console.hpp"

using namespace std;

void console(){
    string in;
    while(1){
        cout<<"ironclad>\t";
        cin>>in;
        if(in == ".help"){
            cout <<".quit Shut down the world engine \n .help       Show this message \n .version    Show engine version \n .status     Show world status\n";

        }
        else if(in ==".version"){
            cout << "Chronicle v0.1 — Ironhold World Engine\n";
        }
        else if(in==".quit"){
            break;
        }
        else if(in==".status"){
            cout << "World: Offline\n";
        }
        else if(in.substr(0,1)=="."){
                cout << "AppState::UNKNOWN \n";
        }
        else{
            cout << "World command queued: \n";
        }
    }
   
}
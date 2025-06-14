#include <iostream>
#include <vector> 
#include <string>
#include "vault.h"


int main(){
    
    
    
    //Will add functionality requiring an encrpyted master password
    int mainMenuInput;
    std::string entry;
    std::cout << "/n/nMain Menu: /n";
    std::cout << "[1] Add Entry:/n[2] View Entries/n[3] Edit Entries/n[4] Delete Entry/n[5] Change Master Password/n[6] Exit";
    std::cin >> mainMenuInput;
    switch(mainMenuInput){
        case 1:
        //
        break;
        case 2:
        //
        break;
        case 3:
        //
        break;
        case 4:
        //
        break;
        case 5:
        //
        break;
        case 6:
        //
        break;
        default:
        std::cout <<"INVALID INPUT: Please enter an integer value 1-6";
        return 0;
        break;
    }
}
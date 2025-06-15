#include <iostream>
#include <vector> 
#include <string>
#include "vault.h"
#include <fstream>
#include <sstream>
#include "sha256.h"
#include <iomanip>
#include "hash_util.h"
//compile with -> g++ main.cpp vault.cpp hash_util.cpp sha256.c -o vaultApp


int main(){
    VaultManager vault;
    std::string key;
    const std::string masterFile = "master.txt";
    const std::string dataFile = "data.txt";


    std::cout << "Welcome to Matt's Rad Password Manager!\n";
    
   std::ifstream check("master.txt");
bool needsSetup = false;

if (!check.good()) {
    needsSetup = true;  // File does not exist
} else {
    std::string line;
    std::getline(check, line);
    if (line.empty()) {
        needsSetup = true;  // File exists but is empty
    }
}
check.close();

if (needsSetup) {
    std::cout << "No master password found. Let's set one up.\n";
    vault.setMasterPassword();
}

     if (!vault.verifyMasterPassword(key)) {
        std::cerr << "Exiting: Failed to verify master password.\n";
        return 1;
    }
    vault.loadFromFile(dataFile, key);
    int mainMenuInput = 0;
    std::string entry;
    

    while (mainMenuInput != 7) {
        std::cout << "\n--- Main Menu ---\n";
        std::cout << "[1] Add a New Credential\n";
        std::cout << "[2] View All Credentials\n";
        std::cout << "[3] Edit Credential\n";
        std::cout << "[4] Delete Credential\n";
        std::cout << "[5] Update Master Password\n";
        std::cout << "[6] Search Credential\n";  // ✅ New option
        std::cout << "[7] Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> mainMenuInput;

    switch(mainMenuInput){
        case 1:
            vault.addCredential(Credential());
            vault.saveToFile(dataFile, key);
        break;
        case 2:
            vault.listCredentials();
        break;
        case 3:
            vault.editCredential();
            vault.saveToFile(dataFile, key);
        break;
        case 4:
            vault.deleteCredential();
            vault.saveToFile(dataFile, key);
        break;
        case 5: {
                std::string oldPass, newPass;
                std::cout << "Enter current master password: ";
                std::cin >> oldPass;
                std::cout << "Enter new master password: ";
                std::cin >> newPass;
                vault.updateMasterPassword(oldPass, newPass, dataFile);
                key = newPass;
                break;
            }
        case 6:
            vault.searchCredential();
        break;
        case 7:
            std::cout << "Goodbye!\n";
        break;
        default:
        std::cout <<"INVALID INPUT: Please enter an integer value 1-6";
        return 1;
        break;}
    
    }
}
#include <iostream>
#include <vector> 
#include <string>
#include "vault.h"
#include <fstream>
#include <sstream>
#include "sha256.h"
#include <iomanip>
#include "hash_util.h"

    std::string hashString(const std::string& input) {
    return sha256(input);  // wrapper class -> calls the actual function from sha256.h
}


 //Credential class functions
    
    std::string Credential::serialize(const std::string key) const {
        return website + "|" + encrypt(username, key) + "|" + encrypt(password, key) + "|" + encrypt(email, key);
    }
    
    Credential::Credential(const std::string& site, const std::string& user, const std::string& pass, const std::string& em)
        : website(site), username(user), password(pass), email(em){
    }

    Credential::Credential(){}
    
    Credential::~Credential() {
    }


     Credential Credential::fromSerialized(const std::string& line, const std::string& key){
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> fields;
        
        while (std::getline(ss, token, '|')){ 
            //this loop uses "|" as a delimiter to seperate the string into each part
            fields.push_back(token);
        }

        if (fields.size() != 4) {
            throw std::runtime_error("Invalid credential format.");
        }

        std::string decryptedUsername = decrypt(fields[1], key);
        std::string decryptedPassword = decrypt(fields[2], key);
        std::string decryptedEmail = decrypt(fields[3], key);
        return Credential(fields[0], decryptedUsername, decryptedPassword, decryptedEmail);
   
    }

    std::string Credential::encrypt(const std::string& text, const std::string& key) const {
        std::string encrypted;
        for (int i = 0; i < text.size(); i++){
            encrypted.push_back(text[i] ^ (key[i % key.size()]));
        }
        return encrypted;
    }

    std::string Credential::decrypt(const std::string& text, const std::string& key){
        std::string decrypted;
        for (int i = 0; i < text.size(); i++) {
            decrypted.push_back(text[i] ^ key[i % key.size()]);
        }
        return decrypted;   
    }

    std::string Credential::getWebsite() const { //setters/getters
        return this -> website;
    }
    std::string Credential::getUsername() const {
        return this -> username;
    }
    std::string Credential::getPassword() const {
        return this -> password;
    }
    std::string Credential::getEmail() const {
        return this -> email;
    }
    
    void Credential::setWebsite(const std::string& site) {
        website = site;
    }

    void Credential::setUsername(const std::string& uname){
        username = uname;
    }
    void Credential::setPassword(const std::string& pass){
        password = pass;
    }
    void Credential::setEmail(const std::string& em){
        email = em;
    }



    //Vault manager functions
    bool VaultManager::verifyMasterPassword(std::string& key){
    std::ifstream in("master.txt");
    if (!in) {
        std::cerr << "Master password file not found.\n";
        return false;
    }
    std::string storedHash;
    std::getline(in, storedHash);
    in.close();

    std::string attempt;
    std::cout << "Enter your master password: ";
    std::cin >> attempt;

    std::string attemptHash = sha256(attempt);
    if (attemptHash == storedHash) {
        key = attempt;  // Set key for use in encryption/decryption
        return true;
    } else {
        std::cerr << "Incorrect password.\n";
        return false;
    }
}

    void VaultManager::setMasterPassword(){
        std::string password, confirm;
        std::cout << "Enter your new password:\n";
        std::cin >> password;
        std::cout << "Confirm your new password:\n";
        std::cin >> confirm;

        if (password != confirm){
            std::cerr << "Passwords do not match.\n";
            return;
        }
        std::string hashed = sha256(password);
        std::ofstream ofile("master.txt", std::ios::trunc);

        if (!ofile.is_open()){
            std::cerr << "Could not open file\n";
            return;
        }

        ofile << hashed;
        ofile.close();
        std::cout << "Master Password is now set.\n";

    }

    void VaultManager::updateMasterPassword(const std::string& oldPassword, const std::string& newPassword, const std::string& filename) {
    // Step 1: Verify the current master password
    std::ifstream inFile("master.txt");
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open master.txt\n";
        return;
    }

    std::string storedHash;
    std::getline(inFile, storedHash);
    inFile.close();

    if (sha256(oldPassword) != storedHash) {
        std::cerr << "Error: Old master password is incorrect.\n";
        return;
    }

    // Step 2: Load and decrypt credentials using old master password
    std::ifstream credIn(filename);
    if (!credIn.is_open()) {
        std::cerr << "Error: Could not open credentials file.\n";
        return;
    }

    std::vector<Credential> decryptedCreds;
    std::string line;
    while (std::getline(credIn, line)) {
        try {
            Credential c = Credential::fromSerialized(line, oldPassword);
            decryptedCreds.push_back(c);
        } catch (const std::exception& e) {
            std::cerr << "Skipped invalid credential line: " << e.what() << "\n";
        }
    }
    credIn.close();

    // Step 3: Overwrite master.txt with new password hash
    std::ofstream outFile("master.txt", std::ios::trunc);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not write to master.txt\n";
        return;
    }
    outFile << sha256(newPassword);
    outFile.close();

    // Step 4: Re-encrypt and save credentials using new password
    std::ofstream credOut(filename, std::ios::trunc);
    if (!credOut.is_open()) {
        std::cerr << "Error: Could not open credentials file for writing.\n";
        return;
    }

    for (const Credential& c : decryptedCreds) {
        credOut << c.serialize(newPassword) << "\n";
    }
    credOut.close();

    // Step 5: Update in-memory creds list with decrypted versions
    creds = decryptedCreds;

    std::cout << "Master password updated successfully. Credentials re-encrypted and in-memory list updated.\n";
}



    void VaultManager::addCredential(const Credential& c){
        std::string site;
        std::string uname;
        std::string pass;
        std::string em;
        std::string passcheck;
        std::cout << "ADD A CREDENTIAL:\n Enter website name or URL:\n";
        std::cin >> site;
        std::cout << "Enter the username (n/a if none)\n";
        std::cin >> uname;
        std::cout << "Enter the password:\n";
        std::cin >> pass;
        std::cout << "Enter the email for this account:\n";
        std::cin >> em;

        Credential newCred(site, uname, pass, em);
        creds.push_back(newCred);
    }

    
    void VaultManager::saveToFile(const std::string& filename, const std::string& key){
        std::ofstream outFile(filename);
        if (!outFile.is_open()){
            std::cerr << "Error: Could not open file";
            return;
        }
        for (const Credential& cred : creds){
            outFile << cred.serialize(key) << "\n";
        }
            
    }

    
    void VaultManager::loadFromFile(const std::string& filename, const std::string& key){
        creds.clear();
        std::string line;
        std::ifstream inFile(filename);
        if (!inFile.is_open()){
            std::cerr << "Error: Could not open file";
            return;
        }

        while (std::getline(inFile, line)){
            try {
                Credential c = Credential::fromSerialized(line, key);
                creds.push_back(c);
            } catch (const std::exception& e) {
                std::cerr << "Skipped Invalid Line:"  << e.what() << "\n";
            }
        }
        
    }

    
    void VaultManager::listCredentials(){
        for (int i = 0; i < creds.size(); i++){
            std::cout << "Entry #" << i + 1 << ":" << creds[i].getWebsite() << ", ";
            std::cout << "Username:" << creds[i].getUsername() << ", ";
            std::cout << "Password:" << creds[i].getPassword() << ", ";
            std::cout << "Email:" << creds[i].getEmail() << "\n";
        }
    }

    void VaultManager::editCredential(){
        std::string input;
        std::string newValue;
        int index;
        int var;
        bool found = false;
        std::cout << "Please enter the website you'd like to edit (case sensitive)\n";
        std::cin >> input;
        for (int i = 0; i < creds.size(); i++){
            if (creds[i].getWebsite() == input){
            index = i;
            found = true;
            break;
            } 
         }
         if (!found){
            std::cout << "Website not found. Please check your spelling and try again\n";
            return;
         }
        std::cout << "Which field would you like to change?\n";
        std::cout << "[1] Username\n[2] Password\n[3] Email\n";
        std:: cin >> var;

        switch (var)
        {
        case 1:
            std::cout << "Enter your new username:\n";
            std::cin >> newValue;
            creds[index].setUsername(newValue);
            break;

         case 2:
            std::cout << "Enter your new password:\n";
            std::cin >> newValue;
            creds[index].setPassword(newValue);
            break;
        
         case 3:
            std::cout << "Enter your new email:\n";
            std::cin >> newValue;
            creds[index].setEmail(newValue);
            break;
        
        
        default:
            std::cout << "Invalid Input\nPlease enter an integer 1-3\n";
            break;
        }

    }
   
    void VaultManager::deleteCredential(){
        std::string input;
        std::string confirm;
        int index;
        bool found = false;
        std::cout << "Please enter the website you'd like to delete (case sensitive)\n";
        std::cin >> input;
        for (int i = 0; i < creds.size(); i++){
            if (creds[i].getWebsite() == input){
            index = i;
            found = true;
            std::cout << "To confirm, please enter the exact website name again:\n";
            std::cin >> confirm;
            if (confirm == input){
                creds.erase(creds.begin() + index);
                break;
            } else {
            std::cout << "Websites do not match.\n";
            return;
            }
            } 
         }
         if (!found){
            std::cout << "Website not found. Please check your spelling and try again\n";
            return;
         }
    }

    void VaultManager::searchCredential(){
        std::string input;
        bool found = false;
        std::cout << "Please enter the website you'd like to view: (case sensitive)\n";
        std::cin >> input;
        for (int i = 0; i < creds.size(); i++){
            if (creds[i].getWebsite() == input){
            found = true;
            std::cout << "Login information for: " << input << "\n";
            std::cout<< "Username: " << creds[i].getUsername() << "\n";
            std::cout<< "Password: " << creds[i].getPassword() << "\n";
            std::cout<< "Email: " << creds[i].getEmail() << "\n";
            break;
            } 
         }
         if (!found){
            std::cout << "Website not found. Please check your spelling and try again\n";
            return;
         }    
    }
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "sha256.h"
#include <iomanip>

std::string hashString(const std::string& input);


 class Credential {
    private:
        std::string website;
        std::string username;
        std::string password;
        std::string email;

        std::string encrypt(const std::string& text, const std::string& key) const;
        static std::string decrypt(const std::string& text, const std::string& key);

    public:
        std::string serialize(const std::string key) const;
        static Credential fromSerialized(const std::string& line, const std::string& key);
        std::string getWebsite() const;
        std::string getUsername() const;
        std::string getPassword() const;
        std::string getEmail() const;
        void setWebsite(const std::string& site);
        void setUsername(const std::string& uname);
        void setPassword(const std::string& pass);
        void setEmail(const std::string& em);

        //constructors
        Credential(const std::string& website, const std::string& username, const std::string& password, const std::string& email);
        Credential(); 
        ~Credential();  


    };

class VaultManager {
    //class to contain vect of all Credentials
    private:
    std::vector<Credential> creds;
    
    
    public: 
        int index;

        void addCredential(const Credential& c);
        void saveToFile(const std::string& filename, const std::string& key);
        void loadFromFile(const std::string& filename, const std::string& key);
        void listCredentials();
        void editCredential();
        void deleteCredential();
        void searchCredential();
        void setMasterPassword();
        void updateMasterPassword(const std::string& oldPassword, const std::string& newPassword, const std::string& filename);
        bool verifyMasterPassword(std::string& key);

};





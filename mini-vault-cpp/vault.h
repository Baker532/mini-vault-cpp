#include <iostream>
#include <string>
#include <vector>

std::vector<credential> creds;

 class credential {
        private:
        std::string website;
        std::string username;
        std::string password;
        std::string email;
        bool require_username;

        public:
        void addCredential(std::vector<credential>& creds);
        void saveCredential(const std::vector<credential>& creds);
        void loadCredential(std::vector<credential>& creds, const std::string& filename);
        void viewCredential(const std::vector<credential>& creds);
        void setField(std::string entry);
    };




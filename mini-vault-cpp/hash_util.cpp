#include "sha256.h"
#include <string>
#include <sstream>
#include <iomanip>
//Side note to study hashing more 
std::string sha256(const std::string& input) {
    BYTE hash[SHA256_BLOCK_SIZE];
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, (const BYTE*)input.c_str(), input.length());
    sha256_final(&ctx, hash);

    std::stringstream ss;
    for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

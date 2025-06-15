# 🔐 Matt's Rad Password Manager

A lightweight terminal-based password manager written in modern C++, using SHA-256 for master password hashing and XOR-based encryption for stored credentials.

FUTURE PLANS: More secure Encryption, sort entries, import from csv (maybe), export to csv 
## Eventually will port to java and spring boot with angulat front end

---

## 📋 Features

- Set and verify a **master password** (stored as a SHA-256 hash)
- Add, edit, view, search, and delete **encrypted credentials**
- Encrypts each credential’s **username**, **password**, and **email**
- Automatically **re-encrypts all credentials** when master password is updated
- Credentials are saved to and loaded from a file
- Clean and simple terminal interface

---

## 📁 File Structure

├── main.cpp # Main program logic and menu
├── vault.cpp # VaultManager and Credential class implementations
├── vault.h # VaultManager and Credential class declarations
├── hash_util.cpp # Wrapper to simplify SHA-256 usage
├── hash_util.h
├── sha256.c # SHA-256 algorithm implementation (C)
├── sha256.h
├── master.txt # Stores hashed master password (SHA-256)
├── data.txt # Encrypted credentials storage
└── README.md


---

## 🔧 How to Compile

Use `g++` to compile all source files:

```bash
g++ main.cpp vault.cpp hash_util.cpp sha256.c -o vaultApp

./vaultApp

#include <iostream>
#include <string>
#include <limits>

// Added the source code for sqlite3 as
// it doesn't look like the library is 
// installed on thelinux machines
#include "sqlite3.h"

bool authenticate(sqlite3* db, const std::string& username, const std::string& password)
{
    // Create a query to count the number of rows with the inputted username and password
    std::string query =
        "SELECT COUNT(*) FROM users WHERE username = '" + username + "' AND password = '" + password + "'";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) 
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    bool isAuthenticated = false;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);

        // If the count is more than 0, then the account exists and the credentials
        // are correct.
        isAuthenticated = (count > 0);
    }

    return isAuthenticated;
}

bool get_secret(sqlite3* db, const std::string& username, std::string* secret)
{
    // Create a qyery to get the secret that corresponds to the inputted username
    std::string query =
        "SELECT secret FROM users WHERE username = '" + username + "'";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) 
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        *secret = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        return true;
    }

    return false;
}

int main()
{
    sqlite3* users_db;

    if (sqlite3_open("./users.db", &users_db) != 0)
    {
        std::cerr << "Could not open the database: " << sqlite3_errmsg(users_db) << std::endl;
        return 1;
    }

    std::string username;
    std::string password;

    std::cout << "Please sign in with your account:\n";
    
    std::cout << "Username: ";
    if (!std::getline(std::cin, username))
    {
        std::cerr << "Invalid username input!" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return 1;
    }

    std::cout << "Password: ";
    if (!std::getline(std::cin, password))
    {
        std::cerr << "Invalid password input!" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return 1;
    }

    // Authenticate the user with the inputted credentials
    if (authenticate(users_db, username, password))
    {
        // If the authentication is successful, then we retrive
        // the secret.
        std::string secret;
        if (get_secret(users_db, username, &secret))
        {
            std::cout << "You've logged in successfully!\n";
            std::cout << "Your secret is: " << secret << "\n";
        }
        else
        {
            std::cout << "Something went wrong while getting secret\n";
            return 1;
        }
    }
    else
    {
        std::cout << "Invalid credentials!\n";
    }

    return 0;
}
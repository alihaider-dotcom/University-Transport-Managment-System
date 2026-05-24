#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
using namespace std;

// Abstract Base Class — cannot be instantiated directly
class User {
protected:
    string userId;
    string name;
    string email;
    string password;
    string role;   // "Student", "Admin"

public:
    User();
    User(string uid, string n, string e, string pwd, string r);
    User(const User& other);
    User& operator=(const User& other);
    virtual ~User();

    // Pure virtual functions — must be overridden by all derived classes
    virtual void displayMenu()  = 0;
    virtual void displayInfo()  const = 0;
    virtual string getType()    const = 0;

    // Common methods
    bool verifyPassword(const string& pwd) const;

    string getUserId()  const;
    string getName()    const;
    string getEmail()   const;
    string getRole()    const;

    // Operator overloading
    bool operator==(const User& other) const;
    friend ostream& operator<<(ostream& os, const User& u);

    // File I/O
    virtual string serialize()   const;
    virtual void   deserialize(const string& line);
};

#endif

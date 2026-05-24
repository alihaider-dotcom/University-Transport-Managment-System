#include "User.h"
#include <sstream>

User::User() : userId(""), name(""), email(""), password(""), role("") {}

User::User(string uid, string n, string e, string pwd, string r)
    : userId(uid), name(n), email(e), password(pwd), role(r) {}

User::User(const User& other)
    : userId(other.userId), name(other.name), email(other.email),
      password(other.password), role(other.role) {}

User& User::operator=(const User& other) {
    if (this != &other) {
        userId   = other.userId;
        name     = other.name;
        email    = other.email;
        password = other.password;
        role     = other.role;
    }
    return *this;
}

User::~User() {}

bool User::verifyPassword(const string& pwd) const {
    return password == pwd;
}

string User::getUserId() const { return userId; }
string User::getName()   const { return name; }
string User::getEmail()  const { return email; }
string User::getRole()   const { return role; }

bool User::operator==(const User& other) const {
    return userId == other.userId;
}

ostream& operator<<(ostream& os, const User& u) {
    os << "ID: " << u.userId
       << " | Name: " << u.name
       << " | Role: " << u.role
       << " | Email: " << u.email;
    return os;
}

string User::serialize() const {
    return userId + "|" + name + "|" + email + "|" + password + "|" + role;
}

void User::deserialize(const string& line) {
    stringstream ss(line);
    getline(ss, name,     '|');
    getline(ss, email,    '|');
    getline(ss, userId,   '|');
    getline(ss, password, '|');
    getline(ss, role,     '|');
}

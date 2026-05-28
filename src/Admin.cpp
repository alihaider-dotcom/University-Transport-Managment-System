#include "Admin.h"
#include <iostream>
#include <sstream>

Admin::Admin() : User(), adminCode("") {}

Admin::Admin(string uid, string name, string email, string pwd, string code)
    : User(uid, name, email, pwd, "Admin"), adminCode(code) {}

Admin::Admin(const Admin& other)
    : User(other), adminCode(other.adminCode) {}

Admin& Admin::operator=(const Admin& other) {
    if (this != &other) {
        User::operator=(other);
        adminCode = other.adminCode;
    }
    return *this;
}

Admin::~Admin() {}

void Admin::displayMenu() {
    cout << "\n===== ADMIN MENU =====\n"
         << "1.  Add Vehicle\n"
         << "2.  Edit Vehicle\n"
         << "3.  Remove Vehicle\n"
         << "4.  View All Vehicles\n"
         << "5.  Add Route\n"
         << "6.  Assign Vehicle to Route\n"
         << "7.  View All Routes\n"
         << "8.  View Applications\n"
         << "9.  Approve / Reject Request\n"
         << "10. Apply Late Fine\n"
         << "11. Generate Revenue Report\n"
         << "12. Generate Route Usage Report\n"
         << "13. Save Data\n"
         << "14. Logout\n"
         << "======================\n"
         << "Choice: ";
}

void Admin::displayInfo() const {
    cout << "Admin | ID: " << userId
         << " | Name: " << name
         << " | Email: " << email << "\n";
}

string Admin::getType()      const { return "Admin"; }
string Admin::getAdminCode() const { return adminCode; }

string Admin::serialize() const {
    return User::serialize() + "|" + adminCode;
}

void Admin::deserialize(const string& line) {
    stringstream ss(line);
    string uid, n, e, pwd, r;
    getline(ss, uid,  '|');
    getline(ss, n,    '|');
    getline(ss, e,    '|');
    getline(ss, pwd,  '|');
    getline(ss, r,    '|');
    userId = uid; name = n; email = e; password = pwd; role = r;
    getline(ss, adminCode, '|');
}
#include "Student.h"
#include <sstream>
#include <iostream>

Student::Student() : User(), department(""), semester(""), passId("") {}

Student::Student(string uid, string name, string email, string pwd,
                 string dept, string sem)
    : User(uid, name, email, pwd, "Student"),
      department(dept), semester(sem), passId("") {}

Student::Student(const Student& other)
    : User(other), department(other.department),
      semester(other.semester), passId(other.passId) {}

Student& Student::operator=(const Student& other) {
    if (this != &other) {
        User::operator=(other);
        department = other.department;
        semester   = other.semester;
        passId     = other.passId;
    }
    return *this;
}

Student::~Student() {}

void Student::displayMenu() {
    cout << "\n===== STUDENT MENU =====\n"
         << "1. View All Routes\n"
         << "2. Apply for Transport\n"
         << "3. View My Registration\n"
         << "4. Cancel Registration\n"
         << "5. Logout\n"
         << "========================\n"
         << "Choice: ";
}

void Student::displayInfo() const {
    cout << "Student  | ID: " << userId
         << " | Name: " << name
         << " | Dept: " << department
         << " | Sem: " << semester
         << " | Email: " << email
         << " | Pass: " << (passId.empty() ? "None" : passId) << "\n";
}

string Student::getType()       const { return "Student"; }
void   Student::setPassId(const string& pid) { passId = pid; }
string Student::getPassId()     const { return passId; }
string Student::getDepartment() const { return department; }
string Student::getSemester()   const { return semester; }
bool   Student::hasPass()       const { return !passId.empty(); }

string Student::serialize() const {
    return User::serialize() + "|" + department + "|" + semester + "|" + passId;
}

void Student::deserialize(const string& line) {
    stringstream ss(line);
    string uid, n, e, pwd, r;
    getline(ss, uid,  '|');
    getline(ss, n,    '|');
    getline(ss, e,    '|');
    getline(ss, pwd,  '|');
    getline(ss, r,    '|');
    userId = uid; name = n; email = e; password = pwd; role = r;
    getline(ss, department, '|');
    getline(ss, semester,   '|');
    getline(ss, passId,     '|');
}

#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include "TransportPass.h"

class Student : public User {
private:
    string department;
    string semester;
    string passId;      // Association: Student links to TransportPass by ID

public:
    Student();
    Student(string uid, string name, string email, string pwd,
            string dept, string sem);
    Student(const Student& other);
    Student& operator=(const Student& other);
    ~Student();

    // Implement pure virtual functions
    void   displayMenu()      override;
    void   displayInfo() const override;
    string getType()     const override;

    void   setPassId(const string& pid);
    string getPassId()    const;
    string getDepartment() const;
    string getSemester()   const;
    bool   hasPass()       const;

    string serialize()              const override;
    void   deserialize(const string& line) override;
};

#endif
#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

class Admin : public User {
private:
    string adminCode;

public:
    Admin();
    Admin(string uid, string name, string email, string pwd, string code);
    Admin(const Admin& other);
    Admin& operator=(const Admin& other);
    ~Admin();

    void   displayMenu()      override;
    void   displayInfo() const override;
    string getType()     const override;

    string getAdminCode() const;

    string serialize()              const override;
    void   deserialize(const string& line) override;
};

#endif

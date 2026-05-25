#ifndef BILL_H
#define BILL_H

#include <iostream>
#include <string>
using namespace std;

class Bill {
private:
    float  baseFee;
    float  lateFine;
    bool   isPaid;
    string dueDate;
    string issueDate;

public:
    Bill();
    Bill(float fee, string due, string issued);
    Bill(const Bill& other);
    Bill& operator=(const Bill& other);
    ~Bill();

    void  applyLateFine(float fine);
    void  markPaid();
    float getTotalAmount() const;
    float getBaseFee()     const;
    float getLateFine()    const;
    bool  getIsPaid()      const;
    string getDueDate()    const;
    string getIssueDate()  const;

    // Operator overloading — print invoice
    friend ostream& operator<<(ostream& os, const Bill& b);
    bool operator==(const Bill& other) const;

    string serialize()              const;
    void   deserialize(const string& line);
};

#endif

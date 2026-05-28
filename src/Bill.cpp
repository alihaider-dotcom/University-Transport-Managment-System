#include "Bill.h"
#include <sstream>
#include <iomanip>

Bill::Bill() : baseFee(0), lateFine(0), isPaid(false), dueDate("N/A"), issueDate("N/A") {}

Bill::Bill(float fee, string due, string issued)
    : baseFee(fee), lateFine(0), isPaid(false), dueDate(due), issueDate(issued) {}

Bill::Bill(const Bill& other)
    : baseFee(other.baseFee), lateFine(other.lateFine),
      isPaid(other.isPaid), dueDate(other.dueDate), issueDate(other.issueDate) {}

Bill& Bill::operator=(const Bill& other) {
    if (this != &other) {
        baseFee   = other.baseFee;
        lateFine  = other.lateFine;
        isPaid    = other.isPaid;
        dueDate   = other.dueDate;
        issueDate = other.issueDate;
    }
    return *this;
}

Bill::~Bill() {}

void Bill::applyLateFine(float fine) { lateFine += fine; }
void Bill::markPaid()                { isPaid = true; }

float  Bill::getTotalAmount() const { return baseFee + lateFine; }
float  Bill::getBaseFee()     const { return baseFee; }
float  Bill::getLateFine()    const { return lateFine; }
bool   Bill::getIsPaid()      const { return isPaid; }
string Bill::getDueDate()     const { return dueDate; }
string Bill::getIssueDate()   const { return issueDate; }

bool Bill::operator==(const Bill& other) const {
    return baseFee == other.baseFee && dueDate == other.dueDate;
}

ostream& operator<<(ostream& os, const Bill& b) {
    os << fixed << setprecision(2)
       << "  Issue Date : " << b.issueDate
       << "\n  Due Date   : " << b.dueDate
       << "\n  Base Fee   : Rs. " << b.baseFee
       << "\n  Late Fine  : Rs. " << b.lateFine
       << "\n  Total      : Rs. " << b.getTotalAmount()
       << "\n  Status     : " << (b.isPaid ? "PAID" : "UNPAID");
    return os;
}

string Bill::serialize() const {
    return to_string(baseFee) + "|" + to_string(lateFine) + "|" +
           to_string(isPaid)  + "|" + dueDate + "|" + issueDate;
}

void Bill::deserialize(const string& line) {
    stringstream ss(line);
    string tmp;
    getline(ss, tmp, '|'); baseFee   = stof(tmp);
    getline(ss, tmp, '|'); lateFine  = stof(tmp);
    getline(ss, tmp, '|'); isPaid    = (tmp == "1");
    getline(ss, dueDate,   '|');
    getline(ss, issueDate, '|');
}

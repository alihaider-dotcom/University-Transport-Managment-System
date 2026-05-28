#include "TransportPass.h"
#include <sstream>

TransportPass::TransportPass()
    : passId(""), studentId(""), routeId(""), status("Pending"), bill() {}

TransportPass::TransportPass(string pid, string sid, string rid,
                             float fee, string due, string issued)
    : passId(pid), studentId(sid), routeId(rid),
      status("Pending"), bill(fee, due, issued) {}

TransportPass::TransportPass(const TransportPass& other)
    : passId(other.passId), studentId(other.studentId),
      routeId(other.routeId), status(other.status),
      bill(other.bill) {}   // Bill copy constructor called here

TransportPass& TransportPass::operator=(const TransportPass& other) {
    if (this != &other) {
        passId    = other.passId;
        studentId = other.studentId;
        routeId   = other.routeId;
        status    = other.status;
        bill      = other.bill;
    }
    return *this;
}

TransportPass::~TransportPass() {
    // bill destroyed automatically — composition in action
}

void TransportPass::approve()              { status = "Approved";  }
void TransportPass::reject()               { status = "Rejected";  }
void TransportPass::cancel()               { status = "Cancelled"; }
void TransportPass::payBill()              { bill.markPaid(); }
void TransportPass::applyLateFine(float f) { bill.applyLateFine(f); }

string TransportPass::getPassId()    const { return passId; }
string TransportPass::getStudentId() const { return studentId; }
string TransportPass::getRouteId()   const { return routeId; }
string TransportPass::getStatus()    const { return status; }
Bill&  TransportPass::getBill()            { return bill; }
const Bill& TransportPass::getBill() const { return bill; }

bool TransportPass::operator==(const TransportPass& other) const {
    return passId == other.passId;
}

ostream& operator<<(ostream& os, const TransportPass& tp) {
    os << "Pass ID  : " << tp.passId
       << "\nStudent  : " << tp.studentId
       << "\nRoute    : " << tp.routeId
       << "\nStatus   : " << tp.status
       << "\n--- Invoice ---\n" << tp.bill;
    return os;
}

string TransportPass::serialize() const {
    return passId + "|" + studentId + "|" + routeId + "|" +
           status + "|" + bill.serialize();
}

void TransportPass::deserialize(const string& line) {
    stringstream ss(line);
    getline(ss, passId,    '|');
    getline(ss, studentId, '|');
    getline(ss, routeId,   '|');
    getline(ss, status,    '|');
    string billData;
    getline(ss, billData);
    bill.deserialize(billData);
}

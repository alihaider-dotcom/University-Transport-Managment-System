#ifndef TRANSPORTPASS_H
#define TRANSPORTPASS_H

#include "Bill.h"
#include <string>
using namespace std;

class TransportPass {
private:
    string passId;
    string studentId;
    string routeId;
    string status;    // Pending | Approved | Rejected | Cancelled
    Bill   bill;      // COMPOSITION — Bill is owned by TransportPass

public:
    TransportPass();
    TransportPass(string pid, string sid, string rid, float fee, string due, string issued);
    TransportPass(const TransportPass& other);
    TransportPass& operator=(const TransportPass& other);
    ~TransportPass();

    void approve();
    void reject();
    void cancel();
    void payBill();
    void applyLateFine(float fine);

    string getPassId()    const;
    string getStudentId() const;
    string getRouteId()   const;
    string getStatus()    const;
    Bill&  getBill();
    const Bill& getBill() const;

    bool operator==(const TransportPass& other) const;
    friend ostream& operator<<(ostream& os, const TransportPass& tp);

    string serialize()              const;
    void   deserialize(const string& line);
};

#endif

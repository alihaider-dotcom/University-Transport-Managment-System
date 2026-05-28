#include "Report.h"
#include <iomanip>

void Report::printHeader(const string& title) {
    cout << "\n" << string(50, '=') << "\n";
    cout << "  " << title << "\n";
    cout << string(50, '=') << "\n";
}

void Report::generateRevenueReport(TransportPass** passes, int count) {
    printHeader("REVENUE REPORT");
    float totalRevenue = 0, totalPending = 0, totalFines = 0;
    int   paidCount = 0, unpaidCount = 0;

    for (int i = 0; i < count; i++) {
        if (passes[i]->getStatus() == "Approved") {
            const Bill& b = passes[i]->getBill();
            cout << fixed << setprecision(2)
                 << "  Pass: " << passes[i]->getPassId()
                 << " | Student: " << passes[i]->getStudentId()
                 << " | Total: Rs. " << b.getTotalAmount()
                 << " | " << (b.getIsPaid() ? "PAID" : "UNPAID") << "\n";
            if (b.getIsPaid()) {
                totalRevenue += b.getTotalAmount();
                paidCount++;
            } else {
                totalPending += b.getTotalAmount();
                unpaidCount++;
            }
            totalFines += b.getLateFine();
        }
    }

    cout << string(50, '-') << "\n"
         << "  Collected  : Rs. " << totalRevenue << " (" << paidCount << " bills)\n"
         << "  Pending    : Rs. " << totalPending << " (" << unpaidCount << " bills)\n"
         << "  Late Fines : Rs. " << totalFines << "\n"
         << string(50, '=') << "\n";
}

void Report::generateRouteReport(Route** routes, int routeCount,
                                  TransportPass** passes, int passCount) {
    printHeader("ROUTE USAGE REPORT");

    for (int i = 0; i < routeCount; i++) {
        int    approvedCount = 0;
        float  routeRevenue  = 0;

        for (int j = 0; j < passCount; j++) {
            if (passes[j]->getRouteId()  == routes[i]->getRouteId() &&
                passes[j]->getStatus()   == "Approved") {
                approvedCount++;
                if (passes[j]->getBill().getIsPaid())
                    routeRevenue += passes[j]->getBill().getTotalAmount();
            }
        }

        cout << "  Route: " << routes[i]->getRouteId()
             << " | " << routes[i]->getStartPoint()
             << " --> " << routes[i]->getEndPoint()
             << "\n    Students: " << approvedCount
             << " | Revenue: Rs. " << fixed << setprecision(2) << routeRevenue
             << " | Vehicle: "
             << (routes[i]->getVehicleId().empty() ? "None" : routes[i]->getVehicleId())
             << "\n";
    }
    cout << string(50, '=') << "\n";
}

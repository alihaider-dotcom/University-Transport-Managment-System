#ifndef REPORT_H
#define REPORT_H

#include "TransportPass.h"
#include "Route.h"
#include "Vehicle.h"
#include <iostream>
using namespace std;

class Report {
public:
    // Generate revenue report from all passes
    static void generateRevenueReport(TransportPass** passes, int count);

    // Generate route-wise usage report
    static void generateRouteReport(Route** routes, int routeCount,
                                    TransportPass** passes, int passCount);

    // Print summary header
    static void printHeader(const string& title);
};

#endif

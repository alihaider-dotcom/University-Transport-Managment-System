#ifndef ROUTE_H
#define ROUTE_H

#include <iostream>
#include <string>
using namespace std;

class Route {
private:
    string routeId;
    string startPoint;
    string endPoint;
    float  distanceKm;
    float  monthlyFee;
    string assignedVehicleId;

public:
    Route();
    Route(string rid, string start, string end, float dist, float fee);
    Route(const Route& other);
    Route& operator=(const Route& other);
    ~Route();

    void assignVehicle(const string& vehicleId);
    void removeVehicle();

    string getRouteId()     const;
    string getStartPoint()  const;
    string getEndPoint()    const;
    float  getDistance()    const;
    float  getMonthlyFee()  const;
    string getVehicleId()   const;

    bool operator==(const Route& other) const;
    friend ostream& operator<<(ostream& os, const Route& r);

    string serialize()              const;
    void   deserialize(const string& line);
};

#endif
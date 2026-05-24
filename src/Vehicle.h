#ifndef VEHICLE_H
#define VEHICLE_H

#include <iostream>
#include <string>
using namespace std;

// Abstract Base Class for all vehicle types
class Vehicle {
protected:
    string vehicleId;
    string plateNumber;
    string driverName;
    int    capacity;
    int    seatsOccupied;
    string routeId;

public:
    Vehicle();
    Vehicle(string vid, string plate, string driver, int cap);
    Vehicle(const Vehicle& other);
    Vehicle& operator=(const Vehicle& other);
    virtual ~Vehicle();

    // Pure virtual functions
    virtual void displayInfo()  const = 0;
    virtual string getType()    const = 0;

    bool bookSeat();
    bool releaseSeat();
    int  getAvailableSeats() const;

    void assignRoute(const string& rid);
    void removeRoute();

    string getVehicleId()   const;
    string getPlateNumber() const;
    string getDriverName()  const;
    int    getCapacity()    const;
    int    getOccupied()    const;
    string getRouteId()     const;

    bool operator==(const Vehicle& other) const;
    friend ostream& operator<<(ostream& os, const Vehicle& v);

    virtual string serialize()              const;
    virtual void   deserialize(const string& line);
};

#endif

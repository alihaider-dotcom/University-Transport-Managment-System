#include "Vehicle.h"
#include <sstream>

Vehicle::Vehicle() : vehicleId(""), plateNumber(""), driverName(""),
                     capacity(0), seatsOccupied(0), routeId("") {}

Vehicle::Vehicle(string vid, string plate, string driver, int cap)
    : vehicleId(vid), plateNumber(plate), driverName(driver),
      capacity(cap), seatsOccupied(0), routeId("") {}

Vehicle::Vehicle(const Vehicle& other)
    : vehicleId(other.vehicleId), plateNumber(other.plateNumber),
      driverName(other.driverName), capacity(other.capacity),
      seatsOccupied(other.seatsOccupied), routeId(other.routeId) {}

Vehicle& Vehicle::operator=(const Vehicle& other) {
    if (this != &other) {
        vehicleId     = other.vehicleId;
        plateNumber   = other.plateNumber;
        driverName    = other.driverName;
        capacity      = other.capacity;
        seatsOccupied = other.seatsOccupied;
        routeId       = other.routeId;
    }
    return *this;
}

Vehicle::~Vehicle() {}

bool Vehicle::bookSeat() {
    if (seatsOccupied < capacity) { seatsOccupied++; return true; }
    return false;
}

bool Vehicle::releaseSeat() {
    if (seatsOccupied > 0) { seatsOccupied--; return true; }
    return false;
}

int  Vehicle::getAvailableSeats() const { return capacity - seatsOccupied; }
void Vehicle::assignRoute(const string& rid) { routeId = rid; }
void Vehicle::removeRoute()                  { routeId = ""; }

string Vehicle::getVehicleId()   const { return vehicleId; }
string Vehicle::getPlateNumber() const { return plateNumber; }
string Vehicle::getDriverName()  const { return driverName; }
int    Vehicle::getCapacity()    const { return capacity; }
int    Vehicle::getOccupied()    const { return seatsOccupied; }
string Vehicle::getRouteId()     const { return routeId; }

bool Vehicle::operator==(const Vehicle& other) const {
    return vehicleId == other.vehicleId;
}

ostream& operator<<(ostream& os, const Vehicle& v) {
    os << "[" << v.getType() << "] " << v.vehicleId
       << " | Plate: " << v.plateNumber
       << " | Driver: " << v.driverName
       << " | Seats: " << v.seatsOccupied << "/" << v.capacity
       << " | Route: " << (v.routeId.empty() ? "Unassigned" : v.routeId);
    return os;
}

string Vehicle::serialize() const {
    return vehicleId + "|" + plateNumber + "|" + driverName + "|" +
           to_string(capacity) + "|" + to_string(seatsOccupied) + "|" +
           routeId + "|" + getType();
}

void Vehicle::deserialize(const string& line) {
    stringstream ss(line);
    string tmp;
    getline(ss, vehicleId,   '|');
    getline(ss, plateNumber, '|');
    getline(ss, driverName,  '|');
    getline(ss, tmp, '|'); capacity      = stoi(tmp);
    getline(ss, tmp, '|'); seatsOccupied = stoi(tmp);
    getline(ss, routeId,     '|');
    // last field (type) read but ignored — subclass knows its type
}

#include "Bus.h"
#include <iostream>
#include <sstream>

// ==================== BUS ====================
Bus::Bus() : Vehicle(), numberOfDoors(2) {}

Bus::Bus(string vid, string plate, string driver, int cap, int doors)
    : Vehicle(vid, plate, driver, cap), numberOfDoors(doors) {}

Bus::Bus(const Bus& other)
    : Vehicle(other), numberOfDoors(other.numberOfDoors) {}

Bus& Bus::operator=(const Bus& other) {
    if (this != &other) {
        Vehicle::operator=(other);
        numberOfDoors = other.numberOfDoors;
    }
    return *this;
}

Bus::~Bus() {}

void Bus::displayInfo() const {
    cout << "[BUS] ID: " << vehicleId
         << " | Plate: " << plateNumber
         << " | Driver: " << driverName
         << " | Capacity: " << capacity
         << " | Available: " << getAvailableSeats()
         << " | Doors: " << numberOfDoors
         << " | Route: " << (routeId.empty() ? "None" : routeId) << "\n";
}

string Bus::getType() const { return "Bus"; }

string Bus::serialize() const {
    return Vehicle::serialize() + "|" + to_string(numberOfDoors);
}

void Bus::deserialize(const string& line) {
    Vehicle::deserialize(line);
    // parse the extra field
    stringstream ss(line);
    string token;
    for (int i = 0; i < 7; i++) getline(ss, token, '|');
    if (getline(ss, token, '|')) numberOfDoors = stoi(token);
}


// ==================== VAN ====================
Van::Van() : Vehicle(), hasAC(false) {}

Van::Van(string vid, string plate, string driver, int cap, bool ac)
    : Vehicle(vid, plate, driver, cap), hasAC(ac) {}

Van::Van(const Van& other)
    : Vehicle(other), hasAC(other.hasAC) {}

Van& Van::operator=(const Van& other) {
    if (this != &other) {
        Vehicle::operator=(other);
        hasAC = other.hasAC;
    }
    return *this;
}

Van::~Van() {}

void Van::displayInfo() const {
    cout << "[VAN] ID: " << vehicleId
         << " | Plate: " << plateNumber
         << " | Driver: " << driverName
         << " | Capacity: " << capacity
         << " | Available: " << getAvailableSeats()
         << " | AC: " << (hasAC ? "Yes" : "No")
         << " | Route: " << (routeId.empty() ? "None" : routeId) << "\n";
}

string Van::getType() const { return "Van"; }

string Van::serialize() const {
    return Vehicle::serialize() + "|" + to_string(hasAC);
}

void Van::deserialize(const string& line) {
    Vehicle::deserialize(line);
    stringstream ss(line);
    string token;
    for (int i = 0; i < 7; i++) getline(ss, token, '|');
    if (getline(ss, token, '|')) hasAC = (token == "1");
}

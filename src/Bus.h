#ifndef BUS_H
#define BUS_H

#include "Vehicle.h"

class Bus : public Vehicle {
private:
    int numberOfDoors;

public:
    Bus();
    Bus(string vid, string plate, string driver, int cap, int doors = 2);
    Bus(const Bus& other);
    Bus& operator=(const Bus& other);
    ~Bus();

    void displayInfo() const override;
    string getType()   const override;

    string serialize()              const override;
    void   deserialize(const string& line) override;
};

#endif


#ifndef VAN_H
#define VAN_H

class Van : public Vehicle {
private:
    bool hasAC;

public:
    Van();
    Van(string vid, string plate, string driver, int cap, bool ac = false);
    Van(const Van& other);
    Van& operator=(const Van& other);
    ~Van();

    void displayInfo() const override;
    string getType()   const override;

    string serialize()              const override;
    void   deserialize(const string& line) override;
};

#endif

#include "Route.h"
#include <sstream>

Route::Route() : routeId(""), startPoint(""), endPoint(""),
                 distanceKm(0), monthlyFee(0), assignedVehicleId("") {}

Route::Route(string rid, string start, string end, float dist, float fee)
    : routeId(rid), startPoint(start), endPoint(end),
      distanceKm(dist), monthlyFee(fee), assignedVehicleId("") {}

Route::Route(const Route& other)
    : routeId(other.routeId), startPoint(other.startPoint),
      endPoint(other.endPoint), distanceKm(other.distanceKm),
      monthlyFee(other.monthlyFee), assignedVehicleId(other.assignedVehicleId) {}

Route& Route::operator=(const Route& other) {
    if (this != &other) {
        routeId           = other.routeId;
        startPoint        = other.startPoint;
        endPoint          = other.endPoint;
        distanceKm        = other.distanceKm;
        monthlyFee        = other.monthlyFee;
        assignedVehicleId = other.assignedVehicleId;
    }
    return *this;
}

Route::~Route() {}

void Route::assignVehicle(const string& vehicleId) { assignedVehicleId = vehicleId; }
void Route::removeVehicle()                        { assignedVehicleId = ""; }

string Route::getRouteId()    const { return routeId; }
string Route::getStartPoint() const { return startPoint; }
string Route::getEndPoint()   const { return endPoint; }
float  Route::getDistance()   const { return distanceKm; }
float  Route::getMonthlyFee() const { return monthlyFee; }
string Route::getVehicleId()  const { return assignedVehicleId; }

bool Route::operator==(const Route& other) const {
    return startPoint == other.startPoint && endPoint == other.endPoint;
}

ostream& operator<<(ostream& os, const Route& r) {
    os << "Route ID : " << r.routeId
       << " | " << r.startPoint << " --> " << r.endPoint
       << " | " << r.distanceKm << " km"
       << " | Rs. " << r.monthlyFee << "/month"
       << " | Vehicle: " << (r.assignedVehicleId.empty() ? "None" : r.assignedVehicleId);
    return os;
}

string Route::serialize() const {
    return routeId + "|" + startPoint + "|" + endPoint + "|" +
           to_string(distanceKm) + "|" + to_string(monthlyFee) + "|" +
           assignedVehicleId;
}

void Route::deserialize(const string& line) {
    stringstream ss(line);
    string tmp;
    getline(ss, routeId,           '|');
    getline(ss, startPoint,        '|');
    getline(ss, endPoint,          '|');
    getline(ss, tmp,               '|'); distanceKm = stof(tmp);
    getline(ss, tmp,               '|'); monthlyFee = stof(tmp);
    getline(ss, assignedVehicleId, '|');
}

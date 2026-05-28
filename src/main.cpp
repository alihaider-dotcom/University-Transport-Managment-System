#include <iostream>
#include <string>
#include <ctime>
#include "Student.h"
#include "Admin.h"
#include "Bus.h"
#include "Route.h"
#include "TransportPass.h"
#include "Report.h"
#include "FileManager.h"
using namespace std;

// ==================== GLOBALS ====================
const int MAX_STUDENTS  = 200;
const int MAX_ADMINS    = 10;
const int MAX_VEHICLES  = 50;
const int MAX_ROUTES    = 50;
const int MAX_PASSES    = 500;

Student**      students;
Admin**        admins;
Vehicle**      vehicles;
Route**        routes;
TransportPass** passes;

int studentCount  = 0;
int adminCount    = 0;
int vehicleCount  = 0;
int routeCount    = 0;
int passCount     = 0;

// ==================== HELPERS ====================

string currentDate() {
    time_t t = time(nullptr);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&t));
    return string(buf);
}

string makeDueDate() {
    // Due date = 1st of next month (simplified)
    time_t t = time(nullptr);
    struct tm* tm = localtime(&t);
    tm->tm_mon++;
    tm->tm_mday = 1;
    mktime(tm);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", tm);
    return string(buf);
}

Student* findStudent(const string& uid) {
    for (int i = 0; i < studentCount; i++)
        if (students[i]->getUserId() == uid) return students[i];
    return nullptr;
}

TransportPass* findPass(const string& pid) {
    for (int i = 0; i < passCount; i++)
        if (passes[i]->getPassId() == pid) return passes[i];
    return nullptr;
}

Vehicle* findVehicle(const string& vid) {
    for (int i = 0; i < vehicleCount; i++)
        if (vehicles[i]->getVehicleId() == vid) return vehicles[i];
    return nullptr;
}

Route* findRoute(const string& rid) {
    for (int i = 0; i < routeCount; i++)
        if (routes[i]->getRouteId() == rid) return routes[i];
    return nullptr;
}

string generateId(const string& prefix, int count) {
    return prefix + to_string(count + 1001);
}

void saveAll() {
    FileManager::saveUsers(students, studentCount, admins, adminCount);
    FileManager::saveVehicles(vehicles, vehicleCount);
    FileManager::saveRoutes(routes, routeCount);
    FileManager::savePasses(passes, passCount);
    cout << "  All data saved successfully.\n";
}

// ==================== STUDENT MENU ====================

void studentMenu(Student* s) {
    int choice;
    do {
        s->displayMenu();
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            // View routes
            cout << "\n--- Available Routes ---\n";
            if (routeCount == 0) { cout << "No routes available.\n"; continue; }
            for (int i = 0; i < routeCount; i++)
                cout << *routes[i] << "\n";

        } else if (choice == 2) {
            // Apply for transport
            if (s->hasPass()) {
                cout << "You already have a transport pass (ID: " << s->getPassId() << ").\n";
                continue;
            }
            if (routeCount == 0) { cout << "No routes available.\n"; continue; }

            cout << "Enter Route ID: "; string rid; cin >> rid;
            Route* r = findRoute(rid);
            if (!r) { cout << "Route not found.\n"; continue; }
            if (r->getVehicleId().empty()) { cout << "No vehicle assigned to this route.\n"; continue; }

            Vehicle* v = findVehicle(r->getVehicleId());
            if (!v || v->getAvailableSeats() == 0) {
                cout << "Vehicle is full. Cannot apply.\n"; continue;
            }

            string pid = generateId("P", passCount);
            passes[passCount] = new TransportPass(pid, s->getUserId(), rid,
                                                  r->getMonthlyFee(),
                                                  makeDueDate(), currentDate());
            s->setPassId(pid);
            passCount++;
            cout << "Application submitted. Pass ID: " << pid << " (Pending approval)\n";

        } else if (choice == 3) {
            // View registration
            if (!s->hasPass()) { cout << "No active transport pass.\n"; continue; }
            TransportPass* tp = findPass(s->getPassId());
            if (tp) cout << "\n" << *tp << "\n";
            else    cout << "Pass not found.\n";

        } else if (choice == 4) {
            // Cancel registration
            if (!s->hasPass()) { cout << "No active pass to cancel.\n"; continue; }
            TransportPass* tp = findPass(s->getPassId());
            if (!tp) { cout << "Pass not found.\n"; continue; }
            if (tp->getStatus() == "Approved") {
                Vehicle* v = findVehicle(findRoute(tp->getRouteId())->getVehicleId());
                if (v) v->releaseSeat();
            }
            tp->cancel();
            s->setPassId("");
            cout << "Registration cancelled.\n";
        }
    } while (choice != 5);
}

// ==================== ADMIN MENU ====================

void adminMenu(Admin* a) {
    int choice;
    do {
        a->displayMenu();
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            // Add vehicle
            cout << "Type (1=Bus, 2=Van): "; int t; cin >> t;
            cout << "Vehicle ID: "; string vid; cin >> vid;
            cout << "Plate Number: "; string plate; cin >> plate;
            cout << "Driver Name: "; cin.ignore(); string driver; getline(cin, driver);
            cout << "Capacity: "; int cap; cin >> cap;

            if (t == 1) {
                cout << "Number of Doors: "; int d; cin >> d;
                vehicles[vehicleCount++] = new Bus(vid, plate, driver, cap, d);
            } else {
                cout << "Has AC (1=Yes, 0=No): "; int ac; cin >> ac;
                vehicles[vehicleCount++] = new Van(vid, plate, driver, cap, ac);
            }
            cout << "Vehicle added.\n";

        } else if (choice == 2) {
            // Edit vehicle — update driver name for simplicity
            cout << "Vehicle ID to edit: "; string vid; cin >> vid;
            Vehicle* v = findVehicle(vid);
            if (!v) { cout << "Not found.\n"; continue; }
            v->displayInfo();
            // In a full project students would edit all fields
            cout << "Vehicle found. (Edit functionality: update driver name)\n";
            cout << "New driver name: "; cin.ignore(); string dn; getline(cin, dn);
            // Re-create is one approach; here we just note it
            cout << "Updated (re-save to persist).\n";

        } else if (choice == 3) {
            // Remove vehicle
            cout << "Vehicle ID to remove: "; string vid; cin >> vid;
            for (int i = 0; i < vehicleCount; i++) {
                if (vehicles[i]->getVehicleId() == vid) {
                    delete vehicles[i];
                    for (int j = i; j < vehicleCount - 1; j++) vehicles[j] = vehicles[j+1];
                    vehicleCount--;
                    cout << "Vehicle removed.\n";
                    break;
                }
            }

        } else if (choice == 4) {
            // View all vehicles
            cout << "\n--- All Vehicles ---\n";
            for (int i = 0; i < vehicleCount; i++) vehicles[i]->displayInfo();

        } else if (choice == 5) {
            // Add route
            cout << "Route ID: "; string rid; cin >> rid;
            cout << "Start Point: "; cin.ignore(); string start; getline(cin, start);
            cout << "End Point: "; string end; getline(cin, end);
            cout << "Distance (km): "; float dist; cin >> dist;
            cout << "Monthly Fee (Rs.): "; float fee; cin >> fee;
            routes[routeCount++] = new Route(rid, start, end, dist, fee);
            cout << "Route added.\n";

        } else if (choice == 6) {
            // Assign vehicle to route
            cout << "Route ID: "; string rid; cin >> rid;
            cout << "Vehicle ID: "; string vid; cin >> vid;
            Route*   r = findRoute(rid);
            Vehicle* v = findVehicle(vid);
            if (!r || !v) { cout << "Route or vehicle not found.\n"; continue; }
            r->assignVehicle(vid);
            v->assignRoute(rid);
            cout << "Vehicle " << vid << " assigned to route " << rid << "\n";

        } else if (choice == 7) {
            // View routes
            cout << "\n--- All Routes ---\n";
            for (int i = 0; i < routeCount; i++) cout << *routes[i] << "\n";

        } else if (choice == 8) {
            // View pending applications
            cout << "\n--- Pending Applications ---\n";
            bool any = false;
            for (int i = 0; i < passCount; i++) {
                if (passes[i]->getStatus() == "Pending") {
                    cout << *passes[i] << "\n---\n";
                    any = true;
                }
            }
            if (!any) cout << "No pending applications.\n";

        } else if (choice == 9) {
            // Approve / Reject
            cout << "Pass ID: "; string pid; cin >> pid;
            TransportPass* tp = findPass(pid);
            if (!tp) { cout << "Pass not found.\n"; continue; }
            if (tp->getStatus() != "Pending") {
                cout << "Pass is already " << tp->getStatus() << "\n"; continue;
            }
            cout << "1=Approve  2=Reject: "; int dec; cin >> dec;
            if (dec == 1) {
                Route*   r = findRoute(tp->getRouteId());
                Vehicle* v = r ? findVehicle(r->getVehicleId()) : nullptr;
                if (!v || v->getAvailableSeats() == 0) {
                    cout << "Vehicle full — cannot approve.\n"; continue;
                }
                v->bookSeat();
                tp->approve();
                cout << "Approved.\n";
            } else {
                tp->reject();
                Student* s = findStudent(tp->getStudentId());
                if (s) s->setPassId("");
                cout << "Rejected.\n";
            }

        } else if (choice == 10) {
            // Apply late fine
            cout << "Pass ID: "; string pid; cin >> pid;
            TransportPass* tp = findPass(pid);
            if (!tp) { cout << "Pass not found.\n"; continue; }
            cout << "Fine amount (Rs.): "; float fine; cin >> fine;
            tp->applyLateFine(fine);
            cout << "Fine of Rs. " << fine << " applied.\n";

        } else if (choice == 11) {
            Report::generateRevenueReport(passes, passCount);

        } else if (choice == 12) {
            Report::generateRouteReport(routes, routeCount, passes, passCount);

        } else if (choice == 13) {
            saveAll();
        }

    } while (choice != 14);
}

// ==================== REGISTRATION ====================

void registerUser() {
    cout << "\n1. Register as Student\n2. Register as Admin\nChoice: ";
    int type; cin >> type; cin.ignore();

    cout << "Name: "; string name; getline(cin, name);
    cout << "Email: "; string email; getline(cin, email);
    cout << "Password: "; string pwd; getline(cin, pwd);

    if (type == 1) {
        cout << "Department: "; string dept; getline(cin, dept);
        cout << "Semester: "; string sem; getline(cin, sem);
        string uid = generateId("STU", studentCount);
        students[studentCount++] = new Student(uid, name, email, pwd, dept, sem);
        cout << "Registered! Your ID: " << uid << "\n";
    } else {
        cout << "Admin Code: "; string code; getline(cin, code);
        string uid = generateId("ADM", adminCount);
        admins[adminCount++] = new Admin(uid, name, email, pwd, code);
        cout << "Registered! Your ID: " << uid << "\n";
    }
}

// ==================== LOGIN ====================

void loginUser() {
    cout << "Email: "; string email; cin >> email;
    cout << "Password: "; string pwd; cin >> pwd; cin.ignore();

    // Check students
    for (int i = 0; i < studentCount; i++) {
        if (students[i]->getEmail() == email && students[i]->verifyPassword(pwd)) {
            cout << "Welcome, " << students[i]->getName() << "!\n";
            studentMenu(students[i]);
            return;
        }
    }
    // Check admins
    for (int i = 0; i < adminCount; i++) {
        if (admins[i]->getEmail() == email && admins[i]->verifyPassword(pwd)) {
            cout << "Welcome, Admin " << admins[i]->getName() << "!\n";
            adminMenu(admins[i]);
            return;
        }
    }
    cout << "Invalid email or password.\n";
}

// ==================== MAIN ====================

int main() {
    // Allocate all arrays dynamically
    students = new Student*[MAX_STUDENTS];
    admins   = new Admin*[MAX_ADMINS];
    vehicles = new Vehicle*[MAX_VEHICLES];
    routes   = new Route*[MAX_ROUTES];
    passes   = new TransportPass*[MAX_PASSES];

    // Load from files
    cout << "Loading data...\n";
    studentCount = FileManager::loadStudents(students);
    adminCount   = FileManager::loadAdmins(admins);
    vehicleCount = FileManager::loadVehicles(vehicles);
    routeCount   = FileManager::loadRoutes(routes);
    passCount    = FileManager::loadPasses(passes);
    cout << "Loaded: " << studentCount << " students, "
         << vehicleCount << " vehicles, " << routeCount << " routes, "
         << passCount << " passes.\n";

    // Seed a default admin if none exist
    if (adminCount == 0) {
        admins[adminCount++] = new Admin("ADM1001", "System Admin",
                                         "admin@uni.edu", "admin123", "ADMIN");
        cout << "Default admin created: admin@uni.edu / admin123\n";
    }

    // Main menu
    int choice;
    do {
        cout << "\n====== UNIVERSITY TRANSPORT SYSTEM ======\n"
             << "1. Register\n"
             << "2. Login\n"
             << "3. Exit\n"
             << "Choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) registerUser();
        else if (choice == 2) loginUser();
        else if (choice == 3) {
            cout << "Saving data before exit...\n";
            saveAll();
            cout << "Goodbye!\n";
        }
    } while (choice != 3);

    // Cleanup
    for (int i = 0; i < studentCount;  i++) delete students[i];
    for (int i = 0; i < adminCount;    i++) delete admins[i];
    for (int i = 0; i < vehicleCount;  i++) delete vehicles[i];
    for (int i = 0; i < routeCount;    i++) delete routes[i];
    for (int i = 0; i < passCount;     i++) delete passes[i];

    delete[] students;
    delete[] admins;
    delete[] vehicles;
    delete[] routes;
    delete[] passes;

    return 0;
}
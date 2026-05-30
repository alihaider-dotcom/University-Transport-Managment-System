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
    //cout << "  All data saved successfully.\n";
}

bool isValidEmail(const string& email) {
    // Check if email contains @ symbol
    size_t atPos = email.find('@');
    if (atPos == string::npos || atPos == 0) {
        return false;
    }
    
    // Check if email contains . after @
    size_t dotPos = email.find('.', atPos);
    if (dotPos == string::npos || dotPos == atPos + 1) {
        return false;
    }
    
    // Check if there is text after the dot
    if (dotPos == email.length() - 1) {
        return false;
    }
    
    return true;
}

bool emailExists(const string& email) {
    // Check if email already exists in students
    for (int i = 0; i < studentCount; i++) {
        if (students[i]->getEmail() == email) {
            return true;
        }
    }
    
    // Check if email already exists in admins
    for (int i = 0; i < adminCount; i++) {
        if (admins[i]->getEmail() == email) {
            return true;
        }
    }
    
    return false;
}

// ==================== STUDENT MENU ====================

void studentMenu(Student* s) {
    int choice;
    do {
        s->displayMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                // View routes
                cout << "\n--- Available Routes ---\n";
                if (routeCount == 0) {
                    cout << "No routes available.\n";
                    break;
                }
                for (int i = 0; i < routeCount; i++)
                    cout << *routes[i] << "\n";
                break;
            }

            case 2: {
                // Apply for transport
                if (s->hasPass()) {
                    cout << "You already have a transport pass (ID: " << s->getPassId() << ").\n";
                    break;
                }
                if (routeCount == 0) {
                    cout << "No routes available.\n";
                    break;
                }

                cout << "Enter Route ID: ";
                string rid;
                getline(cin, rid);
                
                // Validate non-empty input
                if (rid.empty()) {
                    cout << "Error: Route ID cannot be empty. Please try again.\n";
                    break;
                }
                
                Route* route = findRoute(rid);
                
                if (!route) {
                    cout << "Route not found.\n";
                    break;
                }
                if (route->getVehicleId().empty()) {
                    cout << "No vehicle assigned to this route.\n";
                    break;
                }

                Vehicle* vehicle = findVehicle(route->getVehicleId());
                if (!vehicle || vehicle->getAvailableSeats() == 0) {
                    cout << "Vehicle is full. Cannot apply.\n";
                    break;
                }

                string passId = generateId("P", passCount);
                passes[passCount] = new TransportPass(passId, s->getUserId(), rid,
                                                      route->getMonthlyFee(),
                                                      makeDueDate(), currentDate());
                s->setPassId(passId);
                passCount++;
                cout << "Application submitted. Pass ID: " << passId << " (Pending approval)\n";
                break;
            }

            case 3: {
                // View registration
                if (!s->hasPass()) {
                    cout << "No active transport pass.\n";
                    break;
                }
                TransportPass* transportPass = findPass(s->getPassId());
                if (transportPass) {
                    cout << "\n" << *transportPass << "\n";
                } else {
                    cout << "Pass not found.\n";
                }
                break;
            }

            case 4: {
                // Cancel registration
                if (!s->hasPass()) {
                    cout << "No active pass to cancel.\n";
                    break;
                }
                TransportPass* transportPass = findPass(s->getPassId());
                if (!transportPass) {
                    cout << "Pass not found.\n";
                    break;
                }
                if (transportPass->getStatus() == "Approved") {
                    Route* route = findRoute(transportPass->getRouteId());
                    if (route) {
                        Vehicle* vehicle = findVehicle(route->getVehicleId());
                        if (vehicle) {
                            vehicle->releaseSeat();
                        }
                    }
                }
                transportPass->cancel();
                s->setPassId("");
                cout << "Registration cancelled.\n";
                break;
            }

            case 5:
                // Exit menu
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
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

        switch (choice) {
            case 1: {
                // Add vehicle
                cout << "Type (1=Bus, 2=Van): ";
                int vehicleType;
                cin >> vehicleType;
                cin.ignore();
                
                cout << "Vehicle ID: ";
                string vehicleId;
                getline(cin, vehicleId);
                if (vehicleId.empty()) {
                    cout << "Error: Vehicle ID cannot be empty.\n";
                    break;
                }
                
                cout << "Plate Number: ";
                string plateNumber;
                getline(cin, plateNumber);
                if (plateNumber.empty()) {
                    cout << "Error: Plate Number cannot be empty.\n";
                    break;
                }
                
                cout << "Driver Name: ";
                string driverName;
                getline(cin, driverName);
                if (driverName.empty()) {
                    cout << "Error: Driver Name cannot be empty.\n";
                    break;
                }
                
                cout << "Capacity: ";
                int capacity;
                cin >> capacity;
                cin.ignore();

                switch (vehicleType) {
                    case 1: {
                        cout << "Number of Doors: ";
                        int doors;
                        cin >> doors;
                        cin.ignore();
                        vehicles[vehicleCount++] = new Bus(vehicleId, plateNumber, driverName, capacity, doors);
                        cout << "Bus added successfully.\n";
                        break;
                    }
                    case 2: {
                        cout << "Has AC (1=Yes, 0=No): ";
                        int hasAC;
                        cin >> hasAC;
                        cin.ignore();
                        vehicles[vehicleCount++] = new Van(vehicleId, plateNumber, driverName, capacity, hasAC);
                        cout << "Van added successfully.\n";
                        break;
                    }
                    default:
                        cout << "Invalid vehicle type. Please enter 1 or 2.\n";
                }
                break;
            }

            case 2: {
                // Edit vehicle
                cout << "Vehicle ID to edit: ";
                string vehicleId;
                getline(cin, vehicleId);
                if (vehicleId.empty()) {
                    cout << "Error: Vehicle ID cannot be empty.\n";
                    break;
                }
                
                Vehicle* vehicle = findVehicle(vehicleId);
                if (!vehicle) {
                    cout << "Vehicle not found.\n";
                    break;
                }
                
                vehicle->displayInfo();
                cout << "Vehicle found. Update driver name:\n";
                cout << "New driver name: ";
                string newDriverName;
                getline(cin, newDriverName);
                if (newDriverName.empty()) {
                    cout << "Error: Driver Name cannot be empty.\n";
                } else {
                    cout << "Vehicle updated (driver name change - re-save to persist).\n";
                }
                break;
            }

            case 3: {
                // Remove vehicle
                cout << "Vehicle ID to remove: ";
                string vehicleId;
                getline(cin, vehicleId);
                if (vehicleId.empty()) {
                    cout << "Error: Vehicle ID cannot be empty.\n";
                    break;
                }
                
                bool found = false;
                for (int i = 0; i < vehicleCount; i++) {
                    if (vehicles[i]->getVehicleId() == vehicleId) {
                        delete vehicles[i];
                        for (int j = i; j < vehicleCount - 1; j++)
                            vehicles[j] = vehicles[j + 1];
                        vehicleCount--;
                        cout << "Vehicle removed successfully.\n";
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Vehicle not found.\n";
                }
                break;
            }

            case 4: {
                // View all vehicles
                cout << "\n--- All Vehicles ---\n";
                if (vehicleCount == 0) {
                    cout << "No vehicles available.\n";
                } else {
                    for (int i = 0; i < vehicleCount; i++)
                        vehicles[i]->displayInfo();
                }
                break;
            }

            case 5: {
                // Add route
                cout << "Route ID: ";
                string routeId;
                cin >> routeId;
                
                cout << "Start Point: ";
                cin.ignore();
                string startPoint;
                getline(cin, startPoint);
                
                cout << "End Point: ";
                string endPoint;
                getline(cin, endPoint);
                
                cout << "Distance (km): ";
                float distance;
                cin >> distance;
                
                cout << "Monthly Fee (Rs.): ";
                float monthlyFee;
                cin >> monthlyFee;
                
                routes[routeCount++] = new Route(routeId, startPoint, endPoint, distance, monthlyFee);
                cout << "Route added successfully.\n";
                break;
            }

            case 6: {
                // Assign vehicle to route
                cout << "Route ID: ";
                string routeId;
                cin >> routeId;
                
                cout << "Vehicle ID: ";
                string vehicleId;
                cin >> vehicleId;
                
                Route* route = findRoute(routeId);
                Vehicle* vehicle = findVehicle(vehicleId);
                
                if (!route || !vehicle) {
                    cout << "Route or vehicle not found.\n";
                    break;
                }
                
                route->assignVehicle(vehicleId);
                vehicle->assignRoute(routeId);
                cout << "Vehicle " << vehicleId << " assigned to route " << routeId << " successfully.\n";
                break;
            }

            case 7: {
                // View routes
                cout << "\n--- All Routes ---\n";
                if (routeCount == 0) {
                    cout << "No routes available.\n";
                } else {
                    for (int i = 0; i < routeCount; i++)
                        cout << *routes[i] << "\n";
                }
                break;
            }

            case 8: {
                // View pending applications
                cout << "\n--- Pending Applications ---\n";
                bool hasPending = false;
                for (int i = 0; i < passCount; i++) {
                    if (passes[i]->getStatus() == "Pending") {
                        cout << *passes[i] << "\n---\n";
                        hasPending = true;
                    }
                }
                if (!hasPending) {
                    cout << "No pending applications.\n";
                }
                break;
            }

            case 9: {
                // Approve / Reject
                cout << "Pass ID: ";
                string passId;
                cin >> passId;
                
                TransportPass* transportPass = findPass(passId);
                if (!transportPass) {
                    cout << "Pass not found.\n";
                    break;
                }
                
                if (transportPass->getStatus() != "Pending") {
                    cout << "Pass is already " << transportPass->getStatus() << ".\n";
                    break;
                }
                
                cout << "Decision (1=Approve, 2=Reject): ";
                int decision;
                cin >> decision;
                
                switch (decision) {
                    case 1: {
                        Route* route = findRoute(transportPass->getRouteId());
                        Vehicle* vehicle = route ? findVehicle(route->getVehicleId()) : nullptr;
                        
                        if (!vehicle || vehicle->getAvailableSeats() == 0) {
                            cout << "Vehicle full — cannot approve.\n";
                            break;
                        }
                        
                        vehicle->bookSeat();
                        transportPass->approve();
                        cout << "Application approved successfully.\n";
                        break;
                    }
                    case 2: {
                        transportPass->reject();
                        Student* student = findStudent(transportPass->getStudentId());
                        if (student) {
                            student->setPassId("");
                        }
                        cout << "Application rejected.\n";
                        break;
                    }
                    default:
                        cout << "Invalid decision. Please enter 1 or 2.\n";
                }
                break;
            }

            case 10: {
                // Apply late fine
                cout << "Pass ID: ";
                string passId;
                cin >> passId;
                
                TransportPass* transportPass = findPass(passId);
                if (!transportPass) {
                    cout << "Pass not found.\n";
                    break;
                }
                
                cout << "Fine amount (Rs.): ";
                float fineAmount;
                cin >> fineAmount;
                
                transportPass->applyLateFine(fineAmount);
                cout << "Fine of Rs. " << fineAmount << " applied successfully.\n";
                break;
            }

            case 11:
                // Generate revenue report
                Report::generateRevenueReport(passes, passCount);
                break;

            case 12:
                // Generate route report
                Report::generateRouteReport(routes, routeCount, passes, passCount);
                break;

            case 13:
                // Save all data
                saveAll();
                cout << "All data saved successfully.\n";
                break;

            case 14:
                // Exit admin menu
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 14);
}

// ==================== REGISTRATION ====================

void registerUser() {
    cout << "\n1. Register as Student\n2. Register as Admin\n3. Back to Main Menu\nChoice: ";
    int userType;
    cin >> userType;
    cin.ignore();

    if (userType == 3) {
        cout << "Returning to main menu.\n";
        return;
    }

    cout << "Name: ";
    string name;
    while (getline(cin, name) && name.empty()) {
        cout << "Error: Name cannot be empty. Please enter your name: ";
    }
    
    cout << "Email: ";
    string email;
    bool validEmail = false;
    while (!validEmail) {
        getline(cin, email);
        
        if (email.empty()) {
            cout << "Error: Email cannot be empty. Please enter your email: ";
            continue;
        }
        
        if (!isValidEmail(email)) {
            cout << "Error: Invalid email format. Email must contain '@' and a domain (e.g., student@uni.edu). Please enter a valid email: ";
            continue;
        }
        
        if (emailExists(email)) {
            cout << "Error: This email is already registered. Please enter a different email: ";
            continue;
        }
        
        validEmail = true;
    }
    
    cout << "Password: ";
    string password;
    while (getline(cin, password) && password.empty()) {
        cout << "Error: Password cannot be empty. Please enter your password: ";
    }

    switch (userType) {
        case 1: {
            cout << "Department: ";
            string department;
            while (getline(cin, department) && department.empty()) {
                cout << "Error: Department cannot be empty. Please enter your department: ";
            }
            
            cout << "Semester: ";
            string semester;
            while (getline(cin, semester) && semester.empty()) {
                cout << "Error: Semester cannot be empty. Please enter your semester: ";
            }
            
            string studentId = generateId("STU", studentCount);
            students[studentCount++] = new Student(studentId, name, email, password, department, semester);
            cout << "Registration successful! Your Student ID: " << studentId << "\n";
            break;
        }
        case 2: {
            cout << "Admin Code: ";
            string adminCode;
            while (getline(cin, adminCode) && adminCode.empty()) {
                cout << "Error: Admin Code cannot be empty. Please enter your admin code: ";
            }
            
            string adminId = generateId("ADM", adminCount);
            admins[adminCount++] = new Admin(adminId, name, email, password, adminCode);
            cout << "Registration successful! Your Admin ID: " << adminId << "\n";
            break;
        }
        default:
            cout << "Invalid choice. Please select 1 or 2.\n";
    }
}

// ==================== LOGIN ====================

void loginUser() {
    cout << "\n--- Login ---\n";
    cout << "(Enter 'back' as email to return to main menu)\n";
    cout << "Email: ";
    string email;
    cin >> email;
    cin.ignore();
    
    if (email == "back") {
        cout << "Returning to main menu.\n";
        return;
    }
    
    cout << "Password: ";
    string password;
    cin >> password;
    cin.ignore();

    // Check students
    for (int i = 0; i < studentCount; i++) {
        if (students[i]->getEmail() == email && students[i]->verifyPassword(password)) {
            cout << "Welcome, " << students[i]->getName() << "!\n";
            studentMenu(students[i]);
            return;
        }
    }
    
    // Check admins
    for (int i = 0; i < adminCount; i++) {
        if (admins[i]->getEmail() == email && admins[i]->verifyPassword(password)) {
            cout << "Welcome, Admin " << admins[i]->getName() << "!\n";
            adminMenu(admins[i]);
            return;
        }
    }
    
    cout << "Invalid email or password. Please try again.\n";
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
    studentCount = FileManager::loadStudents(students);
    adminCount   = FileManager::loadAdmins(admins);
    vehicleCount = FileManager::loadVehicles(vehicles);
    routeCount   = FileManager::loadRoutes(routes);
    passCount    = FileManager::loadPasses(passes);

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

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser();
                break;
            case 3:
                saveAll();
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please enter 1, 2, or 3.\n";
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
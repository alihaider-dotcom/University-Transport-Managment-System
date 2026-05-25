#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Student.h"
#include "Admin.h"
#include "Bus.h"
#include "Route.h"
#include "TransportPass.h"
#include <fstream>
using namespace std;

class FileManager {
public:
    // Save functions
    static void saveUsers(Student** students, int sCount,
                          Admin**   admins,   int aCount,
                          const string& filename = "data/users.txt");

    static void saveVehicles(Vehicle** vehicles, int count,
                             const string& filename = "data/vehicles.txt");

    static void saveRoutes(Route** routes, int count,
                           const string& filename = "data/routes.txt");

    static void savePasses(TransportPass** passes, int count,
                           const string& filename = "data/passes.txt");

    // Load functions
    static int loadStudents(Student**& students,
                            const string& filename = "data/users.txt");

    static int loadAdmins(Admin**& admins,
                          const string& filename = "data/users.txt");

    static int loadVehicles(Vehicle**& vehicles,
                            const string& filename = "data/vehicles.txt");

    static int loadRoutes(Route**& routes,
                          const string& filename = "data/routes.txt");

    static int loadPasses(TransportPass**& passes,
                          const string& filename = "data/passes.txt");
};

#endif

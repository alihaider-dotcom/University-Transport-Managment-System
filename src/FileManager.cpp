#include "FileManager.h"
#include <iostream>

// ==================== SAVE ====================

void FileManager::saveUsers(Student** students, int sCount,
                             Admin**   admins,   int aCount,
                             const string& filename) {
    ofstream file(filename);
    if (!file) { cerr << "Cannot open " << filename << "\n"; return; }

    for (int i = 0; i < sCount; i++)
        file << students[i]->serialize() << "\n";
    for (int i = 0; i < aCount; i++)
        file << admins[i]->serialize() << "\n";

    file.close();
    //cout << "  Users saved.\n";
}

void FileManager::saveVehicles(Vehicle** vehicles, int count,
                                const string& filename) {
    ofstream file(filename);
    if (!file) { cerr << "Cannot open " << filename << "\n"; return; }
    for (int i = 0; i < count; i++)
        file << vehicles[i]->serialize() << "\n";
    file.close();
    //cout << "  Vehicles saved.\n";
}

void FileManager::saveRoutes(Route** routes, int count,
                              const string& filename) {
    ofstream file(filename);
    if (!file) { cerr << "Cannot open " << filename << "\n"; return; }
    for (int i = 0; i < count; i++)
        file << routes[i]->serialize() << "\n";
    file.close();
    //cout << "  Routes saved.\n";
}

void FileManager::savePasses(TransportPass** passes, int count,
                              const string& filename) {
    ofstream file(filename);
    if (!file) { cerr << "Cannot open " << filename << "\n"; return; }
    for (int i = 0; i < count; i++)
        file << passes[i]->serialize() << "\n";
    file.close();
    //cout << "  Passes saved.\n";
}

// ==================== LOAD ====================

int FileManager::loadStudents(Student**& students, const string& filename) {
    ifstream file(filename);
    if (!file) return 0;

    int cap = 50, count = 0;
    students = new Student*[cap];
    string line;

    while (getline(file, line)) {
        if (line.find("|Student|") != string::npos ||
            line.substr(line.rfind('|')+1) == "" ) {
            // check role field (index 4 when split by |)
            int pipes = 0; size_t rolePos = 0;
            for (size_t k = 0; k < line.size(); k++) {
                if (line[k] == '|') { pipes++; if (pipes == 4) { rolePos = k+1; break; } }
            }
            string roleChunk = line.substr(rolePos, 7);
            if (roleChunk.substr(0,7) == "Student") {
                if (count == cap) { cap *= 2; Student** tmp = new Student*[cap];
                    for(int i=0;i<count;i++) tmp[i]=students[i]; delete[] students; students=tmp; }
                students[count] = new Student();
                students[count]->deserialize(line);
                count++;
            }
        }
    }
    file.close();
    return count;
}

int FileManager::loadAdmins(Admin**& admins, const string& filename) {
    ifstream file(filename);
    if (!file) return 0;

    int cap = 10, count = 0;
    admins = new Admin*[cap];
    string line;

    while (getline(file, line)) {
        int pipes = 0; size_t rolePos = 0;
        for (size_t k = 0; k < line.size(); k++) {
            if (line[k] == '|') { pipes++; if (pipes == 4) { rolePos = k+1; break; } }
        }
        if (line.substr(rolePos, 5) == "Admin") {
            admins[count] = new Admin();
            admins[count]->deserialize(line);
            count++;
        }
    }
    file.close();
    return count;
}

int FileManager::loadVehicles(Vehicle**& vehicles, const string& filename) {
    ifstream file(filename);
    if (!file) return 0;

    int cap = 20, count = 0;
    vehicles = new Vehicle*[cap];
    string line;

    while (getline(file, line)) {
        // type is the 7th field (index 6)
        int pipes = 0; size_t typePos = 0;
        for (size_t k = 0; k < line.size(); k++) {
            if (line[k] == '|') { pipes++; if (pipes == 6) { typePos = k+1; break; } }
        }
        string typeField = line.substr(typePos, 3);

        if (typeField == "Bus") {
            vehicles[count] = new Bus();
        } else {
            vehicles[count] = new Van();
        }
        vehicles[count]->deserialize(line);
        count++;
    }
    file.close();
    return count;
}

int FileManager::loadRoutes(Route**& routes, const string& filename) {
    ifstream file(filename);
    if (!file) return 0;

    int cap = 20, count = 0;
    routes = new Route*[cap];
    string line;

    while (getline(file, line)) {
        if (!line.empty()) {
            routes[count] = new Route();
            routes[count]->deserialize(line);
            count++;
        }
    }
    file.close();
    return count;
}

int FileManager::loadPasses(TransportPass**& passes, const string& filename) {
    ifstream file(filename);
    if (!file) return 0;

    int cap = 100, count = 0;
    passes = new TransportPass*[cap];
    string line;

    while (getline(file, line)) {
        if (!line.empty()) {
            passes[count] = new TransportPass();
            passes[count]->deserialize(line);
            count++;
        }
    }
    file.close();
    return count;
}

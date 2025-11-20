#include "hospital_system.h"
#include <algorithm> // for sort

// Global variable definitions
vector<Account> accounts;
vector<Patient> patients;
string currentUser = " ";

// COUNTDOWN FUNCTIONS
void countdownClear() {
    for (int i = 3; i > 0; i--) {
        cout << "\rClearing screen in " << i << "... ";
        this_thread::sleep_for(chrono::seconds(1));
    }
    system("cls");
}

void countdownExit() {
    for (int i = 3; i > 0; i--) {
        cout << "\rExiting in " << i << "... ";
        this_thread::sleep_for(chrono::seconds(1));
    }
    exit(0);
}

// START MENU
void startMenu() {
    int choice;
    do {
        cout << "\n ===== ACCOUNT =====\n";
        cout << "1. Login\n";
        cout << "2. Sign up\n";
        cout << "3. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;
        system("cls");

        if (choice == 1) login();
        else if (choice == 2) signup();
        else if (choice == 3) countdownExit();
        else cout << "Invalid choice.\n";
    } while (choice != 3);
}

// SIGN UP FUNCTION
void signup() {
    Account newAcc;
    string verifyID;

    cout << "===== STAFF SIGN UP =====\n";
    cout << "Enter hospital staff ID for verification: ";
    cin >> verifyID;

    if (!verifyStaffID(verifyID)) {
        cout << "Invalid Staff ID. You are not authorized to make an account.\n";
        countdownClear();
        return;
    }

    cout << "Staff ID verified.\n";
    cout << "Create username: ";
    cin >> newAcc.username;
    cout << "Create password: ";
    cin >> newAcc.password;
    newAcc.staffID = verifyID;

    for (auto &acc : accounts) {
        if (acc.username == newAcc.username) {
            cout << "Username already taken. Try again.\n";
            countdownClear();
            return;
        }
    }

    accounts.push_back(newAcc);
    cout << "Account successfully created! You can now log in.\n";
    countdownClear();
}

// VERIFY STAFF ID
bool verifyStaffID(string ID) {
    string validIDs[] = {"RN001", "RN002", "RN003", "DOC001", "DOC002", "DOC003"};
    for (string valid : validIDs) if (ID == valid) return true;
    return false;
}

// LOGIN
void login() {
    string username, password;
    bool found = false;

    cout << "===== LOGIN =====\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    for (auto &acc : accounts) {
        if (acc.username == username && acc.password == password) {
            found = true;
            currentUser = username;
            cout << "Login successful! Welcome, " << username << ".\n";
            countdownClear();
            menu();
            break;
        }
    }

    if (!found) {
        cout << "Invalid credentials. Please try again or sign up.\n";
        system("pause");
        system("cls");
    }
}

// LOGOUT
void logout() {
    cout << "Logging out...\n";
    currentUser = " ";
    countdownClear();
}

// HELPER: get sorted indexes by Hospital ID
vector<int> getSortedIndexes() {
    vector<int> idx(patients.size());
    for (int i = 0; i < patients.size(); i++) idx[i] = i;

    sort(idx.begin(), idx.end(), [](int a, int b) {
        return patients[a].hospitalID < patients[b].hospitalID;
    });

    return idx;
}

// MAIN MENU
void menu() {
    int choice;
    do {
        cout << "\n===== PATIENT RECORD SYSTEM =====\n";
        cout << "Logged in as: " << currentUser << endl;
        cout << "1. Add Patient Record\n";
        cout << "2. Search Patient by Hospital ID\n";
        cout << "3. Update Patient Record (NEW VERSION)\n";
        cout << "4. Delete a Patient Record\n";
        cout << "5. Logout\n";
        cout << "Choose an option: ";
        cin >> choice;
        system("cls");

        if (choice == 1) addRecord();
        else if (choice == 2) searchPatient();
        else if (choice == 3) updatePatient();
        else if (choice == 4) deletePatient();
        else if (choice == 5) logout();
        else cout << "Invalid choice.\n";

    } while (choice != 5 && currentUser != " ");
}

// ADD RECORD
void addRecord() {
    Patient p;
    cout << "\n--- Add New Patient Record ---\n";
    cin.ignore();

    cout << "Full Name: ";
    getline(cin, p.name);
    cout << "Age: ";
    cin >> p.age;
    cin.ignore();
    cout << "Sex: ";
    getline(cin, p.sex);
    cout << "Room/Bed Number: ";
    getline(cin, p.room);
    cout << "Hospital ID: ";
    cin >> p.hospitalID;
    cin.ignore();
    p.dateTime = currentDateTime();
    cout << "Nurse Name: ";
    getline(cin, p.nurseName);
    cout << "Designation (e.g. RN, LPN): ";
    getline(cin, p.designation);

    cout << "\n--- Vital Signs ---\n";
    cout << "Temperature (°C): ";
    cin >> p.temperature;
    cout << "Pulse: ";
    cin >> p.pulse;
    cout << "Respiratory Rate: ";
    cin >> p.respiratoryRate;
    cin.ignore();
    cout << "Blood Pressure: ";
    getline(cin, p.bloodPressure);
    cout << "Oxygen Saturation (SpO2): ";
    cin >> p.oxygenSaturation;
    cout << "Height (m): ";
    cin >> p.height;
    cout << "Weight (kg): ";
    cin >> p.weight;
    p.bmi = p.weight / (p.height * p.height);
    cin.ignore();

    cout << "\n--- Subjective Data ---\n";
    getline(cin, p.subjective);
    cout << "\n--- Objective Data ---\n";
    getline(cin, p.objective);
    cout << "\n--- Lab / Test Results ---\n";
    getline(cin, p.labResults);
    cout << "\n--- Evaluation ---\n";
    getline(cin, p.evaluation);
    cout << "\n--- Unusual Incidents ---\n";
    getline(cin, p.incidents);
    cout << "\n--- Communication ---\n";
    cout << "Doctor Assigned: ";
    getline(cin, p.doctor);
    cout << "Family Notes: ";
    getline(cin, p.family);
    cout << "Orders and Follow-up: ";
    getline(cin, p.orders);
    p.nurseAttended = currentDateTime();

    patients.push_back(p);
    cout << "\nRecord added successfully!\n";
    countdownClear();
}

// SEARCH FUNCTION
void searchPatient() {
    if (patients.empty()) {
        cout << "\nNo records available.\n";
        return;
    }

    cout << "\n===== SEARCH PATIENT RECORD =====\n";
    cout << "Hospital ID     Name     Date/Time\n";
    cout << "--------------------------------------\n";

    vector<int> sorted = getSortedIndexes();
    for (int i : sorted)
        cout << patients[i].hospitalID << "\t\t" << patients[i].name
             << "\t" << patients[i].dateTime;

    cout << "\n--------------------------------------\n";

    int id;
    cout << "\nEnter Hospital ID to view details: ";
    cin >> id;
    cin.ignore();

    vector<int> indexes;
    for (int i = 0; i < patients.size(); i++)
        if (patients[i].hospitalID == id) indexes.push_back(i);

    if (indexes.empty()) {
        cout << "No patient found with Hospital ID: " << id << endl;
        countdownClear();
        return;
    }

    cout << "\nSelect record to view:\n";
    for (int i = 0; i < indexes.size(); i++)
        cout << i + 1 << ". Date/Time: " << patients[indexes[i]].dateTime;
    int choice;
    cout << "\nEnter choice: ";
    cin >> choice;
    cin.ignore();
    choice--;

    if (choice < 0 || choice >= indexes.size()) {
        cout << "Invalid choice.\n";
        countdownClear();
        return;
    }

    Patient &p = patients[indexes[choice]];

    cout << "\n--- PATIENT DETAILS ---\n";
    cout << "Name: " << p.name << "\nAge: " << p.age << " | Sex: " << p.sex
         << "\nRoom: " << p.room << "\nHospital ID: " << p.hospitalID
         << "\nDate/Time: " << p.dateTime
         << "\nNurse: " << p.nurseName << " " << p.designation;
    cout << "\n\nVital Signs:\nTemp: " << p.temperature << "°C, Pulse: " << p.pulse
         << ", RR: " << p.respiratoryRate << ", BP: " << p.bloodPressure
         << ", SpO2: " << p.oxygenSaturation << "%, BMI: " << p.bmi;
    cout << "\n\nSubjective: " << p.subjective
         << "\nObjective: " << p.objective
         << "\nLab Results: " << p.labResults
         << "\nEvaluation: " << p.evaluation
         << "\nIncidents: " << p.incidents;
    cout << "\n\nDoctor: " << p.doctor
         << "\nFamily Notes: " << p.family
         << "\nOrders: " << p.orders
         << "\nNurse Attended: " << p.nurseAttended << endl;

    system("pause");
    system("cls");
}

// UPDATE PATIENT RECORD (NEW VERSION)
void updatePatient() {
    if (patients.empty()) {
        cout << "\nNo records available.\n";
        return;
    }

    cout << "\n===== UPDATE PATIENT RECORD =====\n";
    cout << "Hospital ID     Name     Date/Time\n";
    cout << "--------------------------------------\n";

    vector<int> sorted = getSortedIndexes();
    for (int i : sorted)
        cout << patients[i].hospitalID << "\t\t" << patients[i].name
             << "\t" << patients[i].dateTime;

    cout << "\n--------------------------------------\n";

    int id;
    cout << "\nEnter Hospital ID to update: ";
    cin >> id;
    cin.ignore();

    vector<int> indexes;
    for (int i = 0; i < patients.size(); i++)
        if (patients[i].hospitalID == id) indexes.push_back(i);

    if (indexes.empty()) {
        cout << "No patient found with Hospital ID: " << id << endl;
        countdownClear();
        return;
    }

    cout << "\nSelect record to use as base for new update:\n";
    for (int i = 0; i < indexes.size(); i++)
        cout << i + 1 << ". Date/Time: " << patients[indexes[i]].dateTime;
    int choice;
    cout << "Enter choice: ";
    cin >> choice;
    cin.ignore();
    choice--;

    if (choice < 0 || choice >= indexes.size()) {
        cout << "Invalid choice.\n";
        countdownClear();
        return;
    }

    Patient newRecord = patients[indexes[choice]]; // copy old record
    newRecord.dateTime = currentDateTime();

    cout << "\n--- Creating New Updated Record for " << newRecord.name << " ---\n";
    cout << "Leave blank to keep previous value.\n";

    string input;
    cout << "Full Name (" << newRecord.name << "): ";
    getline(cin, input); if(!input.empty()) newRecord.name = input;

    cout << "Age (" << newRecord.age << "): ";
    getline(cin, input); if(!input.empty()) newRecord.age = stoi(input);

    cout << "Sex (" << newRecord.sex << "): ";
    getline(cin, input); if(!input.empty()) newRecord.sex = input;

    cout << "Room/Bed Number (" << newRecord.room << "): ";
    getline(cin, input); if(!input.empty()) newRecord.room = input;

    cout << "Nurse Name (" << newRecord.nurseName << "): ";
    getline(cin, input); if(!input.empty()) newRecord.nurseName = input;

    cout << "Designation (" << newRecord.designation << "): ";
    getline(cin, input); if(!input.empty()) newRecord.designation = input;

    cout << "\n--- Vital Signs ---\n";

    cout << "Temperature (" << newRecord.temperature << "): ";
    getline(cin, input); if(!input.empty()) newRecord.temperature = stof(input);

    cout << "Pulse (" << newRecord.pulse << "): ";
    getline(cin, input); if(!input.empty()) newRecord.pulse = stoi(input);

    cout << "Respiratory Rate (" << newRecord.respiratoryRate << "): ";
    getline(cin, input); if(!input.empty()) newRecord.respiratoryRate = stoi(input);

    cout << "Blood Pressure (" << newRecord.bloodPressure << "): ";
    getline(cin, input); if(!input.empty()) newRecord.bloodPressure = input;

    cout << "Oxygen Saturation (" << newRecord.oxygenSaturation << "): ";
    getline(cin, input); if(!input.empty()) newRecord.oxygenSaturation = stoi(input);

    cout << "Height (" << newRecord.height << "): ";
    getline(cin, input); if(!input.empty()) newRecord.height = stof(input);

    cout << "Weight (" << newRecord.weight << "): ";
    getline(cin, input); if(!input.empty()) newRecord.weight = stof(input);

    newRecord.bmi = newRecord.weight / (newRecord.height * newRecord.height);

    cout << "\n--- Nursing Chart ---\n";

    cout << "Subjective (" << newRecord.subjective << "): ";
    getline(cin, input); if(!input.empty()) newRecord.subjective = input;

    cout << "Objective (" << newRecord.objective << "): ";
    getline(cin, input); if(!input.empty()) newRecord.objective = input;

    cout << "Lab Results (" << newRecord.labResults << "): ";
    getline(cin, input); if(!input.empty()) newRecord.labResults = input;

    cout << "Evaluation (" << newRecord.evaluation << "): ";
    getline(cin, input); if(!input.empty()) newRecord.evaluation = input;

    cout << "Incidents (" << newRecord.incidents << "): ";
    getline(cin, input); if(!input.empty()) newRecord.incidents = input;

    cout << "\n--- Communication ---\n";

    cout << "Doctor Assigned (" << newRecord.doctor << "): ";
    getline(cin, input); if(!input.empty()) newRecord.doctor = input;

    cout << "Family Notes (" << newRecord.family << "): ";
    getline(cin, input); if(!input.empty()) newRecord.family = input;

    cout << "Orders/Follow-up (" << newRecord.orders << "): ";
    getline(cin, input); if(!input.empty()) newRecord.orders = input;

    newRecord.nurseAttended = currentDateTime();

    patients.push_back(newRecord);
    cout << "\nNew patient record update saved successfully!\n";
    system("pause");
    system("cls");
}

// DELETE FUNCTION
void deletePatient() {
    if (patients.empty()) {
        cout << "\nNo records available.\n";
        return;
    }

    cout << "\n===== DELETE PATIENT RECORD =====\n";
    cout << "Hospital ID     Name     Date/Time\n";
    cout << "--------------------------------------\n";

    vector<int> sorted = getSortedIndexes();
    for (int i : sorted)
        cout << patients[i].hospitalID << "\t\t" << patients[i].name
             << "\t" << patients[i].dateTime;

    cout << "\n--------------------------------------\n";

    int id;
    cout << "\nEnter Hospital ID to delete: ";
    cin >> id;
    cin.ignore();

    vector<int> indexes;
    for (int i = 0; i < patients.size(); i++)
        if (patients[i].hospitalID == id) indexes.push_back(i);

    if (indexes.empty()) {
        cout << "No patient found with Hospital ID: " << id << endl;
        countdownClear();
        return;
    }

    cout << "\nSelect record to delete:\n";
    for (int i = 0; i < indexes.size(); i++)
        cout << i + 1 << ". Date/Time: " << patients[indexes[i]].dateTime;
    int choice;
    cout << "\nEnter choice: ";
    cin >> choice;
    cin.ignore();
    choice--;

    if (choice < 0 || choice >= indexes.size()) {
        cout << "Invalid choice.\n";
        countdownClear();
        return;
    }

    cout << "Are you sure you want to delete this record? (y/n): ";
    char confirm;
    cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        patients.erase(patients.begin() + indexes[choice]);
        cout << "Record deleted successfully!\n";
    } else {
        cout << "Deletion cancelled.\n";
    }
    countdownClear();
}

// CURRENT DATE/TIME
string currentDateTime() {
    time_t now = time(0);
    char* dt = ctime(&now);
    return dt;
}

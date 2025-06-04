#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <cctype>
#include <ctime>
#include <algorithm>
#include <conio.h>
using namespace std;

bool isDigits(const string& str) {
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}

bool isValidName(const string& name) {
    if (name.empty()) return false;
    for (char ch : name) if (!isalpha(ch) && ch != ' ') return false;
    return true;
}

string capitalize(string input) {
    if (!input.empty()) {
        input[0] = toupper(input[0]);
        for (size_t i = 1; i < input.size(); ++i)
            input[i] = tolower(input[i]);
    }
    return input;
}

void clearBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getValidatedChoice() {
    string input;
    int choice;
    while (true) {
        cout << "Select an option: ";
        if (!getline(cin, input) || input.empty()) {
            cin.clear();
            cout << "Invalid input. Try again.\n";
            continue;
        }
        if (!isDigits(input)) {
            cout << "Input must contain only digits. Try again.\n";
            continue;
        }
        try {
            choice = stoi(input);
            return choice;
        } catch (...) {
            cout << "Input too large or invalid format. Try again.\n";
        }
    }
}
struct Room {
    int id, typeId, capacity, availability;
    double price;
    string typeName;

    void display() const {
        cout << left << setw(8) << id << setw(10) << typeName
             << setw(10) << capacity << setw(10) << price << setw(15);
        switch (availability) {
            case 1: cout << "Available"; break;
            case 2: cout << "Booked"; break;
            case 3: cout << "Maintenance"; break;
            default: cout << "Unknown";
        }
        cout << endl;
    }
};

class RoomManager {
    vector<Room> rooms;
    string filePath = "rooms/room.txt";

    string getTypeName(int id) {
        switch (id) {
            case 1: return "Suite";
            case 2: return "Double";
            case 3: return "Deluxe";
            default: return "Unknown";
        }
    }

public:
    RoomManager() { loadRooms(); }

    void loadRooms() {
        rooms.clear();
        ifstream file(filePath);
        Room r;
        while (file >> r.id >> r.typeId >> r.capacity >> r.availability >> r.price) {
            r.typeName = getTypeName(r.typeId);
            rooms.push_back(r);
        }
    }

    void saveRooms() {
        ofstream file(filePath);
        for (Room& r : rooms)
            file << r.id << " " << r.typeId << " " << r.capacity << " "
                 << r.availability << " " << r.price << "\n";
    }

    void displayRooms() {
        cout << "\n=== All Rooms ===\n";
        cout << left << setw(8) << "RoomID" << setw(10) << "Type" << setw(10) << "Guests" << setw(10) << "Price" << setw(15) << "Status" << endl;
        for (Room& r : rooms) r.display();
    }

    Room* findAvailable(int typeId, int headcount) {
        for (auto& r : rooms)
            if (r.availability == 1 && r.typeId == typeId && r.capacity >= headcount)
                return &r;
        return nullptr;
    }

    Room* findById(int id) {
        for (auto& r : rooms)
            if (r.id == id) return &r;
        return nullptr;
    }

    vector<Room>& getAll() { return rooms; }

    void markBooked(int id) {
        if (auto r = findById(id)) {
            r->availability = 2;
            saveRooms();
        }
    }
};

class PaymentStrategy {
public:
    virtual bool pay(double amount) = 0;
    virtual string getMethodName() = 0;
    virtual ~PaymentStrategy() {}
};

class CashPayment : public PaymentStrategy {
private:
    double cashReceived = 0.0;
    double change = 0.0;

public:
    bool pay(double amount) override {
        cout << "Enter cash amount: ";
        cin >> cashReceived;
        clearBuffer();

        if (cashReceived < amount) {
            cout << "Insufficient amount. Payment failed.\n";
            return false;
        }

        change = cashReceived - amount;
        return true;
    }

    string getMethodName() override {
        stringstream ss;
        ss << "Cash" << endl;
        ss << "Cash Received: ₱" << fixed << setprecision(2) << cashReceived << endl;
        ss << "Change: ₱" << fixed << setprecision(2) << change << endl;
        return ss.str();
    }
};


class CardPayment : public PaymentStrategy {
public:
    bool pay(double amount) override {
        string name, number, expiry, cvv;
        cout << "Card Owner Name: "; getline(cin, name);
        cout << "Card Number (16 digits): "; getline(cin, number);
        cout << "Expiry (MM/YY): "; getline(cin, expiry);
        cout << "CVV: "; getline(cin, cvv);
        return isValidName(name) && number.length() == 16 && isDigits(number) && cvv.length() == 3;
    }
    string getMethodName() override { return "Credit Card"; }
};

class GcashPayment : public PaymentStrategy {
public:
    bool pay(double amount) override {
        string name, txn, amountPaid;
        cout << "Gcash Account Name: "; getline(cin, name);
        cout << "Transaction #: "; getline(cin, txn);
        cout << "Amount Paid: "; getline(cin, amountPaid);
        return isValidName(name) && isDigits(txn) && isDigits(amountPaid);
    }
    string getMethodName() override { return "GCash"; }
};

struct GuestAccount {
    string username, password, name, contact;
    vector<string> reservations;
    vector<string> receipts;

    void saveToFile() {
        ofstream out("guests/" + username + ".txt");
        out << username << "\n";
        out << password << "\n" << name << "\n" << contact << "\n";
        for (const string& res : reservations) out << "RES:" << res << "\n";
        for (const string& rec : receipts) out << "REC:" << rec << "\n";
    }

    bool loadFromFile(const string& uname) {
        reservations.clear();
        receipts.clear();
        ifstream in("guests/" + uname + ".txt");
        if (!in) return false;
        username = uname;
        string line;
        getline(in, line);
        getline(in, password);
        getline(in, name);
        getline(in, contact);
        while (getline(in, line)) {
            if (line.rfind("RES:", 0) == 0) reservations.push_back(line.substr(4));
            else if (line.rfind("REC:", 0) == 0) receipts.push_back(line.substr(4));
        }
        return true;
    }

    void viewReservations() {
        if (reservations.empty()) {
            cout << "No reservations found.\n";
        } else {
            cout << "\n=== My Reservations ===\n";
            for (const auto& res : reservations) cout << "- " << res << "\n";
        }
    }

    void viewReceipts() {
        if (receipts.empty()) {
            cout << "No receipts found.\n";
        } else {
            cout << "\n=== My Receipts ===\n";
            for (const auto& rec : receipts) cout << "- " << rec << "\n";
        }
    }
};

GuestAccount currentGuest;

void guestBooking(RoomManager& rm) {
    int headcount;
    cout << "Enter guest count: ";
    cin >> headcount; clearBuffer();
    
    cout << "Room Types:\n1. Suite\n2. Double\n3. Deluxe\n";
    cout << "Choose Type ID: ";
    int typeId;
    cin >> typeId; clearBuffer();

    Room* room = rm.findAvailable(typeId, headcount);
    if (!room) {
        cout << "No available room for that type and headcount.\n";
        return;
    }

    string ref = to_string(rand() % 900000000000 + 100000000000);
    string name = currentGuest.name;
    string contact = currentGuest.contact;
    string sex = "N/A";

    int paymentType;
    cout << "[1] Cash [2] Credit Card [3] GCash\nChoose Payment: ";
    cin >> paymentType; clearBuffer();

    PaymentStrategy* strategy;
    if (paymentType == 1) strategy = new CashPayment();
    else if (paymentType == 2) strategy = new CardPayment();
    else if (paymentType == 3) strategy = new GcashPayment();
    else return void(cout << "Invalid choice.\n");

    if (!strategy->pay(room->price)) {
        cout << "Payment failed.\n";
        delete strategy;
        return;
    }

    rm.markBooked(room->id);

    time_t now = time(0);
    string dt = ctime(&now);
    string res = "Room " + to_string(room->id) + " " + room->typeName + " (" + dt + ")";
    currentGuest.reservations.push_back(res);
    
    stringstream ss;
    ss << "Ref#" << ref << " | Room: " << room->id << " | Type: " << room->typeName
       << " | ₱" << room->price << " | Paid via: " << strategy->getMethodName() << " | " << dt;
    currentGuest.receipts.push_back(ss.str());
    currentGuest.saveToFile();

    cout << "\n=== BOOKING CONFIRMED ===\n";
    cout << "Reference #: " << ref << "\n";
    cout << "Room #: " << room->id << " (" << room->typeName << ")\n";
    cout << "Total: ₱" << room->price << "\n";
    cout << "Payment Method : " << strategy->getMethodName() << "\n";
    cout << "===========================\n";

    delete strategy;

    cout << "Would you like to book again? (Y/N): ";
    char again; cin >> again; clearBuffer();
    if (tolower(again) == 'y') guestBooking(rm);
}

bool signupGuest() {
    string uname, pass, name, contact;
    cout << "Choose a username: ";
    getline(cin, uname);
    ifstream check("guests/" + uname + ".txt");
    if (check) {
        cout << "Username already taken.\n";
        return false;
    }
    cout << "Choose a password: ";
    getline(cin, pass);
    cout << "Your name: ";
    getline(cin, name);
    cout << "Your contact #: ";
    getline(cin, contact);

    currentGuest.username = uname;
    currentGuest.password = pass;
    currentGuest.name = name;
    currentGuest.contact = contact;
    currentGuest.saveToFile();
    cout << "Signup successful!\n";
    return true;
}

bool loginGuest() {
    string uname, pass;
    cout << "Enter username: ";
    getline(cin, uname);
    cout << "Enter password: ";
    getline(cin, pass);
    if (!currentGuest.loadFromFile(uname)) {
        cout << "No account found.\n";
        return false;
    }
    if (pass != currentGuest.password) {
        cout << "Incorrect password.\n";
        return false;
    }
    cout << "Welcome back, " << currentGuest.name << "!\n";
    return true;
}
class Admin {
    string logPath = "logs/audit.txt";
public:
    bool adminLogin() {
        cout << "Enter admin password: ";
        string input;
        char ch;
        while ((ch = _getch()) != 13) {
            if (ch == 8 && !input.empty()) { input.pop_back(); cout << "\b \b"; }
            else { input += ch; cout << '*'; }
        }
        cout << endl;
        if (input == "admin123") {
            log("Logged in");
            return true;
        }
        cout << "Invalid password.\n";
        log("Failed login attempt");
        return false;
    }

    void log(const string& msg) {
        ofstream log(logPath, ios::app);
        time_t now = time(0);
        log << "[ADMIN][" << ctime(&now) << "] " << msg << endl;
    }

    void viewLogs() {
        ifstream in(logPath);
        cout << "\n--- Admin Logs ---\n";
        string line;
        while (getline(in, line)) cout << line << "\n";
    }

    void adminMenu(RoomManager& rm) {
        while (true) {
            cout << "\n--- Admin Panel ---\n[1] View Rooms\n[2] Add Room\n[3] Update Room\n[4] Delete Room\n[5] View Logs\n[6] Change Room Status\n[0] Logout\n";
            int choice = getValidatedChoice();
            if (choice == 0) break;
            if (choice == 1) rm.displayRooms();
            else if (choice == 2) {
                Room r;
                cout << "Enter Room ID: "; cin >> r.id; clearBuffer();
                cout << "1: Suite, 2: Double, 3: Deluxe" << endl;
                cout << "Type ID: "; cin >> r.typeId; clearBuffer();
                cout << "Suite:2-4, Double:1-2, Deluxe:1-4" << endl;
                cout << "Capacity: "; cin >> r.capacity; clearBuffer();
                cout << "Price: "; cin >> r.price; clearBuffer();
                r.typeName = (r.typeId == 1 ? "Suite" : r.typeId == 2 ? "Double" : "Deluxe");
                r.availability = 1;
                rm.getAll().push_back(r);
                rm.saveRooms();
                log("Added Room ID " + to_string(r.id));
            } else if (choice == 3) {
                int id;
                cout << "Enter Room ID to update: ";
                cin >> id;
                Room* r = rm.findById(id);
                if (!r) cout << "Not found.\n";
                else {
                    cout << "New Type ID (1=Suite, 2=Double, 3=Deluxe): "; cin >> r->typeId; clearBuffer();
                    cout << "New Capacity: "; cin >> r->capacity; clearBuffer();
                    cout << "New Price: "; cin >> r->price; clearBuffer();
                    cout << "Update Status (1=Available, 2=Booked, 3=Maintenance): "; cin >> r->availability; clearBuffer();
                    r->typeName = (r->typeId == 1 ? "Suite" : r->typeId == 2 ? "Double" : "Deluxe");
                    rm.saveRooms();
                    log("Updated Room ID " + to_string(r->id) + " (type, capacity, price, status)");
                }
            } else if (choice == 4) {
                int id;
                cout << "Enter Room ID to delete: ";
                cin >> id;
                auto& all = rm.getAll();
                auto it = remove_if(all.begin(), all.end(), [&](Room& r) { return r.id == id; });
                if (it != all.end()) {
                    log("Deleted Room ID " + to_string(id));
                    all.erase(it, all.end());
                    rm.saveRooms();
                } else cout << "Room not found.\n";
            } else if (choice == 5) viewLogs();
            else if (choice == 6) {
            int id;
                cout << "Enter Room ID to update status: ";
                cin >> id;
                clearBuffer();
            Room* r = rm.findById(id);
            if (!r) {
                cout << "Room not found.\n";
            } else {
                cout << "Current Status: " << (r->availability == 1 ? "Available" : r->availability == 2 ? "Booked" : "Maintenance") << "\n";
                cout << "Would you like to change the status? (Y/N): ";
            char confirm;
                cin >> confirm;
                clearBuffer();
            if (tolower(confirm) == 'y') {
                cout << "Select new status:\n";
                cout << "[1] Available\n[2] Booked\n[3] Maintenance\n[0] Cancel\n";
            int newStatus = getValidatedChoice();
            if (newStatus >= 1 && newStatus <= 3) {
                r->availability = newStatus;
                rm.saveRooms();
                log("Changed status of Room ID " + to_string(r->id));
                cout << "Room status updated successfully.\n";
            } else {
                cout << "Cancelled.\n";
            }
        }
    }
}
        }
        log("Logged out");
    }
};
int main() {
    RoomManager rm;
    Admin admin;
    bool isLoggedIn = false;

    while (true) {
        cout << "\n=== HOTEL RESERVATION SYSTEM ===\n";
        cout << "[1] View Rooms\n";
        cout << "[2] Guest Login\n";
        cout << "[3] Guest Signup\n";
        cout << "[4] Admin Login\n";
        cout << "[0] Exit\n";

        int choice = getValidatedChoice();
        if (choice == 0) break;

        switch (choice) {
            case 1:
                rm.displayRooms();
                cout << "\nWould you like to book now? (Y/N): ";
                char goBook;
                cin >> goBook;
                clearBuffer();
                if (tolower(goBook) == 'y') {
                    if (!isLoggedIn) {
                        cout << "Please login or signup to book.\n";
                    } else {
                        guestBooking(rm);
                    }
                }
                break;

            case 2: {
                clearBuffer();
                if (loginGuest()) {
                    isLoggedIn = true;
                    while (true) {
                        cout << "\n=== GUEST MENU ===\n";
                        cout << "[1] View Rooms\n";
                        cout << "[2] Book Now\n";
                        cout << "[3] My Reservations\n";
                        cout << "[4] My Receipts\n";
                        cout << "[0] Logout\n";
                        int guestChoice = getValidatedChoice();
                        if (guestChoice == 0) {
                            isLoggedIn = false;
                            break;
                        } else if (guestChoice == 1) {
                            rm.displayRooms();
                        } else if (guestChoice == 2) {
                            guestBooking(rm);
                        } else if (guestChoice == 3) {
                            currentGuest.viewReservations();
                        } else if (guestChoice == 4) {
                            currentGuest.viewReceipts();
                        } else {
                            cout << "Invalid option.\n";
                        }
                    }
                }
                break;
            }

            case 3:
                clearBuffer();
                signupGuest();
                break;

            case 4:
                clearBuffer();
                if (admin.adminLogin()) {
                    admin.adminMenu(rm);
                }
                break;

            default:
                cout << "Invalid option.\n";
        }
    }

    cout << "Exiting system. Goodbye!\n";
    return 0;
}

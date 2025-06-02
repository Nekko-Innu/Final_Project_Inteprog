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

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

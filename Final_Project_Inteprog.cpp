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

int main(){
    
}
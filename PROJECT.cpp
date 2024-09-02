#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <string>

using namespace std;

class Login {
private:
    string LoginId, Password;
public:
    Login() : LoginId(""), Password("") {}

    void setId(string id) {
        LoginId = id;
    }

    void setPW(string pw) {
        Password = pw;
    }

    string getId() const {
        return LoginId;
    }

    string getPW() const {
        return Password;
    }
};

void registration(Login& log) {
    system("cls");
    string id, pw;
    cout << "\tEnter Login Id: ";
    cin >> id;
    log.setId(id);

start:
    cout << "\tEnter A Strong Password (Minimum 8-character): ";
    cin >> pw;
    if (pw.length() >= 8) {
        log.setPW(pw);
    } else {
        cout << "\tEnter a minimum 8-character password!" << endl;
        goto start;
    }

    ofstream outFile("Login.txt", ios::app); // writing to the file
    if (!outFile) {
        cout << "\tError: File can't open!" << endl;
    } else {
        outFile << log.getId() << " : " << log.getPW() << endl;
        cout << "\tUser Registered Successfully!" << endl;
    }
    outFile.close();
    Sleep(1000);
}

bool login(Login& log) {
    system("cls");
    string id, pw;
    cout << "\tEnter Login Id: ";
    cin >> id;
    cout << "\tEnter Password: ";
    cin >> pw;

    ifstream infile("Login.txt");
    if (!infile) {
        cout << "\tError: File Can't Open!" << endl;
    } else {
        string line;
        while (getline(infile, line)) {
            stringstream ss(line);
            string userId, userPW;
            char delimiter;
            ss >> userId >> delimiter >> userPW;

            if (id == userId && pw == userPW) {
                log.setId(id); // Store the current login ID
                cout << "\tPlease Wait";
                for (int i = 0; i < 3; i++) {
                    cout << ".";
                    Sleep(800);
                }
                system("cls");
                cout << "\tWelcome To Library Management System" << endl;
                return true;
            }
        }
        cout << "\tError: Credentials Not Found" << endl;
    }
    infile.close();
    Sleep(5000);
    return false;
}

void addBook(const string& currentLoginID) {
    system("cls");
    string title, author;
    cout << "\tEnter Book Title: ";
    cin.ignore();
    getline(cin, title);
    cout << "\tEnter Book Author: ";
    getline(cin, author);

    ofstream outFile("Library.txt", ios::app); // writing and appending to file
    if (!outFile) {
        cout << "\tError: File can't open!" << endl;
    } else {
        outFile << currentLoginID << "," << title << " : " << author << endl;
        cout << "\tBook Added Successfully!" << endl;
    }
    outFile.close();
    Sleep(1000);
}

void viewBooks(const string& currentLoginID) {
    system("cls");
    ifstream infile("Library.txt"); // reading from file
    if (!infile) {
        cout << "\tError: File Can't Open!" << endl;
    } else {
        string line;
        bool bookFound = false;
        cout << "\tList of Books for Login Id :" << currentLoginID << endl;
        cout << "\t-----------------------------" << endl;

        // Read each line from the file
        while (getline(infile, line)) {
            size_t pos = line.find(",");
            if (pos != string::npos) {
                string loginID = line.substr(0, pos); // Extract login ID
                string bookDetails = line.substr(pos + 1); // Extract book details

                // Display the book if the login ID matches the current user
                if (loginID == currentLoginID) {
                    cout << "\t" << bookDetails << endl;
                    bookFound = true;
                }
            }
        }

        if (!bookFound) {
            cout << "\tNo books found for this user." << endl;
        }
    }

    infile.close();  // Close the file
    Sleep(2000);  // Pause the program for 2 seconds before continuing
}

void deleteBook(const string& currentLoginID) {
    system("cls");
    
    // Display the user's books before proceeding with the deletion
    ifstream infile("Library.txt");
    if (!infile) {
        cout << "\tError: File Can't Open!" << endl;
        return;
    }
    
    string line;
    bool bookFound = false;
    cout << "\tList of Books for Login Id :" << currentLoginID  << endl;
    cout << "\t-----------------------------" << endl;
    while (getline(infile, line)) {
        size_t pos = line.find(",");
        if (pos != string::npos) {
            string loginID = line.substr(0, pos);
            string bookDetails = line.substr(pos + 1);
            if (loginID == currentLoginID) {
                cout << "\t" << bookDetails << endl;
                bookFound = true;
            }
        }
    }
    infile.close();
    if (!bookFound) {
        cout << "\tNo books found for this user." << endl;
        Sleep(1000);
        return;
    
    }
    
    // Ask the user to enter the title of the book to delete
    cout << "\n\tEnter Book Title to Delete: ";
    string title;
    cin.ignore(); // Clear the input buffer
    getline(cin, title);

    // Open the file again to process the deletion
    infile.open("Library.txt");
    ofstream tempFile("Temp.txt"); // Temporary file for storing remaining books

    if (!infile || !tempFile) {
        cout << "\tError: File Can't Open!" << endl;
        return;
    }

    bool found = false;
    while (getline(infile, line)) {
        size_t pos = line.find(",");
        if (pos != string::npos) {
            string loginID = line.substr(0, pos);
            string bookDetails = line.substr(pos + 1);

            if (loginID == currentLoginID && bookDetails.find(title) != string::npos) {
                found = true; // Book to delete found
            } else {
                tempFile << line << endl; // Keep other books
            }
        }
    }

    infile.close();
    tempFile.close();

    remove("Library.txt"); // Delete the original file
    rename("Temp.txt", "Library.txt"); // Rename temp file to original file name

    if (found) {
        cout << "\tBook Deleted Successfully!" << endl;
    } else {
        cout << "\tBook Not Found!" << endl;
    }
    Sleep(1000);
}

void libraryMenu(const string& currentLoginID) {
    bool exit = false;
    while (!exit) {
        system("cls");
        int val;
        cout << "\tLibrary Management System" << endl;
        cout << "\t-------------------------" << endl;
        cout << "\t1. Add Book" << endl;
        cout << "\t2. View Books" << endl;
        cout << "\t3. Delete Book" << endl;
        cout << "\t4. Logout" << endl;
        cout << "\tEnter Choice: ";
        cin >> val;

        if (val == 1) {
            addBook(currentLoginID);
        } else if (val == 2) {
            viewBooks(currentLoginID);
        } else if (val == 3) {
            deleteBook(currentLoginID);
        } else if (val == 4) {
            exit = true;
        }
    }
}

int main() {
    Login log;
    bool exit = false;

    while (!exit) {
        system("cls");
        int val;
        cout << "\tWelcome To Registration & Login Form" << endl;
        cout << "\t------------------------------------" << endl;
        cout << "\t1. Register" << endl;
        cout << "\t2. Login" << endl;
        cout << "\t3. Exit" << endl;
        cout << "\tEnter Choice: ";
        cin >> val;

        if (val == 1) {
            registration(log);
        } else if (val == 2) {
            if (login(log)) {
                libraryMenu(log.getId());
            }
        } else if (val == 3) {
            system("cls");
            exit = true;
            cout << "\tThank You!!!" << endl;
            Sleep(1000);
        }
        Sleep(1000);
    }
}

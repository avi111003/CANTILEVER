#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>

using namespace std;

class User {
public:
    string username;
    string name;
    set<string> friends;
    vector<string> messages;

    User() {}

    User(string uname, string n) : username(uname), name(n) {}

    void addFriend(const string& friendUsername) {
        friends.insert(friendUsername);
    }

    void postMessage(const string& msg) {
        messages.push_back(msg);
    }
};

map<string, User> userDB;

const string USERS_FILE = "users.txt";
const string FRIENDS_FILE = "friends.txt";
const string POSTS_FILE = "posts.txt";

void loadUsers() {
    ifstream fin(USERS_FILE);
    if (!fin) return;

    string uname, name;
    while (fin >> uname) {
        getline(fin >> ws, name);
        userDB[uname] = User(uname, name);
    }
    fin.close();

    ifstream ffin(FRIENDS_FILE);
    string u1, u2;
    while (ffin >> u1 >> u2) {
        userDB[u1].addFriend(u2);
        userDB[u2].addFriend(u1);
    }
    ffin.close();

    ifstream pfin(POSTS_FILE);
    string line;
    while (getline(pfin, line)) {
        size_t arrow = line.find("->");
        size_t colon = line.find(":");

        if (arrow == string::npos || colon == string::npos) continue;

        string sender = line.substr(0, arrow - 1);
        string receiver = line.substr(arrow + 3, colon - arrow - 3);
        string msg = line.substr(colon + 2);

        userDB[sender].postMessage("To " + receiver + ": " + msg);
        userDB[receiver].postMessage("From " + sender + ": " + msg);
    }
    pfin.close();
}

void saveUsers() {
    ofstream fout(USERS_FILE);
    for (const auto& pair : userDB) {
        fout << pair.first << "\n" << pair.second.name << "\n";
    }
    fout.close();
}

void createProfile() {
    string uname, name;
    cout << "Enter username: ";
    cin >> uname;

    if (userDB.find(uname) != userDB.end()) {
        cout << "Username already exists.\n";
        return;
    }

    cout << "Enter your full name: ";
    cin.ignore();
    getline(cin, name);

    userDB[uname] = User(uname, name);
    saveUsers();

    cout << "Profile created successfully.\n";
}

void addFriend() {
    string u1, u2;
    cout << "Enter your username: ";
    cin >> u1;
    cout << "Enter friend's username: ";
    cin >> u2;

    if (u1 == u2) {
        cout << "You cannot add yourself as a friend.\n";
        return;
    }

    if (userDB.find(u1) == userDB.end() || userDB.find(u2) == userDB.end()) {
        cout << "One or both usernames not found.\n";
        return;
    }

    if (userDB[u1].friends.count(u2)) {
        cout << "You are already friends with " << u2 << ".\n";
        return;
    }

    userDB[u1].addFriend(u2);
    userDB[u2].addFriend(u1);

    ofstream fout(FRIENDS_FILE, ios::app);
    fout << u1 << " " << u2 << "\n";
    fout.close();

    cout << "Friend added successfully.\n";
}

void postMessage() {
    string sender, receiver, msg;

    cout << "Enter your username (sender): ";
    cin >> sender;
    cout << "Enter receiver's username: ";
    cin >> receiver;
    cin.ignore();

    if (userDB.find(sender) == userDB.end() || userDB.find(receiver) == userDB.end()) {
        cout << "One or both usernames not found.\n";
        return;
    }

    cout << "Enter your message: ";
    getline(cin, msg);

    userDB[sender].postMessage("To " + receiver + ": " + msg);
    userDB[receiver].postMessage("From " + sender + ": " + msg);

    ofstream fout(POSTS_FILE, ios::app);
    fout << sender << " -> " << receiver << ": " << msg << "\n";
    fout.close();

    cout << "Message sent successfully.\n";
}

void viewProfile() {
    string uname;
    cout << "Enter username to view: ";
    cin >> uname;

    if (userDB.find(uname) == userDB.end()) {
        cout << "User not found.\n";
        return;
    }

    User& u = userDB[uname];
    cout << "\n--- Profile ---\n";
    cout << "Name: " << u.name << "\n";
    cout << "Username: " << u.username << "\n";

    cout << "Friends: ";
    for (const auto& f : u.friends) cout << f << " ";
    cout << "\n";

    cout << "Messages:\n";
    for (const auto& m : u.messages) cout << "- " << m << "\n";
    cout << "---------------\n";
}

void showMenu() {
    cout << "\n===== Simple Social Network =====\n";
    cout << "1. Create Profile\n";
    cout << "2. Add Friend\n";
    cout << "3. Post Message\n";
    cout << "4. View Profile\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    loadUsers();

    int choice;
    while (true) {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1: createProfile(); break;
            case 2: addFriend(); break;
            case 3: postMessage(); break;
            case 4: viewProfile(); break;
            case 5: cout << "Goodbye.\n"; return 0;
            default: cout << "Invalid choice.\n";
        }
    }
}

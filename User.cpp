#include <iostream>
#include "User.h"
using namespace std;
int User::nextUserId = 1;
User::User() : userId(nextUserId++), email(), password(), isLoggedIn(false) {
    cout << "User created with auto-assigned ID: " << userId << endl;
}
User::User(const Str& email, const Str& password)
    : userId(nextUserId++), email(email), password(password), isLoggedIn(false) {
    cout << "User registered with ID: " << userId << " and email: " << email << endl;
}
User::User(int userId, const Str& email, const Str& password)
    : userId(userId), email(email), password(password), isLoggedIn(false) {
    if (userId >= nextUserId) {
        nextUserId = userId + 1;
    }
    cout << "User loaded with ID: " << userId << " and email: " << email << endl;
}
User::User(const User& other)
    : userId(nextUserId++), email(other.email), password(other.password), isLoggedIn(false) {
    cout << "User copied with new ID: " << userId << endl;
}
User& User::operator=(const User& other) {
    if (this != &other) {
        email = other.email;
        password = other.password;
        isLoggedIn = other.isLoggedIn;
    }
    return *this;
}
User::~User() {
}

bool User::login(const Str& email, const Str& password) {
    if (this->email == email && this->password == password) {
        isLoggedIn = true;
        cout << "Login successful for user ID " << userId << ": " << email << endl;
        return true;
    }
    cout << "Login failed for user ID " << userId << ": " << email << endl;
    return false;
}

void User::logout() {
    if (isLoggedIn) {
        isLoggedIn = false;
        cout << "User ID " << userId << " (" << email << ") has logged out." << endl;
    }
    else {
        cout << "User ID " << userId << " (" << email << ") is already logged out." << endl;
    }
}

bool User::changePassword(const Str& oldPass, const Str& newPass) {
    if (password == oldPass) {
        password = newPass;
        cout << "Password changed successfully for user ID " << userId << "." << endl;
        return true;
    }
    else {
        cout << "Old password incorrect for user ID " << userId << ". Password not changed." << endl;
        return false;
    }
}

int User::getUserId() const {
    return userId;
}

const Str& User::getEmail() const
{ return email; }


bool User::getLoginStatus() const {
    return isLoggedIn;
}
int User::getNextUserId() {
    return nextUserId;
}

void User::setNextUserId(int id) {
    if (id > 0) {
        nextUserId = id;
    }
}
void User::saveToBinary(ofstream& out) const {
    out.write((char*)&userId, sizeof(userId));
    email.writeToBinary(out);
    password.writeToBinary(out);
    out.write((char*)&isLoggedIn, sizeof(isLoggedIn));
}

void User::loadFromBinary(ifstream& in) {
    in.read((char*)&userId, sizeof(userId));
    email.readFromBinary(in);
    password.readFromBinary(in);
    in.read((char*)&isLoggedIn, sizeof(isLoggedIn));

    if (userId >= nextUserId) {
        nextUserId = userId + 1;
    }
}

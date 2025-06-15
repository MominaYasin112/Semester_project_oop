#include "AuthenticationManager.h"
#include "Student.h"
#include <iostream>
#include"AppException.h"
AuthenticationManager* AuthenticationManager::instance = nullptr;

AuthenticationManager* AuthenticationManager::getInstance() {
    if (!instance)
        instance = new AuthenticationManager();
    return instance;
}
AuthenticationManager::AuthenticationManager() {
}
bool AuthenticationManager::validateEmail(const Str& email) {
    for (int i = 0; i < users.size(); ++i) {
        if (users[i]->getEmail().is_equal(email))
            return false; 
    }
    return true;
}

Str AuthenticationManager::hashPassword(const Str& password) {
    Str hashed;
    for (int i = password.length(password.return_array()) - 1; i >= 0; --i)
        hashed += password[i];
    return hashed;
}

Student* AuthenticationManager::registerUser(const Str& email, const Str& password) {
    if (!validateEmail(email)) {
        throw AuthException("Email already in use.");
    }
    Str hashedPassword = hashPassword(password);
    std::cout << "Registering user with email: " << email << ", hashed password: " << hashedPassword << std::endl;
    Student* newStudent = new Student(email, hashedPassword,0);
    newStudent->setReputationPoints(50);
    users.push(newStudent);
    return newStudent;
}

User* AuthenticationManager::loginUser(const Str& email, const Str& password) {
    Str hashedPass = hashPassword(password);
    std::cout << "Attempting login with email: " << email << ", hashed password: " << hashedPass << std::endl;
    for (int i = 0; i < users.size(); ++i) {
        std::cout << "Checking against stored user email: " << users[i]->getEmail()
            << ", stored password: " << users[i]->getPassword() << std::endl;
        if (users[i]->getEmail()==email && users[i]->getPassword() == hashedPass) {
            users[i]->setLoggedIn(true);
            currentUser = users[i];
            return users[i];
        }
    }
    throw AuthException("Invalid credentials. Please try again.");
}



User* AuthenticationManager::getCurrentUser() const {
    return currentUser;
}

void AuthenticationManager::setCurrentUser(User* user) {
    currentUser = user;
}
bool AuthenticationManager::isEmailAvailable(const Str& email) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getEmail().is_equal(email)) {
            return false;
        }
    }
    return true;
}
Student* AuthenticationManager::verifyLogin(const Str& email, const Str& password) {
    return dynamic_cast<Student*>(loginUser(email, password));
}
void AuthenticationManager::saveToFile(const char* filename) {
    ofstream out(filename, ios::binary);
    int count = users.size();
    out.write((char*)&count, sizeof(count));

    for (int i = 0; i < count; i++) {
        Student* s = dynamic_cast<Student*>(users[i]);
        if (s) {
            s->saveToBinary(out);
        }
    }

    out.close();
}

void AuthenticationManager::loadFromFile(const char* filename) {
    ifstream in(filename, ios::binary);
    if (!in) return;

    int count;
    in.read((char*)&count, sizeof(count));
    for (int i = 0; i < count; i++) {
        Student* s = new Student();
        s->loadFromBinary(in);
        for (int j = 0; j < s->getOfferedSkills().size(); ++j) {
            s->getOfferedSkills()[j]->setStudentId(s->getUserId());
        }
        for (int j = 0; j < s->getRequestedSkills().size(); ++j) {
            s->getRequestedSkills()[j]->setStudentId(s->getUserId());
        }
        users.push(s);
    }
    in.close();
}

Student* AuthenticationManager::getStudentById(int studentId) {
    for (int i = 0; i < users.size(); ++i) {
        Student* s = dynamic_cast<Student*>(users[i]);
        if (s && s->getUserId() == studentId) {
            return s;
        }
    }
    return nullptr;
}
Dynamic_array<Student*>& AuthenticationManager::getAllRegisteredStudents() {
    static Dynamic_array<Student*> allStudents;
    allStudents.clear();
    for (int i = 0; i < users.size(); ++i) 
    {
        Student* student = dynamic_cast<Student*>(users[i]);
        if (student) {
            allStudents.push(student);
        }
    }

    return allStudents;
}
bool AuthenticationManager::isEmailRegistered(const Str& email) const {
    for (int i = 0; i < users.size(); ++i) {
        if (users[i]->getEmail().is_equal(email)) {
            return true;
        }
    }
    return false;
}
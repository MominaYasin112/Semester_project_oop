#ifndef AUTHENTICATION_MANAGER_H
#define AUTHENTICATION_MANAGER_H

#include "Str.h"
#include "Dynamic_array.h"
#include "TimeSlot.h"
#include "User.h"
#include "Student.h"
#include "DateTime.h" 

class AuthenticationManager {
private:
    static AuthenticationManager* instance;
    Dynamic_array<User*> users;
    User* currentUser = nullptr;
    AuthenticationManager();
public:
    static AuthenticationManager* getInstance();

    bool validateEmail(const Str& email);
    Str hashPassword(const Str& password);
    bool isEmailAvailable(const Str& email);

    User* loginUser(const Str& email, const Str& password);
    Student* verifyLogin(const Str& email, const Str& password);
    Student* registerUser(const Str& email, const Str& password);
    User* getCurrentUser() const;
    void setCurrentUser(User* user);
    void loadFromFile(const char* filename);
    void saveToFile(const char* filename);
    Student* getStudentById(int studentId);
    Dynamic_array<Student*>& getAllRegisteredStudents();
    bool isEmailRegistered(const Str& email) const;
    bool isLoggedIn() const {
        return currentUser != nullptr;
    }
    void logout() {
        currentUser = nullptr;
    }
    Student* getLoggedInUser() {
        return dynamic_cast<Student*>(currentUser);
    }
};

#endif

#ifndef USER_H
#define USER_H
#include "Str.h"
#include<fstream>
using namespace std;
class User {
private:
    static int nextUserId;

protected:
    int userId;
    Str email;
    Str password;
    bool isLoggedIn;

public:
    User();
    User(const Str& email, const Str& password);
    User(int userId, const Str& email, const Str& password);
    User(const User& other);

    User& operator=(const User& other);
    virtual ~User();
    bool login(const Str& email, const Str& password);
    void logout();
    bool changePassword(const Str& oldPass, const Str& newPass);
    int getUserId() const;
    const Str& getEmail() const;
    bool getLoginStatus() const;
    static int getNextUserId();
    static void setNextUserId(int id);
    Str getPassword() const { return password; }
    void setLoggedIn(bool status) { isLoggedIn = status; }
    virtual void saveToBinary(ofstream& out) const;
    virtual void loadFromBinary(ifstream& in);
};

#endif
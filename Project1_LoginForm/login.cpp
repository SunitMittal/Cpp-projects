#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <functional>

using namespace std;

class User
{
private:
    string username;
    string email;
    string passwordHash;

public:
    User(string u, string e, string pHash)
        : username(u), email(e), passwordHash(pHash) {}

    string getUsername() const { return username; }
    string getEmail() const { return email; }
    string getPasswordHash() const { return passwordHash; }

    string toFileString() const{
        return username + "*" + email + "*" + passwordHash;
    }
};

class AuthSystem
{
private:
    const string filename = "loginData.txt";

    string hashPassword(const string &password){
        return to_string(hash<string>{}(password));
    }

    bool isEmailValid(const string &email){
        const regex pattern(R"(^[\w\.-]+@[\w\.-]+\.\w+$)");
        return regex_match(email, pattern);
    }

    bool usernameExists(const string &username){
        ifstream file(filename);
        string u, e, p;

        while(getline(file, u, '*') && getline(file, e, '*') && getline(file, p)){
            if (u == username)
                return true;
        }
        return false;
    }

public:
    void signUp(){
        string username, email, password;

        cout<<"------ SIGN UP ------"<<endl;
        cout<<"Username: ";
        getline(cin, username);

        if (usernameExists(username)){
            cout<<"Username already exists!"<<endl;
            return;
        }

        cout<<"Email: ";
        getline(cin, email);

        if (!isEmailValid(email)){
            cout<<"Invalid email format!"<<endl;
            return;
        }

        cout<<"Password: ";
        getline(cin, password);

        string hashedPassword = hashPassword(password);
        User user(username, email, hashedPassword);

        ofstream file(filename, ios::app);
        file << user.toFileString() << endl;
        file.close();

        cout<<"Signup successful!"<<endl<<endl;
    }

    void login(){
        string username, password;
        bool found = false;

        cout<<"------ LOGIN ------"<<endl;
        cout<<"Username: ";
        getline(cin, username);
        cout<<"Password: ";
        getline(cin, password);

        string hashedInput = hashPassword(password);

        ifstream file(filename);
        string u, e, p;

        while(getline(file, u, '*') && getline(file, e, '*') && getline(file, p)){
            if (u == username){
                found = true;
                if (p == hashedInput){
                    cout<<"Login successful!"<<endl;
                    cout<<"Username: "<<u<<endl;
                    cout<<"Email: "<<e<<endl<<endl;
                }
                else
                    cout<<"Incorrect password!"<<endl<<endl;
                break;
            }
        }

        if (!found)
            cout<<"Account not found!"<<endl<<endl;
    }

    void forgotPassword(){
        string username, email;
        bool found = false;

        cout<<"------ FORGOT PASSWORD ------"<<endl;
        cout << "Username: ";
        getline(cin, username);
        cout << "Email: ";
        getline(cin, email);

        ifstream file(filename);
        string u, e, p;

        while(getline(file, u, '*') && getline(file, e, '*') && getline(file, p)){
            if (u == username && e == email){
                cout<<"Account verified."<<endl;
                cout<<"Password recovery is disabled for security."<<endl<<endl;
                found = true;
                break;
            }
        }

        if (!found)
            cout<<"Account not found!"<<endl<<endl;
    }
};

int main()
{
    AuthSystem auth;
    char choice;

    do{
        cout << "1- Login" << endl;
        cout << "2- Sign-Up" << endl;
        cout << "3- Forgot Password" << endl;
        cout << "4- Exit" << endl;
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();
        cout << endl;

        switch (choice){
        case '1':
            auth.login();
            break;
        case '2':
            auth.signUp();
            break;
        case '3':
            auth.forgotPassword();
            break;
        case '4':
            cout << "Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice!" << endl;
        }
    } while (choice != '4');

    return 0;
}
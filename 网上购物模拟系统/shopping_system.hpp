#include<iostream>

using namespace std;

class ShoppingSystem{
public:
    ShoppingSystem();
    //~ShoppingSystem();
    void Run();
    void Exit(); //退出系统
};

class Administrator{
private:
    string name;
    string password;
public:
    Administrator();
    //~Administrator();
    int LogIn(string name, string password);
    void LogOut(); //退出登录
};

class Customer{
private:
    string username;
    string password;
public:
    Customer(string username, string password);
    //~Customer();
    void SignUp(string username, string password);
    int LogIn(string username, string password);
    void ChangePassword(string new_password);
    void LogOut(); //退出登录
};


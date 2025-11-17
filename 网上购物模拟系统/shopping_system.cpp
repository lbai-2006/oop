#include<iostream>
#include<fstream> //文件流
#include<cstdlib>
#include "shopping_system.hpp"
#include<vector>
#include<sstream>
#include<string>

using namespace std;

Administrator::Administrator() {
    this->name = "admin";  //管理员账号：admin 密码：admin123
    this->password = "admin123";
}
int Administrator::LogIn(string name, string password){
    if(name == this->name &&password == this->password){
        cout<<"管理员登录成功！"<<endl;
        return 1;
    }else{
        cout<<"管理员登录失败！"<<endl;
        return 0;
    }
}
void Administrator::LogOut(){
    this->name = "";
    this->password = "";
    cout<<"管理员退出登录成功！"<<endl;
}

Customer::Customer(string username, string password) {
    this->username = username; //初始化为空
    this->password = password;
}
void Customer::ChangePassword(string new_password) {
    // 读取所有用户
    ifstream infile("customers.txt");
    if (!infile.is_open()) {
        cout << "无法打开文件，密码修改失败！" << endl;
        return;
    }
    vector<pair<string, string>> users;
    string line;
    bool found = false;
    while (getline(infile, line)) {
        string user, pass;
        istringstream iss(line);
        iss >> user >> pass;
        if (user == this->username) {
            // 将新密码替换原密码
            users.push_back({user, new_password});
            found = true;
        } else if (!user.empty()) {
            users.push_back({user, pass});
        }
    }
    infile.close();

    if (found) {
        // 以覆盖模式写回更新后的全部用户信息
        ofstream outfile("customers.txt", ios::trunc);
        for (const auto& p : users) {
            outfile << p.first << " " << p.second << endl;
        }
        outfile.close();
        this->password = new_password;
        cout << "密码修改成功！" << endl;
    } else {
        cout << "用户不存在，密码修改失败！" << endl;
    }
}
void Customer::SignUp(string username, string password){
    //保存新用户信息到文件customers.txt
    ofstream outfile("customers.txt", ios::app);
    if (outfile.is_open()) {
        outfile << username << " " << password << endl; //用户名和密码用空格分隔
        outfile.close();
        cout << "注册成功，用户信息已保存！" << endl;
    } else {
        cout << "无法打开文件，注册失败！" << endl;
    }
}
int Customer::LogIn(string username, string password){
    //从文件customers.txt中读取用户信息
    ifstream infile("customers.txt");
    bool found = false;
    if (infile.is_open()) {
        string line;
        while (getline(infile, line)) {
            if (line == username + " " + password) {
                cout << "登录成功！" << endl;
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "用户名或密码错误，登录失败！" << endl;
        }
        infile.close();
    }
    else{
        cout << "无法打开文件，登录失败！" << endl;
    }
    return found ? 1 : 0;
}
void Customer::LogOut(){
    this->username = "";
    this->password = "";
    cout<<"用户退出登录成功！"<<endl;
}

ShoppingSystem::ShoppingSystem(){
    //初始化
}
void ShoppingSystem::Run(){
    //Customer customer("", "");
    int logined = 0; //0未登录，1用户登录，2管理员登录
    //运行，渲染界面
    while(true){
        printf("--------------------------------\n");
        printf("欢迎来到网上购物模拟系统！\n");
        printf("请选择登录方式：\n");
        printf("1. 管理员登录\n");
        printf("2. 用户登录\n");
        printf("3. 注册\n");
        printf("4. 修改密码\n");
        printf("5. 查看商品信息\n");
        printf("6. 购买商品\n");
        printf("7. 退出登录\n");
        printf("8. 退出系统\n");
        int choice;
        cin >> choice;
        switch (choice) {
            case 1: {
                printf("请输入管理员账号：");
                string name;
                cin >> name;
                printf("请输入管理员密码：");
                string password;
                cin >> password;
                Administrator admin;
                logined = admin.LogIn(name, password);
                break;
            }
            case 2: {
                printf("请输入用户名：");
                string username;
                cin >> username;
                printf("请输入用户密码：");
                string password;
                cin >> password;
                Customer customer(username, password);
                logined = customer.LogIn(username, password);
                break;
            }
            case 3: {
                printf("请输入用户名：");
                string username;
                cin >> username;
                printf("请输入用户密码：");
                string password;
                cin >> password;
                Customer customer(username, password);
                customer.SignUp(username, password);
                break;
            }
            case 4: {
                printf("请输入用户名：");
                string username;
                cin >> username;
                printf("请输入新密码：");
                string new_password;
                cin >> new_password;
                Customer customer(username, new_password);
                customer.ChangePassword(new_password);
                break;
            }
            case 5: {
                printf("商品信息：\n");
                printf("1. 商品ID：1001\n");
                printf("2. 商品名称：商品1\n");
                printf("3. 商品价格：100元\n");
                printf("4. 商品库存：100个\n");
                printf("5. 商品描述：商品1的描述\n");
                printf("6. 商品图片：商品1的图片\n");
                printf("7. 商品评价：商品1的评价\n");
                printf("8. 商品销量：商品1的销量\n");
                printf("9. 商品评论：商品1的评论\n");
                printf("10. 商品评价：商品1的评价\n");
                printf("11. 商品销量：商品1的销量\n");
                break;
            }
            case 6: {
                printf("购买商品！\n");
                if(logined<1){
                    printf("请先登录！\n");
                    printf("请输入用户名：");
                    string username;
                    cin >> username;
                    printf("请输入用户密码：");
                    string password;
                    cin >> password;
                    Customer customer(username, password);
                    logined = customer.LogIn(username, password);
                    break;
                }
                if(logined == 1){
                    printf("商品信息：————————————————————————————省略\n");
                    printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
                    printf("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\n");
                    //customer.Buy(product_id);
                }
                break;
            }
            case 7: {
                logined = 0;
                printf("退出登录！\n");
                break;
            }
            case 8: {
                printf("退出系统！\n");
                exit(0);
                break;
            }
            default:
                printf("无效的选择！\n");
                break;
            }
    }
}
void ShoppingSystem::Exit(){
    //退出系统
    printf("退出系统！\n");
    exit(0);
}
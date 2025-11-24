#include<iostream>
#include<fstream> //文件流
#include<cstdlib>
#include "customer.hpp"
#include "shopping_system.hpp"
#include<vector>
#include<sstream>
#include<string>

using namespace std;

//自己创建一个用户，同时添加到数据库中
Customer::Customer(string username, string password) {
    this->username = username; //初始化为空
    this->password = password;
    ShoppingSystem::customers.push_back(*this);
}

void Customer::ChangePassword(string username, string old_password, string new_password) {
    //在数据库中查找用户并验证旧密码
    bool found = false;
    bool password_updated = false;
    for(size_t i = 0; i < ShoppingSystem::customers.size(); i++){
        if(ShoppingSystem::customers.at(i).username == username){
            found = true;
            // 验证旧密码是否正确
            if(ShoppingSystem::customers.at(i).password == old_password){
                // 修改密码（更新所有匹配的用户，防止重复用户问题）
                ShoppingSystem::customers.at(i).password = new_password;
                password_updated = true;
            }
        }
    }
    
    if(!found){
        cout << "用户名不存在，密码修改失败！" << endl;
    } else if(password_updated){
        // 如果当前对象已登录且是同一个用户，也更新当前对象的密码
        if(this->username == username){
            this->password = new_password;
        }
        cout << "密码修改成功！" << endl;
    } else {
        cout << "旧密码错误，密码修改失败！" << endl;
    }
}


void Customer::SignUp(string username, string password){
    //检查用户名是否已存在
    for(size_t i = 0; i < ShoppingSystem::customers.size(); i++){
        if(ShoppingSystem::customers.at(i).username == username){
            cout << "用户名已存在，注册失败！" << endl;
            return;
        }
    }
    //在数据库中添加新用户（构造函数会自动添加到数据库，不需要再次push_back）
    Customer newCustomer(username, password);
    cout << "注册成功，用户信息已保存！" << endl;
}

int Customer::LogIn(string username, string password){
    //动态数组中查找用户名和密码是否匹配
    for(size_t i = 0; i < ShoppingSystem::customers.size(); i++){
        if(ShoppingSystem::customers.at(i).username == username && ShoppingSystem::customers.at(i).password == password){
            cout << "登录成功！" << endl;
            this->username = username;
            this->password = password;
            return 1;
        }
    }
    cout << "用户名或密码错误，登录失败！" << endl;
    return 0;
}

void Customer::LogOut(){
    this->username = "";
    this->password = "";
    cout<<"用户退出登录成功！"<<endl;
}

void Customer::BrowseProducts(){
    //浏览商品，不需要验证身份信息
    cout << "商品列表：" << endl;
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        ShoppingSystem::products.at(i).Display();
        cout << endl;
    }
    cout << "商品列表显示完毕！" << endl;
}

void Customer::PreciseSearchProducts(string product_name){
    cout << "精确搜索商品结果：" << endl;
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        if(ShoppingSystem::products.at(i).product_name == product_name){
            ShoppingSystem::products.at(i).Display();
            cout << endl;
            return;
        }
    }
    cout << "没有找到商品！请使用模糊搜索！" << endl;
}
void Customer::FuzzySearchProducts(string keyword){
    cout << "模糊搜索商品结果：" << endl;
    int has = 0;
    //返回最接近的商品
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        if(ShoppingSystem::products.at(i).product_name.find(keyword) != string::npos){
            ShoppingSystem::products.at(i).Display();
            cout << endl;
            has = 1;
        }else if(ShoppingSystem::products.at(i).product_class.find(keyword) != string::npos){
            ShoppingSystem::products.at(i).Display();
            cout << endl;
            has = 1;
        }else if(ShoppingSystem::products.at(i).product_description.find(keyword) != string::npos){
            ShoppingSystem::products.at(i).Display();
            cout << endl;
            has = 1;
        }
    }
    if(has==0){
        cout << "没有找到商品！" << endl;
    }
}
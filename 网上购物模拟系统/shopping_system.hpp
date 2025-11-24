#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include "product.hpp"
using namespace std;

// 前向声明，避免循环依赖
class Customer;
class Administrator;

class ShoppingSystem{
friend class Administrator; //需要访问数据库
friend class Customer; //需要访问数据库
private:
    static vector<Customer> customers; //用户列表数据库（静态成员）
    static vector<Product> products; //商品列表数据库（静态成员）
public:
    ShoppingSystem();
    void ShowCustomerFile();
    void ShowProductFile();
    void Init();
    //~ShoppingSystem();
    void Run();
    void Exit(); //退出系统
};





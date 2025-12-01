#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include<map>  //查找快
#include "product.hpp"
using namespace std;

// 前向声明，避免循环依赖
class Customer;
class Administrator;
class Cart;

class ShoppingSystem{
    // 友元类：允许这些类访问私有成员
    friend class Administrator;
    friend class Customer;
    friend class Cart;
    
private:
    //声明，没分配内存，静态成员
    static vector<Customer> customers; //用户列表数据库
    static vector<Product> products; //商品列表数据库
    static map<string, vector<pair<Product, int>>> customer_carts; //用户购物车数据库，键为用户名
    
    // 文件路径常量
    static const string CUSTOMERS_FILE;
    static const string PRODUCTS_FILE;
    static const string CUSTOMER_CARTS_FILE;
public:
    ShoppingSystem();
    void ShowCustomerFile();
    void ShowProductFile();
    void Init();
    //~ShoppingSystem();
    void Run();
    void Exit(); //退出系统
    
    // 数据持久化方法
    static void LoadFromFile(); //从文件加载数据
    static void SaveToFile(); //保存数据到文件
    static void SaveCustomersToFile(); //保存用户数据
    static void SaveProductsToFile(); //保存商品数据
    static void LoadCustomersFromFile(); //加载用户数据
    static void LoadProductsFromFile(); //加载商品数据
    static void LoadCustomerCartsFromFile(); //加载用户购物车数据
    static void SaveCustomerCartsToFile(); //保存用户购物车数据
};





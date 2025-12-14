#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include<map>  //映射
#include "product.hpp"
#include "order.hpp"
#include "activity.hpp"
using namespace std;

// 前向声明，避免循环依赖

class Customer;
class Administrator;
class Cart;

class ShoppingSystem{
    // 权限控制
    friend class Administrator;
    friend class Customer;
    friend class Cart;
    
private:
    // 内存数据
    static vector<Customer> customers; //用户列表
    static vector<Product> products; //商品列表
    static map<string, vector<pair<Product, int>>> customer_carts; //用户购物车数据库，键为用户名
    static map<string, Order> orders; //订单数据库，键为订单编号，值为订单对象
    
    // 文件路径常量
    static const string CUSTOMERS_FILE;
    static const string PRODUCTS_FILE;
    static const string CUSTOMER_CARTS_FILE;
    static const string ORDERS_FILE;
    static const string ACTIVITIES_FILE;
public:
    // 活动列表 - 设为public以便其他类访问
    static vector<Activity> activities;
public:
    ShoppingSystem();
    void ShowCustomerFile();
    void ShowProductFile();
    void Init();
    //~ShoppingSystem();
    void Run();
    void Exit(); //退出系统
    
    // 加载和保存数据到文件
    static void LoadFromFile(); //加载数据到内存
    static void SaveToFile(); //保存数据到文件
    static void SaveCustomersToFile(); //保存用户数据到文件
    static void SaveProductsToFile(); //保存商品数据到文件
    static void LoadCustomersFromFile(); //加载用户数据到内存
    static void LoadProductsFromFile(); //加载商品数据到内存
    static void LoadCustomerCartsFromFile(); //加载用户购物车数据到内存
    static void SaveCustomerCartsToFile(); //保存用户购物车数据到文件
    static void LoadOrdersFromFile(); //加载订单数据到内存
    static void SaveOrdersToFile(); //保存订单数据到文件
    static void SaveActivitiesToFile(); //保存活动数据到文件
    static void LoadActivitiesFromFile(); //加载活动数据到内存
    
    // 自动更新订单状态
    static void AutoUpdateOrderStatuses();
    // 自动更新活动状态
    static void AutoUpdateActivityStatuses();

    // 同步商品信息到所有购物车
    static void SyncProductInAllCarts(const Product& product);
};





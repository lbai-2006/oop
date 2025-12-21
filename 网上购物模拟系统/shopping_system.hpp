#pragma once
#ifndef SHOPPING_SYSTEM_HPP
#define SHOPPING_SYSTEM_HPP

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
    friend class FileProcess;
    friend class Product;
    friend class Statistic;
    
private:
    // 内存数据
    static vector<Customer> customers; //用户列表
    static vector<Administrator> administrators; //管理员列表
    static vector<Product> products; //商品列表
    static map<string, vector<pair<Product, int>>> customer_carts; //用户购物车数据库，键为用户名
    static map<string, Order> orders; //订单数据库，键为订单编号，值为订单对象
    
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
    
    
    // 自动更新订单状态
    static void AutoUpdateOrderStatuses();
    // 自动更新活动状态
    static void AutoUpdateActivityStatuses();

    // 同步商品信息到所有购物车
    static void SyncProductInAllCarts(const Product& product);
    //自动检查商品文件是否修改并更新
    static void AutoCheckAndUpdateProducts();
};

#endif
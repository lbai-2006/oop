#pragma once
#ifndef FILE_PROCESS_HPP
#define FILE_PROCESS_HPP

#include<iostream>
#include<vector>
#include<string>

using namespace std;

//都是静态成员，不需要实例，直接用类名约束调用即可
class FileProcess{
    friend class ShoppingSystem;
    friend class Customer;
    friend class Administrator;
    friend class Cart;
    friend class Product;

public:
    FileProcess(){};
    //~FileProcess(){};
    // 文件路径常量
    static const string ADMINISTRATOR_FILE;
    static const string CUSTOMERS_FILE;
    static const string PRODUCTS_FILE;
    static const string CUSTOMER_CARTS_FILE;
    static const string ORDERS_FILE;
    static const string ACTIVITIES_FILE;

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
};

#endif
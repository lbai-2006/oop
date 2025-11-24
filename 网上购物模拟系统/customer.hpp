#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include "product.hpp"
using namespace std;

// 前向声明，避免循环依赖
class ShoppingSystem;

class Customer{
    friend class ShoppingSystem; //需要访问数据库
    private:
        string username;
        string password;
    public:
        Customer(string username, string password, bool add_to_db = true);
        //~Customer();
        void SignUp(string username, string password);
        int LogIn(string username, string password);
        void ChangePassword(string username, string old_password, string new_password);
        void LogOut(); //退出登录
        void BrowseProducts(); //浏览商品，不需要验证身份信息
        void PreciseSearchProducts( string product_name); //精确搜索商品，不需要验证身份信息
        void FuzzySearchProducts( string keyword); //模糊搜索商品，不需要验证身份信息
    };
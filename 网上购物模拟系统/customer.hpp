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
    friend class Administrator; //管理员需要修改购物车中的商品信息
    private:
        string username;
        string password;
        vector<pair<Product, int>> cart; //购物车，使用pair<Product, int>表示商品和数量 
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
        void AddToCart(string product_name); //添加商品到购物车
        void RemoveFromCart(vector<string> product_names); //从购物车中移除商品，product_names为商品名称列表
        void ModifyCart(string product_name, int quantity); //修改购物车中商品的数量
        void ShowCart(); //显示购物车

        void LoadCustomerCartsFromList(); //从列表加载用户购物车数据到用户变量
        void SaveCustomerCartsToList(); //保存用户购物车数据到列表
    };

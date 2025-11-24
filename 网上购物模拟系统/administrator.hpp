#pragma once
#include<iostream>
#include<string>
#include<vector>
#include "product.hpp"
using namespace std;

// 前向声明，避免循环依赖
class ShoppingSystem;

class Administrator{
    private:
        string name;
        string password;
    public:
        Administrator();
        //~Administrator();
        int LogIn(string name, string password);
        void LogOut(); //退出登录
        void AddProduct(Product product); //添加商品
        void EditProduct(Product product); //修改商品信息
        void DeleteProduct(Product product); //删除商品
    };
#pragma once
#include<iostream>
#include<string>
#include<vector>
#include "product.hpp"
#include "order.hpp"
using namespace std;

// 前向声明，避免循环依赖，头文件最好不要include其他头文件
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

        //修改订单状态
        void UpdateOrderStatus(Order order);
    };
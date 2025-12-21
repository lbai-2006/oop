#pragma once
#ifndef ADMINISTRATOR_HPP
#define ADMINISTRATOR_HPP

#include<iostream>
#include<string>
#include<vector>
#include "product.hpp"
#include "order.hpp"
#include "activity.hpp"
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

        // 解耦函数，访问属性
        string GetName () const {return name;}
        string GetPassword () const {return password;}

        int LogIn(string name, string password);
        void LogOut(); //退出登录
        void AddProduct(Product product); //添加商品
        void EditProduct(Product product); //修改商品信息
        void DeleteProduct(Product product); //删除商品

        //修改订单状态
        void UpdateOrderStatus(Order order);
        void AddActivity(); //添加活动
        void EditActivity(); //修改活动信息
        void DeleteActivity(); //删除活动
    };

#endif
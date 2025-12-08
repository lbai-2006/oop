#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include "cart.hpp"
#include "order.hpp"
using namespace std;

// 前向声明，避免循环依赖
class ShoppingSystem;
class Administrator;

class Customer{
    friend class ShoppingSystem; 
    friend class Administrator; 
    
    private:
        string username;
        string password;
        Cart cart; // 购物车，使用Cart类表示
        vector<Order> orders; // 订单，使用Order类表示
    public:
        Customer(string username, string password, bool add_to_db = true);
        //~Customer();
        void SignUp(string username, string password);
        int LogIn(string username, string password);
        void ChangePassword(string username, string old_password, string new_password);
        void LogOut(); // 退出登录
        void BrowseProducts(); // 浏览商品，不需要验证身份信息
        void PreciseSearchProducts(string product_name); // 精确搜索商品，不需要验证身份信息
        void FuzzySearchProducts(string keyword); // 模糊搜索商品，不需要验证身份信息
        
        // 购物车操作方法（用 Cart 类）
        void AddToCart(string product_name) { cart.AddToCart(product_name); }
        void RemoveFromCart(vector<string> product_names) { cart.RemoveFromCart(product_names); }
        void ModifyCart(string product_name, int quantity) { cart.ModifyCart(product_name, quantity); }
        void ShowCart() { cart.ShowCart(); }

        //购买商品，顾客可以从购物车或商品列表进行购买，一次可以选择购买一件或多件商品。生成订单，并保存到订单列表中。
        void BuyProducts();
        //签收订单
        void ConfirmOrderReceipt(string order_id);
        // 订单操作方法（用 Order 类）
        void AddOrder(Order order);
        void ShowOrders();
};
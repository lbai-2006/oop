#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<utility>

using namespace std;
class Product;

class Cart {
    friend class Customer;
    friend class ShoppingSystem;
    friend class Administrator;
    
    private:
        vector<pair<Product, int>> items;  // 使用 Product 对象而不是指针
        string username;  // 添加 username 成员，用于保存数据
        
    public:
        Cart();  // 添加构造函数
        
        void SetUsername(string username);  // 设置用户名
        void ClearCart();  // 清空购物车
        
        void AddToCart(string product_name); // 添加商品到购物车
        void RemoveFromCart(vector<string> product_names); // 从购物车中移除商品
        void ModifyCart(string product_name, int quantity); // 修改购物车中商品的数量
        void ShowCart(); // 显示购物车
        
        void LoadCustomerCartsFromList(); // 从列表加载用户购物车数据
        void SaveCustomerCartsToList(); // 保存用户购物车数据到列表
        
        size_t GetSize() const { return items.size(); }  // 获取购物车大小
};

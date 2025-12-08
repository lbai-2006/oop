#pragma once
#include<iostream>

using namespace std;

class Product{
    friend class Administrator; //管理员需要修改商品信息
    private:
        string product_class;
        string product_name;
        double product_price;
        int product_stock;
        string product_description;
    public:
        Product(string product_class, string product_name, double product_price, int product_stock, string product_description);
        //~Product();
        void Display();
        
        // Getter 方法：提供只读访问，保证数据安全
        string GetProductClass() const { return product_class; }
        string GetProductName() const { return product_name; }
        double GetProductPrice() const { return product_price; }
        int GetProductStock() const { return product_stock; }
        string GetProductDescription() const { return product_description; }
        //修改商品库存
        void ModifyProductStock(int quantity);
    }; 
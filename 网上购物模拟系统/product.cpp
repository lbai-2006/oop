#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include "product.hpp"

using namespace std;

Product::Product(string product_class, string product_name, double product_price, int product_stock, string product_description){
    this->product_class = product_class;
    this->product_name = product_name; //商品名称唯一
    this->product_price = product_price;
    this->product_stock = product_stock;
    this->product_description = product_description;
}
void Product::Display(){
    cout << "商品类别：" << this->product_class << endl;
    cout << "商品名称：" << this->product_name << endl;
    cout << "商品价格：" << this->product_price << endl;
    cout << "商品库存：" << this->product_stock << endl;
    cout << "商品描述：" << this->product_description << endl;
}
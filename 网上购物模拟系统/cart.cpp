#include "cart.hpp"
#include "shopping_system.hpp"
#include "product.hpp"
#include<iostream>
#include<vector>
#include<string>
#include<utility>
using namespace std;

Cart::Cart() {
    this->username = "";
    this->items.clear();
}

void Cart::SetUsername(string username) {
    this->username = username;
}

void Cart::ClearCart() {
    this->items.clear();
}

void Cart::AddToCart(string product_name){
    // 根据商品名称查找商品（在 ShoppingSystem::products 中）
    Product* product_ptr = nullptr;
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        if(ShoppingSystem::products.at(i).GetProductName() == product_name){
            product_ptr = &ShoppingSystem::products.at(i);  // 指向静态数组中的商品
            break;
        }
    }
    
    if(product_ptr == nullptr){
        printf("没有找到商品！\n");
        return;
    }
    
    // 检查购物车中是否已存在该商品
    for(size_t i = 0; i < this->items.size(); i++){
        if(this->items.at(i).first.GetProductName() == product_ptr->GetProductName()){
            printf("购物车中已存在该商品，是否继续添加？(y/n)\n");
            char choice;
            cin >> choice;
            if(choice == 'y'){
                //如果商品库存不足，则提示库存不足
                if(this->items.at(i).first.GetProductStock() <= 0){
                    printf("商品库存不足，无法继续添加！\n");
                }
                else{
                    this->items.at(i).second += 1;
                    printf("购物车中已存在该商品，数量增加1！\n");
                }
                // 同步保存到内存和文件
                SaveCustomerCartsToList();
                return;
            }else{
                printf("取消添加！\n");
                return;
            }
        }
    }
    
    // 购物车中不存在该商品，则添加到购物车（复制商品对象）
    this->items.push_back(make_pair(*product_ptr, 1));
    printf("添加商品到购物车成功！\n");
    
    // 同步保存到内存和文件
    SaveCustomerCartsToList();
}

void Cart::RemoveFromCart(vector<string> product_names){
    // 检查购物车中是否存在该商品
    for(size_t i = 0; i < product_names.size(); i++){
        bool found = false;
        for(size_t j = 0; j < this->items.size(); j++){
            if(product_names.at(i) == this->items.at(j).first.GetProductName()){
                this->items.erase(this->items.begin() + j);
                found = true;
                printf("从购物车中移除商品%s成功！\n", product_names.at(i).c_str());
                break;
            }
        }
        if(!found){
            printf("购物车中不存在商品%s，移除失败！\n", product_names.at(i).c_str());
        }
    }
    
    // 同步保存到内存和文件
    SaveCustomerCartsToList();
} 

void Cart::ModifyCart(string product_name, int quantity){
    // 检查购物车中是否存在该商品
    bool found = false;
    for(size_t j = 0; j < this->items.size(); j++){
        if(this->items.at(j).first.GetProductName() == product_name){
            if(quantity <= 0){
                // 数量为0或负数时，从购物车中删除该商品
                this->items.erase(this->items.begin() + j);
                printf("商品%s数量为0，已从购物车中移除！\n", product_name.c_str());
            } else {
                //如果商品库存不足，则提示库存不足
                if(this->items.at(j).first.GetProductStock() <= quantity){
                    printf("商品库存不足，无法继续修改！\n");
                }
                else{
                    this->items.at(j).second = quantity;
                    printf("修改购物车中商品%s数量为%d成功！\n", product_name.c_str(), quantity);
                }
            }
            found = true;
            break;
        }
    }
    if(!found){
        printf("购物车中不存在商品%s，修改失败！\n", product_name.c_str());
    }
    
    // 同步保存到内存和文件
    SaveCustomerCartsToList();
}

void Cart::ShowCart(){
    // 显示购物车内各商品信息，总数及总价格
    printf("========== 购物车 ==========\n");
    int total_quantity = 0;
    double total_price = 0;
    for(size_t i = 0; i < this->items.size(); i++){
        this->items.at(i).first.Display();
        printf("数量：%d  单价：%.2f  总价：%.2f\n", 
               this->items.at(i).second, 
               this->items.at(i).first.GetProductPrice(), 
               this->items.at(i).first.GetProductPrice() * this->items.at(i).second);
        total_quantity += this->items.at(i).second;
        total_price += this->items.at(i).first.GetProductPrice() * this->items.at(i).second;
    }
    printf("总数：%d  总价格：%.2f\n", total_quantity, total_price);
    printf("购物车显示完成！\n");
}

// 从ShoppingSystem::customer_carts内存中加载该用户的购物车数据到用户变量
void Cart::LoadCustomerCartsFromList(){
    if(this->username.empty()){
        printf("用户未登录，无法加载购物车数据！\n");
        return;
    }
    
    // 清空当前购物车
    this->items.clear();
    
    // 从ShoppingSystem::customer_carts中查找该用户的购物车
    if(ShoppingSystem::customer_carts.find(this->username) != ShoppingSystem::customer_carts.end()){
        vector<pair<Product, int>>& saved_cart = ShoppingSystem::customer_carts[this->username];
        
        // 遍历购物车，同步最新的商品信息
        for(size_t i = 0; i < saved_cart.size(); i++){
            string product_name = saved_cart[i].first.GetProductName();  // 使用 . 而非 ->
            int quantity = saved_cart[i].second;
            
            // 在商品列表中查找最新的商品信息
            bool product_exists = false;
            for(size_t j = 0; j < ShoppingSystem::products.size(); j++){
                if(ShoppingSystem::products[j].GetProductName() == product_name){
                    // 使用最新的商品信息，保持原有数量
                    this->items.push_back(make_pair(ShoppingSystem::products[j], quantity));
                    product_exists = true;
                    break;
                }
            }
            
            // 如果商品已被删除，跳过（不添加到购物车）
            if(!product_exists){
                printf("商品 %s 已下架，已从购物车中移除。\n", product_name.c_str());
            }
        }
        
        // 更新 customer_carts 中的数据（移除已下架的商品）
        ShoppingSystem::customer_carts[this->username] = this->items;
        
        printf("已从内存中加载用户 %s 的购物车数据，共 %zu 项。\n", this->username.c_str(), this->items.size());
    } else {
        printf("用户 %s 的购物车为空。\n", this->username.c_str());
    }
}

// 保存该用户的购物车数据到ShoppingSystem::customer_carts内存和文件
void Cart::SaveCustomerCartsToList(){
    if(this->username.empty()){
        printf("用户未登录，无法保存购物车数据！\n");
        return;
    }
    
    // 保存到内存ShoppingSystem::customer_carts
    ShoppingSystem::customer_carts[this->username] = this->items;
    
    // 保存到文件
    ShoppingSystem::SaveCustomerCartsToFile();
    
    printf("已保存用户 %s 的购物车数据到内存和文件。\n", this->username.c_str());
}

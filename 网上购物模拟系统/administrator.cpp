#include<iostream>
#include<fstream> 
#include<cstdlib>
#include "administrator.hpp"
#include "shopping_system.hpp"
#include "customer.hpp"
#include<vector>
#include<sstream>
#include<string>
#include<map>

using namespace std;

Administrator::Administrator() {
    this->name = "admin";  //管理员账号：admin 密码：admin123
    this->password = "admin123";
}
int Administrator::LogIn(string name, string password){
    if(name == this->name &&password == this->password){
        printf("管理员登录成功！\n");
        return 2;
    }else{
        printf("管理员账号或密码错误，登录失败！\n");
        return 0;
    }
}
void Administrator::LogOut(){
    this->name = "";
    this->password = "";
    printf("管理员退出登录成功！\n");
}

void Administrator::AddProduct(Product product){
    ShoppingSystem::products.push_back(product);
    printf("商品添加成功！\n");
    
    // 保存到文件
    ShoppingSystem::SaveProductsToFile();
}

void Administrator::EditProduct(Product product){
    bool found = false;
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        if(ShoppingSystem::products.at(i).product_name == product.product_name){
            ShoppingSystem::products.at(i) = product;
            found = true;
            break;
        }
    }
    if(found){
        printf("商品修改成功！\n");
        
        // 同步更新 ShoppingSystem::customer_carts 中的该商品信息
        int updated_carts = 0;
        for(auto& user_cart : ShoppingSystem::customer_carts){
            vector<pair<Product, int>>& cart = user_cart.second;
            for(size_t j = 0; j < cart.size(); j++){
                // 如果购物车中有同名商品，更新商品信息（保持数量不变）
                if(cart.at(j).first.product_name == product.product_name){
                    int quantity = cart.at(j).second; // 保存数量
                    cart.at(j).first = product; // 更新商品信息
                    cart.at(j).second = quantity; // 恢复数量
                    updated_carts++;
                }
            }
        }
        if(updated_carts > 0){
            printf("已同步更新 %d 个用户购物车中的商品信息！\n", updated_carts);
        }
        
        // 保存购物车数据到文件
        ShoppingSystem::SaveCustomerCartsToFile();
        // 保存商品数据到文件
        ShoppingSystem::SaveProductsToFile();
    } else {
        printf("商品不存在，修改失败！\n");
    }
}

void Administrator::DeleteProduct(Product product){
    int found = 0;
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        if(ShoppingSystem::products.at(i).product_name == product.product_name){
            ShoppingSystem::products.erase(ShoppingSystem::products.begin() + i);
            found = 1;
            break;
        }
    }
    if(found == 0){
        printf("商品不存在！\n"); 
    }else{
        printf("商品删除成功！\n");
        
        // 同步删除所有用户购物车中的该商品
        int removed_carts = 0;
        for(auto& user_cart : ShoppingSystem::customer_carts){
            vector<pair<Product, int>>& cart = user_cart.second;
            for(size_t j = 0; j < cart.size(); ){
                if(cart.at(j).first.product_name == product.product_name){
                    cart.erase(cart.begin() + j);
                    removed_carts++;
                    // 不增加j，因为删除后后面的元素会前移
                } else {
                    j++;
                }
            }
        }
        if(removed_carts > 0){
            printf("已从 %d 个用户购物车中移除该商品！\n", removed_carts);
        }
        
        // 保存购物车数据到文件
        ShoppingSystem::SaveCustomerCartsToFile();
        // 保存商品数据到文件
        ShoppingSystem::SaveProductsToFile();
    }
}
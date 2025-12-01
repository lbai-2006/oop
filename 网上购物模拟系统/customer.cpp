#include<iostream>
#include<fstream> //文件流
#include<cstdlib>
#include "customer.hpp"
#include "cart.hpp"
#include "shopping_system.hpp"
#include<vector>
#include<sstream>
#include<string>

using namespace std;

//添加一个用户add_to_db是否自动添加到数据库
Customer::Customer(string username, string password, bool add_to_db) {
    this->username = username;
    this->password = password;
    if(add_to_db) {
        ShoppingSystem::customers.push_back(*this);
    }
    //初始化购物车
    this->cart.ClearCart();
    this->cart.SetUsername(username);
}

void Customer::ChangePassword(string username, string old_password, string new_password) {
    //数据库中查找用户验证是否存在
    bool found = false;
    bool password_updated = false;
    for(size_t i = 0; i < ShoppingSystem::customers.size(); i++){
        if(ShoppingSystem::customers.at(i).username == username){
            found = true;
            // 验证密码是否正确
            if(ShoppingSystem::customers.at(i).password == old_password){
                // 修改密码（匹配用户名，防止重复用户）
                ShoppingSystem::customers.at(i).password = new_password;
                password_updated = true;
            }
        }
    }
    
    if(!found){
        printf("用户不存在，修改失败！\n");
    } else if(password_updated){
        // 当前登录用户和修改用户是同一个用户，也修改当前用户的密码
        if(this->username == username){
            this->password = new_password;
        }
        printf("密码修改成功！\n");
        
        // 保存到文件
        ShoppingSystem::SaveCustomersToFile();
    } else {
        printf("旧密码错误，修改失败！\n");
    }
}


void Customer::SignUp(string username, string password){
        //检查用户是否存在
    for(size_t i = 0; i < ShoppingSystem::customers.size(); i++){
        if(ShoppingSystem::customers.at(i).username == username){
            printf("用户名已存在，注册失败！\n");
            return;
        }
    }
    //数据库中添加用户，需要手动push_back
    Customer newCustomer(username, password, true);
    printf("注册成功！用户信息已保存。\n");
    
    // 保存到文件
    ShoppingSystem::SaveCustomersToFile();
}

int Customer::LogIn(string username, string password){
    //动态查找用户验证是否存在
    for(size_t i = 0; i < ShoppingSystem::customers.size(); i++){
        if(ShoppingSystem::customers.at(i).username == username && ShoppingSystem::customers.at(i).password == password){
            printf("登录成功！\n");
            this->username = username;
            this->password = password;
            
            // 设置用户名为购物车的用户名
            this->cart.SetUsername(username);
            // 从内存ShoppingSystem::customer_carts中加载该用户的购物车数据
            this->cart.LoadCustomerCartsFromList();
            
            return 1;
        }
    }
    printf("用户名或密码错误，登录失败！\n");
    return 0;
}

void Customer::LogOut(){
    // 退出登录前保存购物车数据
    if(!this->username.empty()){
        this->cart.SaveCustomerCartsToList();
    }
    
    this->username = "";
    this->password = "";
    this->cart.ClearCart(); // 清空本地购物车
    printf("用户退出登录成功！\n");
}

void Customer::BrowseProducts(){
    //浏览商品需要验证身份信息
    printf("========== 商品列表 ==========\n");
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        ShoppingSystem::products.at(i).Display();
        printf("\n");
    }
    printf("商品列表显示完成！\n");
}

void Customer::PreciseSearchProducts(string product_name){
    printf("========== 精确搜索结果 ==========\n");
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        if(ShoppingSystem::products.at(i).GetProductName() == product_name){
            ShoppingSystem::products.at(i).Display();
            printf("\n");
            return;
        }
    }
    printf("没有找到商品！请使用模糊搜索。\n");
}

void Customer::FuzzySearchProducts(string keyword){
    printf("========== 模糊搜索结果 ==========\n");
    int has = 0;
    //模糊搜索商品
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        // 使用getter方法获取商品信息
        if(ShoppingSystem::products.at(i).GetProductName().find(keyword) != string::npos){
            ShoppingSystem::products.at(i).Display();
            printf("\n");
            has = 1;
        }else if(ShoppingSystem::products.at(i).GetProductClass().find(keyword) != string::npos){
            ShoppingSystem::products.at(i).Display();
            printf("\n");
            has = 1;
        }else if(ShoppingSystem::products.at(i).GetProductDescription().find(keyword) != string::npos){
            ShoppingSystem::products.at(i).Display();
            printf("\n");
            has = 1;
        }
    }
    if(has==0){
        printf("没有找到商品！\n");
    }
}

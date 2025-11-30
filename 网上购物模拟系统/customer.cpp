#include<iostream>
#include<fstream> //文件流
#include<cstdlib>
#include "customer.hpp"
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
    //初始化购物车，清空
    this->cart.clear();
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
            
            // 从内存ShoppingSystem::customer_carts中加载该用户的购物车数据
            LoadCustomerCartsFromList();
            
            return 1;
        }
    }
    printf("用户名或密码错误，登录失败！\n");
    return 0;
}

void Customer::LogOut(){
    // 退出登录前保存购物车数据
    if(!this->username.empty()){
        SaveCustomerCartsToList();
    }
    
    this->username = "";
    this->password = "";
    this->cart.clear(); // 清空本地购物车
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

void Customer::AddToCart(string product_name){
    //根据商品名称查找商品
    Product product("", "", 0.0, 0, ""); // 初始化一个空的Product对象
    bool product_found = false;
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        if(ShoppingSystem::products.at(i).GetProductName() == product_name){
            product = ShoppingSystem::products.at(i);
            product_found = true;
            break;
        }
    }
    if(!product_found){
        printf("没有找到商品！\n");
        return;
    }
    //检查购物车中是否已存在该商品
    for(size_t i = 0; i < this->cart.size(); i++){
        if(this->cart.at(i).first.GetProductName() == product.GetProductName()){
            printf("购物车中已存在该商品，是否继续添加？(y/n)\n");
            char choice;
            cin >> choice;
            if(choice == 'y'){
                this->cart.at(i).second += 1;
                printf("购物车中已存在该商品，数量增加1！\n");
                // 同步保存到内存和文件
                SaveCustomerCartsToList();
                return;
            }else{
                printf("取消添加！\n");
                return;
            }
        }
    }
    //购物车中不存在该商品，则添加到购物车
    this->cart.push_back(make_pair(product, 1));
    printf("添加商品到购物车成功！\n");
    
    // 同步保存到内存和文件
    SaveCustomerCartsToList();
}

void Customer::RemoveFromCart(vector<string> product_names){
    //检查购物车中是否存在该商品
    for(size_t i = 0; i < product_names.size(); i++){
        bool found = false;
        for(size_t j = 0; j < this->cart.size(); j++){
            if(product_names.at(i) == this->cart.at(j).first.GetProductName()){
                this->cart.erase(this->cart.begin() + j);
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

void Customer::ModifyCart(string product_name, int quantity){
    //检查购物车中是否存在该商品
    bool found = false;
    for(size_t j = 0; j < this->cart.size(); j++){
        if(this->cart.at(j).first.GetProductName() == product_name){
            if(quantity <= 0){
                // 数量为0或负数时，从购物车中删除该商品
                this->cart.erase(this->cart.begin() + j);
                printf("商品%s数量为0，已从购物车中移除！\n", product_name.c_str());
            } else {
                this->cart.at(j).second = quantity;
                printf("修改购物车中商品%s数量为%d成功！\n", product_name.c_str(), quantity);
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

void Customer::ShowCart(){
    //显示购物车内各商品信息，总数及总价格
    printf("========== 购物车 ==========\n");
    int total_quantity = 0;
    double total_price = 0;
    for(size_t i = 0; i < this->cart.size(); i++){
        this->cart.at(i).first.Display();
        printf("数量：%d  单价：%.2f  总价：%.2f\n", this->cart.at(i).second, this->cart.at(i).first.GetProductPrice(), this->cart.at(i).first.GetProductPrice() * this->cart.at(i).second);
        total_quantity += this->cart.at(i).second;
        total_price += this->cart.at(i).first.GetProductPrice() * this->cart.at(i).second;
    }
    printf("总数：%d  总价格：%.2f\n", total_quantity, total_price);
    printf("购物车显示完成！\n");
}

// 从ShoppingSystem::customer_carts内存中加载该用户的购物车数据到用户变量
void Customer::LoadCustomerCartsFromList(){
    if(this->username.empty()){
        printf("用户未登录，无法加载购物车数据！\n");
        return;
    }
    
    // 清空当前购物车
    this->cart.clear();
    
    // 从ShoppingSystem::customer_carts中查找该用户的购物车
    if(ShoppingSystem::customer_carts.find(this->username) != ShoppingSystem::customer_carts.end()){
        vector<pair<Product, int>>& saved_cart = ShoppingSystem::customer_carts[this->username];
        
        // 遍历购物车，同步最新的商品信息
        for(size_t i = 0; i < saved_cart.size(); i++){
            string product_name = saved_cart[i].first.GetProductName();
            int quantity = saved_cart[i].second;
            
            // 在商品列表中查找最新的商品信息
            bool product_exists = false;
            for(size_t j = 0; j < ShoppingSystem::products.size(); j++){
                if(ShoppingSystem::products[j].GetProductName() == product_name){
                    // 使用最新的商品信息，保持原有数量
                    this->cart.push_back(make_pair(ShoppingSystem::products[j], quantity));
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
        ShoppingSystem::customer_carts[this->username] = this->cart;
        
        printf("已从内存中加载用户 %s 的购物车数据，共 %zu 项。\n", this->username.c_str(), this->cart.size());
    } else {
        printf("用户 %s 的购物车为空。\n", this->username.c_str());
    }
}

// 保存该用户的购物车数据到ShoppingSystem::customer_carts内存和文件
void Customer::SaveCustomerCartsToList(){
    if(this->username.empty()){
        printf("用户未登录，无法保存购物车数据！\n");
        return;
    }
    
    // 保存到内存ShoppingSystem::customer_carts
    ShoppingSystem::customer_carts[this->username] = this->cart;
    
    // 保存到文件
    ShoppingSystem::SaveCustomerCartsToFile();
    
    printf("已保存用户 %s 的购物车数据到内存和文件。\n", this->username.c_str());
}
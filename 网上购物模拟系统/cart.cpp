#include "cart.hpp"
#include "shopping_system.hpp"
#include "product.hpp"
#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include<ctime>
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
    // 
    Product* product_ptr = nullptr;
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        if(ShoppingSystem::products.at(i).GetProductName() == product_name){
            product_ptr = &ShoppingSystem::products.at(i);  // 找到商品对象指针
            break;
        }
    }
    
    if(product_ptr == nullptr){
        printf("商品不存在，添加失败！\n");
        return;
    }
    
    // 检查购物车中是否已存在该商品
    for(size_t i = 0; i < this->items.size(); i++){
        if(this->items.at(i).first.GetProductName() == product_ptr->GetProductName()){
            printf("购物车中已存在该商品，是否增加数量？(y/n)\n");
            char choice;
            cin >> choice;
            if(choice == 'y'){
                //如果商品库存小于0，则提示库存不足
                if(this->items.at(i).first.GetProductStock() <= 0){
                    printf("库存不足，增加失败！\n");
                }
                else{
                    this->items.at(i).second += 1;
                    printf("增加成功！\n");
                }
                // 保存到文件
                SaveCustomerCartsToList();
                return;
            }else{
                printf("取消增加！\n");
                return;
            }
        }
    }

    this->items.push_back(make_pair(*product_ptr, 1));
    printf("添加成功！\n");
    
    // 保存到文件
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
                printf("删除成功！\n");
                break;
            }
        }
        if(!found){
            printf("购物车中不存在该商品，删除失败！\n");
        }
    }
    
    // 保存到文件
    SaveCustomerCartsToList();
} 

void Cart::ModifyCart(string product_name, int quantity){
    // 检查购物车中是否存在该商品
    bool found = false;
    for(size_t j = 0; j < this->items.size(); j++){
        if(this->items.at(j).first.GetProductName() == product_name){
            if(quantity <= 0){
                // 如果数量小于0，则删除该商品
                this->items.erase(this->items.begin() + j);
                printf("商品%s数量小于0，删除成功！\n", product_name.c_str());
            } else {
                // 如果数量大于0，则修改该商品数量
                if(this->items.at(j).first.GetProductStock() <= quantity){
                    printf("库存不足，修改失败！\n");
                }
                else{
                    this->items.at(j).second = quantity;
                    printf("修改成功！\n");
                }
            }
            found = true;
            break;
        }
    }
    if(!found){
        printf("购物车中不存在该商品，修改失败！\n");
    }
    
    // 保存到文件
    SaveCustomerCartsToList();
}

void Cart::ShowCart(){
    // 显示购物车内容
    printf("========== 购物车内容 ==========\n");
    int total_quantity = 0;
    double total_price_original = 0; // 原价总和
    double total_price_after_discount = 0; // 折扣后总和
    vector<pair<Product, int>> full_reduction_items; // 满减商品列表
    
    time_t now = time(nullptr);
    
    for(size_t i = 0; i < this->items.size(); i++){
        const Product& product = this->items.at(i).first;
        int quantity = this->items.at(i).second;
        double original_price = product.GetProductPrice();
        double item_total_original = original_price * quantity;
        double item_total_after_discount = item_total_original;
        
        this->items.at(i).first.Display();
        printf("商品数量：%d  商品原价：%.2f  商品小计：%.2f", 
               quantity, original_price, item_total_original);
        
        // 检查是否有折扣活动
        bool has_discount = false;
        double discount_rate = 1.0;
        for(size_t j = 0; j < ShoppingSystem::activities.size(); j++){
            Activity& activity = ShoppingSystem::activities.at(j);
            // 检查是否有折扣活动
            if(activity.activity_status == "进行中" && 
               activity.activity_start_time <= now && 
               activity.activity_end_time >= now &&
               activity.activity_type_discount){
                // 检查是否有该商品的折扣活动
                for(size_t k = 0; k < activity.activity_discount_products.size(); k++){
                    if(activity.activity_discount_products.at(k).first.GetProductName() == product.GetProductName()){
                        has_discount = true;
                        discount_rate = activity.activity_discount_products.at(k).second;
                        item_total_after_discount = item_total_original * discount_rate;
                        printf(" [折扣率：%.0f%%  折扣后价格：%.2f]", discount_rate * 100, item_total_after_discount);
                        break;
                    }
                }
                if(has_discount) break;
            }
        }
        
        // 检查是否有满减活动
        bool has_full_reduction = false;
        for(size_t j = 0; j < ShoppingSystem::activities.size(); j++){
            Activity& activity = ShoppingSystem::activities.at(j);
            // 检查是否有满减活动
            if(activity.activity_status == "进行中" && 
               activity.activity_start_time <= now && 
               activity.activity_end_time >= now &&
               activity.activity_type_full_reduction){
                // 检查是否有该商品的满减活动
                for(size_t k = 0; k < activity.activity_full_reduction_products.size(); k++){
                    if(activity.activity_full_reduction_products.at(k).GetProductName() == product.GetProductName()){
                        has_full_reduction = true;
                        full_reduction_items.push_back(make_pair(product, quantity));
                        printf(" [满减阈值：%.2f  满减金额：%.2f]", activity.activity_threshold, activity.activity_full_reduction_amount);
                        break;
                    }
                }
                if(has_full_reduction) break;
            }
        }
        
        printf("\n");
        
        total_quantity += quantity;
        total_price_original += item_total_original;
        total_price_after_discount += item_total_after_discount;
    }
    
    // 计算满减总和
    double full_reduction_total = 0;
    if(!full_reduction_items.empty()){
        // 计算满减商品总和
        double full_reduction_subtotal = 0;
        for(size_t i = 0; i < full_reduction_items.size(); i++){
            const Product& product = full_reduction_items.at(i).first;
            int quantity = full_reduction_items.at(i).second;
            double price = product.GetProductPrice();
            
            // 检查是否有折扣活动
            double discount_rate = 1.0;
            for(size_t j = 0; j < ShoppingSystem::activities.size(); j++){
                Activity& activity = ShoppingSystem::activities.at(j);
                if(activity.activity_status == "进行中" && 
                   activity.activity_start_time <= now && 
                   activity.activity_end_time >= now &&
                   activity.activity_type_discount){
                    for(size_t k = 0; k < activity.activity_discount_products.size(); k++){
                        if(activity.activity_discount_products.at(k).first.GetProductName() == product.GetProductName()){
                            discount_rate = activity.activity_discount_products.at(k).second;
                            break;
                        }
                    }
                }
            }
            
            full_reduction_subtotal += price * quantity * discount_rate;
        }
        
        // 检查是否有满减活动
        for(size_t j = 0; j < ShoppingSystem::activities.size(); j++){
            Activity& activity = ShoppingSystem::activities.at(j);
            if(activity.activity_status == "进行中" && 
               activity.activity_start_time <= now && 
               activity.activity_end_time >= now &&
               activity.activity_type_full_reduction){
                if(full_reduction_subtotal >= activity.activity_threshold){
                    full_reduction_total = activity.activity_full_reduction_amount;
                    printf("\n满减阈值：%.2f  满减金额：%.2f", activity.activity_threshold, full_reduction_total);
                    break;
                }
            }
        }
    }
    
    double final_total = total_price_after_discount - full_reduction_total;
    if(final_total < 0) final_total = 0;
    
    printf("----------------------------------------\n");
    printf("商品数量：%d\n", total_quantity);
    if(total_price_original != total_price_after_discount){
        printf("原价总和：%.2f\n", total_price_original);
        printf("折扣后总和：%.2f\n", total_price_after_discount);
    }
    if(full_reduction_total > 0){
        printf("满减总和：-%.2f\n", full_reduction_total);
    }
    printf("结算金额：%.2f\n", final_total);
    printf("结算成功！\n");
}

// 从购物车列表中加载购物车内容
void Cart::LoadCustomerCartsFromList(){
    if(this->username.empty()){
        printf("用户名不存在，加载失败！\n");
        return;
    }
    
    // 清空购物车
    this->items.clear();
    
    // 从购物车列表中加载购物车内容
    if(ShoppingSystem::customer_carts.find(this->username) != ShoppingSystem::customer_carts.end()){
        vector<pair<Product, int>>& saved_cart = ShoppingSystem::customer_carts[this->username];
        
        // 添加商品到购物车
        for(size_t i = 0; i < saved_cart.size(); i++){
            string product_name = saved_cart[i].first.GetProductName();  // 商品名称
            int quantity = saved_cart[i].second;
            
            // 检查商品是否存在
            bool product_exists = false;
            for(size_t j = 0; j < ShoppingSystem::products.size(); j++){
                if(ShoppingSystem::products[j].GetProductName() == product_name){
                    // 添加商品到购物车
                    this->items.push_back(make_pair(ShoppingSystem::products[j], quantity));
                    product_exists = true;
                    break;
                }
            }
            
            // 商品
            if(!product_exists){
                printf("商品 %s 不存在，加载失败！\n", product_name.c_str());
            }
        }
        
        // 保存购物车内容到购物车列表
        ShoppingSystem::customer_carts[this->username] = this->items;
        
        printf("购物车加载成功！用户：%s  商品数量：%zu\n", this->username.c_str(), this->items.size());
    } else {
        printf("用户 %s 不存在，加载失败！\n", this->username.c_str());
    }
}

// 保存购物车内容到购物车列表
void Cart::SaveCustomerCartsToList(){
    if(this->username.empty()){
        printf("用户名不存在，保存失败！\n");
        return;
    }
    
    // 保存购物车内容到购物车列表
    ShoppingSystem::customer_carts[this->username] = this->items;
    
    // 保存购物车内容到文件
    ShoppingSystem::SaveCustomerCartsToFile();
    
    printf("购物车保存成功！用户：%s  商品数量：%zu\n", this->username.c_str(), this->items.size());
}

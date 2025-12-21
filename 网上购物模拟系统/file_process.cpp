#include<iostream>
#include<fstream> 
#include<cstdlib>
#include "shopping_system.hpp"
#include "administrator.hpp"
#include "customer.hpp"
#include "product.hpp"
#include "file_process.hpp"
#include<vector>
#include<sstream>
#include<string>
#include<limits>

using namespace std;

// 定义文件路径常量
const string FileProcess::ADMINISTRATOR_FILE = "administrator.txt";
const string FileProcess::CUSTOMERS_FILE = "customers.txt";
const string FileProcess::PRODUCTS_FILE = "products.txt";
const string FileProcess::CUSTOMER_CARTS_FILE = "customer_carts.txt";
const string FileProcess::ORDERS_FILE = "orders.txt";
const string FileProcess::ACTIVITIES_FILE = "activities.txt";

// 保存用户数据到文件
void FileProcess::SaveCustomersToFile(){
    ofstream outfile(CUSTOMERS_FILE);
    if(!outfile.is_open()){
        printf("无法打开用户数据文件进行写入！\n");
        return;
    }
    
    for(size_t i = 0; i < ShoppingSystem::customers.size(); i++){
        // 格式：用户名|密码
        outfile << ShoppingSystem::customers[i].GetUsername() << "|" << ShoppingSystem::customers[i].GetPassword() << endl;
    }
    
    outfile.close();
    printf("用户数据已保存到 %s\n", CUSTOMERS_FILE.c_str());
}


// 保存商品数据到文件（使用getter方法，更安全）
void FileProcess::SaveProductsToFile(){
    ofstream outfile(PRODUCTS_FILE);
    if(!outfile.is_open()){
        printf("无法打开商品数据文件进行写入！\n");
        return;
    }
    
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        // 格式：类别|名称|价格|库存|描述
        outfile << ShoppingSystem::products[i].GetProductClass() << "|" 
                << ShoppingSystem::products[i].GetProductName() << "|" 
                << ShoppingSystem::products[i].GetProductPrice() << "|" 
                << ShoppingSystem::products[i].GetProductStock() << "|" 
                << ShoppingSystem::products[i].GetProductDescription() << endl;
    }
    
    outfile.close();
    printf("商品数据已保存到 %s\n", PRODUCTS_FILE.c_str());
}

// 从文件加载用户数据
void FileProcess::LoadCustomersFromFile(){
    ifstream infile(CUSTOMERS_FILE);
    if(!infile.is_open()){
        printf("用户数据文件不存在，将创建新文件。\n");
        return;
    }
    
    ShoppingSystem::customers.clear(); // 清空现有数据
    string line;
    while(getline(infile, line)){
        if(line.empty()) continue;
        
        // 解析格式：用户名|密码
        size_t pos = line.find('|');
        if(pos != string::npos){
            string username = line.substr(0, pos);
            string password = line.substr(pos + 1);
            
            // 创建用户对象，但不自动添加到数据库（add_to_db=false）
            ShoppingSystem::customers.push_back(Customer(username, password, false));
        }
    }
    
    infile.close();
    printf("已从 %s 加载 %zu 个用户。\n", CUSTOMERS_FILE.c_str(), ShoppingSystem::customers.size());
}

// 从文件加载商品数据
void FileProcess::LoadProductsFromFile(){
    ifstream infile(PRODUCTS_FILE);
    if(!infile.is_open()){
        printf("商品数据文件不存在，将创建新文件。\n");
        return;
    }
    
    ShoppingSystem::products.clear(); // 清空现有数据
    string line;
    while(getline(infile, line)){
        if(line.empty()) continue;
        
        // 解析格式：类别|名称|价格|库存|描述
        stringstream ss(line);
        string product_class, product_name, product_description;
        double product_price;
        int product_stock;
        
        getline(ss, product_class, '|');
        getline(ss, product_name, '|');
        ss >> product_price;
        ss.ignore(); // 忽略 '|'
        ss >> product_stock;
        ss.ignore(); // 忽略 '|'
        getline(ss, product_description);
        
        ShoppingSystem::products.push_back(Product(product_class, product_name, product_price, product_stock, product_description));
    }
    
    infile.close();
    printf("已从 %s 加载 %zu 个商品。\n", PRODUCTS_FILE.c_str(), ShoppingSystem::products.size());
}

// 从文件加载用户购物车数据到内存
void FileProcess::LoadCustomerCartsFromFile(){
    ifstream infile(CUSTOMER_CARTS_FILE);
    if(!infile.is_open()){
        printf("用户购物车数据文件不存在，将创建新文件。\n");
        return;
    }
    
    ShoppingSystem::customer_carts.clear(); // 清空现有数据
    string line;
    int total_items = 0;
    while(getline(infile, line)){
        if(line.empty()) continue;
        
        // 解析格式：用户名|商品类别|商品名称|商品价格|商品库存|商品描述|数量
        stringstream ss(line);
        string username, product_class, product_name, product_description;
        double product_price;
        int product_stock, quantity;
        
        getline(ss, username, '|');
        getline(ss, product_class, '|');
        getline(ss, product_name, '|');
        ss >> product_price;
        ss.ignore(); // 忽略 '|'
        ss >> product_stock;
        ss.ignore(); // 忽略 '|'
        getline(ss, product_description, '|');
        ss >> quantity;
        
        // 创建商品对象
        Product product(product_class, product_name, product_price, product_stock, product_description);
        
        // 添加到对应用户的购物车
        ShoppingSystem::customer_carts[username].push_back(make_pair(product, quantity));
        total_items++;
    }
    
    infile.close();
    printf("已从 %s 加载 %d 个用户购物车项。\n", CUSTOMER_CARTS_FILE.c_str(), total_items);
}

// 保存用户购物车数据到文件
void FileProcess::SaveCustomerCartsToFile(){
    ofstream outfile(CUSTOMER_CARTS_FILE);
    if(!outfile.is_open()){
        printf("无法打开用户购物车数据文件进行写入！\n");
        return;
    }
    
    int total_items = 0;
    // 遍历所有用户的购物车
    for(auto& user_cart : ShoppingSystem::customer_carts){
        string username = user_cart.first;
        vector<pair<Product, int>>& cart = user_cart.second;
        
        // 遍历该用户购物车中的所有商品
        for(size_t i = 0; i < cart.size(); i++){
            const Product& product = cart[i].first;
            int quantity = cart[i].second;
            
            // 格式：用户名|商品类别|商品名称|商品价格|商品库存|商品描述|数量
            outfile << username << "|"
                    << product.GetProductClass() << "|"
                    << product.GetProductName() << "|"
                    << product.GetProductPrice() << "|"
                    << product.GetProductStock() << "|"
                    << product.GetProductDescription() << "|"
                    << quantity << endl;
            total_items++;
        }
    }
    
    outfile.close();
    printf("用户购物车数据已保存到 %s，共 %d 项。\n", CUSTOMER_CARTS_FILE.c_str(), total_items);
}

// 统一保存接口
void FileProcess::SaveToFile(){
    SaveCustomersToFile();
    SaveProductsToFile();
    SaveCustomerCartsToFile();
    SaveOrdersToFile();
    SaveActivitiesToFile();
}

// 统一加载接口
void FileProcess::LoadFromFile(){
    LoadCustomersFromFile();
    LoadProductsFromFile();
    LoadCustomerCartsFromFile();
    LoadOrdersFromFile();
    LoadActivitiesFromFile();
}

// 从文件加载订单数据到内存
void FileProcess::LoadOrdersFromFile(){
    ifstream infile(ORDERS_FILE);
    if(!infile.is_open()){
        printf("订单数据文件不存在，将创建新文件。\n");
        return;
    }
    ShoppingSystem::orders.clear(); // 清空现有数据
    string line;
    while(getline(infile, line)){
        if(line.empty()) continue;
        // 新格式：订单编号|用户名|购买时间|订单总额|收货地址|订单状态|商品列表|时间戳|自动更新标志
        // 兼容旧格式（没有商品列表 / 自动更新标志）
        stringstream ss(line);
        string id, username, purchase_time, total_amount_str, delivery_address, status;
        getline(ss, id, '|');
        getline(ss, username, '|');
        getline(ss, purchase_time, '|');
        getline(ss, total_amount_str, '|');
        getline(ss, delivery_address, '|');
        getline(ss, status, '|');

        // 读取剩余部分（可能是：timestamp，timestamp|auto，items|timestamp|auto）
        string rest;
        string items_str, ts_str, auto_str;
        if(getline(ss, rest)) {
            // 按 '|' 拆分剩余字段
            vector<string> parts;
            string tmp;
            stringstream ss_rest(rest);
            while(getline(ss_rest, tmp, '|')){
                parts.push_back(tmp);
            }
            if(parts.size() == 1){
                // 只有时间戳
                ts_str = parts[0];
            }else if(parts.size() == 2){
                // 时间戳 + 自动更新标志
                ts_str = parts[0];
                auto_str = parts[1];
            }else if(parts.size() >= 3){
                // 商品列表 + 时间戳 + 自动更新标志
                items_str = parts[0];
                ts_str = parts[1];
                auto_str = parts[2];
            }
        }

        double total_amount = atof(total_amount_str.c_str());
        long long ts = 0;
        if(!ts_str.empty()){
            ts = atoll(ts_str.c_str());
        }
        bool auto_update = true;
        if(!auto_str.empty()){
            auto_update = (auto_str != "0");
        }

        // 还原所购商品信息：items_str 形如 "name1,2;name2,1"
        vector<pair<Product,int>> items;
        if(!items_str.empty()){
            stringstream ss_items(items_str);
            string item_token;
            while(getline(ss_items, item_token, ';')){
                if(item_token.empty()) continue;
                size_t comma_pos = item_token.find(',');
                if(comma_pos == string::npos) continue;
                string pname = item_token.substr(0, comma_pos);
                string qstr = item_token.substr(comma_pos + 1);
                int qty = atoi(qstr.c_str());
                if(qty <= 0) continue;

                // 在当前商品列表中查找同名商品，尽量使用最新信息
                bool found = false;
                for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
                    if(ShoppingSystem::products[i].GetProductName() == pname){
                        items.push_back(make_pair(ShoppingSystem::products[i], qty));
                        found = true;
                        break;
                    }
                }
                if(!found){
                    // 如果商品已下架，构造一个只包含名称的占位商品
                    Product placeholder("unknown", pname, 0.0, 0, "已下架商品");
                    items.push_back(make_pair(placeholder, qty));
                }
            }
        }

        ShoppingSystem::orders[id] = Order(id, username, items, purchase_time, total_amount, delivery_address, status, ts, auto_update);
    }
    infile.close();
    printf("已从 %s 加载 %zu 个订单。\n", ORDERS_FILE.c_str(), ShoppingSystem::orders.size());
}

// 保存订单数据到文件（仅保存汇总信息）
void FileProcess::SaveOrdersToFile(){
    ofstream outfile(ORDERS_FILE);
    if(!outfile.is_open()){
        printf("无法打开订单数据文件进行写入！\n");
        return;
    }
    for(const auto& kv : ShoppingSystem::orders){
        const Order& order = kv.second;
        // 将所购商品列表序列化为：name1,qty1;name2,qty2
        string items_str;
        for(size_t i = 0; i < order.items.size(); i++){
            const Product& p = order.items[i].first;
            int qty = order.items[i].second;
            if(i > 0) items_str += ";";
            items_str += p.GetProductName() + "," + to_string(qty);
        }

        // 格式：订单编号|用户名|购买时间|订单总额|收货地址|订单状态|商品列表|时间戳|自动更新标志
        outfile << order.id << "|"
                << order.username << "|"
                << order.purchase_time << "|"
                << order.total_amount << "|"
                << order.delivery_address << "|"
                << order.status << "|"
                << items_str << "|"
                << order.create_timestamp << "|"
                << (order.auto_update ? 1 : 0) << endl;
    }
    outfile.close();
    printf("订单数据已保存到 %s\n", ORDERS_FILE.c_str());
}

void FileProcess::SaveActivitiesToFile(){
    ofstream outfile(ACTIVITIES_FILE);
    if(!outfile.is_open()){
        printf("无法打开活动数据文件进行写入！\n");
        return;
    }
    for(size_t i = 0; i < ShoppingSystem::activities.size(); i++){
        outfile << ShoppingSystem::activities.at(i).activity_name << "|"
                << ShoppingSystem::activities.at(i).activity_type_discount << "|"
                << ShoppingSystem::activities.at(i).activity_type_full_reduction << "|"
                << ShoppingSystem::activities.at(i).activity_start_time << "|"
                << ShoppingSystem::activities.at(i).activity_end_time << "|"   
                << ShoppingSystem::activities.at(i).activity_discount << "|"
                << ShoppingSystem::activities.at(i).activity_threshold << "|"
                << ShoppingSystem::activities.at(i).activity_full_reduction_amount << "|"
                << ShoppingSystem::activities.at(i).activity_status << "|"
                << ShoppingSystem::activities.at(i).activity_discount_products.size() << "|";
        for(size_t j = 0; j < ShoppingSystem::activities.at(i).activity_discount_products.size(); j++){
            outfile << ShoppingSystem::activities.at(i).activity_discount_products.at(j).first.GetProductName() << "," << ShoppingSystem::activities.at(i).activity_discount_products.at(j).second << ";";
        }
        outfile << "|" << ShoppingSystem::activities.at(i).activity_full_reduction_products.size() << "|";
        for(size_t j = 0; j < ShoppingSystem::activities.at(i).activity_full_reduction_products.size(); j++){
            outfile << ShoppingSystem::activities.at(i).activity_full_reduction_products.at(j).GetProductName() << ";";
        }
        outfile << endl;
    }
    outfile.close();
    printf("活动数据已保存到 %s\n", ACTIVITIES_FILE.c_str());
}

//加载活动数据到内存ShoppingSystem::activities
void FileProcess::  LoadActivitiesFromFile(){
    ifstream infile(ACTIVITIES_FILE);
    if(!infile.is_open()){
        printf("活动数据文件不存在，将创建新文件。\n");
        return;
    }
    ShoppingSystem::activities.clear();
    string line;
    while(getline(infile, line)){
        if(line.empty()) continue;
        stringstream ss(line);
        
        // 读取前9个字段
        string activity_name_str, activity_type_discount_str, activity_type_full_reduction_str, 
               activity_start_time_str, activity_end_time_str, activity_discount_str, 
               activity_threshold_str, activity_full_reduction_amount_str, activity_status;
        
        getline(ss, activity_name_str, '|');
        getline(ss, activity_type_discount_str, '|');
        getline(ss, activity_type_full_reduction_str, '|');
        getline(ss, activity_start_time_str, '|');
        getline(ss, activity_end_time_str, '|');
        getline(ss, activity_discount_str, '|');
        getline(ss, activity_threshold_str, '|');
        getline(ss, activity_full_reduction_amount_str, '|');
        getline(ss, activity_status, '|');
        
        bool activity_type_discount = (activity_type_discount_str == "1");
        bool activity_type_full_reduction = (activity_type_full_reduction_str == "1");
        long long activity_start_time = stoll(activity_start_time_str);
        long long activity_end_time = stoll(activity_end_time_str);
        double activity_discount = stod(activity_discount_str);
        double activity_threshold = stod(activity_threshold_str);
        double activity_full_reduction_amount = stod(activity_full_reduction_amount_str);
        
        // 读取折扣商品数量和列表
        string discount_count_str;
        getline(ss, discount_count_str, '|');
        
        string discount_products_str;
        getline(ss, discount_products_str, '|');
        
        vector<pair<Product, double>> discount_products;
        if(!discount_products_str.empty()){
            stringstream ss_discount(discount_products_str);
            string product_token;
            while(getline(ss_discount, product_token, ';')){
                if(product_token.empty()) continue;
                size_t comma_pos = product_token.find(',');
                if(comma_pos == string::npos) continue;
                string pname = product_token.substr(0, comma_pos);
                string rate_str = product_token.substr(comma_pos + 1);
                double rate = stod(rate_str);
                
                for(size_t k = 0; k < ShoppingSystem::products.size(); k++){
                    if(ShoppingSystem::products.at(k).GetProductName() == pname){
                        discount_products.push_back(make_pair(ShoppingSystem::products.at(k), rate));
                        break;
                    }
                }
            }
        }
        
        // 读取满减商品数量和列表
        string full_reduction_count_str;
        getline(ss, full_reduction_count_str, '|');
        
        string full_reduction_products_str;
        getline(ss, full_reduction_products_str);
        
        vector<Product> full_reduction_products;
        if(!full_reduction_products_str.empty()){
            stringstream ss_full_reduction(full_reduction_products_str);
            string product_name;
            while(getline(ss_full_reduction, product_name, ';')){
                if(product_name.empty()) continue;
                for(size_t k = 0; k < ShoppingSystem::products.size(); k++){
                    if(ShoppingSystem::products.at(k).GetProductName() == product_name){
                        full_reduction_products.push_back(ShoppingSystem::products.at(k));
                        break;
                    }
                }
            }
        }
        
        ShoppingSystem::activities.push_back(Activity(activity_name_str, activity_type_discount, activity_type_full_reduction, activity_start_time, activity_end_time, activity_discount, activity_threshold, activity_full_reduction_amount, activity_status, discount_products, full_reduction_products));
    }
    infile.close();
    printf("已从 %s 加载 %zu 个活动。\n", ACTIVITIES_FILE.c_str(), ShoppingSystem::activities.size());
}
#include<iostream>
#include<fstream> //文件流
#include<cstdlib>
#include<ctime>
#include "customer.hpp"
#include "cart.hpp"
#include "shopping_system.hpp"
#include<vector>
#include<sstream>
#include<string>
#include<limits>

using namespace std;

//构造函数，add_to_db为true时，将客户对象添加到内存中
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
    //检查用户是否存在
    bool found = false;
    bool password_updated = false;
    for(size_t i = 0; i < ShoppingSystem::customers.size(); i++){
        if(ShoppingSystem::customers.at(i).username == username){
            found = true;
            // 检查密码是否正确
            if(ShoppingSystem::customers.at(i).password == old_password){
                // 修改密码
                ShoppingSystem::customers.at(i).password = new_password;
                password_updated = true;
            }
        }
    }
    
    if(!found){
        printf("用户不存在，密码修改失败！\n");
    } else if(password_updated){
        // 如果用户是当前登录用户，则修改密码
        if(this->username == username){
            this->password = new_password;
        }
        printf("密码修改成功！\n");
        
        // 保存到文件
        ShoppingSystem::SaveCustomersToFile();
    } else {
        printf("密码不正确，密码修改失败！\n");
    }
}


void Customer::SignUp(string username, string password){
    //检查用户是否存在
    for(size_t i = 0; i < ShoppingSystem::customers.size(); i++){
        if(ShoppingSystem::customers.at(i).username == username){
            printf("用户已存在，注册失败！\n");
            return;
        }
    }
    //添加到内存
    Customer newCustomer(username, password, true);
    printf("注册成功！\n");
    
    // 保存到文件
    ShoppingSystem::SaveCustomersToFile();
}

int Customer::LogIn(string username, string password){
    //检查用户是否存在
    for(size_t i = 0; i < ShoppingSystem::customers.size(); i++){
        if(ShoppingSystem::customers.at(i).username == username && ShoppingSystem::customers.at(i).password == password){
            printf("登录成功！\n");
            this->username = username;
            this->password = password;
            
            // 设置购物车用户名
            this->cart.SetUsername(username);
            // 从购物车列表中加载购物车内容
            this->cart.LoadCustomerCartsFromList();
            
            return 1;
        }
    }
    printf("用户名或密码错误，登录失败！\n");
    return 0;
}

void Customer::LogOut(){
    // 退出登录前保存购物车内容
    if(!this->username.empty()){
        this->cart.SaveCustomerCartsToList();
    }
    
    this->username = "";
    this->password = "";
    this->cart.ClearCart(); // 清空购物车
    printf("退出登录成功！\n");
}

void Customer::BrowseProducts(){
    //显示商品列表
    printf("========== 商品列表 ==========\n");
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        ShoppingSystem::products.at(i).Display();
        printf("\n");
    }
    printf("商品列表显示完毕！\n");
}

void Customer::PreciseSearchProducts(string product_name){
    printf("========== 精确搜索商品信息 ==========\n");
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        if(ShoppingSystem::products.at(i).GetProductName() == product_name){
            ShoppingSystem::products.at(i).Display();
            printf("\n");
            return;
        }
    }
    printf("没有找到该商品，精确搜索失败！\n");
}

void Customer::FuzzySearchProducts(string keyword){
    printf("========== 模糊搜索商品信息 ==========\n");
    int has = 0;
    //模糊搜索商品
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        // 检查商品名称、类别、描述是否包含关键词
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
        printf("没有找到该商品，模糊搜索失败！\n");
    }
}

// 获取当前时间
static string GetCurrentTime(){
    time_t now = time(nullptr);
    tm *lt = localtime(&now);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt);
    return string(buf);
}

// 计算促销价
static void CalculatePromotionalPrice(const vector<pair<Product, int>>& items, 
                                      double& total_before_discount, 
                                      double& total_after_discount, 
                                      double& full_reduction_amount){
    total_before_discount = 0;
    total_after_discount = 0;
    full_reduction_amount = 0;
    
    time_t now = time(nullptr);
    vector<pair<Product, int>> full_reduction_items;
    
    // 计算每个商品的促销价
    for(size_t i = 0; i < items.size(); i++){
        const Product& product = items.at(i).first;
        int quantity = items.at(i).second;
        double original_price = product.GetProductPrice();
        double item_total = original_price * quantity;
        double discounted_total = item_total;
        
        total_before_discount += item_total;
        
        // 检查是否有折扣活动
        bool has_discount = false;
        for(size_t j = 0; j < ShoppingSystem::activities.size(); j++){
            Activity& activity = ShoppingSystem::activities.at(j);
            if(activity.activity_status == "进行中" && 
               activity.activity_start_time <= now && 
               activity.activity_end_time >= now &&
               activity.activity_type_discount){
                for(size_t k = 0; k < activity.activity_discount_products.size(); k++){
                    if(activity.activity_discount_products.at(k).first.GetProductName() == product.GetProductName()){
                        double discount_rate = activity.activity_discount_products.at(k).second;
                        discounted_total = item_total * discount_rate;
                        has_discount = true;
                        printf("商品 %s 参与折扣活动，折扣率：%.0f%%，原价：%.2f -> 促销价：%.2f\n", 
                               product.GetProductName().c_str(), 
                               discount_rate * 100, 
                               item_total, 
                               discounted_total);
                        break;
                    }
                }
                if(has_discount) break;
            }
        }
        
        total_after_discount += discounted_total;
        
        // 检查是否有满减活动
        for(size_t j = 0; j < ShoppingSystem::activities.size(); j++){
            Activity& activity = ShoppingSystem::activities.at(j);
            if(activity.activity_status == "进行中" && 
               activity.activity_start_time <= now && 
               activity.activity_end_time >= now &&
               activity.activity_type_full_reduction){
                for(size_t k = 0; k < activity.activity_full_reduction_products.size(); k++){
                    if(activity.activity_full_reduction_products.at(k).GetProductName() == product.GetProductName()){
                        full_reduction_items.push_back(items.at(i));
                        break;
                    }
                }
            }
        }
    }
    
    // 计算满减总和
    if(!full_reduction_items.empty()){
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
                    full_reduction_amount = activity.activity_full_reduction_amount;
                    printf("满减阈值：%.2f 元，满减金额：%.2f 元\n", 
                           activity.activity_threshold, 
                           full_reduction_amount);
                    break;
                }
            }
        }
    }
}

// 购买商品
void Customer::BuyProducts(){
    if(this->username.empty()){
        printf("请先登录！\n");
        return;
    }
    
    printf("请选择购买方式：\n");
    printf("1. 从购物车购买\n");
    printf("2. 从商品列表购买\n");
    int choice;
    cin >> choice;
    
    vector<pair<Product,int>> order_items;
    double total_amount = 0.0;
    
    if(choice == 1){
        // 显示购物车内容
        this->cart.ShowCart();
        printf("请输入要购买的商品数量：");
        int kind_count;
        cin >> kind_count;
        
        for(int k = 0; k < kind_count; k++){
            printf("请输入商品名称：");
            string product_name;
            cin >> product_name;
            printf("请输入购买数量：");
            int buy_quantity;
            cin >> buy_quantity;
            
            if(buy_quantity <= 0){
                printf("购买数量不能小于0！\n");
                continue;
            }
            
            // 检查购物车中是否存在该商品
            bool in_cart = false;
            for(size_t i = 0; i < this->cart.items.size(); i++){
                if(this->cart.items.at(i).first.GetProductName() == product_name){
                    int cart_quantity = this->cart.items.at(i).second;
                    if(buy_quantity > cart_quantity){
                        printf("购物车中商品%s数量不足，当前数量为%d！\n",
                               product_name.c_str(), cart_quantity);
                        break;
                    }
                    
                    // 检查商品库存是否足够
                    bool stock_ok = false;
                    for(size_t j = 0; j < ShoppingSystem::products.size(); j++){
                        if(ShoppingSystem::products.at(j).GetProductName() == product_name){
                            int stock = ShoppingSystem::products.at(j).GetProductStock();
                            if(buy_quantity > stock){
                                printf("商品%s库存不足，当前库存为%d！\n",
                                       product_name.c_str(), stock);
                                stock_ok = false;
                            }else{
                                // 修改商品库存
                                ShoppingSystem::products.at(j).ModifyProductStock(stock - buy_quantity);
                                // 记录订单商品信息
                                order_items.push_back(make_pair(ShoppingSystem::products.at(j), buy_quantity));
                                stock_ok = true;
                            }
                            break;
                        }
                    }
                    
                    if(stock_ok){
                        // 用最新的商品信息覆盖购物车中的商品（索引j在上面的循环中定义）
                        // 需要在外层循环找到对应的商品索引
                        for(size_t m = 0; m < ShoppingSystem::products.size(); m++){
                            if(ShoppingSystem::products.at(m).GetProductName() == product_name){
                                this->cart.items.at(i).first = ShoppingSystem::products.at(m);
                                break;
                            }
                        }
                        
                        // 更新购物车商品数量
                        this->cart.items.at(i).second -= buy_quantity;
                        if(this->cart.items.at(i).second <= 0){
                            this->cart.items.erase(this->cart.items.begin() + i);
                        }
                    }
                    
                    in_cart = true;
                    break;
                }
            }
            
            if(!in_cart){
                printf("购物车中不存在该商品%s！\n", product_name.c_str());
            }
        }
        
        // 保存购物车信息
        this->cart.SaveCustomerCartsToList();
        // 保存商品信息
        ShoppingSystem::SaveProductsToFile();
        // 保存购物车信息
        ShoppingSystem::SaveCustomerCartsToFile();
    }else if(choice == 2){
        // 显示商品列表
        this->BrowseProducts();
        printf("请输入要购买的商品数量：");
        int kind_count;
        cin >> kind_count;
        
        for(int k = 0; k < kind_count; k++){
            printf("请输入商品名称：");
            string product_name;
            cin >> product_name;
            printf("请输入购买数量：");
            int buy_quantity;
            if(!(cin >> buy_quantity)){
                printf("购买数量不能小于0！\n");
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if(buy_quantity <= 0){
                printf("购买数量不能小于0！\n");
                continue;
            }
            
            bool found = false;
            for(size_t j = 0; j < ShoppingSystem::products.size(); j++){
                if(ShoppingSystem::products.at(j).GetProductName() == product_name){
                    found = true;
                    int stock = ShoppingSystem::products.at(j).GetProductStock();
                    if(buy_quantity > stock){
                        printf("商品%s库存不足，当前库存为%d！\n",
                               product_name.c_str(), stock);
                    }else{
                        // 修改商品库存
                        ShoppingSystem::products.at(j).ModifyProductStock(stock - buy_quantity);
                        // 记录订单商品信息
                        order_items.push_back(make_pair(ShoppingSystem::products.at(j), buy_quantity));
                    }
                    break;
                }
            }
            if(!found){
                printf("未找到该商品%s！\n", product_name.c_str());
            }
        }
        
        // 保存商品信息
        ShoppingSystem::SaveProductsToFile();
        // 保存购物车信息
        ShoppingSystem::SaveCustomerCartsToFile();
    }else{
        printf("输入错误，购买失败！\n");
        return;
    }
    
    if(order_items.empty()){
        printf("未选择有效商品，购买失败！\n");
        return;
    }
    
    // 显示订单细信息
    printf("\n========== 订单细信息 ==========\n");
    double total_before_discount = 0;
    double total_after_discount = 0;
    double full_reduction_amount = 0;
    
    // 计算促销价
    CalculatePromotionalPrice(order_items, total_before_discount, total_after_discount, full_reduction_amount);
    
    // 计算订单总金额
    total_amount = total_after_discount - full_reduction_amount;
    if(total_amount < 0) total_amount = 0;
    
    printf("\n原价：%.2f\n", total_before_discount);
    if(total_before_discount != total_after_discount){
        printf("促销价：%.2f\n", total_after_discount);
    }
    if(full_reduction_amount > 0){
        printf("满减金额：%.2f\n", full_reduction_amount);
    }
    printf("订单总金额：%.2f\n", total_amount);
    printf("================================\n\n");
    
    printf("请输入收货地址：");
    string address;
    cin >> address;
    
    string purchase_time = GetCurrentTime();
    // 生成订单编号
    time_t now = time(nullptr);
    string order_id = this->username + "_" + to_string(ShoppingSystem::orders.size() + 1) + "_" + to_string(now);
    
    // 创建订单对象
    Order order(order_id, this->username, order_items, purchase_time, total_amount, address, "待发货");
    ShoppingSystem::orders[order_id] = order;
    // 保存订单信息
    ShoppingSystem::SaveOrdersToFile();
    
    printf("订单创建成功！订单编号：%s，订单总金额：%.2f\n", order_id.c_str(), total_amount);
    // 显示订单详情
    ShoppingSystem::orders[order_id].Display();
}

void Customer::AddOrder(Order order){
    ShoppingSystem::orders[order.id] = order;
    printf("订单添加成功！\n");
}

void Customer::ShowOrders(){
    printf("========== 订单列表 ==========\n");
    for(const auto& kv : ShoppingSystem::orders){
        const Order& order = kv.second;
        if(order.username == this->username){
            order.Display();
            printf("\n");
        }
    }
    printf("订单列表显示完毕！\n");
}

void Customer::ConfirmOrderReceipt(string order_id){
    auto it = ShoppingSystem::orders.find(order_id);
    if(it == ShoppingSystem::orders.end()){
        printf("订单不存在，确认收货失败！\n");
        return;
    }
    Order& order = it->second;
    if(order.username != this->username){
        printf("无权限确认收货，确认收货失败！\n");
        return;
    }
    if(order.status != "待收货"){
        printf("订单状态不为待收货，确认收货失败！\n");
        return;
    }
    order.status = "已收货";
    printf("订单%s确认收货成功！\n", order_id.c_str());
    
    // 保存订单信息
    ShoppingSystem::SaveOrdersToFile();
}

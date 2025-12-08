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

// 获取当前时间的简单工具函数
static string GetCurrentTime(){
    time_t now = time(nullptr);
    tm *lt = localtime(&now);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt);
    return string(buf);
}

// 购买商品：从购物车或商品列表中购买，生成订单并同步库存 / 购物车 / 订单文件
void Customer::BuyProducts(){
    if(this->username.empty()){
        printf("请先登录再购买商品！\n");
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
        // 从购物车购买
        this->cart.ShowCart();
        printf("请输入要购买的商品种类数量：");
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
                printf("购买数量必须大于0！\n");
                continue;
            }
            
            // 在购物车中查找该商品
            bool in_cart = false;
            for(size_t i = 0; i < this->cart.items.size(); i++){
                if(this->cart.items.at(i).first.GetProductName() == product_name){
                    int cart_quantity = this->cart.items.at(i).second;
                    if(buy_quantity > cart_quantity){
                        printf("购物车中商品%s数量不足，最多可购买%d件！\n",
                               product_name.c_str(), cart_quantity);
                        break;
                    }
                    
                    // 在商品列表中查找最新信息并检查库存
                    bool stock_ok = false;
                    for(size_t j = 0; j < ShoppingSystem::products.size(); j++){
                        if(ShoppingSystem::products.at(j).GetProductName() == product_name){
                            int stock = ShoppingSystem::products.at(j).GetProductStock();
                            if(buy_quantity > stock){
                                printf("商品%s库存不足，当前库存为%d！\n",
                                       product_name.c_str(), stock);
                                stock_ok = false;
                            }else{
                                // 扣减库存
                                ShoppingSystem::products.at(j).ModifyProductStock(stock - buy_quantity);
                                // 记录到订单条目（使用最新的商品信息）
                                order_items.push_back(make_pair(ShoppingSystem::products.at(j), buy_quantity));
                                total_amount += ShoppingSystem::products.at(j).GetProductPrice() * buy_quantity;
                                stock_ok = true;
                            }
                            break;
                        }
                    }
                    
                    if(stock_ok){
                        // 扣减购物车数量 / 移除，并且把库存也扣减
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
                printf("购物车中不存在商品%s！\n", product_name.c_str());
            }
        }
        
        // 同步购物车到内存和文件
        this->cart.SaveCustomerCartsToList();
        // 同步商品库存到文件
        ShoppingSystem::SaveProductsToFile();
        // 保存购物车数据到文件
        ShoppingSystem::SaveCustomerCartsToFile();
    }else if(choice == 2){
        // 直接从商品列表购买
        this->BrowseProducts();
        printf("请输入要购买的商品种类数量：");
        int kind_count;
        cin >> kind_count;
        
        for(int k = 0; k < kind_count; k++){
            printf("请输入商品名称：");
            string product_name;
            cin >> product_name;
            printf("请输入购买数量：");
            int buy_quantity;
            if(!(cin >> buy_quantity)){
                printf("数量输入非法，本次商品跳过。\n");
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if(buy_quantity <= 0){
                printf("购买数量必须大于0！\n");
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
                        // 扣减库存
                        ShoppingSystem::products.at(j).ModifyProductStock(stock - buy_quantity);
                        // 记录到订单条目
                        order_items.push_back(make_pair(ShoppingSystem::products.at(j), buy_quantity));
                        total_amount += ShoppingSystem::products.at(j).GetProductPrice() * buy_quantity;
                    }
                    break;
                }
            }
            if(!found){
                printf("未找到商品%s！\n", product_name.c_str());
            }
        }
        
        // 同步商品库存到文件
        ShoppingSystem::SaveProductsToFile();
        // 保存购物车数据到文件
        ShoppingSystem::SaveCustomerCartsToFile();
    }else{
        printf("输入错误，请重新输入！\n");
        return;
    }
    
    if(order_items.empty()){
        printf("未选择任何有效商品，本次未生成订单。\n");
        return;
    }
    
    printf("请输入收货地址：");
    string address;
    cin >> address;
    
    string purchase_time = GetCurrentTime();
    // 简单生成订单编号：用户名 + 当前已有订单数 + 时间戳
    time_t now = time(nullptr);
    string order_id = this->username + "_" + to_string(ShoppingSystem::orders.size() + 1) + "_" + to_string(now);
    
    // 创建订单并加入系统内存（默认自动更新状态）
    Order order(order_id, this->username, order_items, purchase_time, total_amount, address, "待发货");
    ShoppingSystem::orders[order_id] = order;
    // 保存订单到文件（AutoUpdateOrderStatuses 会在主循环中统一调用）
    ShoppingSystem::SaveOrdersToFile();
    
    printf("订单生成成功！订单编号：%s，总金额：%.2f\n", order_id.c_str(), total_amount);
    //打印当前订单编号的订单信息
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
    printf("订单显示完成！\n");
}

void Customer::ConfirmOrderReceipt(string order_id){
    auto it = ShoppingSystem::orders.find(order_id);
    if(it == ShoppingSystem::orders.end()){
        printf("订单不存在！\n");
        return;
    }
    Order& order = it->second;
    if(order.username != this->username){
        printf("您无权操作该订单！\n");
        return;
    }
    if(order.status != "已发货"){
        printf("订单当前状态为%s，无法签收！\n", order.status.c_str());
        return;
    }
    order.status = "已签收";
    printf("订单%s签收成功！\n", order_id.c_str());
    
    // 保存订单状态到文件
    ShoppingSystem::SaveOrdersToFile();
}

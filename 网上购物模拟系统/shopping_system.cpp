#include <sys/stat.h>  // 用于stat结构体和stat()函数
#include <time.h>      // 用于time_t类型
#include <cstdio>      // 用于printf()
#include<iostream>
#include<fstream> 
#include<cstdlib>
#include "shopping_system.hpp"
#include "administrator.hpp"
#include "customer.hpp"
#include "product.hpp"
#include "file_process.hpp"
#include "statistic.hpp"
#include<vector>
#include<sstream>
#include<string>
#include<limits>

using namespace std;

// 内存
// 定义静态成员（全局唯一性）--所有ShoppingSystem对象（如果有两个system类变量的话）共享一个数据库
vector<Customer> ShoppingSystem::customers; //后续使用getter方法访问，更安全
vector<Administrator> ShoppingSystem::administrators; //管理员列表
vector<Product> ShoppingSystem::products; //后续使用getter方法访问，更安全
map<string, vector<pair<Product, int>>> ShoppingSystem::customer_carts; //用户购物车数据库，键为用户名
map<string, Order> ShoppingSystem::orders; //订单数据库，键为订单编号，值为订单对象
vector<Activity> ShoppingSystem::activities; //活动列表数据库

void ShoppingSystem::Init(){
    // 先尝试从文件加载数据
    FileProcess::LoadFromFile();
    
    // 如果文件不存在或为空，则初始化默认数据
    if(ShoppingSystem::customers.empty()){
        printf("未找到用户数据文件，初始化默认用户...\n");
        ShoppingSystem::customers.push_back(Customer("admin", "admin123"));
    }
    
    if(ShoppingSystem::products.empty()){
        printf("未找到商品数据文件，初始化默认商品...\n");
        ShoppingSystem::products.push_back(Product("food", "cake", 20, 100, "delicious"));
        ShoppingSystem::products.push_back(Product("Books", "C++Primer", 128, 50, "A classic beginner's guide to C++."));
        ShoppingSystem::products.push_back(Product("Electronics", "iPhone14", 5999, 30, "The latest smartphone from Apple."));
        
        // 保存初始数据到文件
        FileProcess::SaveToFile();
    }
}

ShoppingSystem::ShoppingSystem(){
    //初始化静态成员
    ShoppingSystem::Init();
}

void ShoppingSystem::ShowCustomerFile(){
    //显示用户列表
    for(size_t i = 0; i < ShoppingSystem::customers.size(); i++){
        printf("用户名：%s  密码：%s\n", 
               ShoppingSystem::customers.at(i).username.c_str(), 
               ShoppingSystem::customers.at(i).password.c_str());
    }
    printf("用户列表显示完毕！\n");
}

void ShoppingSystem::ShowProductFile(){
    //显示商品列表（使用getter方法，更安全）
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        printf("商品类别：%s  商品名称：%s  商品价格：%.2f  商品库存：%d  商品描述：%s\n",
               ShoppingSystem::products.at(i).GetProductClass().c_str(),
               ShoppingSystem::products.at(i).GetProductName().c_str(),
               ShoppingSystem::products.at(i).GetProductPrice(),
               ShoppingSystem::products.at(i).GetProductStock(),
               ShoppingSystem::products.at(i).GetProductDescription().c_str());
    }
    printf("商品列表显示完毕！\n");
}

void ShoppingSystem::Run(){
    int logined = 0; //0未登录，1用户登录，2管理员登录
    Administrator admin;
    Customer customer("", "");
    //运行，渲染界面
    while(true){
        // 每次循环自动检查一次订单状态（根据下单时间自动流转）
        ShoppingSystem::AutoUpdateOrderStatuses();
        // 每次循环自动检查一次活动状态（根据活动结束时间自动流转）
        ShoppingSystem::AutoUpdateActivityStatuses();
        // 每次循环自动检查商品文件是否修改并更新
        ShoppingSystem::AutoCheckAndUpdateProducts();
        printf("================================\n");
        printf("欢迎来到网上购物模拟系统！\n");
        printf("==1. 管理员登录\n");
        printf("==2. 用户登录\n");
        printf("==3. 注册\n");
        printf("==4. 用户修改密码\n");
        printf("==5. 浏览商品列表\n");
        printf("==6. 精确查询商品信息\n");  
        printf("==7. 模糊查询商品信息\n");
        printf("==8. 购买商品\n");
        printf("==9. 退出登录\n");
        printf("==10. 退出系统\n");
        if(logined == 2){
            printf("==11.添加商品\n");
            printf("==12.修改商品\n");
            printf("==13.删除商品\n");
            printf("==18.查看所有订单\n");
            printf("==19.手动修改订单状态/更新模式\n");
            printf("==23.查看活动列表\n");
            printf("==24.添加活动\n");
            printf("==25.修改活动\n");
            printf("==26.删除活动\n");
            printf("==27.统计销售数据\n");
        }else if(logined == 1){
            printf("==14.查看购物车\n");
            printf("==15.添加商品到购物车\n");
            printf("==16.从购物车中移除商品\n");
            printf("==17.修改购物车中商品数量\n");
            printf("==20.购买商品\n");
            printf("==21.查看订单\n");
            printf("==22.确认收货\n");
        }
        printf("================================\n");
        printf("请选择操作：\n");
        int choice;
        if(!(cin >> choice)){
            printf("输入无效，请输入数字选项！\n");
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        switch (choice) {
            case 1: {
                printf("请输入管理员账号：");
                string name;
                cin >> name;
                printf("请输入管理员密码：");
                string password;
                cin >> password;
                logined = admin.LogIn(name, password);
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 2: {
                printf("请输入用户名：");
                string username;
                cin >> username;
                printf("请输入用户密码：");
                string password;
                cin >> password;
                logined = customer.LogIn(username, password);
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 3: {
                printf("请输入用户名：");
                string username;
                cin >> username;
                printf("请输入用户密码：");
                string password;
                cin >> password;
                customer.SignUp(username, password);
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 4: {
                printf("请输入用户名：");
                string username;
                cin >> username;
                printf("请输入旧密码：");
                string old_password;
                cin >> old_password;
                printf("请输入新密码：");
                string new_password;
                cin >> new_password;
                customer.LogIn(username, old_password);
                customer.ChangePassword(username, old_password, new_password);
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 5: {
                customer.BrowseProducts();
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 6: {
                printf("请输入商品名称：");
                string product_name;
                cin >> product_name;
                customer.PreciseSearchProducts(product_name);
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 7: { 
                printf("请输入商品关键词：");
                string keyword;
                cin >> keyword;
                customer.FuzzySearchProducts(keyword);
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 8: {
                printf("购买商品！\n");
                if(logined==0){
                    printf("请先登录！\n");
                    printf("请输入用户名：");
                    string username;
                    cin >> username;
                    printf("请输入用户密码：");
                    string password;
                    cin >> password;
                    logined = customer.LogIn(username, password);
                    printf("\n按回车键继续...");
                    cin.ignore();
                    cin.get();
                    break;
                }
                if(logined == 1){
                    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
                        ShoppingSystem::products.at(i).Display();
                    }
                    //customer.Buy(product_id);
                }
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 9: {
                if(logined == 1){
                    customer.LogOut();
                }
                if(logined == 2){
                    admin.LogOut();
                }
                logined = 0;
                printf("退出登录！\n");
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 10: {
                if(logined == 1){
                    customer.LogOut();
                }
                if(logined == 2){
                    admin.LogOut();
                }
                logined = 0;
                printf("退出系统！\n");
                exit(0);
                break; 
            }
            case 11: {
                printf("添加商品！\n");
                if(logined == 2){
                    printf("请输入商品类别：");
                    string product_class;
                    cin >> product_class;
                    printf("请输入商品名称：");
                    string product_name;
                    cin >> product_name;
                    printf("请输入商品价格：");
                    double product_price;
                    cin >> product_price;
                    printf("请输入商品库存：");
                    int product_stock;
                    cin >> product_stock;
                    printf("请输入商品描述：");
                    string product_description;
                    cin >> product_description;
                    Product product(product_class, product_name, product_price, product_stock, product_description);
                    admin.AddProduct(product);
                }else{
                    printf("您没有权限添加商品！\n");
                }
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 12: {
                printf("修改商品！\n");
                if(logined == 2){
                    printf("请输入商品类别：");
                    string product_class;
                    cin >> product_class;
                    printf("请输入商品名称：");
                    string product_name;
                    cin >> product_name;
                    printf("请输入商品价格：");
                    double product_price;
                    cin >> product_price;
                    printf("请输入商品库存：");
                    int product_stock;
                    cin >> product_stock;
                    printf("请输入商品描述：");
                    string product_description;
                    cin >> product_description;
                    Product product(product_class, product_name, product_price, product_stock, product_description);
                    admin.EditProduct(product);
                }else{
                    printf("您没有权限修改商品！\n");
                }
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 13: {
                printf("删除商品！\n");
                if(logined == 2){
                    printf("请输入商品名称：");
                    string product_name;
                    cin >> product_name;
                    // 创建一个临时 Product 对象用于查找和删除
                    // 由于只需要 product_name 来匹配，其他参数可以设为默认值
                    Product product("", product_name, 0.0, 0, "");
                    admin.DeleteProduct(product);
                }else{
                    printf("您没有权限删除商品！\n");
                }
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 18: {
                printf("查看所有订单！\n");
                if(logined != 2){
                    printf("只有管理员可以查看所有订单！\n");
                }else{
                    for(const auto& kv : ShoppingSystem::orders){
                        kv.second.Display();
                        printf("\n");
                    }
                }
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 19: {
                printf("手动修改订单状态 / 更新模式！\n");
                if(logined != 2){
                    printf("只有管理员可以修改订单状态！\n");
                    printf("\n按回车键继续...");
                    cin.ignore();
                    cin.get();
                    break;
                }
                printf("请输入订单编号：");
                string oid;
                cin >> oid;
                auto it = ShoppingSystem::orders.find(oid);
                if(it == ShoppingSystem::orders.end()){
                    printf("订单不存在！\n");
                }else{
                    Order& o = it->second;
                    printf("当前订单状态：%s\n", o.status.c_str());
                    printf("当前更新模式：%s\n", o.auto_update ? "自动" : "手动");
                    
                    printf("是否修改订单状态？(y/n)：");
                    char ch;
                    cin >> ch;
                    if(ch == 'y' || ch == 'Y'){
                        printf("请输入新的订单状态（待发货/已发货/已签收）：");
                        string new_status;
                        cin >> new_status;
                        o.status = new_status;
                    }
                    
                    printf("请选择订单状态更新模式：1.自动  2.手动：");
                    int mode;
                    cin >> mode;
                    if(mode == 1){
                        o.auto_update = true;
                        printf("该订单已设置为【自动更新】模式。\n");
                    }else if(mode == 2){
                        o.auto_update = false;
                        printf("该订单已设置为【手动更新】模式。\n");
                    }else{
                        printf("输入无效，保持原更新模式不变。\n");
                    }
                    FileProcess::SaveOrdersToFile();
                    printf("订单信息保存成功！\n");
                }
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 14: {
                printf("查看购物车！\n");
                if(logined != 1){  //身份验证，只有用户可以查看购物车
                    printf("您没有权限查看购物车，请先登录！\n");
                    printf("\n按回车键继续...");
                    cin.ignore();
                    cin.get();
                    break;
                }
                customer.ShowCart();
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get(); 
                break;
            }
            case 15: {
                printf("添加商品到购物车！\n");
                if(logined != 1){  //身份验证，只有用户可以添加商品到购物车
                    printf("您没有权限添加商品到购物车，请先登录！\n");
                    printf("\n按回车键继续...");
                    cin.ignore();
                    cin.get();
                    break;
                }
                printf("请输入商品名称：");
                string product_name;
                cin >> product_name;
                customer.cart.AddToCart(product_name);
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 16: {
                printf("从购物车中移除商品！\n");
                if(logined != 1){  //身份验证，只有用户可以从购物车中移除商品
                    printf("您没有权限从购物车中移除商品，请先登录！\n");
                    printf("\n按回车键继续...");
                    cin.ignore();
                    cin.get();
                    break;
                }
                vector<string> temp_products;
                printf("要移除的商品数量：");
                int quantity;
                cin >> quantity;
                for(int i = 0; i < quantity; i++){
                    printf("请输入商品名称：");
                    string product_name;
                    cin >> product_name; 
                    temp_products.push_back(product_name);
                }
                customer.cart.RemoveFromCart(temp_products);
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 17: {
                printf("修改购物车中商品数量！\n");
                if(logined != 1){  //身份验证，只有用户可以修改购物车中商品数量
                    printf("您没有权限修改购物车中商品数量，请先登录！\n");
                    printf("\n按回车键继续...");
                    cin.ignore();
                    cin.get();
                    break;
                }
                printf("请输入商品名称：");
                string product_name;
                cin >> product_name;
                printf("请输入商品数量：");
                int quantity;
                cin >> quantity;
                customer.cart.ModifyCart(product_name, quantity);
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 20: {
                printf("购买商品！\n");
                customer.BuyProducts();
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 21: {
                printf("查看订单！\n");
                customer.ShowOrders();
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 22: {
                printf("确认收货！\n");
                if(logined != 1){  //身份验证，只有用户可以确认收货
                    printf("您没有权限确认收货，请先登录！\n");
                    printf("\n按回车键继续...");
                    cin.ignore();
                    cin.get();
                    break;
                }
                printf("请输入订单编号：");
                string order_id;
                cin >> order_id;
                customer.ConfirmOrderReceipt(order_id);
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 23: {
                printf("查看活动列表！\n");
                for(const auto& activity : ShoppingSystem::activities){
                    activity.Display();
                }
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 24: {
                printf("添加活动！\n");
                if(logined != 2){
                    printf("您没有权限添加活动，请先登录！\n");
                    printf("\n按回车键继续...");
                    cin.ignore();
                    cin.get();
                    break;
                }
                admin.AddActivity();
                printf("活动添加成功！\n");
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 25: {
                printf("修改活动！\n");
                if(logined != 2){
                    printf("您没有权限修改活动，请先登录！\n");
                    printf("\n按回车键继续...");
                    cin.ignore();
                    cin.get();
                    break;
                }
                admin.EditActivity();
                printf("活动修改成功！\n");
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 26: {
                printf("删除活动！\n");
                if(logined != 2){
                    printf("您没有权限删除活动，请先登录！\n");
                    printf("\n按回车键继续...");
                    cin.ignore();
                    cin.get();
                    break;
                }
                admin.DeleteActivity();
                printf("活动删除成功！\n");
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            case 27: {
                printf("统计销售数据！\n");
                if(logined != 2){
                    printf("您没有权限统计销售数据，请先登录！\n");
                    printf("\n按回车键继续...");
                    cin.ignore();
                    cin.get();
                    break;
                }
                Statistic::Statisticing();
                printf("销售数据统计完成！\n");
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
            default:{
                printf("无效的选择！\n");
                printf("\n按回车键继续...");
                cin.ignore();
                cin.get();
                break;
            }
        }
    }
}

void ShoppingSystem::Exit(){
    //保存数据到文件
    FileProcess::SaveToFile();
    //清空内存中的数据库信息，不清除文件中的数据
    ShoppingSystem::customers.clear();
    ShoppingSystem::administrators.clear();
    ShoppingSystem::products.clear();
    printf("数据已保存，退出系统！\n");
    exit(0);
}



// 根据时间自动更新所有订单状态：
//  订单创建 10 秒后：待发货 -> 已发货
void ShoppingSystem::AutoUpdateOrderStatuses(){
    time_t now = time(nullptr);
    bool changed = false;
    for(auto& kv : orders){
        Order& order = kv.second;
        if(!order.auto_update) continue; // 手动模式的订单不自动更新
        long long diff = static_cast<long long>(now) - order.create_timestamp;
        if(order.status == "待发货" && diff >= 10){
            order.status = "已发货";
            changed = true;
        }
    }
    if(changed){
        FileProcess::SaveOrdersToFile();
    }
}

// 同步更新所有用户购物车中某个商品的信息（特别是库存），并写回 customer_carts.txt
void ShoppingSystem::SyncProductInAllCarts(const Product& product){
    // 使用最新商品信息构造一个标准副本
    Product updated(product.GetProductClass(),
                    product.GetProductName(),
                    product.GetProductPrice(),
                    product.GetProductStock(),
                    product.GetProductDescription());
    int updated_count = 0;

    for(auto& user_cart : customer_carts){
        vector<pair<Product,int>>& cart = user_cart.second;
        for(size_t i = 0; i < cart.size(); i++){
            if(cart[i].first.GetProductName() == updated.GetProductName()){
                cart[i].first = updated;
                updated_count++;
            }
        }
    }

    if(updated_count > 0){
        FileProcess::SaveCustomerCartsToFile();
        printf("已同步更新 %d 条购物车中的商品信息（含库存）。\n", updated_count);
    }
}

// 自动更新活动状态
void ShoppingSystem::AutoUpdateActivityStatuses(){
    time_t now = time(nullptr);
    for(size_t i = 0; i < ShoppingSystem::activities.size(); i++){
        Activity& activity = ShoppingSystem::activities.at(i);
        if(activity.activity_status == "进行中"){
            if(now >= activity.activity_end_time){
                activity.activity_status = "已结束";
            }
        }
    }
}

// 自动检查商品文件是否修改并更新内存中的商品列表
void ShoppingSystem::AutoCheckAndUpdateProducts(){
    static time_t last_mod_time = 0;
    struct stat file_stat;
    if(stat("products.txt", &file_stat) == 0){
        if(file_stat.st_mtime != last_mod_time){
            last_mod_time = file_stat.st_mtime;
            // 文件已修改，重新加载商品数据
            FileProcess::LoadProductsFromFile();
            printf("商品文件已更新，内存中的商品列表已同步刷新。\n");
        }
    }
}
#include<iostream>
#include<fstream> 
#include<cstdlib>
#include "shopping_system.hpp"
#include "administrator.hpp"
#include "customer.hpp"
#include "product.hpp"
#include<vector>
#include<sstream>
#include<string>
#include<limits>

using namespace std;

// 内存
// 定义静态成员（全局唯一性）--所有ShoppingSystem对象（如果有两个system类变量的话）共享一个数据库
vector<Customer> ShoppingSystem::customers; //后续使用getter方法访问，更安全
vector<Product> ShoppingSystem::products; //后续使用getter方法访问，更安全
map<string, vector<pair<Product, int>>> ShoppingSystem::customer_carts; //用户购物车数据库，键为用户名
map<string, Order> ShoppingSystem::orders; //订单数据库，键为订单编号，值为订单对象
vector<Activity> ShoppingSystem::activities; //活动列表数据库

// 定义文件路径常量
const string ShoppingSystem::CUSTOMERS_FILE = "customers.txt";
const string ShoppingSystem::PRODUCTS_FILE = "products.txt";
const string ShoppingSystem::CUSTOMER_CARTS_FILE = "customer_carts.txt";
const string ShoppingSystem::ORDERS_FILE = "orders.txt";
const string ShoppingSystem::ACTIVITIES_FILE = "activities.txt";

void ShoppingSystem::Init(){
    // 先尝试从文件加载数据
    LoadFromFile();
    
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
        SaveToFile();
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
                    ShoppingSystem::SaveOrdersToFile();
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
    SaveToFile();
    //清空内存中的数据库信息，不清除文件中的数据
    ShoppingSystem::customers.clear();
    ShoppingSystem::products.clear();
    printf("数据已保存，退出系统！\n");
    exit(0);
}

// 保存用户数据到文件
void ShoppingSystem::SaveCustomersToFile(){
    ofstream outfile(CUSTOMERS_FILE);
    if(!outfile.is_open()){
        printf("无法打开用户数据文件进行写入！\n");
        return;
    }
    
    for(size_t i = 0; i < customers.size(); i++){
        // 格式：用户名|密码
        outfile << customers[i].username << "|" << customers[i].password << endl;
    }
    
    outfile.close();
    printf("用户数据已保存到 %s\n", CUSTOMERS_FILE.c_str());
}

// 保存商品数据到文件（使用getter方法，更安全）
void ShoppingSystem::SaveProductsToFile(){
    ofstream outfile(PRODUCTS_FILE);
    if(!outfile.is_open()){
        printf("无法打开商品数据文件进行写入！\n");
        return;
    }
    
    for(size_t i = 0; i < products.size(); i++){
        // 格式：类别|名称|价格|库存|描述
        outfile << products[i].GetProductClass() << "|" 
                << products[i].GetProductName() << "|" 
                << products[i].GetProductPrice() << "|" 
                << products[i].GetProductStock() << "|" 
                << products[i].GetProductDescription() << endl;
    }
    
    outfile.close();
    printf("商品数据已保存到 %s\n", PRODUCTS_FILE.c_str());
}

// 从文件加载用户数据
void ShoppingSystem::LoadCustomersFromFile(){
    ifstream infile(CUSTOMERS_FILE);
    if(!infile.is_open()){
        printf("用户数据文件不存在，将创建新文件。\n");
        return;
    }
    
    customers.clear(); // 清空现有数据
    string line;
    while(getline(infile, line)){
        if(line.empty()) continue;
        
        // 解析格式：用户名|密码
        size_t pos = line.find('|');
        if(pos != string::npos){
            string username = line.substr(0, pos);
            string password = line.substr(pos + 1);
            
            // 创建用户对象，但不自动添加到数据库（add_to_db=false）
            customers.push_back(Customer(username, password, false));
        }
    }
    
    infile.close();
    printf("已从 %s 加载 %zu 个用户。\n", CUSTOMERS_FILE.c_str(), customers.size());
}

// 从文件加载商品数据
void ShoppingSystem::LoadProductsFromFile(){
    ifstream infile(PRODUCTS_FILE);
    if(!infile.is_open()){
        printf("商品数据文件不存在，将创建新文件。\n");
        return;
    }
    
    products.clear(); // 清空现有数据
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
        
        products.push_back(Product(product_class, product_name, product_price, product_stock, product_description));
    }
    
    infile.close();
    printf("已从 %s 加载 %zu 个商品。\n", PRODUCTS_FILE.c_str(), products.size());
}

// 从文件加载用户购物车数据到内存
void ShoppingSystem::LoadCustomerCartsFromFile(){
    ifstream infile(CUSTOMER_CARTS_FILE);
    if(!infile.is_open()){
        printf("用户购物车数据文件不存在，将创建新文件。\n");
        return;
    }
    
    customer_carts.clear(); // 清空现有数据
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
        customer_carts[username].push_back(make_pair(product, quantity));
        total_items++;
    }
    
    infile.close();
    printf("已从 %s 加载 %d 个用户购物车项。\n", CUSTOMER_CARTS_FILE.c_str(), total_items);
}

// 保存用户购物车数据到文件
void ShoppingSystem::SaveCustomerCartsToFile(){
    ofstream outfile(CUSTOMER_CARTS_FILE);
    if(!outfile.is_open()){
        printf("无法打开用户购物车数据文件进行写入！\n");
        return;
    }
    
    int total_items = 0;
    // 遍历所有用户的购物车
    for(auto& user_cart : customer_carts){
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
void ShoppingSystem::SaveToFile(){
    SaveCustomersToFile();
    SaveProductsToFile();
    SaveCustomerCartsToFile();
    SaveOrdersToFile();
    SaveActivitiesToFile();
}

// 统一加载接口
void ShoppingSystem::LoadFromFile(){
    LoadCustomersFromFile();
    LoadProductsFromFile();
    LoadCustomerCartsFromFile();
    LoadOrdersFromFile();
    LoadActivitiesFromFile();
}

// 从文件加载订单数据到内存
void ShoppingSystem::LoadOrdersFromFile(){
    ifstream infile(ORDERS_FILE);
    if(!infile.is_open()){
        printf("订单数据文件不存在，将创建新文件。\n");
        return;
    }
    orders.clear(); // 清空现有数据
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
                for(size_t i = 0; i < products.size(); i++){
                    if(products[i].GetProductName() == pname){
                        items.push_back(make_pair(products[i], qty));
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

        orders[id] = Order(id, username, items, purchase_time, total_amount, delivery_address, status, ts, auto_update);
    }
    infile.close();
    printf("已从 %s 加载 %zu 个订单。\n", ORDERS_FILE.c_str(), orders.size());
}

// 保存订单数据到文件（仅保存汇总信息）
void ShoppingSystem::SaveOrdersToFile(){
    ofstream outfile(ORDERS_FILE);
    if(!outfile.is_open()){
        printf("无法打开订单数据文件进行写入！\n");
        return;
    }
    for(const auto& kv : orders){
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
        SaveOrdersToFile();
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
        SaveCustomerCartsToFile();
        printf("已同步更新 %d 条购物车中的商品信息（含库存）。\n", updated_count);
    }
}

void ShoppingSystem::SaveActivitiesToFile(){
    ofstream outfile(ACTIVITIES_FILE);
    if(!outfile.is_open()){
        printf("无法打开活动数据文件进行写入！\n");
        return;
    }
    for(size_t i = 0; i < activities.size(); i++){
        outfile << activities.at(i).activity_name << "|"
                << activities.at(i).activity_type_discount << "|"
                << activities.at(i).activity_type_full_reduction << "|"
                << activities.at(i).activity_start_time << "|"
                << activities.at(i).activity_end_time << "|"   
                << activities.at(i).activity_discount << "|"
                << activities.at(i).activity_threshold << "|"
                << activities.at(i).activity_full_reduction_amount << "|"
                << activities.at(i).activity_status << "|"
                << activities.at(i).activity_discount_products.size() << "|";
        for(size_t j = 0; j < activities.at(i).activity_discount_products.size(); j++){
            outfile << activities.at(i).activity_discount_products.at(j).first.GetProductName() << "," << activities.at(i).activity_discount_products.at(j).second << ";";
        }
        outfile << "|" << activities.at(i).activity_full_reduction_products.size() << "|";
        for(size_t j = 0; j < activities.at(i).activity_full_reduction_products.size(); j++){
            outfile << activities.at(i).activity_full_reduction_products.at(j).GetProductName() << ";";
        }
        outfile << endl;
    }
    outfile.close();
    printf("活动数据已保存到 %s\n", ACTIVITIES_FILE.c_str());
}

//加载活动数据到内存ShoppingSystem::activities
void ShoppingSystem::LoadActivitiesFromFile(){
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
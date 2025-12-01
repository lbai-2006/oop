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

using namespace std;

// 内存
// 定义静态成员（全局唯一性）--所有ShoppingSystem对象（如果有两个system类变量的话）共享一个数据库
vector<Customer> ShoppingSystem::customers; //后续使用getter方法访问，更安全
vector<Product> ShoppingSystem::products; //后续使用getter方法访问，更安全
map<string, vector<pair<Product, int>>> ShoppingSystem::customer_carts; //用户购物车数据库，键为用户名

// 定义文件路径常量
const string ShoppingSystem::CUSTOMERS_FILE = "customers.txt";
const string ShoppingSystem::PRODUCTS_FILE = "products.txt";
const string ShoppingSystem::CUSTOMER_CARTS_FILE = "customer_carts.txt";

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
        }else if(logined == 1){
            printf("==14.查看购物车\n");
            printf("==15.添加商品到购物车\n");
            printf("==16.从购物车中移除商品\n");
            printf("==17.修改购物车中商品数量\n");
        }
        printf("================================\n");
        printf("请选择操作：\n");
        int choice;
        cin >> choice;
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
}

// 统一加载接口
void ShoppingSystem::LoadFromFile(){
    LoadCustomersFromFile();
    LoadProductsFromFile();
    LoadCustomerCartsFromFile();
}
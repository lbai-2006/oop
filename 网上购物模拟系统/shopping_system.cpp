#include<iostream>
#include<fstream> //文件流
#include<cstdlib>
#include "shopping_system.hpp"
#include "administrator.hpp"
#include "customer.hpp"
#include "product.hpp"
#include<vector>
#include<sstream>
#include<string>

using namespace std;

// 定义静态成员（每个系统有自己的数据库）
vector<Customer> ShoppingSystem::customers;
vector<Product> ShoppingSystem::products;

void ShoppingSystem::Init(){
    //初始化用户列表数据库
    ShoppingSystem::customers.push_back(Customer("admin", "admin123"));
    //初始化商品列表数据库，添加5个商品
    ShoppingSystem::products.push_back(Product("food", "cake", 20, 100, "delicious"));
    // Example 1: A book
    ShoppingSystem::products.push_back(Product("Books", "C++ Primer", 128, 50, "A classic beginner's guide to C++."));
    // Example 2: A smartphone
    ShoppingSystem::products.push_back(Product("Electronics", "iPhone 14", 5999, 30, "The latest smartphone from Apple."));
    // Example 3: A bag of rice
    ShoppingSystem::products.push_back(Product("Food", "Northeast Rice", 69, 500, "High-quality rice from the origin."));
    // Example 4: An electric toothbrush
    ShoppingSystem::products.push_back(Product("Daily Necessities", "Philips Electric Toothbrush", 299, 100, "Electric toothbrush for healthy teeth."));
    // Example 5: A pair of headphones
    ShoppingSystem::products.push_back(Product("Electronics", "Sony WH-1000XM4", 1999, 20, "Wireless noise-canceling headphones."));
}

ShoppingSystem::ShoppingSystem(){
    //初始化静态成员
    ShoppingSystem::Init();
}

void ShoppingSystem::ShowCustomerFile(){
    //显示用户列表
    for(size_t i = 0; i < ShoppingSystem::customers.size(); i++){
        cout << "用户名：" << ShoppingSystem::customers.at(i).username << " 密码：" << ShoppingSystem::customers.at(i).password << endl;
    }
    cout << "用户列表显示完毕！" << endl;
}

void ShoppingSystem::ShowProductFile(){
    //显示商品列表
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        cout << "商品ID：" << ShoppingSystem::products.at(i).product_class << " 商品名称：" << ShoppingSystem::products.at(i).product_name << " 商品价格：" << ShoppingSystem::products.at(i).product_price << " 商品库存：" << ShoppingSystem::products.at(i).product_stock << " 商品描述：" << ShoppingSystem::products.at(i).product_description << endl;
    }
    cout << "商品列表显示完毕！" << endl;
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
                break;
            }
            case 5: {
                customer.BrowseProducts();
                break;
            }
            case 6: {
                printf("请输入商品名称：");
                string product_name;
                cin >> product_name;
                customer.PreciseSearchProducts(product_name);
                break;
            }
            case 7: { 
                printf("请输入商品关键词：");
                string keyword;
                cin >> keyword;
                customer.FuzzySearchProducts(keyword);
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
                    break;
                }
                if(logined == 1){
                    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
                        ShoppingSystem::products.at(i).Display();
                    }
                    //customer.Buy(product_id);
                }
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
                break;
            }
            default:{
                printf("无效的选择！\n");
                break;
            }
        }
    }
}

void ShoppingSystem::Exit(){
    //退出系统
    //清空数据库信息
    ShoppingSystem::customers.clear();
    ShoppingSystem::products.clear();
    printf("退出系统！\n");
    exit(0);
}
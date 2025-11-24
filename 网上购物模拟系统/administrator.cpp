#include<iostream>
#include<fstream> //文件流
#include<cstdlib>
#include "administrator.hpp"
#include "shopping_system.hpp"
#include<vector>
#include<sstream>
#include<string>

using namespace std;

Administrator::Administrator() {
    this->name = "admin";  //管理员账号：admin 密码：admin123
    this->password = "admin123";
}
int Administrator::LogIn(string name, string password){
    if(name == this->name &&password == this->password){
        cout<<"管理员登录成功！"<<endl;
        return 2;
    }else{
        cout<<"管理员登录失败！"<<endl;
        return 0;
    }
}
void Administrator::LogOut(){
    this->name = "";
    this->password = "";
    cout<<"管理员退出登录成功！"<<endl;
}

void Administrator::AddProduct(Product product){
    ShoppingSystem::products.push_back(product);
    cout << "商品添加成功！" << endl;
}

void Administrator::EditProduct(Product product){
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        if(ShoppingSystem::products.at(i).product_name == product.product_name){
            ShoppingSystem::products.at(i) = product;
            break;
        }
    }
    cout << "商品修改成功！" << endl;
}

void Administrator::DeleteProduct(Product product){
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        if(ShoppingSystem::products.at(i).product_name == product.product_name){
            ShoppingSystem::products.erase(ShoppingSystem::products.begin() + i);
            break;
        }
    }
    cout << "商品删除成功！" << endl;
}
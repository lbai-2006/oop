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
        printf("管理员登录成功！\n");
        return 2;
    }else{
        printf("管理员账号或密码错误，登录失败！\n");
        return 0;
    }
}
void Administrator::LogOut(){
    this->name = "";
    this->password = "";
    printf("管理员退出登录成功！\n");
}

void Administrator::AddProduct(Product product){
    ShoppingSystem::products.push_back(product);
    printf("商品添加成功！\n");
    
    // 保存到文件
    ShoppingSystem::SaveProductsToFile();
}

void Administrator::EditProduct(Product product){
    bool found = false;
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        if(ShoppingSystem::products.at(i).product_name == product.product_name){
            ShoppingSystem::products.at(i) = product;
            found = true;
            break;
        }
    }
    if(found){
        printf("商品修改成功！\n");
        // 修改操作需要完整保存（覆盖整个文件）
        ShoppingSystem::SaveProductsToFile();
    } else {
        printf("商品不存在，修改失败！\n");
    }
}

void Administrator::DeleteProduct(Product product){
    int found = 0;
    for(size_t i = 0; i < ShoppingSystem::products.size(); i++){
        if(ShoppingSystem::products.at(i).product_name == product.product_name){
            ShoppingSystem::products.erase(ShoppingSystem::products.begin() + i);
            found = 1;
            break;
        }
    }
    if(found == 0){
        printf("商品不存在！\n");
    }else{
        printf("商品删除成功！\n");
        // 删除操作需要完整保存（覆盖整个文件）
        ShoppingSystem::SaveProductsToFile();
    }
}
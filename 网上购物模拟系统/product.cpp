#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include "product.hpp"
#include "shopping_system.hpp"

using namespace std;

Product::Product(string product_class, string product_name, double product_price, int product_stock, string product_description){
    this->product_class = product_class;
    this->product_name = product_name; //商品名称唯一
    this->product_price = product_price;
    this->product_stock = product_stock;
    this->product_description = product_description;
}
void Product::Display(){
    printf("商品类别：%s\n", this->product_class.c_str());
    printf("商品名称：%s\n", this->product_name.c_str());
    printf("商品价格：%.2f\n", this->product_price);
    printf("商品库存：%d\n", this->product_stock);
    printf("商品描述：%s\n", this->product_description.c_str());
}

void Product::ModifyProductStock(int quantity){
    //如果库存小于0，则提示库存不足
    if(quantity < 0){
        printf("库存不足，修改失败！\n");
        return;
    }
    //如果库存大于0，则修改库存
    this->product_stock = quantity;
    printf("商品库存修改成功！\n");
    // 保存到文件（通过 ShoppingSystem 统一管理商品数据）
    ShoppingSystem::SaveProductsToFile();
    // 同步更新所有用户购物车中的该商品信息（内存 + customer_carts.txt）
    ShoppingSystem::SyncProductInAllCarts(*this);
}
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
    //如果该商品在当前活动促销商品列表中（从最新活动开始），显示参与活动类型，如果不在则不显示
    for(size_t i = 0; i < ShoppingSystem::activities.size(); i++){
        if(ShoppingSystem::activities.at(i).activity_status == "进行中"){
            for(size_t j = 0; j < ShoppingSystem::activities.at(i).activity_discount_products.size(); j++){
                if(ShoppingSystem::activities.at(i).activity_discount_products.at(j).first.GetProductName() == this->product_name){
                    printf("参与折扣活动，折扣率：%.0f%%\n", ShoppingSystem::activities.at(i).activity_discount_products.at(j).second * 100);
                    break;
                }
            }
            for(size_t j = 0; j < ShoppingSystem::activities.at(i).activity_full_reduction_products.size(); j++){
                if(ShoppingSystem::activities.at(i).activity_full_reduction_products.at(j).GetProductName() == this->product_name){
                    printf("参与满减活动，满减金额：满%.2f元 减%.2f元\n", ShoppingSystem::activities.at(i).activity_threshold, ShoppingSystem::activities.at(i).activity_full_reduction_amount);
                    break;
                }
            }
        }
    }
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
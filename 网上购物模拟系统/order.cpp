#include<iostream>
#include "order.hpp"

using namespace std;

Order::Order(const string& id,
             const string& username,
             const vector<pair<Product, int>>& items,
             const string& purchase_time,
             double total_amount,
             const string& delivery_address,
             const string& status,
             long long create_timestamp,
             bool auto_update){
    this->id = id;
    this->username = username;
    this->items = items;
    this->purchase_time = purchase_time;
    this->total_amount = total_amount;
    this->delivery_address = delivery_address;
    this->status = status;
    this->auto_update = auto_update;
    if(create_timestamp == 0){
        this->create_timestamp = static_cast<long long>(time(nullptr));
    }else{
        this->create_timestamp = create_timestamp;
    }
}

void Order::Display() const{
    printf("订单编号：%s\n", this->id.c_str());
    printf("下单用户：%s\n", this->username.c_str());
    printf("所购商品信息：\n");
    for(size_t i = 0; i < this->items.size(); i++){
        const Product& p = this->items.at(i).first;
        int quantity = this->items.at(i).second;
        double price = p.GetProductPrice();
        printf("  商品名称：%s  商品数量：%d  单价：%.2f  小计：%.2f\n",
               p.GetProductName().c_str(),
               quantity,
               price,
               price * quantity);
    }
    printf("购买时间：%s\n", this->purchase_time.c_str());
    printf("订单总额：%.2f\n", this->total_amount);
    printf("收货地址：%s\n", this->delivery_address.c_str());
    printf("订单状态：%s\n", this->status.c_str());
    printf("订单显示完毕！\n");
}

void Order::UpdateOrderStatus(){
    // 简单示例：如果订单状态为待发货，则更新为已发货
    if(this->status == "待发货"){
        this->status = "已发货";
    }
}
#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include "product.hpp"

using namespace std;

// 订单信息：编号、用户名、所购商品信息、购买时间、订单总额、收货地址、订单状态
class Order{
public:
    string id;                               // 订单编号
    string username;                         // 下单用户
    vector<pair<Product, int>> items;        // 购买的商品及数量
    string purchase_time;                    // 购买时间
    double total_amount;                     // 订单总金额
    string delivery_address;                 // 收货地址
    string status;                           // 订单状态（如：待发货 / 已发货 / 已签收）
    long long create_timestamp;              // 下单时间戳（秒），用于自动状态更新
    bool auto_update;                        // 是否自动更新状态（true 自动，false 手动）

    Order() = default;

    Order(const string& id,
          const string& username,
          const vector<pair<Product, int>>& items,
          const string& purchase_time,
          double total_amount,
          const string& delivery_address,
          const string& status,
          long long create_timestamp = 0,
          bool auto_update = true);

    void Display() const;
    
    // 简单状态更新示例：待发货 -> 已发货
    void UpdateOrderStatus();
};
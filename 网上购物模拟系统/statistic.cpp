#include "statistic.hpp"
#include "shopping_system.hpp"
#include "order.hpp"
#include "statistic.hpp"
#include "shopping_system.hpp"
#include "order.hpp"
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

// 静态成员定义
unordered_map<string,int> Statistic::class_counts_;
unordered_map<string,double> Statistic::class_amounts_;
unordered_map<string,int> Statistic::name_counts_;

void Statistic::ClearStatistics() {
    class_counts_.clear();
    class_amounts_.clear();
    name_counts_.clear();
}

const unordered_map<string,int>& Statistic::GetClassCounts() {
    return class_counts_;
}

const unordered_map<string,double>& Statistic::GetClassAmounts() {
    return class_amounts_;
}

const unordered_map<string,int>& Statistic::GetNameCounts() {
    return name_counts_;
}

void Statistic::Statisticing(){
    // 先清空以前的统计结果
    ClearStatistics();

    // 遍历所有订单并统计，采用下标的方式
    for (const auto& order_pair : ShoppingSystem::orders) {
        const Order& order = order_pair.second;
        for (const auto& item : order.items) {
            const Product& product = item.first;
            int quantity = item.second;

            string cls = product.GetProductClass();
            string name = product.GetProductName();
            double price = product.GetProductPrice();

            class_counts_[cls] += quantity;
            class_amounts_[cls] += price * quantity;
            name_counts_[name] += quantity;
        }
    }
    //打印
    cout << "按类别统计的销售数量：" << endl;
    for (const auto& pair : class_counts_) {
        cout << "类别: " << pair.first << ", 销售数量: " << pair.second << endl;
    }
    cout << "按类别统计的销售金额：" << endl;
    for (const auto& pair : class_amounts_) {
        cout << "类别: " << pair.first <<", 销售金额: " << pair.second << endl;
    }
    cout << "按名称统计的销售数量：" << endl;
    for (const auto& pair : name_counts_) {
        cout << "名称: " << pair.first << ", 销售数量: " << pair.second << endl;
    }
}
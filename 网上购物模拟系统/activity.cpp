#include<iostream>
#include<cstdio>
#include "activity.hpp"

using namespace std;

Activity::Activity(string activity_name, bool activity_type_discount, bool activity_type_full_reduction, long long activity_start_time, long long activity_end_time, double activity_discount, double activity_threshold, double activity_full_reduction_amount, string activity_status, vector<pair<Product, double>> activity_discount_products, vector<Product> activity_full_reduction_products){
    this->activity_name = activity_name;
    this->activity_type_discount = activity_type_discount;
    this->activity_type_full_reduction = activity_type_full_reduction;
    this->activity_start_time = activity_start_time;
    this->activity_end_time = activity_end_time;
    this->activity_discount = activity_discount;
    this->activity_threshold = activity_threshold;
    this->activity_full_reduction_amount = activity_full_reduction_amount;
    this->activity_status = activity_status;
    this->activity_discount_products = activity_discount_products;
    this->activity_full_reduction_products = activity_full_reduction_products;
}

void Activity::Display() const{
    printf("活动名称：%s\n", this->activity_name.c_str());
    printf("活动类型：折扣 %s\n", (this->activity_type_discount ? "是" : "否"));
    printf("活动类型：满减 %s\n", (this->activity_type_full_reduction ? "是" : "否"));
    printf("活动开始时间：%lld（时间戳）\n", this->activity_start_time);
    printf("活动结束时间：%lld（时间戳）\n", this->activity_end_time);
    printf("活动折扣：%.0f%%\n", this->activity_discount * 100);
    printf("活动阈值：%.2f元\n", this->activity_threshold);
    printf("活动满减金额：%.2f元\n", this->activity_full_reduction_amount);
    printf("活动状态：%s\n", this->activity_status.c_str());
    printf("参加折扣活动的商品列表：\n");
    for(size_t i = 0; i < this->activity_discount_products.size(); i++){
        printf("  商品名称：%s，折扣率：%.0f%%\n",
               this->activity_discount_products.at(i).first.GetProductName().c_str(),
               this->activity_discount_products.at(i).second * 100);
    }
    printf("参加满减活动的商品列表：\n");
    for(size_t i = 0; i < this->activity_full_reduction_products.size(); i++){
        printf("  商品名称：%s\n", this->activity_full_reduction_products.at(i).GetProductName().c_str());
    }
    printf("\n");
}



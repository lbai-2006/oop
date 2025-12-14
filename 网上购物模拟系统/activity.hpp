#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include "product.hpp"

using namespace std;

class Activity{

    public:
        string activity_name; //活动名称
        bool activity_type_discount; //活动类型（折扣）
        bool activity_type_full_reduction; //活动类型（满减）
        long long activity_start_time; //活动开始时间（时间戳）
        long long activity_end_time; //活动结束时间（时间戳）
        double activity_discount; //活动折扣（折扣率）
        double activity_threshold; //活动阈值（满减阈值）
        double activity_full_reduction_amount; //活动满减金额（满减金额）
        string activity_status; //活动状态（进行中、已结束）
        //参加折扣活动的商品列表，不同商品可以有不同的折扣
        vector<pair<Product, double>> activity_discount_products; //商品对象，折扣率
        //参加满减活动的商品列表，同一个满减标准
        vector<Product> activity_full_reduction_products;

        Activity(string activity_name, 
            bool activity_type_discount,
            bool activity_type_full_reduction, 
            long long activity_start_time, 
            long long activity_end_time, 
            double activity_discount, 
            double activity_threshold, 
            double activity_full_reduction_amount,
            string activity_status,
            vector<pair<Product, double>> activity_discount_products = vector<pair<Product, double>>(),
            vector<Product> activity_full_reduction_products = vector<Product>()); //构造函数
        //~Activity(); //析构函数
        void Display() const; //显示活动信息
        
};
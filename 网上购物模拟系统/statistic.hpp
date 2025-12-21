// 工具类，静态函数
#pragma once
#ifndef STATISTIC_HPP
#define STATISTIC_HPP

#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

class Statistic {
private:
    static unordered_map<string,int> class_counts_;
    static unordered_map<string,double> class_amounts_;
    static unordered_map<string,int> name_counts_;

public:
    static void Statisticing();
    static const unordered_map<string,int>& GetClassCounts();
    static const unordered_map<string,double>& GetClassAmounts();
    static const unordered_map<string,int>& GetNameCounts();
    static void ClearStatistics();
};

#endif
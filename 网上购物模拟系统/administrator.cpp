#include<iostream>
#include<fstream> 
#include<cstdlib>
#include "administrator.hpp"
#include "shopping_system.hpp"
#include "customer.hpp"
#include "file_process.hpp"
#include<vector>
#include<sstream>
#include<string>
#include<map>
#include<limits>
#include<functional>

using namespace std;

template<typename T>
static T Read(const char* prompt, function<bool(const T&)> ok = nullptr){
    while(true){
        if(prompt) printf("%s", prompt);
        T v{};
        if(cin >> v && (!ok || ok(v))) return v;
        printf("输入无效，请重新输入。\n");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// 支持“直接回车使用默认值”的 double 输入（用于折扣率这种可选项）
static double ReadLineDoubleOrDefault(const char* prompt, double def){
    printf("%s", prompt);
    // 清理掉上一轮用 >> 遗留在缓冲区的一整行（通常是换行）
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string line;
    getline(cin, line);
    if(line.empty()) return def;
    try{
        return stod(line);
    }catch(...){
        printf("输入无效，使用默认值：%.2f\n", def);
        return def;
    }
}

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
    // 不要清空管理员账号/密码，否则下一次登录会永远失败
    // 管理员账号是固定的（admin/admin123），退出登录只需要返回到未登录状态即可
    this->name = "admin";
    this->password = "admin123";
    printf("管理员退出登录成功！\n");
}

void Administrator::AddProduct(Product product){
    ShoppingSystem::products.push_back(product);
    printf("商品添加成功！\n");
    
    // 保存到文件
    FileProcess::SaveProductsToFile();
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
        
        // 同步更新 ShoppingSystem::customer_carts 中的该商品信息
        int updated_carts = 0;
        for(auto& user_cart : ShoppingSystem::customer_carts){
            vector<pair<Product, int>>& cart = user_cart.second;
            for(size_t j = 0; j < cart.size(); j++){
                // 如果购物车中有同名商品，更新商品信息（保持数量不变）
                if(cart.at(j).first.product_name == product.product_name){
                    int quantity = cart.at(j).second; // 保存数量
                    cart.at(j).first = product; // 更新商品信息
                    cart.at(j).second = quantity; // 恢复数量
                    updated_carts++;
                }
            }
        }
        if(updated_carts > 0){
            printf("已同步更新 %d 个用户购物车中的商品信息！\n", updated_carts);
        }
        
        // 保存购物车数据到文件
        FileProcess::SaveCustomerCartsToFile();
        // 保存商品数据到文件
        FileProcess::SaveProductsToFile();
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
        
        // 同步删除所有用户购物车中的该商品
        int removed_carts = 0;
        for(auto& user_cart : ShoppingSystem::customer_carts){
            vector<pair<Product, int>>& cart = user_cart.second;
            for(size_t j = 0; j < cart.size(); ){
                if(cart.at(j).first.product_name == product.product_name){
                    cart.erase(cart.begin() + j);
                    removed_carts++;
                    // 不增加j，因为删除后后面的元素会前移
                } else {
                    j++;
                }
            }
        }
        if(removed_carts > 0){
            printf("已从 %d 个用户购物车中移除该商品！\n", removed_carts);
        }
        
        // 保存购物车数据到文件
        FileProcess::SaveCustomerCartsToFile();
        // 保存商品数据到文件
        FileProcess::SaveProductsToFile();
    }
}
void Administrator::UpdateOrderStatus(Order order){
    order.UpdateOrderStatus();
}

void Administrator::AddActivity(){
    printf("请输入活动名称：");
    string activity_name;
    cin >> activity_name;

    bool activity_type_discount = (Read<int>("请输入活动类型（折扣）是(1)/否(0)：", [](const int& x){ return x==0 || x==1; }) == 1);
    bool activity_type_full_reduction = (Read<int>("请输入活动类型（满减）是(1)/否(0)：", [](const int& x){ return x==0 || x==1; }) == 1);
    
    printf("活动现在开始！\n");
    long long activity_start_time = time(NULL);
    int activity_duration = Read<int>("请输入活动持续时间（小时）：", [](const int& x){ return x > 0; });
    long long activity_end_time = activity_start_time + activity_duration * 3600;
    
    double activity_discount = 1.0;
    double activity_threshold = 0;
    double activity_full_reduction_amount = 0;
    
    vector<pair<Product, double>> activity_discount_products;
    vector<Product> activity_full_reduction_products;
    
    if(activity_type_discount){
        activity_discount = Read<double>("请输入默认折扣率（0-1）：", [](const double& x){ return x > 0 && x <= 1; });
        
        int activity_discount_products_count = Read<int>("请输入要添加进折扣活动的商品数量：", [](const int& x){ return x > 0; });
        for(int i = 0; i < activity_discount_products_count; i++){
            printf("请输入第%d个商品名称：", i+1);
            string activity_discount_product_name;
            cin >> activity_discount_product_name;

            char prompt_buf[128];
            snprintf(prompt_buf, sizeof(prompt_buf), "请输入该商品的折扣率（0-1，直接回车使用默认%.2f）：", activity_discount);
            double product_discount = ReadLineDoubleOrDefault(prompt_buf, activity_discount);
            if(product_discount <= 0 || product_discount > 1){
                printf("折扣率范围应为 (0,1]，使用默认折扣 %.2f\n", activity_discount);
                product_discount = activity_discount;
            }
            
            // 查找商品并添加到折扣商品列表
            bool found = false;
            for(size_t j = 0; j < ShoppingSystem::products.size(); j++){
                if(ShoppingSystem::products.at(j).GetProductName() == activity_discount_product_name){
                    activity_discount_products.push_back(make_pair(ShoppingSystem::products.at(j), product_discount));
                    found = true;
                    break;
                }
            }
            if(!found){
                printf("未找到商品：%s，请重新输入。\n", activity_discount_product_name.c_str());
                i--; // 重输本项
            }
        }
    }
    if(activity_type_full_reduction){
        activity_threshold = Read<double>("请输入满减阈值（元）：", [](const double& x){ return x > 0; });
        activity_full_reduction_amount = Read<double>("请输入满减金额（元）：", [&](const double& x){ return x > 0 && x <= activity_threshold; });
        
        int activity_full_reduction_products_count = Read<int>("请输入要添加进满减活动的商品数量：", [](const int& x){ return x > 0; });
        for(int i = 0; i < activity_full_reduction_products_count; i++){
            printf("请输入第%d个商品名称：", i+1);
            string activity_full_reduction_product_name;
            cin >> activity_full_reduction_product_name;
            
            // 查找商品并添加到满减商品列表
            bool found = false;
            for(size_t j = 0; j < ShoppingSystem::products.size(); j++){
                if(ShoppingSystem::products.at(j).GetProductName() == activity_full_reduction_product_name){
                    activity_full_reduction_products.push_back(ShoppingSystem::products.at(j));
                    found = true;
                    break;
                }
            }
            if(!found){
                printf("未找到商品：%s，请重新输入。\n", activity_full_reduction_product_name.c_str());
                i--; // 重输本项
            }
        }
    }
    
    //修改活动状态
    printf("请输入活动状态：1.进行中  2.已结束：");
    int activity_status_op = Read<int>("请输入活动状态：1.进行中  2.已结束：", [](const int& x){ return x==1 || x==2; });
    string activity_status = (activity_status_op == 1 ? "进行中" : "已结束");
    Activity activity(activity_name, activity_type_discount, activity_type_full_reduction, activity_start_time, activity_end_time, activity_discount, activity_threshold, activity_full_reduction_amount, activity_status, activity_discount_products, activity_full_reduction_products);
    ShoppingSystem::activities.push_back(activity);
    printf("活动添加成功！\n");
    //保存活动列表到文件
    FileProcess::SaveActivitiesToFile();
}

void Administrator::DeleteActivity(){
    printf("请输入要删除的活动名称：");
    string activity_name;
    cin >> activity_name;
    
    bool found = false;
    for(size_t i = 0; i < ShoppingSystem::activities.size(); i++){
        if(ShoppingSystem::activities.at(i).activity_name == activity_name){
            ShoppingSystem::activities.erase(ShoppingSystem::activities.begin() + i);
            found = true;
            break;
        }
    }
    if(found){
        printf("活动删除成功！\n");
    }else{
        printf("活动不存在，删除失败！\n");
    }
    //保存活动列表到文件
    FileProcess::SaveActivitiesToFile();
}

void Administrator::EditActivity(){
    //和添加活动一样，先删除原来的活动，再重新添加
    printf("请输入要修改的活动名称：");
    string activity_name;
    cin >> activity_name;
    bool found = false;
    for(size_t i = 0; i < ShoppingSystem::activities.size(); i++){
        if(ShoppingSystem::activities.at(i).activity_name == activity_name){
            ShoppingSystem::activities.erase(ShoppingSystem::activities.begin() + i);
            found = true;
            break;
        }
    }
    if(found){
        printf("找到活动，请重新输入活动信息：\n");
        AddActivity();
        printf("活动修改成功！\n");
    }else{
        printf("活动不存在，修改失败！\n");
    }
}
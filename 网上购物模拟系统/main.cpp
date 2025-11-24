//网上购物模拟系统主函数入口
#include "shopping_system.hpp" //只需要hpp头文件，不需要cpp文件
#include "administrator.hpp"
#include "customer.hpp"

using namespace std;

int main()
{
    ShoppingSystem shoppingSystem; //创建购物系统实例（变量）
    shoppingSystem.Run();
    //shoppingSystem.Exit();
    return 0;
}
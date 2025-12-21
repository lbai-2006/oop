#pragma once
#ifndef CART_HPP
#define CART_HPP

#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include "product.hpp"
 
using namespace std;
 
class Cart {
    friend class Customer;
    friend class ShoppingSystem;
    friend class Administrator;
    
    private:
        vector<pair<Product, int>> items; //vector会自动释放内存  
        string username;  
        
    public:
        Cart();  
        
        void SetUsername(string username);  
        void ClearCart();  
        
        void AddToCart(string product_name); 
        void RemoveFromCart(vector<string> product_names); 
        void ModifyCart(string product_name, int quantity); 
        void ShowCart(); 
        
        void LoadCustomerCartsFromList(); 
        void SaveCustomerCartsToList(); 
        
        size_t GetSize() const { return items.size(); }  
};

#endif
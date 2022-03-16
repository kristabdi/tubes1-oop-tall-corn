#include "Item.hpp"
#include <string>
#include <iostream>
using namespace std;
int Item::countItem=0;
Item::Item():Item("Item","type"){
}

Item::Item(string name, string type):ID(countItem){
    this->name=name;
    this->type=type;
    countItem++;
}

string Item::getName(){
    return this->name;
}
string Item::getType(){
    return this->type;
}
int Item::getID(){
    return this->ID;
}

void Item::printInfo(){
    cout << "ID\t\t: "<< this->ID <<endl;
    cout << "Name\t\t: "<< this->name <<endl;
    cout << "type\t\t: "<< this->type <<endl;

}
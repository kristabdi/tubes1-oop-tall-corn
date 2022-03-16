#ifndef _ITEM_HPP_
#define _ITEM_HPP_
#include <string>
using namespace std;
class Item {
    private:
        int ID;
        string name;
        string type;
        static int countItem;
    public:
        Item();
        Item(string name, string type);
        string getName();
        string getType();
        int getID();
        virtual void printInfo();
};


#endif
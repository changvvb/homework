#ifndef FOOD_H
#define FOOD_H
#include <commodity.hpp>
#include <iostream>
using namespace std;
class food : commodity {
    public:
        void checkquantity() {
            for(list<commodity*>::iterator pc = commodity_list.begin(); pc != commodity_list.end(); pc ++) {
                cout << (*pc)->quantity << endl;
            }
        }
};

#endif

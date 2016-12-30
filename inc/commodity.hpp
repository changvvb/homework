#ifndef COMMODITY_H
#define COMMODITY_H

#include <ctime>
#include <list>
#include <cstring>
#include <fstream>

#define FOOD      1
#define DRESS     2
#define APPLIANCE 3
using namespace std;

class commodity {
    protected:   
        int id;
        char name[20];
        int type;
        float purchase_price;
        float sales_price; 
        static int quantity;
        time_t time;            
        time_t shelf_life;
        int season;

    public:
        static list<commodity*> commodity_list;
        char describe[64];

        commodity(){}
        commodity(int i,char* n,int t,int s,float purches,float sales) {
            strcpy(name,n);
            id = i;
            type = t;
            season = s;
            purchase_price = purches;
            sales_price = sales;
        }

        void set_id(int i) {
            id = i;
        }

        void set_name(char *n) {
            strcpy(name,n);
        }

        void set_type(int t) {
            type = t;
        }

        void set_purchase_price(float f) {
            purchase_price = f;
        }

        void set_sales_price(float f) {
            sales_price = f;
        }

        void set_season(int s) {
            season = s;
        }
        
        int get_id() {
            return id;
        }

        int get_season() {
            return season;
        }

        int get_type(){
            return type;
        }

        float get_purchase_price(){
            return purchase_price;
        }

        float get_sales_price(){
            return sales_price;
        } 

        char * get_name() {
            return name;
        }

        static void additem(commodity* c) {
            commodity_list.push_front(c);
        }

        static void delitem(commodity* c) {
            commodity_list.remove(c);
        }


        static commodity* get_commodity_by_id(int id){
            for(list<commodity*>::iterator e=commodity::commodity_list.begin();e!=commodity::commodity_list.end();e++) {
        if((*e)->id == id) {
            return *e;
        }
    }
    return NULL;

        }



        static void checkquantity() {

        }

        static void save() {
            ofstream out("./commodity",ios::out|ios::binary);
            for (list<commodity*>::iterator pe = commodity_list.begin(); pe != commodity_list.end(); pe++){
                out.write((char*)(*pe),sizeof(commodity));
            }
            out.close();
        }

        static void load() {
            int count = 0;
            commodity*  laste;
            ifstream in("./commodity",ios::in|ios::binary);
            commodity_list.clear();
            while(!in.eof()) {
                commodity *e = new commodity();
                in.read(reinterpret_cast<char *>(e), sizeof(commodity));
                if(e->id == 0) {
                    e->id = count;
                    count ++;
                }
                commodity::additem(e);
                laste = e;
            }
            commodity_list.remove(laste);

            in.close();
        }
        // virtual void checkquality();
};

list<commodity*> commodity::commodity_list;

#endif

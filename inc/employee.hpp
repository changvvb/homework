#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <iostream>
#include <cstring>
#include <fstream>
#include <curses.h>
#include <list>

using namespace std;

#define MANAGER 1
#define SALEMAN 2
#define WAREMAN 3

class employee;
employee* get_new_obj(employee* e); 
class employee{
    protected:
        int id;
        char name[20];
        char tag[10];
        int counter;
        int position;
        int month;
        float salary;
        float money;
    public:
        employee() {}
        employee(int i,char *n,int c,int p,int m,float mon) {
            id = i;
            strcpy(name,n);
            // strcpy(name,"changvvb");
            counter = c;
            position = p;
            month = m;
            money = mon;
        }
        char describe[64];
        int get_id() {
            return id;
        }

        char* get_name() {
            return name;
        }

        char* get_tag() {
            return tag;
        }

        float get_money() {
            return money;
        }

        float get_salary() {
            return salary;
        }

        int get_counter() {
            return counter;
        } 

        int get_month() {
            return month;
        }

        int get_position() {
            return position;
        }

        void set_money(float m) {
            money = m;
        }

        void set_id(int i) {
            id = i;
        }

        virtual void set_position(int p) {
            position = p;
        }

        void set_counter(int c) {
            counter = c;
        }

        void set_month(int m) {
            month = m;
        }
        static char* employee_file;
        static list<employee*> employee_list;

        static void addemployee(employee* e){
            employee_list.push_front(e); 
        }

        static void delemployee(employee* e){
            employee_list.remove(e);
        }

        static void listemployee() {
            int count = 0;
            for (list<employee*>::iterator pe = employee_list.begin(); pe != employee_list.end(); pe++) {
                count ++;
                mvprintw(count,30,"employee:%d %d",(*pe)->id,(*pe)->position);
            }
        }

        static employee* get_employee_by_id(int id){
            for(list<employee*>::iterator e=employee::employee_list.begin();e!=employee::employee_list.end();e++) {
        if((*e)->id == id) {
            return *e;
        }
    }
    return NULL;

        }

        void set_name(char *n) {
            strcpy(name,n);
        }


        void statuschange(int counter,int position) {

        }

        int findstatus() {
            return id;
        }

        void recordsalary() {
        }

        void query() {

        }
        
        virtual void calculate_salary(){
            salary = 0;
        }
        
        static void save() {
            ofstream out(employee_file,ios::out|ios::binary);
            for (list<employee*>::iterator pe = employee_list.begin(); pe != employee_list.end(); pe++){
                out.write((char*)(*pe),sizeof(employee));
            }
            out.close();
        }

        static void load() {
            int count = 0;
            employee*  laste;
            ifstream in(employee_file,ios::in|ios::binary);
            employee_list.clear();
            employee *e = new employee();
            while(!in.eof()) {
                employee *emp = NULL;
                in.read(reinterpret_cast<char *>(e), sizeof(employee));
                if(e->id == 0) {
                    e->id = count;
                    count ++;
                }
                emp = get_new_obj(e);
                if (emp !=NULL) 
                {
                    employee::addemployee(emp);
                    laste = emp;
                }
            }
            employee_list.remove(laste);

            in.close();
        }
};


class manager : public employee {
    public:
        manager(){}
        manager(employee& e) {
            id = e.get_id();
            strcpy(name,e.get_name());
            month = e.get_month();
            counter = e.get_counter();
            strcpy(tag,"manager");
            money = e.get_money();
            position = e.get_position();
        }

        virtual void calculate_salary(){
            int m = 0;
            for (list<employee*>::iterator pe = employee_list.begin(); pe != employee_list.end(); pe++){
                if(counter == (*pe)->get_counter())
                m += (*pe)->get_money(); 
            }
            salary = 4000 + m*0.005;
        }
        virtual void set_position(int p) {
            if(p!= position) {
                position = p;
                employee* emp = get_new_obj(this);
                employee::addemployee(emp); 
                delete(this);
            }
        }
        ~manager() {
            employee::delemployee(this);
        }

};

class saleman: public employee {
    public:
        saleman(employee& e) {
            id = e.get_id();
            strcpy(name,e.get_name());
            month = e.get_month();
            counter = e.get_counter();
            strcpy(tag,"saleman");
            money = e.get_money();
            position = e.get_position();
        }

        virtual void calculate_salary(){
            salary = 2000 + money*0.05;
        } 
        virtual void set_position(int p) {
            if(p!= position) {
                position = p;
                employee* emp = get_new_obj(this);
                employee::addemployee(emp); 
                delete(this);
            }
        }
        ~saleman() {
            employee::delemployee(this);
        }

};


class warehouseman: public employee {
    private:
    public:
        warehouseman(employee& e) {
            id = e.get_id();
            strcpy(name,e.get_name());
            month = e.get_month();
            counter = e.get_counter();
            strcpy(tag,"wareman");
            money = e.get_money();
            position = e.get_position();
        }

        virtual void calculate_salary(){
            salary = 3000 + money; 
        }

        virtual void set_position(int p) {
            if(p!= WAREMAN) {
                position = p;
                employee* emp = get_new_obj(this);
                employee::addemployee(emp); 
                delete(this);
            }
        }
        ~warehouseman() {
            employee::delemployee(this);
        }
};


#endif


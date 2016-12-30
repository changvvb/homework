#ifndef MANAGER_H 
#define MANAGER_H
#include <employee.hpp>
#include <string.h>

class manager : public employee {
    public:
        manager(){}
        manager(employee& e) {
            id = e.get_id();
            strcpy(name,e.get_name());
            month = e.get_month();
            counter = e.get_counter();
        }

        void calculate_salary() {
            int m = 0;
            for (list<employee*>::iterator pe = employee_list.begin(); pe != employee_list.end(); pe++){
                if(counter == (*pe)->get_counter())
                m += (*pe)->get_money(); 
            }
            salary = 4000 + m*0.005;
        }
};



#endif

#include <employee.hpp>
#include<list>


list<employee*> employee::employee_list;//定义员工列表
char* employee::employee_file = (char*)"./employee";//定义文件名

employee* get_new_obj(employee* e) {       //职位升降
    int p = e->get_position();
    employee* emp = NULL;
    if (p == MANAGER) {
        emp = new manager(*e);
    } else if(p == SALEMAN)
        emp = new saleman(*e);
    else if (p == WAREMAN )
        emp = new warehouseman(*e);
    return emp;

}

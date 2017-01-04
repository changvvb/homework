#include <stdlib.h>
#include <curses.h>
#include <menu.h>
#include <form.h>
#include <string.h>
#include <employee.hpp>
#include <commodity.hpp>
#include <list>
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD

MENU *cur_menu;

ITEM **level1_items;
MENU *level1_menu;

MENU *employee_menu;
ITEM **employee_items;

MENU *commodity_menu;
ITEM **commodity_items;

MENU *employee_list_menu;
MENU *commodity_list_menu;

#define N_FIELDS 7
#define FORM_WIDTH 20
#define FORM_STARTX 30
#define FORM_STARTY 10
FORM* employee_form;
FIELD *employee_field[N_FIELDS];
FORM* cur_form;

#define COMMODITY_FORM_N 7
FORM* commodity_form;
FIELD *commodity_field[COMMODITY_FORM_N];


employee* cur_employee;
commodity* cur_commodity;

typedef void (*func_ptr)(char *);

char *level1_choices[] = {
    (char*)"Employee manage",
    (char*)"Commodity manage",
    (char*)"EXIT",
};


char *employee_choices[] = {
    (char*)"Add",
    (char*)"List",
    (char*)"Load",
    (char*)"Search",
    (char*)"Back",
};

char *commodity_choices[] = {
    (char*)"Add",
    (char*)"List",
    (char*)"Load",
    (char*)"Search",
    (char*)"Back",
};

char idbuf[64];
char name[20];
char name2[20];
char name3[20];
char name4[20];

void debug_print(const char* format,...) {
    mvprintw(LINES-1,2,format);
}

void commodity_form_init() {
    for(int i = 0; i < COMMODITY_FORM_N- 1; ++i) {
        commodity_field[i] = new_field(1, FORM_WIDTH, FORM_STARTY + i * 2, FORM_STARTX, 0, 20);
        set_field_back(commodity_field[i], A_UNDERLINE);
        // field_opts_on(employee_field[0], O_STATIC);
    }
    set_field_type(commodity_field[0],TYPE_INTEGER,0,0,99999999); //id
    set_field_type(commodity_field[1],TYPE_ALPHA,3);        //name
    set_field_type(commodity_field[2],TYPE_INTEGER,1,1,3); //type
    set_field_type(commodity_field[3],TYPE_NUMERIC,2,0,999999999); //purchase_price
    set_field_type(commodity_field[4],TYPE_NUMERIC,2,0,999999999); //sales_price
    set_field_type(commodity_field[5],TYPE_INTEGER,1,1,4); //sales_price
    employee_field[COMMODITY_FORM_N- 1] = NULL;
    commodity_form= new_form(commodity_field);
}

void employee_form_init() {
    for(int i = 0; i < N_FIELDS - 1; ++i) {
        employee_field[i] = new_field(1, FORM_WIDTH, FORM_STARTY + i * 2, FORM_STARTX, 0, 20);
        set_field_back(employee_field[i], A_UNDERLINE);
        // field_opts_on(employee_field[0], O_STATIC);
    }
    set_field_type(employee_field[0],TYPE_INTEGER,0,0,99999999);
    set_field_type(employee_field[1],TYPE_ALPHA,3);
    set_field_type(employee_field[2],TYPE_INTEGER,0,1,3);
    set_field_type(employee_field[3],TYPE_INTEGER,0,0,100);
    set_field_type(employee_field[4],TYPE_INTEGER,1,1,12);
    set_field_type(employee_field[5],TYPE_NUMERIC,2,0,999999999);
    employee_field[N_FIELDS - 1] = NULL;
    employee_form = new_form(employee_field);
}

void commodity_mod_form_show(commodity* e) {
    cur_form = commodity_form;
    unpost_menu(cur_menu);
    post_form(cur_form);
    set_current_field(cur_form, commodity_field[0]);

    char buf[64];
    char buf1[64];
    char buf2[64];
    char buf3[64];
    sprintf(buf,"%d",e->get_id());
    set_field_buffer(commodity_field[0],0,buf);
    set_field_buffer(commodity_field[1],0,e->get_name());
    sprintf(buf1,"%d",e->get_type());
    set_field_buffer(commodity_field[2],0,buf1);
    sprintf(buf2,"%f",e->get_purchase_price());
    set_field_buffer(commodity_field[3],0,buf2);
    sprintf(buf3,"%f",e->get_sales_price());
    set_field_buffer(commodity_field[4],0,buf3);
    sprintf(buf,"%d",e->get_season());
    set_field_buffer(commodity_field[5],0,buf);

    mvprintw(FORM_STARTY+0*2,FORM_STARTX-15,"ID:");
    mvprintw(FORM_STARTY+1*2,FORM_STARTX-15,"Name:");
    mvprintw(FORM_STARTY+2*2,FORM_STARTX-15,"Type:");
    mvprintw(FORM_STARTY+3*2,FORM_STARTX-15,"Purchase price:");
    mvprintw(FORM_STARTY+4*2,FORM_STARTX-15,"Sales price:");
    mvprintw(FORM_STARTY+5*2,FORM_STARTX-15,"Season:");
}




void employee_mod_form_show(employee* e) {
    cur_form = employee_form;
    unpost_menu(cur_menu);
    post_form(cur_form);
    set_current_field(cur_form, employee_field[0]);

    char buf[64];
    char buf1[64];
    char buf2[64];
    char buf3[64];
    sprintf(buf,"%d",e->get_id());
    set_field_buffer(employee_field[0],0,buf);
    set_field_buffer(employee_field[1],0,e->get_name());
    sprintf(buf1,"%d",e->get_position());
    set_field_buffer(employee_field[2],0,buf1);
    sprintf(buf2,"%d",e->get_counter());
    set_field_buffer(employee_field[3],0,buf2);
    sprintf(buf3,"%d",e->get_month());
    set_field_buffer(employee_field[4],0,buf3);
    sprintf(buf,"%f",e->get_money());
    set_field_buffer(employee_field[5],0,buf);

    mvprintw(FORM_STARTY+0*2,FORM_STARTX-10,"ID:");
    mvprintw(FORM_STARTY+1*2,FORM_STARTX-10,"Name:");
    mvprintw(FORM_STARTY+2*2,FORM_STARTX-10,"Position:");
    mvprintw(FORM_STARTY+3*2,FORM_STARTX-10,"Counter:");
    mvprintw(FORM_STARTY+4*2,FORM_STARTX-10,"Month:");
    mvprintw(FORM_STARTY+5*2,FORM_STARTX-10,"Money:");

    e->calculate_salary();

    mvprintw(FORM_STARTY+6*2,FORM_STARTX-10,"Salary:    %f",e->get_salary());
}


void commodity_form_show() {
    commodity_form_init();
    cur_commodity = NULL;
    cur_form = commodity_form;
    unpost_menu(cur_menu);
    post_form(cur_form);
    set_current_field(cur_form, commodity_field[0]);

    mvprintw(FORM_STARTY+0*2,FORM_STARTX-15,"ID:");
    mvprintw(FORM_STARTY+1*2,FORM_STARTX-15,"Name:");
    mvprintw(FORM_STARTY+2*2,FORM_STARTX-15,"Type:");
    mvprintw(FORM_STARTY+3*2,FORM_STARTX-15,"Purchase price:");
    mvprintw(FORM_STARTY+4*2,FORM_STARTX-15,"Sales price:");
    mvprintw(FORM_STARTY+5*2,FORM_STARTX-15,"Season:");
}

void employee_form_show() {
    employee_form_init();
    cur_employee = NULL;
    cur_form = employee_form;
    unpost_menu(cur_menu);
    post_form(cur_form);
    set_current_field(cur_form, employee_field[0]);

    mvprintw(FORM_STARTY+0*2,FORM_STARTX-10,"ID:");
    mvprintw(FORM_STARTY+1*2,FORM_STARTX-10,"Name:");
    mvprintw(FORM_STARTY+2*2,FORM_STARTX-10,"Position:");
    mvprintw(FORM_STARTY+3*2,FORM_STARTX-10,"Counter:");
    mvprintw(FORM_STARTY+4*2,FORM_STARTX-10,"Month:");
    mvprintw(FORM_STARTY+5*2,FORM_STARTX-10,"Money:");
}

void commodity_form_submit(commodity* e) {
    int id,type,season;
    float purchase_price,sales_price;
    char name[20];
    form_driver(cur_form,REQ_VALIDATION);
    sscanf(field_buffer(commodity_field[0],0),"%d",&id);
    sscanf(field_buffer(commodity_field[1],0),"%s",name);
    sscanf(field_buffer(commodity_field[2],0),"%d",&type);
    sscanf(field_buffer(commodity_field[3],0),"%f",&purchase_price);
    sscanf(field_buffer(commodity_field[4],0),"%f",&sales_price);
    sscanf(field_buffer(commodity_field[5],0),"%d",&season);
    if (e == NULL) {
        e = new commodity(id,name,type,season,purchase_price,sales_price);
        commodity::additem(e);
        post_menu(cur_menu);
    }
    else { //just modify
        e->set_id(id);
        e->set_name(name);
        e->set_type(type);
        e->set_purchase_price(purchase_price);
        e->set_sales_price(sales_price);
        e->set_season(season);
        cur_menu = commodity_list_menu;
        post_menu(cur_menu);
    }
    commodity::save();
    // unpost_form(cur_form);
    cur_form = NULL;
}



void employee_form_submit(employee* e) {
    int id,counter,position,month;
    float money;
    char name[20];
    form_driver(cur_form,REQ_VALIDATION);
    sscanf(field_buffer(employee_field[0],0),"%d",&id);
    sscanf(field_buffer(employee_field[1],0),"%s",name);
    sscanf(field_buffer(employee_field[2],0),"%d",&position);
    sscanf(field_buffer(employee_field[3],0),"%d",&counter);
    sscanf(field_buffer(employee_field[4],0),"%d",&month);
    sscanf(field_buffer(employee_field[5],0),"%f",&money);
    if (e == NULL) {
        e = new employee(id,name,counter,position,month,money);
        employee* emp= get_new_obj(e);
        employee::addemployee(emp);
        delete(e);
        post_menu(cur_menu);
    }
    else { //just modify
        e->set_id(id);
        e->set_name(name);
        e->set_position(position);
        e->set_counter(counter);
        e->set_month(month);
        e->set_money(money);
        cur_menu = employee_list_menu;
        post_menu(cur_menu);
    }
    employee::save();
    cur_form = NULL;
    mvprintw(LINES-1,2,"id,%d",id);
    mvprintw(LINES-2,2,"name:%s",name);
    mvprintw(LINES-3,2,"position,%d",position);
    mvprintw(LINES-4,2,"counter,%d",counter);
    mvprintw(LINES-5,2,"month,%d",month);
    noecho();
}

void level1_func(char* name) {
    unpost_menu(cur_menu);
    if(!strcmp(name,level1_choices[0])){
        cur_menu = employee_menu;
    }else if(!strcmp(name,level1_choices[1])){
        cur_menu = commodity_menu;
    }else {
        exit(0);
    }
    post_menu(cur_menu);

}
void* commodity_list_func(char* s) {
    int id;
    if (!strcmp(s,"Back")){
        unpost_menu(cur_menu);
        cur_menu = commodity_menu;
        post_menu(cur_menu);
    }
    sscanf(s,"id:%d",&id);
    commodity* e = commodity::get_commodity_by_id(id);
    if (e != NULL) {
        cur_commodity = e;
        commodity_mod_form_show(cur_commodity);
    }else {
        mvprintw(LINES-1,2,"No result");
    }
}



void* employee_list_func(char* s) {
    int id;
    if (!strcmp(s,"Back")){
        unpost_menu(cur_menu);
        cur_menu = employee_menu;
        post_menu(cur_menu);
    }
    sscanf(s,"id:%d",&id);
    employee* e = employee::get_employee_by_id(id);
    if (e != NULL) {
        cur_employee = e;
        employee_mod_form_show(cur_employee);
    }else {
        mvprintw(LINES-1,2,"No result");
    }
}
//内存的清理
void employee_list_item_clear() {
    int n = ARRAY_SIZE(employee_items);
    for(int i=0;i<n;i++)
        free_item(employee_items[i]);
    delete(employee_items);
}


void commodity_list_menu_init() {
    int n = commodity::commodity_list.size() + 1;//获得链表大小
    commodity_items= (ITEM**)calloc(n+1,sizeof(ITEM*));
    int i = 0;
    for(list<commodity*>::iterator e=commodity::commodity_list.begin();e!=commodity::commodity_list.end();e++,i++) {
        snprintf((*e)->describe,62,"id:%d name:%s ",(*e)->get_id(),(*e)->get_name());
        commodity_items[i] = new_item((*e)->describe,"");
        set_item_userptr(commodity_items[i],(void*)commodity_list_func);
    }
    commodity_items[n-1] = new_item("Back","");
    set_item_userptr(commodity_items[n-1],(void*)commodity_list_func);
    commodity_items[n] = (ITEM*)NULL;
    commodity_list_menu = new_menu(commodity_items);
    debug_print("size:%d",n);
}

void employee_list_menu_init() {
    int n = employee::employee_list.size() + 1;
    employee_items= (ITEM**)calloc(n+1,sizeof(ITEM*));
    int i = 0;
    //用迭代器将员工信息存到数组describe
    for(list<employee*>::iterator e=employee::employee_list.begin();e!=employee::employee_list.end();e++,i++) {
        snprintf((*e)->describe,62,"id:%d name:%s tag:%s",(*e)->get_id(),(*e)->get_name(),(*e)->get_tag());
        employee_items[i] = new_item((*e)->describe,"");//
        set_item_userptr(employee_items[i],(void*)employee_list_func);//制表
    }
    employee_items[n-1] = new_item("Back","");
    set_item_userptr(employee_items[n-1],(void*)employee_list_func);
    employee_items[n] = (ITEM*)NULL;
    employee_list_menu = new_menu(employee_items);
}

void commodity_func(char* name) {
    unpost_menu(cur_menu);//不显示菜单
    if(!strcmp(name,commodity_choices[0])){ //add
        commodity_form_show();
    }else if(!strcmp(name,commodity_choices[1])){ //list
        commodity_list_menu_init();
        unpost_menu(cur_menu);//
        cur_menu = commodity_list_menu;//
        post_menu(commodity_list_menu);//替換上一级
    }else if(!strcmp(name,commodity_choices[2])){ //load
        commodity::load();
    }else if(!strcmp(name,commodity_choices[3])){ //search
        int id;
        echo();
        mvprintw(5,30,"Please input id: ");
        scanw("%d",&id);
        commodity* e = commodity::get_commodity_by_id(id);
        if(e != NULL) {
            cur_commodity = e;
            commodity_mod_form_show(cur_commodity);
        } else {
            mvprintw(7,30,"No result, id:%d",id);
        }
        noecho();
    }
    else if(!strcmp(name,commodity_choices[4])){
        unpost_menu(cur_menu);
        cur_menu = level1_menu;
        post_menu(level1_menu);
    }

    post_menu(cur_menu);

}

void employee_func(char* name) {
    unpost_menu(cur_menu);
    if(!strcmp(name,employee_choices[0])){ //add
        employee_form_show();
    }else if(!strcmp(name,employee_choices[1])){ //list
        employee_list_menu_init();
        unpost_menu(cur_menu);
        cur_menu = employee_list_menu;
        post_menu(employee_list_menu);
    }else if(!strcmp(name,employee_choices[2])){ //load
        employee::load();
     }else if(!strcmp(name,commodity_choices[3])){ //search
        int id;
        echo();
        mvprintw(5,30,"Please input id: ");
        scanw("%d",&id);
        employee *e = employee::get_employee_by_id(id);
        if(e != NULL) {
            cur_employee = e;
            employee_mod_form_show(cur_employee);
        } else {
            mvprintw(7,30,"No result, id:%d",id);
        }
        noecho();
    }

    else if(!strcmp(name,employee_choices[4])){
        unpost_menu(cur_menu);
        cur_menu = level1_menu;
        post_menu(level1_menu);
    }
    post_menu(cur_menu);
}
//员工菜单
void employee_menu_init() {
    int n = sizeof(employee_choices)/sizeof(employee_choices[0]);
    ITEM **employee_items= (ITEM**)calloc(n+1,sizeof(ITEM*));
    for(int i=0;i<n;i++) {
        employee_items[i] = new_item(employee_choices[i],"employee");
        set_item_userptr(employee_items[i],(void*)employee_func);
    }
    employee_items[n] = (ITEM*)NULL;
    employee_menu = new_menu(employee_items);
}

//商品菜单
void commodity_menu_init() {
    int n = sizeof(commodity_choices)/sizeof(commodity_choices[0]);
    commodity_items = (ITEM**)calloc(n+1,sizeof(ITEM*));
    for(int i=0;i<n;i++) {
        commodity_items[i] = new_item(commodity_choices[i],"commodity");
        set_item_userptr(commodity_items[i],(void*)commodity_func);
    }
    commodity_items[n] = (ITEM*)NULL;
    commodity_menu= new_menu(commodity_items);
}

//一级菜单
void level1_menu_init() {
    int n = 3;
    level1_items = (ITEM**)calloc(n+1,sizeof(ITEM*));
    for(int i=0;i<n;i++) {
        level1_items[i] = new_item(level1_choices[i],"");//给每一个分配指针分配空间
        set_item_userptr(level1_items[i],(void*)level1_func);//选择指针执行对应函数
    }
    level1_items[n] = (ITEM*)NULL;//最后一个指针=NULL
    level1_menu = new_menu(level1_items);//制出菜单
}
//窗体
void curses_init() {
    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(1);
    keypad(stdscr,TRUE);
    init_pair(1, COLOR_RED, COLOR_WHITE);
    init_pair(2, COLOR_GREEN, COLOR_WHITE);
    init_pair(3, COLOR_MAGENTA, COLOR_WHITE);
    move(10,10);
    set_menu_mark(cur_menu, "--> ");
    set_menu_fore(cur_menu, COLOR_PAIR(1) | A_REVERSE);
    set_menu_back(cur_menu, COLOR_PAIR(2));
}

void startwin() {

    int c;
    curses_init();      //初始化ncurses库（图形库）
    employee_menu_init();   //初始化员工菜单
    commodity_menu_init();  //初始化商品菜单
    level1_menu_init();     //一级菜单（员工，商品）初始化
    employee_form_init();   //员工表单初始化
    cur_menu = level1_menu;     //把当前菜单设置为一级菜单
    post_menu(cur_menu);        //显示当前菜单
    refresh();      //刷新界面
    //当按下F2时退出程序
    while((c = getch()) != KEY_F(2)) {
        //form
        if(cur_form != NULL) {      //判断当前是否表单，否则操作菜单
            switch(c) {
                case KEY_DOWN:  //跳到下一个表单字段，光标行末
                    form_driver(cur_form, REQ_NEXT_FIELD);
                    form_driver(cur_form, REQ_END_LINE);
                    break;
                case KEY_UP://上
                    form_driver(cur_form, REQ_PREV_FIELD);
                    form_driver(cur_form, REQ_END_LINE);
                    break;
                case KEY_BACKSPACE://退格
                    form_driver(cur_form,REQ_DEL_PREV);
                    break;
                case 10: //enter
                    if (cur_form == employee_form)
                        employee_form_submit(cur_employee);
                    else if(cur_form == commodity_form)
                        commodity_form_submit(cur_commodity);
                    break;
                case KEY_F(9)://退出表单编辑
                    cur_form = NULL;
                    post_menu(cur_menu);
                    break;
                default:
                    form_driver(cur_form, c);//读取输入内容
            }

        }
        else {

            switch(c) {
                case KEY_DOWN://下一个菜单项目
                    menu_driver(cur_menu,REQ_DOWN_ITEM);
                    break;
                case KEY_UP:
                    menu_driver(cur_menu,REQ_UP_ITEM);
                    break;
                case 'd'://删除
                    if(cur_menu == employee_list_menu) {//判断是否为员工列表菜单
                        mvprintw(5,30,"Are you sure to delete? y/n");//在No.5行,No.30列插入提醒
                        int ch = getch(); //获取字符
                        if (ch == 'y' || ch == 'Y') {
                            ITEM *cur;
                            cur = current_item(cur_menu);//获取当前选中项
                            int id;
                            sscanf(item_name(cur),"id:%d",&id);//读取所删id
                            employee* e = employee::get_employee_by_id(id);
                            employee::delemployee(e);
                            employee::save();
                        }
                    } else  if(cur_menu == commodity_list_menu) {//判断是否为员工列表菜单
                        mvprintw(5,30,"Are you sure to delete? y/n");//在No.5行,No.30列插入提醒
                        int ch = getch(); //获取字符
                        if (ch == 'y' || ch == 'Y') {
                            ITEM *cur;
                            cur = current_item(cur_menu);//获取当前选中项
                            int id;
                            sscanf(item_name(cur),"id:%d",&id);//读取所删id
                            commodity* e = commodity::get_commodity_by_id(id);
                            commodity::delitem(e);
                            commodity::save();
                        }
                    }
                    break;
                case 10: //enter，执行用户指针函数
                    ITEM *cur;  //定义菜单选项指针
                    cur = current_item(cur_menu);   //获取当前菜单选项
                    func_ptr f = (func_ptr)item_userptr(cur); //获得该选项的功能函数指针
                    f((char*)item_name(cur));   //执行该函数
                    pos_menu_cursor(cur_menu);  //保存光标位置
                    break;
            }
        }
    }

    endwin();   //结束窗体
}

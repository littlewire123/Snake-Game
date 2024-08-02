#ifndef __SINGLE_MENU_H__
#define __SINGLE_MENU_H__

#include "../llist/llist.h"
#include "static.h"
#include "../Classic_Mode/classic_mode.h"
#include "../Challenge_Mode/challenge_mode.h"
#include "../Power_up_Mode/power_up_mode.h"
#include "./online_menu.h"


void submenu_select(int *mod);
void Classic_mod();
void Challenge_mod();
void Power_Up_mod();
void print_submenu(int highlight);
void get_terminal(int *rows, int *cols);
void print_centered_menu(const char **menu, int menu_size, int highlight);
int getch();


#endif
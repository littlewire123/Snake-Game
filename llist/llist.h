#ifndef __LLIST_H__
#define __LLIST_H__

#include <stdio.h>
#include "./share.h"
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

//函数指针
typedef void (llist_op_t)(const void *);
typedef int (llist_cmp_t)(const void * , const void *);
typedef void (llist_mod_t)(void *, const void *);

//抽象数据类型
typedef struct node_t{
	void *data;
	struct node_t *next;
	struct node_t *prev;
}NODE;

typedef struct llist_t{
	NODE head;
	int size;
	int count;	
}LLIST;

//==============insert======================
void llist_append(LLIST *handle, void *data);
void llist_appprev(LLIST *handle, void *data);
void llist_index_insert(LLIST *handle, void *data, int index);

//================del=================
void llist_first_del(LLIST *handle, void *key, llist_cmp_t *cmp);
void llist_end_del(LLIST *handle, void *key, llist_cmp_t *cmp);
void llist_all_del(LLIST *handle, void *key, llist_cmp_t *cmp);
void llist_index_del(LLIST *handle, int index);

//=============find=======================
void *llist_first_find(LLIST *handle, void *key, llist_cmp_t *cmp);
void *llist_end_find(LLIST *handle, void *key, llist_cmp_t *cmp);
LLIST *llist_all_find(LLIST *handle, void *key, llist_cmp_t *cmp);
void *llist_index_find(LLIST *handle, int index);

//===============mod=====================
void llist_first_mod(LLIST *handle, void *old_key, llist_cmp_t *cmp, void *new_key, llist_mod_t *mod);
void llist_end_mod(LLIST *handle, void *old_key, llist_cmp_t *cmp, void *new_key, llist_mod_t *mod);
void llist_all_mod(LLIST *handle, void *old_key, llist_cmp_t *cmp, void *new_key, llist_mod_t *mod);
void llist_index_mod(LLIST *handle, int index, void *new_key, llist_mod_t *mod);

//============sort=================
void llist_sort(LLIST *handle, llist_cmp_t *cmp);

//================fileio===============
void llist_store(LLIST *handle, const char *path);
LLIST *llist_load(const char *path);

//=================llist_copy==========
LLIST *llist_copy(LLIST *handle);

//==========llist_inti==============
LLIST *llist_creat(int size);
//===========llist_travel===========
void llist_travel(LLIST *handle, llist_op_t *op);
//==========llist_destroy============
void llist_destroy(LLIST **handle);
//==========llist_count=============
int llist_num(LLIST *handle);

#endif 

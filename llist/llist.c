#include "llist.h"

//==============insert======================
void llist_append(LLIST *handle, void *data)
{
    ERRP(NULL == data, data NULL, goto ERR1);

    NODE *new = NULL;

    new = (NODE *)malloc(sizeof(NODE));
    ERRP(NULL == new, malloc new, goto ERR1);

    new->data = (void *)malloc(handle->size);
    ERRP(NULL == new->data, new->data NULL, goto ERR2);

    memmove(new->data, data, handle->size);

    new->next = &handle->head;
    new->prev = handle->head.prev;
    handle->head.prev->next = new;
    handle->head.prev = new;

    handle->count++;

    return;
ERR2:
    free(new);
ERR1:
    return;
}

void llist_appprev(LLIST *handle, void *data)
{
    ERRP(NULL == data, data NULL, goto ERR1);

    NODE *new = NULL;

    new = (NODE *)malloc(sizeof(NODE));
    ERRP(NULL == new, malloc new, goto ERR1);

    new->data = (void *)malloc(handle->size);
    ERRP(NULL == new->data, new->data NULL, goto ERR2);

    memmove(new->data, data, handle->size);

    new->next = handle->head.next;
    new->prev = &handle->head;
    handle->head.next->prev = new;
    handle->head.next = new;

    handle->count++;
    return;
ERR2:
    free(new);
ERR1:
    return;
}

void llist_index_insert(LLIST *handle, void *data, int index)
{
    ERRP(index > handle->count || index < 0, index error, goto ERR1);
    ERRP(NULL == data, data NULL, goto ERR1);

    NODE *new = NULL;

    new = (NODE *)malloc(sizeof(NODE));
    ERRP(NULL == new, malloc new, goto ERR1);

    new->data = (void *)malloc(handle->size);
    ERRP(NULL == new->data, new->data NULL, goto ERR2);

    memmove(new->data, data, handle->size);

    NODE *tail = &handle->head;
    int count = 0;
    while (count != index)
    {
        tail = tail->next;
        count++;
    }
    new->next = tail->next;
    new->prev = tail;
    tail->next->prev = new;
    tail->next = new;

    handle->count++;
    return;
ERR2:
    free(new);
ERR1:
    return;
}

//================del=================
void llist_first_del(LLIST *handle, void *key, llist_cmp_t *cmp)
{
    NODE *tail = NULL;
    for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        if (cmp(tail->data, key) == 0)
        {
            tail->prev->next = tail->next;
            tail->next->prev = tail->prev;
            free(tail->data);
            free(tail);
            handle->count--;
            return;
        }
    }
    return;
}

void llist_end_del(LLIST *handle, void *key, llist_cmp_t *cmp)
{
    NODE *tail = NULL;
    for (tail = handle->head.prev; tail != &handle->head; tail = tail->prev)
    {
        if (cmp(tail->data, key) == 0)
        {
            tail->prev->next = tail->next;
            tail->next->prev = tail->prev;
            free(tail->data);
            free(tail);
            handle->count--;
            return;
        }
    }
    return;
}

void llist_all_del(LLIST *handle, void *key, llist_cmp_t *cmp)
{
    NODE *tail = NULL, *new = NULL;
    for (tail = handle->head.next; tail != &handle->head; tail = new)
    {
        if (cmp(tail->data, key) == 0)
        {
            new = tail->next;
            tail->prev->next = tail->next;
            tail->next->prev = tail->prev;
            free(tail->data);
            free(tail);
            handle->count--;
            tail = tail->prev;
        }
        else
        {
            new = tail->next;
        }
    }
    return;
}

void llist_index_del(LLIST *handle, int index)
{
    ERRP(index >= handle->count || index < 0, index error, goto ERR1);

    int count = 0;
    NODE *tail = handle->head.next;
    while (count != index)
    {
        tail = tail->next;
        count++;
    }
    tail->prev->next = tail->next;
    tail->next->prev = tail->prev;
    free(tail->data);
    free(tail);
    handle->count--;
    return;

ERR1:
    return;
}

//=============find=======================
void *llist_first_find(LLIST *handle, void *key, llist_cmp_t *cmp)
{
    NODE *tail = NULL;

    for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        if (cmp(tail->data, key) == 0)
        {
            return tail->data;
        }
    }

    return NULL;
}

void *llist_end_find(LLIST *handle, void *key, llist_cmp_t *cmp)
{
    NODE *tail = NULL;

    for (tail = handle->head.prev; tail != &handle->head; tail = tail->prev)
    {
        if (cmp(tail->data, key) == 0)
        {
            return tail->data;
        }
    }

    return NULL;
}

LLIST *llist_all_find(LLIST *handle, void *key, llist_cmp_t *cmp)
{
    LLIST *temp = NULL;
    temp = llist_creat(handle->size);
    ERRP(NULL == temp, malloc temp, goto ERR1);

    NODE *tail = NULL;
    for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        if (cmp(tail->data, key) == 0)
        {
            llist_append(temp, tail->data);
        }
    }
    return temp;
ERR1:
    return NULL;
}

void *llist_index_find(LLIST *handle, int index)
{
    NODE *tail = NULL;
    int count = 0;
    for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        if (index == count)
        {
            return tail->data;
        }
        else
        {
            count++;
        }
    }

    return NULL;
}

//===============mod=====================
void llist_first_mod(LLIST *handle, void *old_key, llist_cmp_t *cmp, void *new_key, llist_mod_t *mod)
{
    NODE *tail = NULL;

    for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        if (cmp(tail->data, old_key) == 0)
        {
            mod(tail->data, new_key);
            return;
        }
    }

    return;
}

void llist_end_mod(LLIST *handle, void *old_key, llist_cmp_t *cmp, void *new_key, llist_mod_t *mod)
{
    NODE *tail = NULL;

    for (tail = handle->head.prev; tail != &handle->head; tail = tail->prev)
    {
        if (cmp(tail->data, old_key) == 0)
        {
            mod(tail->data, new_key);
            return;
        }
    }

    return;
}

void llist_all_mod(LLIST *handle, void *old_key, llist_cmp_t *cmp, void *new_key, llist_mod_t *mod)
{
    NODE *tail = NULL;

    for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        if (cmp(tail->data, old_key) == 0)
        {
            mod(tail->data, new_key);
        }
    }

    return;
}

void llist_index_mod(LLIST *handle, int index, void *new_key, llist_mod_t *mod)
{
    ERRP(index > handle->count || index < 0, index error, goto ERR1);
    void *tail = llist_index_find(handle, index);
    mod(tail, new_key);

ERR1:
    return;
}

//============sort=================
void llist_sort(LLIST *handle, llist_cmp_t *cmp)
{
    NODE *i = NULL;
    NODE *j = NULL;
    void *new = NULL;

    new = (void *)malloc(handle->size);
    ERRP(NULL == new, new NULL, goto ERR1);

    for (i = handle->head.next; i != &handle->head; i = i->next)
    {
        for (j = handle->head.next; j->next != &handle->head; j = j->next)
        {
            if (cmp(j->data, j->next->data))
            {
                memmove(new, j->data, handle->size);
                memmove(j->data, j->next->data, handle->size);
                memmove(j->next->data, new, handle->size);
            }
        }
    }
    free(new);

    return;
ERR1:
    return;
}

//================fileio===============
void llist_store(LLIST *handle, const char *path)
{
    FILE *fp = NULL;
    NODE *tail = NULL;

    fp = fopen(path, "w");
    ERRP(NULL == fp, fp open, goto ERR1);

    if (fwrite(handle, sizeof(*handle), 1, fp) != 1)
    {
        perror("fwrite");
        goto ERR2;
    }

    for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        if(fwrite(tail->data , handle->size , 1 ,fp) != 1)
        {
            perror("fwrite");
            goto ERR2;
        }
    }

    fclose(fp);
    return ;
ERR2:
    fclose(fp);
ERR1:
    return;
}

LLIST *llist_load(const char *path)
{
    FILE *fp = NULL;
    LLIST *handle = NULL;
    void *tail = NULL;
    int i;
    
    handle = (LLIST*)malloc(sizeof(LLIST));
    ERRP(NULL == handle , malloc handle , goto ERR1);

    fp = fopen(path, "r");
    ERRP(NULL == fp, fp open, goto ERR2);

    if(fread(handle , sizeof(*handle) , 1 , fp) != 1)
    {
        perror("fread");
		goto ERR3;
    }

    handle->head.data = NULL;
    handle->head.next = &handle->head;
    handle->head.prev = &handle->head;
    
    for(i=0 ;i < handle->count ;i++)
    {
        tail = (void*)malloc(handle->size);
        ERRP(NULL == tail , tail malloc , goto ERR3);

        if(fread(tail , handle->size , 1 ,fp)!=1)
        {
            perror("fread");
		    goto ERR3;
        }
        
        llist_append(handle,tail);
        handle->count--;
    }

    fclose(fp);
    return handle;

ERR3:	
	fclose(fp);
    llist_destroy(&handle);
ERR2:
	free(handle);
ERR1:
	return NULL;
}

//=================llist_copy==========
LLIST *llist_copy(LLIST *handle)
{
    LLIST * newhandle = NULL;
    NODE *tail = handle->head.next;

    newhandle = llist_creat(handle->size);
    ERRP(NULL == newhandle , newhandle init , goto ERR1);

    while(tail != &handle->head)
    {
        llist_append(newhandle,tail->data);
        tail = tail->next;
    }

    return newhandle;
ERR1: 
    return NULL;
}

//==========llist_count============
int llist_num(LLIST *handle)
{
    return handle->count;
}

//==========llist_inti==============
LLIST *llist_creat(int size)
{
    LLIST *handle = NULL;

    handle = (LLIST *)malloc(sizeof(LLIST));
    ERRP(NULL == handle, malloc handle, goto ERR1);

    handle->head.data = NULL;
    handle->head.next = &handle->head;
    handle->head.prev = &handle->head;
    handle->size = size;
    handle->count = 0;

    return handle;
ERR1:
    return NULL;
}

//===========llist_travel===========
void llist_travel(LLIST *handle, llist_op_t *op)
{
    NODE *tail = NULL;

    for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        op(tail->data);
    }
}

//==========llist_destroy============
void llist_destroy(LLIST **handle)
{
    NODE *tail = NULL;
    NODE *sava = NULL;

    for (tail = (*handle)->head.next; tail != &(*handle)->head; tail = sava)
    {
        sava = tail->next;
        free(tail->data);
        free(tail);
    }
    free(*handle);
    *handle = NULL;
}

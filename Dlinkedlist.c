/*
 * 文件：Dlinkedlist.c 
 * Version: 1.0
 * -----------------------------------------------------
 * 本部分为双向链表操作的具体实现 
 */

#include <stdio.h>
#include <stdlib.h>

#include "Dlinkedlist.h"

#ifndef New
#define New(T) (T)malloc(sizeof(*(T)NULL))
#define FreeBlock(T) free(T)
#endif

/*
 * Functions: NewDLinkedList 
 * ---------------------------------------
 * 首先申请一块listchain类型的地址 
 * 然后申请两块Node类型的地址 
 * 最后一一赋值 
 * 注意，capacity也要在此处定义，暂定为不可变，也可修改为传参输入 
 */
Dlinkedlist NewDLinkedList(void)
{
    Dlinkedlist list = New(Dlinkedlist);
    
    DlinkedlistNode p = New(DlinkedlistNode);
    DlinkedlistNode q = New(DlinkedlistNode);
    p->dataptr = NULL;
    q->dataptr = NULL;
    //dataptr必须为空 
	
	list->head = p;
	p->last = NULL;
	list->tail = q;
	q->next = NULL;
	p->next = q;
	q->last = p;
	list->size = 0;
	list->capacity = 20;
    return (list);
}

/*
 * Functions: ClearDLinkedList 
 * ---------------------------------------
 * 首先声明2个Node类型 
 * tmp用于记录现在要删除的结点，以及判断循环停止条件 
 * next用于记录下一node的地址以便移动tmp 
 * 同时操作链表的size 
 */
void ClearDLinkedList(Dlinkedlist list)
{
	DlinkedlistNode tmp, next;

	tmp = list->head->next;
	// tmp是当下要删除的结点，直到tmp == tail, 才停止操作 
	while (tmp != list->tail) {
	    next = tmp->next;
		FreeBlock(tmp->dataptr);
		FreeBlock(tmp);
		tmp = next;
		list->size--;
	}
	//end while, 
}

/*
 * Functions: InsertHead、InsertTail 
 * ---------------------------------------
 * 首先声明2个Node类型 
 * ptr用于记录要加入的结点内容，内容需要通过传参的方式传入 
 * next用于记录本身head的next结点（tail的last结点）避免丢失 
 * 同时操作链表的size 
 */
void InsertHead(Dlinkedlist list, Ops obj)
{
	DlinkedlistNode ptr, tmp;

	if (obj == NULL || list->size == list->capacity) return;
	tmp = list->head->next;
	
	ptr = New(DlinkedlistNode); /*New a node*/
	ptr->dataptr = obj;  /*Set the data ptr of the node*/
	ptr->next = tmp;
	//需要先把下一个结点记录下来再赋值给新结点的next，以免其丢失 
	 
	ptr->last = list->head;
	tmp->last = ptr;
	list->head->next = ptr;
	list->size ++;
}

void InsertTail(Dlinkedlist list, Ops obj)
{
	DlinkedlistNode ptr, tmp;

	if (obj == NULL || list->size == list->capacity) return;
	tmp = list->tail->last;
	
	ptr = New(DlinkedlistNode); /*New a node*/
	ptr->dataptr = obj;  /*Set the data ptr of the node*/
	ptr->last = tmp;
	ptr->next = list->tail;
	tmp->next = ptr;
	list->tail->last = ptr;
	list->size ++;
	//思路和InsertHead比较相似 
}

/*
 * Functions: DeleteNode 
 * ---------------------------------------
 * 首先声明2个Node类型 
 * 实现思路和clearlist相似 
 * 只不过delete只进行一次 
 */
void DeleteDNode(Dlinkedlist list, DlinkedlistNode cur)
{
	DlinkedlistNode ptr, tmp;
	
	if (list->size == 0) return;
	//用size的操作代替了判断tail/head，更方便 
	tmp = cur->next;
	
	if(tmp != list->tail){
		ptr = tmp->next;
		tmp->last->next = ptr;
		ptr->last = tmp->last;
		free(tmp->dataptr);
		free(tmp);
		//先free数据指针再free结点 
	}
	
}

/*
 * Functions: NextNode, LastNode, IsTail, NodeObj
 * ---------------------------------------
 * 都是简单功能的函数化 
 * 用于进行一些读取操作 
 */
DlinkedlistNode NextNode(Dlinkedlist list, DlinkedlistNode nodeptr)
{
	if (nodeptr != list->tail) return nodeptr->next;
	return NULL;
}

DlinkedlistNode LastNode(Dlinkedlist list, DlinkedlistNode nodeptr)
{
	if (nodeptr != list->head) return nodeptr->last;
	return NULL;
}

bool IsTail(Dlinkedlist list, DlinkedlistNode nodeptr)
{
	if (nodeptr != list->tail) return FALSE;
	return TRUE;
}

Ops NodeObj(Dlinkedlist list, DlinkedlistNode cur)
{
	if (cur != list->tail && cur != list->head) return cur->dataptr;
	return NULL;
}


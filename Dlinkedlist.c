/*
 * �ļ���Dlinkedlist.c 
 * Version: 1.0
 * -----------------------------------------------------
 * ������Ϊ˫����������ľ���ʵ�� 
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
 * ��������һ��listchain���͵ĵ�ַ 
 * Ȼ����������Node���͵ĵ�ַ 
 * ���һһ��ֵ 
 * ע�⣬capacityҲҪ�ڴ˴����壬�ݶ�Ϊ���ɱ䣬Ҳ���޸�Ϊ�������� 
 */
Dlinkedlist NewDLinkedList(void)
{
    Dlinkedlist list = New(Dlinkedlist);
    
    DlinkedlistNode p = New(DlinkedlistNode);
    DlinkedlistNode q = New(DlinkedlistNode);
    p->dataptr = NULL;
    q->dataptr = NULL;
    //dataptr����Ϊ�� 
	
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
 * ��������2��Node���� 
 * tmp���ڼ�¼����Ҫɾ���Ľ�㣬�Լ��ж�ѭ��ֹͣ���� 
 * next���ڼ�¼��һnode�ĵ�ַ�Ա��ƶ�tmp 
 * ͬʱ���������size 
 */
void ClearDLinkedList(Dlinkedlist list)
{
	DlinkedlistNode tmp, next;

	tmp = list->head->next;
	// tmp�ǵ���Ҫɾ���Ľ�㣬ֱ��tmp == tail, ��ֹͣ���� 
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
 * Functions: InsertHead��InsertTail 
 * ---------------------------------------
 * ��������2��Node���� 
 * ptr���ڼ�¼Ҫ����Ľ�����ݣ�������Ҫͨ�����εķ�ʽ���� 
 * next���ڼ�¼����head��next��㣨tail��last��㣩���ⶪʧ 
 * ͬʱ���������size 
 */
void InsertHead(Dlinkedlist list, Ops obj)
{
	DlinkedlistNode ptr, tmp;

	if (obj == NULL || list->size == list->capacity) return;
	tmp = list->head->next;
	
	ptr = New(DlinkedlistNode); /*New a node*/
	ptr->dataptr = obj;  /*Set the data ptr of the node*/
	ptr->next = tmp;
	//��Ҫ�Ȱ���һ������¼�����ٸ�ֵ���½���next�������䶪ʧ 
	 
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
	//˼·��InsertHead�Ƚ����� 
}

/*
 * Functions: DeleteNode 
 * ---------------------------------------
 * ��������2��Node���� 
 * ʵ��˼·��clearlist���� 
 * ֻ����deleteֻ����һ�� 
 */
void DeleteDNode(Dlinkedlist list, DlinkedlistNode cur)
{
	DlinkedlistNode ptr, tmp;
	
	if (list->size == 0) return;
	//��size�Ĳ����������ж�tail/head�������� 
	tmp = cur->next;
	
	if(tmp != list->tail){
		ptr = tmp->next;
		tmp->last->next = ptr;
		ptr->last = tmp->last;
		free(tmp->dataptr);
		free(tmp);
		//��free����ָ����free��� 
	}
	
}

/*
 * Functions: NextNode, LastNode, IsTail, NodeObj
 * ---------------------------------------
 * ���Ǽ򵥹��ܵĺ����� 
 * ���ڽ���һЩ��ȡ���� 
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


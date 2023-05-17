/*
 * �ļ���Dlinkedlist.h 
 * Version: 1.0
 * -----------------------------------------------------
 * ������ΪһЩ˫��˫�ڱ�����Ļ������� 
 * ����ʵ�������ɵĳ���/�������� 
 * ʹ��ʱ��Ҫ����һ��ȫ�ֱ���LastOp�Լ�¼��ǰ����״̬ 
 */

#ifndef _DLinkedList_h
#define _DLinkedList_h

#include "boolean.h"
#include "strlib.h"

/*
 * �������͵�ö�� 
 * ---------------
 * ע�⵽��ʵ���϶��ı����еĲ���ֻ�в��룬ɾ������������ 
 * ������������ʵ�����������ֵĸ��� 
 * ��copy�������ļ����������Բ���Ҫ���г������� 
 * ���ֻ������������� 
 */
typedef enum
{
    INSERT,
    DEL,
} Ops_Type;

/*
 * Type: Operationdata
 * ---------------
 * ���������ڼ�¼������λ�ã����ݺ����� 
 * �ɷ�: dataptrָ��������� ΪOperationdata���� 
 *       last��ָ����һ��Node��ָ�� 
 * 		 next��ָ����һ��Node��ָ�� 
 */
typedef struct Operationdata *Ops;

/*
 * Type: Dlinkedlist1
 * ---------------
 * ��������˫�������еĽڵ����� 
 * �ɷ�: Ops_type ��Ϊ�������� 
 *       dataΪ���в��������� 
 * 		 pos������λ�ã��Ի�������index�� 
 */
typedef struct Dlinkedlist1 *DlinkedlistNode; 

/*
 * Type: DlinkedlistChain
 * ---------------
 * ���������ڼ�¼һ��˫������ 
 * �ɷ�: head/tail ��Ϊͷβ���ڱ�Node 
 *       sizeΪ�����н���������capacityΪ��󳤶� 
 * 		 ��������Ա������������������ʡ�ռ�
 *		 ��Ҳ�����˳��������������Ϊ20�������ı��о�Ҳ���ˣ� 
 */
typedef struct DlinkedlistChain *Dlinkedlist; 

struct Operationdata {
	Ops_Type type;
	string data;
	int pos;
}; 

struct Dlinkedlist1 {
    Ops dataptr;
    struct Dlinkedlist1 *last;
    struct Dlinkedlist1 *next;
};

struct DlinkedlistChain {
    DlinkedlistNode head;
    DlinkedlistNode tail;
    int size;
    int capacity;
};

/*
 * ����: NewNewDLinkedList
 * Usage: Dlinkedlist List = NewDLinkedList();
 * -----------------------------------------------
 * ����������ڳ�ʼ��һ��˫������ 
 */
Dlinkedlist NewDLinkedList(void);

/*
 * ����: ClearLinkedList
 * Usage: ClearLinkedList(Oplist);
 * ------------------------
 * ��Ҫ�ڴ��湦���е��� 
 * �����������˳������ 
 * ���Խ�ʡ�ռ� 
 * ��д��ʱ������˼ӽ����溯�����ϳ���Ŀ��ǵ�ɾ����䣩 
 */
void ClearLinkedList(Dlinkedlist list);

/*
 * ����: InsertHead
 * Usage: ClearLinkedList(list, dataptr);
 * ------------------------
 * ��ʵ����ƹ�����û����; 
 * �������ʧ�󣬵����Ƿ����� 
 */
void InsertHead(Dlinkedlist list, Ops obj);

/*
 * ����: InsertTail 
 * Usage: InsertTail(list, dataptr);
 * ------------------------
 * ��Ҫ���� 
 * ��Ҫ�ڽ����²���ʱ���� 
 * �Ѿ��������Ϣ����˳������ 
 * ���ڽ��г��������� 
 */
void InsertTail(Dlinkedlist list, Ops obj);

/*
 * ����: DeleteDNode
 * Usage: DeleteDNode(Oplist, LastOp);
 * ------------------------
 * ��Ҫ���� 
 * �����ڲ�����¼��20��ʱɾ������ļ�¼ 
 * �Լ��ڳ���������²���ʱʹ��  
 */
void DeleteDNode(Dlinkedlist list, DlinkedlistNode cur);

/*
 * ����: NextNode
 * Usage: Lastop = NextNode(Oplist, LastOp);
 * ------------------------
 * ��Ҫ����������ʱʹ�� 
 * ʹ����״̬ǰ��һ�� 
 * �Լ��ڼ����²���ʱ����LastOp��״̬  
 */                         
DlinkedlistNode NextNode(Dlinkedlist list, DlinkedlistNode nodeptr);

/*
 * ����: NextNode
 * Usage: Lastop = LastNode(Oplist, LastOp);
 * ------------------------
 * ��Ҫ����������ʱʹ�� 
 * ʹ����״̬����һ��  
 */    
DlinkedlistNode LastNode(Dlinkedlist list, DlinkedlistNode nodeptr);

/*
 * ����: IsTail
 * Usage:  if (!IsTail(Oplist, LastOp))
 * ------------------------
 * �����ж��Ƿ�����˳��� 
 * ����жϷ�Tail����Ҫɾ��LastOp��Tail֮��Ľ�� 
 */
bool IsTail(Dlinkedlist list, DlinkedlistNode nodeptr);

/*
 * ����: NodeObj
 * Usage:  Opstate = NodeObj(Oplist, LastOp)
 * ------------------------
 * ���ڻ�ȡLastOp������ 
 */
Ops NodeObj(Dlinkedlist list, DlinkedlistNode cur);

#endif

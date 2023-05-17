/*
 * 文件：Dlinkedlist.h 
 * Version: 1.0
 * -----------------------------------------------------
 * 本部分为一些双向双哨兵链表的基本操作 
 * 用于实现链表构成的撤销/重做操作 
 * 使用时需要定义一个全局变量LastOp以记录当前操作状态 
 */

#ifndef _DLinkedList_h
#define _DLinkedList_h

#include "boolean.h"
#include "strlib.h"

/*
 * 操作类型的枚举 
 * ---------------
 * 注意到，实际上对文本进行的操作只有插入，删除，复制三种 
 * 其他两个操作实际上是这三种的复合 
 * 而copy本身不对文件操作，所以不需要进行撤销重做 
 * 因而只设计这两种类型 
 */
typedef enum
{
    INSERT,
    DEL,
} Ops_Type;

/*
 * Type: Operationdata
 * ---------------
 * 此类型用于记录操作的位置，数据和类型 
 * 成分: dataptr指向操作数据 为Operationdata类型 
 *       last即指向上一个Node的指针 
 * 		 next即指向下一个Node的指针 
 */
typedef struct Operationdata *Ops;

/*
 * Type: Dlinkedlist1
 * ---------------
 * 此类型是双向链表中的节点类型 
 * 成分: Ops_type 即为操作类型 
 *       data为进行操作的内容 
 * 		 pos即操作位置（对缓冲区的index） 
 */
typedef struct Dlinkedlist1 *DlinkedlistNode; 

/*
 * Type: DlinkedlistChain
 * ---------------
 * 此类型用于记录一条双向链表 
 * 成分: head/tail 即为头尾的哨兵Node 
 *       size为链表中结点的数量，capacity为最大长度 
 * 		 这样设计以便控制链表数据量，节省空间
 *		 但也限制了撤销操作数量最大为20步（对文本感觉也够了） 
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
 * 函数: NewNewDLinkedList
 * Usage: Dlinkedlist List = NewDLinkedList();
 * -----------------------------------------------
 * 这个函数用于初始化一条双向链表 
 */
Dlinkedlist NewDLinkedList(void);

/*
 * 函数: ClearLinkedList
 * Usage: ClearLinkedList(Oplist);
 * ------------------------
 * 主要在储存功能中调用 
 * 清除整个操作顺序链表 
 * 可以节省空间 
 * 但写的时候别忘了加进储存函数（合成项目后记得删了这句） 
 */
void ClearLinkedList(Dlinkedlist list);

/*
 * 函数: InsertHead
 * Usage: ClearLinkedList(list, dataptr);
 * ------------------------
 * 其实在设计功能中没有用途 
 * 算是设计失误，但还是放着了 
 */
void InsertHead(Dlinkedlist list, Ops obj);

/*
 * 函数: InsertTail 
 * Usage: InsertTail(list, dataptr);
 * ------------------------
 * 重要函数 
 * 主要在进行新操作时调用 
 * 把具体操作信息操作顺序链表 
 * 便于进行撤销和重做 
 */
void InsertTail(Dlinkedlist list, Ops obj);

/*
 * 函数: DeleteDNode
 * Usage: DeleteDNode(Oplist, LastOp);
 * ------------------------
 * 重要函数 
 * 用于在操作记录满20条时删除最早的记录 
 * 以及在撤销后加入新操作时使用  
 */
void DeleteDNode(Dlinkedlist list, DlinkedlistNode cur);

/*
 * 函数: NextNode
 * Usage: Lastop = NextNode(Oplist, LastOp);
 * ------------------------
 * 重要函数，重做时使用 
 * 使操作状态前进一步 
 * 以及在加入新操作时更新LastOp的状态  
 */                         
DlinkedlistNode NextNode(Dlinkedlist list, DlinkedlistNode nodeptr);

/*
 * 函数: NextNode
 * Usage: Lastop = LastNode(Oplist, LastOp);
 * ------------------------
 * 重要函数，撤销时使用 
 * 使操作状态回退一步  
 */    
DlinkedlistNode LastNode(Dlinkedlist list, DlinkedlistNode nodeptr);

/*
 * 函数: IsTail
 * Usage:  if (!IsTail(Oplist, LastOp))
 * ------------------------
 * 用于判断是否进行了撤销 
 * 如果判断非Tail，需要删除LastOp到Tail之间的结点 
 */
bool IsTail(Dlinkedlist list, DlinkedlistNode nodeptr);

/*
 * 函数: NodeObj
 * Usage:  Opstate = NodeObj(Oplist, LastOp)
 * ------------------------
 * 用于获取LastOp的数据 
 */
Ops NodeObj(Dlinkedlist list, DlinkedlistNode cur);

#endif

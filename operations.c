/*
   文件：operations.c 
   Version: 1.0
   -----------------------------------------------------
   本部分为程序需要用到的基本操作的具体实现 
    
PS：测试时发现，delete可能会由于结束点大于len(Buffer)而产生bug
   但考虑到实际使用时光标会对位置产生限定范围功能，没有处理 
   组员可进行进一步修改 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "strlib.h"
#include "Dlinkedlist.h"

//实际使用时需要一个全局变量LastOperation 去记录上一次操作的情况
//因为要在函数内同时进行缓冲区和LastOp修改，所以不能传参，必须全局
//实际操作时，可以将imgui.c的textbox函数修改，以把退格键也和delete关联起来 
//实际使用例子 
//static DlinkedlistNode LastOp;
//static string Buffer;
//static Dlinkedlist Oplist;

/*
   Function: ControlCopy
  ------------------------
   首先尝试打开系统剪贴板
   然后判断剪贴板内是否是文本内容
   最后获取内容 
*/
string ControlCopy(){
	string tmp = (char*)malloc(sizeof(char)*100);
	string buffer = NULL;
	int pos;
	
	HANDLE Board;
	
	if(OpenClipboard(NULL) == 0){
		return;
	}//是否成功打开剪贴板 
	
	if(!IsClipboardFormatAvailable(CF_TEXT)){
		CloseClipboard();
		return;
	}//剪贴板内不是文本类型 
	
	Board = GetClipboardData(CF_TEXT);
	buffer = (string)GlobalLock(Board);
	GlobalUnlock(Board);
	
	while(1){
		if(*buffer == 0){
			break;
		}//读取全部内容的结束条件 
		
		pos = 0;
		do{
			tmp[pos] = *buffer;
			++buffer;
			++pos;	
		}while(*buffer);
		tmp[pos] = '\0';
	}//读取所有剪贴板内容 
	
	CloseClipboard();
	//关闭剪贴板，否则其他程序无法使用 
	
	return tmp;
}

/*
   Function: WriteClip
  ------------------------
   尝试向剪贴板写入内容 
*/
bool WriteClip(const string str){
	int size = strlen(str) + 1;
	
	HGLOBAL hMemory; LPTSTR lpMemory;
	
    if(!OpenClipboard(NULL))
		return FALSE;
		
    if(!EmptyClipboard())
		return FALSE;
		
	if(!(hMemory = GlobalAlloc(GMEM_MOVEABLE, size)))
		return FALSE;
		//对剪贴板分配内存 
		
    if(!(lpMemory = (LPTSTR)GlobalLock(hMemory)))
		return FALSE;
	//锁定内存区域 
	
	memcpy_s(lpMemory, size, str, size);//复制数据 
    GlobalUnlock(hMemory);//接触内存锁定 
    
    if(!SetClipboardData(CF_TEXT, hMemory))
		return FALSE;
		
    CloseClipboard();
    return TRUE;
}

/*
   Function: MyCopy
  ------------------------
   首先截取缓冲区内部分内容	
   然后判写入剪贴板中  
*/
bool MyCopy(string Buffer, int p1, int p2){
	string ret;
	ret = SubString(Buffer, p1, p2);
	
	if (WriteClip(ret) == TRUE) return TRUE;
	
	return FALSE;
}

/*
   Function: MyInsert
  ------------------------
   首先创建一个操作信息类型指针，将插入的字符，插入位置，
   和操作类型存入操作信息链表，然后对缓冲区进行该操作，
   最后移动操作状态指针LastOp  
   
   ** 文件中截取字符串/添加字符串操作全部通过strlib中的函数执行，
      以保证缓冲区只在结尾有一个 \0
*/

string MyInsert(Dlinkedlist Oplist, string data, string Buffer, int p1){
	string ret;
	
	Ops NewOp = New(Ops);
	NewOp->type = INSERT;
	NewOp->data = data;
	NewOp->pos = p1;
	
	if (LastOp->next != Oplist->tail){
		while(LastOp->next != Oplist->tail){
			DeleteDNode(Oplist, LastOp);
		}
		/* 条件意义为：进行了撤销操作，指针不指向最后一步操作，
		   此时需要删除被撤销的操作信息，并将该次插入作为最新操作 
		*/
	}else if (Oplist->size == Oplist->capacity){
		DeleteDNode(Oplist, Oplist->head);
	}//操作信息链表满了，删除最早一条操作信息 
	
	InsertTail(Oplist, NewOp);
	ret = Concat(SubString(Buffer, 0, p1), Concat(data, SubString(Buffer, p1 + 1, StringLength(Buffer) - 1)));
	LastOp = NextNode(Oplist, LastOp);
	
	return ret; //修改缓冲区信息
}

/*
   Function: MyDelete
  ------------------------
   思路同Insert，只不过操作内容需要通过光标在缓冲区内选取 
   记录进链表的操作位置是被删除字符的首元素下标 
*/
string MyDelete(Dlinkedlist Oplist, string Buffer, int p1, int p2){
	string ret;
	
	Ops NewOp = New(Ops);
	NewOp->type = DEL;
	NewOp->data = SubString(Buffer, p1, p2);
	NewOp->pos = p1;
	
	if (LastOp->next != Oplist->tail){
		while(LastOp->next != Oplist->tail){
			DeleteDNode(Oplist, LastOp);
		}
		/* 条件意义为：进行了撤销操作，指针不指向最后一步操作，
		   此时需要删除被撤销的操作信息，并将该次删除作为最新操作 
		*/
	}else if (Oplist->size == Oplist->capacity){
		DeleteDNode(Oplist, Oplist->head);
	}
	
	InsertTail(Oplist, NewOp);
	ret = Concat(SubString(Buffer,0, p1 - 1), SubString(Buffer, p2 + 1, StringLength(Buffer) - 1));
	LastOp = NextNode(Oplist, LastOp);
	
	return ret; //修改缓冲区信息 
}

/*
   Function: MyPaste
  ------------------------
   其实相当于insert操作，只不过插入内容是从剪贴板内读取的 
*/
string MyPaste(Dlinkedlist Oplist, string Buffer, int p1){
	string paste = ControlCopy();
	string ret = MyInsert(Oplist, paste, Buffer, p1);
	
	return ret;
}

/*
   Function: MyCut
  ------------------------
   相当于Delete操作，只不过需要将删除内容写入剪贴板 
*/
string MyCut(Dlinkedlist Oplist, string Buffer, int p1, int p2){
	string ret, data;
	data = SubString(Buffer, p1, p2);
	WriteClip(data);
	
	ret = MyDelete(Oplist, Buffer, p1, p2);
	return ret;
}

/*
   Function: ReInsert
  ------------------------
   Insert的反操作，大概逻辑类似delete，只不过下标由记录的插入位置
   和插入字符串的长度表示，以便于统一操作，且不向Oplist内加入结点 
*/
string ReInsert(Dlinkedlist Oplist, string Buffer){
	string ret, tmp;
	int p1;
	tmp = LastOp->dataptr->data; 
	//读取操作内容 
	p1 = LastOp->dataptr->pos;
	//读取操作位置 
	
	ret = Concat(SubString(Buffer, 0, p1), SubString(Buffer, p1 + StringLength(tmp) + 1, StringLength(Buffer) - 1));
	return ret;
}

/*
   Function: ReDelete
  ------------------------
   Delete的反操作，大概逻辑类似Insert
*/
string ReDelete(Dlinkedlist Oplist, string Buffer){
	string ret, tmp;
	int p1;
	tmp = LastOp->dataptr->data;
	//读取操作内容 
	p1 = LastOp->dataptr->pos;
	//读取操作位置 
	
	ret = Concat(SubString(Buffer, 0, p1 - 1), tmp);
	ret = Concat(ret, SubString(Buffer, p1, StringLength(Buffer) - 1));
	return ret;
}

/*
   Function: Undo
  ------------------------
   撤销操作，设计逻辑为以双向链表存储操作序列，当调用undo操作，
   首先读取list中的最后一个操作，进行反操作，然后将当前操作指针
   向前推一个 
*/
string Undo(Dlinkedlist Oplist, string Buffer){
	string ret;

	if (LastOp == Oplist->head){
		return Buffer;
	}
	//已经撤销到可撤销的最后一步 
	
	switch (LastOp->dataptr->type){
		case INSERT:
			ret = ReInsert(Oplist, Buffer);
			break;
		case DEL:
			ret = ReDelete(Oplist, Buffer);
			break;
	}
	
	LastOp = LastNode(Oplist, LastOp);
	//改到将操作回退一步的状态 
	
	//不同于Redo，首次调用Undo时，指针所指的链表位置即为上一步操作
	//所以先回溯上一步操作，再进行操作状态指针回退 
	
	return ret;
}

/*
   Function: Redo
  ------------------------
   重做操作，设计逻辑为以双向链表存储操作序列，当调用Redo操作，
   在当前操作位置的基础上前进一步，重复记录的操作 
*/
string Redo(Dlinkedlist Oplist, string Buffer){
	string ret, data;
	int p1;
	
	if (LastOp->next == Oplist->tail){
		return Buffer;
	}
	//先前进再读取，因为Redo调用一定基于LastOp所指位置做了
	//而下一步没做的状态，这种状况下要做的是下一步，这也是判断LastOp->next是否为tail的原因 
	
	LastOp = NextNode(Oplist, LastOp);
	data = LastOp->dataptr->data;
	p1 = LastOp->dataptr->pos;
	
	switch (LastOp->dataptr->type){
		case INSERT:
			ret = Concat(SubString(Buffer, 0, p1), Concat(data, SubString(Buffer, p1 + 1, StringLength(Buffer) - 1)));
			//相当于不进行链表插入的insert操作 
			break;
		case DEL:
			ret = Concat(SubString(Buffer, 0, p1 - 1), SubString(Buffer, p1 + StringLength(data), StringLength(Buffer) - 1));
			//相当于不进行链表插入的delet操作 
			break;
	}

	return ret;
}

// 一些测试案例，组员可于组装时自行删去 
//int main(){
//	Buffer = "abcdefghijklmnopq";
//	Oplist = NewDLinkedList();
//	LastOp = Oplist->head;
//	
//	string x = " x";
//	string z = " z \n";
//	string j = "00j";
////	WriteClip(x);
//	Buffer = MyPaste(Oplist, Buffer, 3);
//	Buffer = MyCut(Oplist, Buffer, 4, 15);
//	Buffer = MyPaste(Oplist, Buffer, 5);
////	Buffer = Undo(Oplist, Buffer);
////	Buffer = Undo(Oplist, Buffer);
////	Buffer = Redo(Oplist, Buffer);
////	Buffer = MyDelete(Oplist, Buffer, 15, 16);
////	Buffer = Undo(Oplist, Buffer);
////	Buffer = Undo(Oplist, Buffer);
//
//	printf("%s",Buffer);
//	return 0;
//}

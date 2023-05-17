/*
   �ļ���operations.c 
   Version: 1.0
   -----------------------------------------------------
   ������Ϊ������Ҫ�õ��Ļ��������ľ���ʵ�� 
    
PS������ʱ���֣�delete���ܻ����ڽ��������len(Buffer)������bug
   �����ǵ�ʵ��ʹ��ʱ�����λ�ò����޶���Χ���ܣ�û�д��� 
   ��Ա�ɽ��н�һ���޸� 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "strlib.h"
#include "Dlinkedlist.h"

//ʵ��ʹ��ʱ��Ҫһ��ȫ�ֱ���LastOperation ȥ��¼��һ�β��������
//��ΪҪ�ں�����ͬʱ���л�������LastOp�޸ģ����Բ��ܴ��Σ�����ȫ��
//ʵ�ʲ���ʱ�����Խ�imgui.c��textbox�����޸ģ��԰��˸��Ҳ��delete�������� 
//ʵ��ʹ������ 
//static DlinkedlistNode LastOp;
//static string Buffer;
//static Dlinkedlist Oplist;

/*
   Function: ControlCopy
  ------------------------
   ���ȳ��Դ�ϵͳ������
   Ȼ���жϼ��������Ƿ����ı�����
   ����ȡ���� 
*/
string ControlCopy(){
	string tmp = (char*)malloc(sizeof(char)*100);
	string buffer = NULL;
	int pos;
	
	HANDLE Board;
	
	if(OpenClipboard(NULL) == 0){
		return;
	}//�Ƿ�ɹ��򿪼����� 
	
	if(!IsClipboardFormatAvailable(CF_TEXT)){
		CloseClipboard();
		return;
	}//�������ڲ����ı����� 
	
	Board = GetClipboardData(CF_TEXT);
	buffer = (string)GlobalLock(Board);
	GlobalUnlock(Board);
	
	while(1){
		if(*buffer == 0){
			break;
		}//��ȡȫ�����ݵĽ������� 
		
		pos = 0;
		do{
			tmp[pos] = *buffer;
			++buffer;
			++pos;	
		}while(*buffer);
		tmp[pos] = '\0';
	}//��ȡ���м��������� 
	
	CloseClipboard();
	//�رռ����壬�������������޷�ʹ�� 
	
	return tmp;
}

/*
   Function: WriteClip
  ------------------------
   �����������д������ 
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
		//�Լ���������ڴ� 
		
    if(!(lpMemory = (LPTSTR)GlobalLock(hMemory)))
		return FALSE;
	//�����ڴ����� 
	
	memcpy_s(lpMemory, size, str, size);//�������� 
    GlobalUnlock(hMemory);//�Ӵ��ڴ����� 
    
    if(!SetClipboardData(CF_TEXT, hMemory))
		return FALSE;
		
    CloseClipboard();
    return TRUE;
}

/*
   Function: MyCopy
  ------------------------
   ���Ƚ�ȡ�������ڲ�������	
   Ȼ����д���������  
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
   ���ȴ���һ��������Ϣ����ָ�룬��������ַ�������λ�ã�
   �Ͳ������ʹ��������Ϣ����Ȼ��Ի��������иò�����
   ����ƶ�����״ָ̬��LastOp  
   
   ** �ļ��н�ȡ�ַ���/����ַ�������ȫ��ͨ��strlib�еĺ���ִ�У�
      �Ա�֤������ֻ�ڽ�β��һ�� \0
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
		/* ��������Ϊ�������˳���������ָ�벻ָ�����һ��������
		   ��ʱ��Ҫɾ���������Ĳ�����Ϣ�������ôβ�����Ϊ���²��� 
		*/
	}else if (Oplist->size == Oplist->capacity){
		DeleteDNode(Oplist, Oplist->head);
	}//������Ϣ�������ˣ�ɾ������һ��������Ϣ 
	
	InsertTail(Oplist, NewOp);
	ret = Concat(SubString(Buffer, 0, p1), Concat(data, SubString(Buffer, p1 + 1, StringLength(Buffer) - 1)));
	LastOp = NextNode(Oplist, LastOp);
	
	return ret; //�޸Ļ�������Ϣ
}

/*
   Function: MyDelete
  ------------------------
   ˼·ͬInsert��ֻ��������������Ҫͨ������ڻ�������ѡȡ 
   ��¼������Ĳ���λ���Ǳ�ɾ���ַ�����Ԫ���±� 
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
		/* ��������Ϊ�������˳���������ָ�벻ָ�����һ��������
		   ��ʱ��Ҫɾ���������Ĳ�����Ϣ�������ô�ɾ����Ϊ���²��� 
		*/
	}else if (Oplist->size == Oplist->capacity){
		DeleteDNode(Oplist, Oplist->head);
	}
	
	InsertTail(Oplist, NewOp);
	ret = Concat(SubString(Buffer,0, p1 - 1), SubString(Buffer, p2 + 1, StringLength(Buffer) - 1));
	LastOp = NextNode(Oplist, LastOp);
	
	return ret; //�޸Ļ�������Ϣ 
}

/*
   Function: MyPaste
  ------------------------
   ��ʵ�൱��insert������ֻ�������������ǴӼ������ڶ�ȡ�� 
*/
string MyPaste(Dlinkedlist Oplist, string Buffer, int p1){
	string paste = ControlCopy();
	string ret = MyInsert(Oplist, paste, Buffer, p1);
	
	return ret;
}

/*
   Function: MyCut
  ------------------------
   �൱��Delete������ֻ������Ҫ��ɾ������д������� 
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
   Insert�ķ�����������߼�����delete��ֻ�����±��ɼ�¼�Ĳ���λ��
   �Ͳ����ַ����ĳ��ȱ�ʾ���Ա���ͳһ�������Ҳ���Oplist�ڼ����� 
*/
string ReInsert(Dlinkedlist Oplist, string Buffer){
	string ret, tmp;
	int p1;
	tmp = LastOp->dataptr->data; 
	//��ȡ�������� 
	p1 = LastOp->dataptr->pos;
	//��ȡ����λ�� 
	
	ret = Concat(SubString(Buffer, 0, p1), SubString(Buffer, p1 + StringLength(tmp) + 1, StringLength(Buffer) - 1));
	return ret;
}

/*
   Function: ReDelete
  ------------------------
   Delete�ķ�����������߼�����Insert
*/
string ReDelete(Dlinkedlist Oplist, string Buffer){
	string ret, tmp;
	int p1;
	tmp = LastOp->dataptr->data;
	//��ȡ�������� 
	p1 = LastOp->dataptr->pos;
	//��ȡ����λ�� 
	
	ret = Concat(SubString(Buffer, 0, p1 - 1), tmp);
	ret = Concat(ret, SubString(Buffer, p1, StringLength(Buffer) - 1));
	return ret;
}

/*
   Function: Undo
  ------------------------
   ��������������߼�Ϊ��˫������洢�������У�������undo������
   ���ȶ�ȡlist�е����һ�����������з�������Ȼ�󽫵�ǰ����ָ��
   ��ǰ��һ�� 
*/
string Undo(Dlinkedlist Oplist, string Buffer){
	string ret;

	if (LastOp == Oplist->head){
		return Buffer;
	}
	//�Ѿ��������ɳ��������һ�� 
	
	switch (LastOp->dataptr->type){
		case INSERT:
			ret = ReInsert(Oplist, Buffer);
			break;
		case DEL:
			ret = ReDelete(Oplist, Buffer);
			break;
	}
	
	LastOp = LastNode(Oplist, LastOp);
	//�ĵ�����������һ����״̬ 
	
	//��ͬ��Redo���״ε���Undoʱ��ָ����ָ������λ�ü�Ϊ��һ������
	//�����Ȼ�����һ���������ٽ��в���״ָ̬����� 
	
	return ret;
}

/*
   Function: Redo
  ------------------------
   ��������������߼�Ϊ��˫������洢�������У�������Redo������
   �ڵ�ǰ����λ�õĻ�����ǰ��һ�����ظ���¼�Ĳ��� 
*/
string Redo(Dlinkedlist Oplist, string Buffer){
	string ret, data;
	int p1;
	
	if (LastOp->next == Oplist->tail){
		return Buffer;
	}
	//��ǰ���ٶ�ȡ����ΪRedo����һ������LastOp��ָλ������
	//����һ��û����״̬������״����Ҫ��������һ������Ҳ���ж�LastOp->next�Ƿ�Ϊtail��ԭ�� 
	
	LastOp = NextNode(Oplist, LastOp);
	data = LastOp->dataptr->data;
	p1 = LastOp->dataptr->pos;
	
	switch (LastOp->dataptr->type){
		case INSERT:
			ret = Concat(SubString(Buffer, 0, p1), Concat(data, SubString(Buffer, p1 + 1, StringLength(Buffer) - 1)));
			//�൱�ڲ�������������insert���� 
			break;
		case DEL:
			ret = Concat(SubString(Buffer, 0, p1 - 1), SubString(Buffer, p1 + StringLength(data), StringLength(Buffer) - 1));
			//�൱�ڲ�������������delet���� 
			break;
	}

	return ret;
}

// һЩ���԰�������Ա������װʱ����ɾȥ 
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

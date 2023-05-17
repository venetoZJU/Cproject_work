/*
   �ļ���operations.h 
   Version: 1.0
   -----------------------------------------------------
   ������Ϊ������Ϊ������Ҫ�õ��Ļ������� 
   û��ʵ�ֺͼ������������ʵʱ�� 
   ����string���͵ķ���ֵ�����ڸ�Buffer��ֵ���޸�����
   �÷���Buffer = function(x, y, z, ����) 
   
PS������ʱ���֣�delete���ܻ����ڽ��������len(Buffer)������bug
   �����ǵ�ʵ��ʹ��ʱ�����λ�ò����޶���Χ���ܣ�û�д��� 
   ��Ա�ɽ��н�һ���޸� 
*/

#ifndef _operations_h
#define _operations_h

#include "boolean.h"

/*
   Function: ControlCopy
   Usage: str = ControlCopy();
  ------------------------
   ���ڶ�ȡ����������ݣ����ַ�����ʽ����
   �ڲο������Ͻ����޸ģ��ο���ַ��
   https://blog.csdn.net/dandelionLYY/article/details/86744110 
*/
string  ControlCopy();

/*
   Function: WriteClip 
   Usage: WriteClip(str);
  ------------------------
   ���ڽ��ַ���д��windowsϵͳ�������У����ַ�����ʽ����
   �ڲο��������޸ģ��ο���ַ��
   https://blog.csdn.net/yu57955/article/details/107294000
*/
bool WriteClip(const string str);

/*
   Function: MyCopy 
   Usage: MyCopy(Buffer, index1, index2);
  ------------------------
   �����ڻ������ַ�ѡȡһ����д������� 
   ��������Ϊcopy���ַ�������ʼ�ַ� 
   ʵ��ʹ��ʱ��Ϲ���ɵ�ѡȡ����ȷ���±� 
*/
bool MyCopy(string Buffer, int p1, int p2);

/*
   Function: MyInsert 
   Usage: MyInsert(Oplist, data, Buffer, index1);
  ------------------------
   �����ڻ������ڼ������� 
   ʵ������Ϊ���ַ��������±�Ϊindex1���ַ�֮�� 
   ʵ��ʹ��ʱ�����Խ�ϼ�ʱ������3-5���ڵļ������봢��Ϊ�ַ���
   Ȼ����Ϊdata�����ݵ���insert���� 
*/
string MyInsert(Dlinkedlist Oplist, string data, string Buffer, int p1);

/*
   Function: MyDelete 
   Usage: MyDelete(Oplist, Buffer, index1, index2);
  ------------------------
   ����ɾ������������ 
   ʵ������Ϊ���±�index1��index2֮�䣨�����䣩���ַ�ɾ�� 
   ʵ��ʹ��ʱ�����Խ�Ϲ��ȷ���±�
   �����˸��ܣ����Խ���ӳ��ΪMyDelete(Oplist, Buffer, strlen(Buffer) - 1, strlen(Buffer) - 1) 
*/
string MyDelete(Dlinkedlist Oplist, string Buffer, int p1, int p2);

/*
   Function: MyPaste 
   Usage: MyPaste(Oplist, Buffer, index1);
  ------------------------
   ���ڽ����������ݼ��뻺���� 
   ʵ�ʵ�ͬ��Insert��ֻ�ǲ���������Դ���� 
   ������ͬ�ڳ���Insert 
*/
string MyPaste(Dlinkedlist Oplist, string Buffer, int p1);

/*
   Function: MyCut 
   Usage: MyCut(Oplist, Buffer, index1, index2);
  ------------------------
   ��Ƶļ��й��� 
   ʵ�ʵ�ͬ��Delete + Copy 
   ������ͬ�ڳ���Delete 
*/
string MyCut(Dlinkedlist Oplist, string Buffer, int p1, int p2);

/*
   Function: ReInsert 
   Usage: ReInsert(Oplist, Buffer);
  ------------------------
   Insert�ķ����������ڳ������� 
   ����ԭ�������Ѿ���¼��Oplist�У�����Ҫ���ഫ����� 
*/
string ReInsert(Dlinkedlist Oplist, string Buffer);

/*
   Function: ReInsert 
   Usage: ReInsert(Oplist, Buffer);
  ------------------------
   Delete�ķ����������ڳ��� 
   ����ԭ�������Ѿ���¼��Oplist�У�����Ҫ���ഫ����� 
*/
string ReDelete(Dlinkedlist Oplist, string Buffer);

/*
   Function: Undo, Redo 
   Usage: Undo/Redo (Oplist, Buffer);
  ------------------------
   �������������ܵ�ʵ�� 
   �����������ƣ��ݶ�Ϊ20�� 
*/
string Undo(Dlinkedlist Oplist, string Buffer);

string Redo(Dlinkedlist Oplist, string Buffer);

#endif

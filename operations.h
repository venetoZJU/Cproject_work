/*
   文件：operations.h 
   Version: 1.0
   -----------------------------------------------------
   本部分为个人认为程序需要用到的基本操作 
   没有实现和键盘输入输出的实时绑定 
   所有string类型的返回值都用于给Buffer赋值以修改内容
   用法：Buffer = function(x, y, z, ……) 
   
PS：测试时发现，delete可能会由于结束点大于len(Buffer)而产生bug
   但考虑到实际使用时光标会对位置产生限定范围功能，没有处理 
   组员可进行进一步修改 
*/

#ifndef _operations_h
#define _operations_h

#include "boolean.h"

/*
   Function: ControlCopy
   Usage: str = ControlCopy();
  ------------------------
   用于读取剪贴板的内容，以字符串形式传出
   在参考基础上进行修改，参考地址：
   https://blog.csdn.net/dandelionLYY/article/details/86744110 
*/
string  ControlCopy();

/*
   Function: WriteClip 
   Usage: WriteClip(str);
  ------------------------
   用于将字符串写入windows系统剪贴板中，以字符串形式传出
   在参考基础上修改，参考地址：
   https://blog.csdn.net/yu57955/article/details/107294000
*/
bool WriteClip(const string str);

/*
   Function: MyCopy 
   Usage: MyCopy(Buffer, index1, index2);
  ------------------------
   用于在缓冲区字符选取一部分写入剪贴板 
   两个坐标为copy的字符串的起始字符 
   实际使用时结合光标达成的选取过程确定下标 
*/
bool MyCopy(string Buffer, int p1, int p2);

/*
   Function: MyInsert 
   Usage: MyInsert(Oplist, data, Buffer, index1);
  ------------------------
   用于在缓冲区内加入内容 
   实际运行为将字符串插入下标为index1的字符之后 
   实际使用时，可以结合计时器，将3-5秒内的键盘输入储存为字符串
   然后作为data的内容调用insert函数 
*/
string MyInsert(Dlinkedlist Oplist, string data, string Buffer, int p1);

/*
   Function: MyDelete 
   Usage: MyDelete(Oplist, Buffer, index1, index2);
  ------------------------
   用于删除缓冲区内容 
   实际运行为将下标index1到index2之间（闭区间）的字符删除 
   实际使用时，可以结合光标确定下标
   对于退格功能，可以将其映射为MyDelete(Oplist, Buffer, strlen(Buffer) - 1, strlen(Buffer) - 1) 
*/
string MyDelete(Dlinkedlist Oplist, string Buffer, int p1, int p2);

/*
   Function: MyPaste 
   Usage: MyPaste(Oplist, Buffer, index1);
  ------------------------
   用于将剪贴板内容加入缓冲区 
   实际等同于Insert，只是插入内容来源区别 
   撤销等同于撤销Insert 
*/
string MyPaste(Dlinkedlist Oplist, string Buffer, int p1);

/*
   Function: MyCut 
   Usage: MyCut(Oplist, Buffer, index1, index2);
  ------------------------
   设计的剪切功能 
   实际等同于Delete + Copy 
   撤销等同于撤销Delete 
*/
string MyCut(Dlinkedlist Oplist, string Buffer, int p1, int p2);

/*
   Function: ReInsert 
   Usage: ReInsert(Oplist, Buffer);
  ------------------------
   Insert的反操作，用于撤销功能 
   由于原本操作已经记录在Oplist中，不需要多余传入参数 
*/
string ReInsert(Dlinkedlist Oplist, string Buffer);

/*
   Function: ReInsert 
   Usage: ReInsert(Oplist, Buffer);
  ------------------------
   Delete的反操作，用于撤销 
   由于原本操作已经记录在Oplist中，不需要多余传入参数 
*/
string ReDelete(Dlinkedlist Oplist, string Buffer);

/*
   Function: Undo, Redo 
   Usage: Undo/Redo (Oplist, Buffer);
  ------------------------
   撤销，重做功能的实现 
   有最大次数限制，暂定为20次 
*/
string Undo(Dlinkedlist Oplist, string Buffer);

string Redo(Dlinkedlist Oplist, string Buffer);

#endif

#pragma  once
#include "pch.h"
#include <vector>
#include <windows.h>

using namespace std;

static class OffsetMgr {
public:
	//ActivePos c22 
	//all pointers work 
	//px + 4 + 4
	//float!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	static vector<SIZE_T> ActiveItemPosition0; //2
	static vector<SIZE_T> ActiveItemPosition1; //3
	static vector<SIZE_T> ActiveItemPosition2; //4
	static vector<SIZE_T> ActiveItemPosition3; //4
	static vector<SIZE_T> ActiveItemPosition4; //4
	static vector<SIZE_T> ActiveItemPosition5; //5 .
	static vector<SIZE_T> ActiveItemPosition6;
	static vector<SIZE_T> ActiveItemPosition7;
	static vector<SIZE_T> ActiveItemPosition8;
	static vector<SIZE_T> ActiveItemPosition9;
	static vector<SIZE_T> ActiveItemPosition10;
	static vector<SIZE_T> ActiveItemPosition11;
	static vector<SIZE_T> ActiveItemPosition12;
	static vector<SIZE_T> ActiveItemPosition13;
	static vector<SIZE_T> ActiveItemPosition14;
	static vector<SIZE_T> ActiveItemPosition15;
	static vector<SIZE_T> ActiveItemPosition16;
	static vector<SIZE_T> ActiveItemPosition17;
	static vector<SIZE_T> ActiveItemPosition18;
	static vector<SIZE_T> ActiveItemPosition19;
	static vector<SIZE_T> ActiveItemPosition20;
	static vector<SIZE_T> ActiveItemPosition21;
	static vector<SIZE_T> ActiveItemPosition22; //5


	//dyn ItemCount
	static vector<SIZE_T> maxItemCount; 
	static vector<SIZE_T> updatedItemCount;  //Offset=Pmax - 4

	//unfreeze
	//"cant place here"
	//C6 87 + byte[rdi+]
/*
RAX=000000D5F7FEF0E0
RBX=0000024B7E6404A0
RCX=1A893A1DBFB70000
RDX=000000003F7FFFFC
RSI=0000024B7E640600
RDI=0000024B7E6404A0 rdi
RBP=000000D5F7FEF250
RSP=000000D5F7FEF150
RIP=00007FF6E0194A8A
R8 =0000024B2E1A5590
R9 =0000024B306A4F90
R10=0000024B4444D6C0
R11=000000D5F7FEF030
R12=0000024B7E6405F0
R13=0000000000000009
R14=00007FF6E186BCC8
R15=0000024B7E640608

*/
/*
   51d jne
   526
565532 je
56553f je
	   mov
	   cmp
*/
	//aobscan failed 
	//addr
	static vector<SIZE_T> Unreeze1;  
	static vector<SIZE_T> Unreeze2;  
	static vector<SIZE_T> Unreeze3;
	static vector<SIZE_T> Unreeze4;
	static vector<SIZE_T> Unreeze5;
	static vector<SIZE_T> Unreeze6;//dt work
	static vector<SIZE_T> Unreeze7;//dt work

	/*
	      ret
	df0-5 int3
		  int3
	df0 test rdx,rdx
	    je ef9
		mov
		push rdi

	df0 -> jmp alloc(m1)
	m1: E9 ad1-ad2
	*/
	static SIZE_T BlueInject;
	static SIZE_T YellowInject;



	static SIZE_T ItemCount;
	static vector<SIZE_T> ItemCount2;
	static SIZE_T IsWarehouseOffset;
	static SIZE_T ItemCount_List;


	/*ONE BYTE //DEC
	-0x20+C (count
	+0x20 ->[b0,b3](int) -> b5 order  -> b8 order  //one page maxcount=200(a byte
	+0x20+0x10*(c-1)
	...
	...
	...
	+0x20+0x10*199
	*/
	//ItemListLength
	static vector<SIZE_T> ItemListLength1;
	static vector<SIZE_T> ItemListLength2;
	static vector<SIZE_T> ItemListLength3;
	static vector<SIZE_T> ItemListLength4;


	static int OutputOffsetsJSON(const char* FilePath);
};
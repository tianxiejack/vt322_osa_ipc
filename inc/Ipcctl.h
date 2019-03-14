#ifndef __IPCCTL_H_
#define __IPCCTL_H_

#include <vector>

#define PARAMLEN 164

typedef enum 
{
    IPC_Class_MSG,  
    IPC_Class_SHA,
    IPC_Class_SEM,
    IPC_ClassMAX
}IPC_Class;

typedef struct{
	char *name;
	int Identify;
	int Class;
	int IPCID;
	int RWmode;
	int length;
	void* ptr;	
}IPC_Handl_t;

typedef struct{
    unsigned char cmd_ID;
	unsigned char param[PARAMLEN];
}SENDST;

extern std::vector <IPC_Handl_t> IpcHandl;


int ipc_sendmsg(SENDST* Param,int Mesgthe );
int ipc_recvmsg(SENDST* Param,int Mesgthe );
void  ipc_status_P();
void  ipc_status_V();


void Ipc_create();
void Ipc_destory();


#endif

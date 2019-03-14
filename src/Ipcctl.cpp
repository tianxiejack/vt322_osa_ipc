#include "MessageQueueWrapper.h"
#include "SharedMemoryWrapper.h"
#include "SemaphoreWrapper.h"
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include "ipc_custom_head.hpp"
#include <vector>
#include <string>
#include <iostream>

std::vector <IPC_Handl_t> IpcHandl;

int ipc_sendmsg(SENDST* Param,int HandlID )
{
	ipc_sendmsg(HandlID,Param);
}

int ipc_sendmsg(int HandlID ,SENDST* Param)
{
	int ret;
	Message msg;
	char buffer[MESSAGE_SIZE];
	memset(buffer,0,sizeof(buffer));
	int mtype=1;
    memcpy(buffer,Param,sizeof(SENDST));
	setMessage(&msg, buffer, MESSAGE_SIZE, mtype);
	/* Send message: */
	ret = messageQueueSend(IpcHandl[HandlID].IPCID, &msg);
	return ret;
}

int ipc_recvmsg(SENDST* Param,int HandlID )
{
	ipc_recvmsg(HandlID,Param);
}

int ipc_recvmsg(int HandlID , SENDST* Param )
{
	int ret;
	Message msg;
	int mtype=1;
	clearMessage(&msg);
	ret = messageQueueReceive(IpcHandl[HandlID].IPCID, &msg, mtype);
    memcpy(Param,msg.buffer,sizeof(SENDST));
	return ret;
}

void  ipc_status_P()
{
	semWait(IpcHandl[IPC_SEM].IPCID,0);
}
void  ipc_status_V()
{
	semSignal(IpcHandl[IPC_SEM].IPCID,0);
}
using namespace std;
int Ipc_create()
{
	int flag = 0;
	if(IpcHandl.size())
	{
		key_t key;
		for(int i=0; i<IpcHandl.size(); i++)
		{
			switch(IpcHandl[i].Class)
			{
				case IPC_Class_MSG:
					key = messageKeyGet(IpcHandl[i].name,IpcHandl[i].Identify);
					IpcHandl[i].IPCID = messageQueueCreate(key);
					break;
				case IPC_Class_SHA:
					key = sharedKeyGet(IpcHandl[i].name,IpcHandl[i].Identify);
					if(IpcHandl[i].length > 0)
					{
						IpcHandl[i].IPCID = sharedMemoryCreateOrGet(key,IpcHandl[i].length);
						IpcHandl[i].ptr =(void *)sharedMemoryAttach(IpcHandl[i].IPCID,IpcHandl[i].RWmode);				
					}
					else
						flag = -1;
					break;

				case IPC_Class_SEM:
					key=semKeyGet(IpcHandl[i].name,IpcHandl[i].Identify);
					IpcHandl[i].IPCID=semCreate(key,1);
					break;

				default:
					flag = -1;
					break;					
			}
		}
	}
	return flag;
}

void Ipc_destory()
{
	if(IpcHandl.size())
	{
		for(int i=0;i<IpcHandl.size();i++)
		{
			if(IpcHandl[i].Class==IPC_Class_MSG)
			{
				messageQueueDelete(IpcHandl[i].IPCID);
			}
			else if(IpcHandl[i].Class==IPC_Class_SHA)
			{
				sharedMemoryDelete(IpcHandl[i].IPCID);
			}
			else if(IpcHandl[i].Class==IPC_Class_SEM)
			{
				semDelete(IpcHandl[i].IPCID);
			}
		}
	}
	return ;
}



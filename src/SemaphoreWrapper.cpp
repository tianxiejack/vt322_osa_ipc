#include "SemaphoreWrapper.h"
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
 #include <error.h>
#include <string.h>

key_t semKeyGet(std::string inPath,int proj_id)
{
	const char* path = inPath.c_str();
	key_t key = ftok(path,proj_id);
	if(key < 0){
		printf("%d:%s\n",errno,strerror(errno));
		return -1;
	}
	return key;
}

int semCreate(key_t key, int number_of_sem)
{
  int semid;

   semid = semget(key, number_of_sem, 0666 | IPC_CREAT|IPC_EXCL);
	if(semid == -1 && errno == EEXIST)
 	{

		semid= semGet(key,number_of_sem);
		if(semid==-1)
			return -1;
		//printf("******************************eexist\n");
		//semSetValue(semid,0,0);
		return semid;
 	}
	else if(semid==-1)
	{

		return -1;
	}
	semSetValue(semid,0,1);
  return semid;
}

/* Returns the semaphore ID for the path specified */                       
int semGet(key_t key, int number_of_sem)
{
  int semid;

  if( (semid = semget(key, number_of_sem, 0666)) == -1 )
    return -1;

  return semid;
}

int semDelete(int semid)
{
  union semun arg;

  return semctl(semid, 0, IPC_RMID, arg);
}

/* Sets semaphore_number assoc. with the semid to value */                        
int semSetValue(int semid, int semaphore_number, int value)
{
  union semun arg;
  arg.val = value;

  return semctl(semid, semaphore_number, SETVAL, arg);
}

/* Gets the semaphore value from semaphore_number assoc. with the semid */       
int semGetValue(int semid, int semaphore_number)
{
  union semun arg;

  return semctl(semid, semaphore_number, GETVAL, arg);
}

/* Increment semaphore value by one */                     
int semSignal(int semid, int semaphore_number)
{
  struct sembuf sops;
  sops.sem_num = semaphore_number;
  sops.sem_op = 1;
  sops.sem_flg = SEM_UNDO;
  return semop(semid, &sops, 1);
}

int semIncrement(int semid, int semaphore_number, int incrementBy)
{
  struct sembuf sops;
  sops.sem_num = semaphore_number;
  sops.sem_op = incrementBy;
  sops.sem_flg = SEM_UNDO;
  return semop(semid, &sops, 1);
}

/* Decrement semaphore value by one */ 
int semWait(int semid, int semaphore_number)
{
  struct sembuf sops;
  sops.sem_num = semaphore_number;
  sops.sem_op = -1;
  sops.sem_flg = SEM_UNDO;
  return semop(semid, &sops, 1);
}

int semDecrment(int semid, int semaphore_number, int decrementBy)
{
  struct sembuf sops;
  sops.sem_num = semaphore_number;
  sops.sem_op = decrementBy;
  sops.sem_flg = SEM_UNDO;
  return semop(semid, &sops, 1);
}

/* Increment semaphore value by one */                     
int semSignal_timeout(int semid, int semaphore_number, long usecs)
{
  struct sembuf sops;
  struct timespec timeout;

  sops.sem_num = semaphore_number;
  sops.sem_op = 1;
  sops.sem_flg = SEM_UNDO;

  timeout.tv_sec = 0;
  timeout.tv_nsec = usecs*1000;

  return semtimedop(semid, &sops, 1, &timeout);
}

int semIncrement_timeout(int semid, int semaphore_number, int incrementBy, long usecs)
{
  struct sembuf sops;
  struct timespec timeout;

  sops.sem_num = semaphore_number;
  sops.sem_op = incrementBy;
  sops.sem_flg = SEM_UNDO;

  timeout.tv_sec = 0;
  timeout.tv_nsec = usecs*1000;

  return semtimedop(semid, &sops, 1, &timeout);
}

/* Decrement semaphore value by one */      
int semWait_timeout(int semid, int semaphore_number, long usecs)
{
  struct sembuf sops;
  struct timespec timeout;

  sops.sem_num = semaphore_number;
  sops.sem_op = -1;
  sops.sem_flg = SEM_UNDO;

  timeout.tv_sec = 0;
  timeout.tv_nsec = usecs*1000;

  return semtimedop(semid, &sops, 1, &timeout);
}

int semDecrement_timeout(int semid, int semaphore_number, int decrementBy, long usecs)
{
  struct sembuf sops;
  struct timespec timeout;

  sops.sem_num = semaphore_number;
  sops.sem_op = decrementBy;
  sops.sem_flg = SEM_UNDO;

  timeout.tv_sec = 0;
  timeout.tv_nsec = usecs*1000;

  return semtimedop(semid, &sops, 1, &timeout);
}

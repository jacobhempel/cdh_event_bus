#ifndef INCLUDE_UTILITY_H
#define INCLUDE_UTILITY_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>

#include <exception>
#include <system_error>
#include <cerrno>


#define MEMKEY 0x45861534
#define SEMKEY 0x45861534
#define MSGKEY 0x45861534

#define MSGLEN 126
#define SHMSIZE 4096

#define NUMMODULES 1
#define SUBCHANNEL 3

const long SUB_BIT = 0x20000000;
const long TEMP_BIT = 0x40000000;

const long CREATE_PUB = 1;
const long PUBLISH_CODE = 2;

typedef struct {
    unsigned rw_array[2];
} shm_object;

typedef struct {
  long type;
  char text[MSGLEN];
} message_buffer;

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

extern intptr_t* connect_shm();
extern int p(int sem, int counter);
extern int v(int sem, int counter);
extern int semsetall (int sem_group, int number_in_group, int value);


#endif  // INCLUDE_UTILITY_H

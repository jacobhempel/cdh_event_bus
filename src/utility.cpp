#include "utility.h"

intptr_t* connect_shm() {
    intptr_t* return_value = NULL;
    int shm_id;

    if ((shm_id = shmget(MEMKEY, SHMSIZE, 0600)) >= 0) {
        if ((return_value = (intptr_t*)shmat(shm_id, NULL, 0))
            < (intptr_t*)0 ) {
            return_value = NULL;
        }
    }

    return return_value;
}

int semsetall (int sem_group, int number_in_group, int value) {
    union semun sem_ctl_un;

    sem_ctl_un.val = value;
    for (int i = 0; i < number_in_group; ++i) {
        if (semctl(sem_group, i, SETVAL, sem_ctl_un) < 0) {
            perror("semsetall failed");
            return -1;
        }
    }

    return 0;
}

int p (int sem_group, int donut_type) {
    struct sembuf sembuf;

    sembuf.sem_num = donut_type;
    sembuf.sem_op  = -1;
    sembuf.sem_flg = 0;

    if (semop(sem_group, &sembuf, 1) < 0) {
        perror("p opperation failed");
        return (-1);
    }

    return 0;
}

int v (int sem_group, int donut_type) {
    struct sembuf sembuf;

    sembuf.sem_num = donut_type;
    sembuf.sem_op  = (+1);
    sembuf.sem_flg = 0;

    if (semop(sem_group, &sembuf, 1) == -1) {
        perror("v opperation failed");
        return (-1);
    }

    return 0;
}

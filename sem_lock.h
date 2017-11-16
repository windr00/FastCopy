//
// Created by windr on 17-11-16.
//

#ifndef FASTCOPY_LOCK_H
#define FASTCOPY_LOCK_H

#include <semaphore.h>

class sem_lock {
private:

    sem_t _sharedLocks;

public:

    sem_lock(int count);

    ~sem_lock();

    void lock();

    void release();

};

#endif //FASTCOPY_LOCK_H

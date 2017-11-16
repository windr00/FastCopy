//
// Created by windr on 17-11-16.
//
#include <semaphore.h>
#include "sem_lock.h"

sem_lock::sem_lock(int count) {
    sem_init(&_sharedLocks, 0, count);
}

sem_lock::~sem_lock() {
    sem_close(&_sharedLocks);
}

void sem_lock::lock() {
    sem_wait(&this->_sharedLocks);
}

void sem_lock::release() {
    sem_post(&this->_sharedLocks);
}

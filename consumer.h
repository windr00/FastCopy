//
// Created by windr on 17-11-16.
//

#ifndef FASTCOPY_CONSUMER_H
#define FASTCOPY_CONSUMER_H

#include <semaphore.h>
#include <cstdio>
#include "sem_lock.h"

class consumer {

private:

    sem_lock *_writterLock;

    sem_lock *_readerLock;

    sem_lock *_consumerWriteDoneLock;

    const char *_fileName;

    int _idx;

    int _writeCount;

    int _consumerCount;

    int *_lastConsumerWriteIdx;

    size_t *_actualReadSize;

    size_t _maxLength;

    pthread_t _thread;

    FILE *_file;

    unsigned char *_buffer;

    static void *writter(consumer *);


public:

    consumer(sem_lock *writterLock,
             sem_lock *readerLock,
             sem_lock *consumerWriteDoneLock,
             const char *fileName,
             unsigned char *buffer,
             int consumerIdx,
             int consumerCount,
             int *lastConsumerWriteIdx,
             size_t *actualReadSize,
             size_t maxLength);

    void start();

    ~consumer();
};

#endif //FASTCOPY_CONSUMER_H

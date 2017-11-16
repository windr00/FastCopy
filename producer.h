//
// Created by windr on 17-10-31.
//

#ifndef FASTCOPY_PRODUCER_H
#define FASTCOPY_PRODUCER_H

#include <pthread.h>
#include <fstream>
#include "sem_lock.h"

class producer {
private:


    pthread_t _thread;

    const char *_readFileName;


    size_t _bufferLength;

    int _consumerCount;

    unsigned char **_buffers;

    size_t **_actualReadSize;

    sem_lock **_readerLock;

    sem_lock **_writterLock;

    int *_lastWriteConsumerIdx;

    FILE *_readFileHandle;



public:

    static producer *getInstance();

    void start();

    producer *setFileName(const char *);

    producer *setSharedLock(sem_lock **readLock, sem_lock **writeLock);

    producer *setReadBuffer(unsigned char **buffers, size_t **actualReadSize, size_t length, int consumerCount);

    producer *setLastWriteConsumerIndex(int *idx);

private:


    explicit producer();

    ~producer();

    static void *reader(producer *);

};


#endif //FASTCOPY_PRODUCER_H

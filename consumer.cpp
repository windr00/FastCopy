//
// Created by windr on 17-11-16.
//

#include <pthread.h>
#include "consumer.h"

consumer::consumer(sem_lock *writterLock,
                   sem_lock *fileLock,
                   sem_lock *readLock,
                   sem_lock *writeDoneLock,
                   const char *fileName,
                   unsigned char *buffer,
                   int consumerIdx,
                   int consumerCount,
                   int *lastConsumerWriteIdx,
                   size_t *actualReadSize,
                   size_t maxLength) {
    this->_writterLock = writterLock;
    this->_readerLock = readLock;
    this->_fileName = fileName;
    this->_idx = consumerIdx;
    this->_actualReadSize = actualReadSize;
    this->_buffer = buffer;
    this->_consumerWriteDoneLock = writeDoneLock;
    this->_maxLength = maxLength;
    this->_lastConsumerWriteIdx = lastConsumerWriteIdx;
    this->_consumerCount = consumerCount;
    this->_writeCount = 0;
    this->_fileLock = fileLock;
}


void consumer::start() {
    _file = fopen(_fileName, "wb");
    fseek(_file, _maxLength * _idx, 0);
    pthread_create(&_thread, NULL, (void *(*)(void *)) this->writter, this);
}

void *consumer::writter(consumer *c) {
    printf("consumer number %d running\n", c->_idx);
    while (true) {
        c->_writterLock->lock();
        c->_fileLock->lock();
        fseek(c->_file, c->_writeCount * c->_maxLength * c->_consumerCount, (int) c->_maxLength * c->_idx);
        fwrite(c->_buffer, sizeof(unsigned char), *(c->_actualReadSize), c->_file);
        printf("consumer number %d wrote %d at %d\n", c->_idx, *(c->_actualReadSize), ftell(c->_file));
        c->_writeCount++;
        c->_fileLock->release();
        if (*(c->_actualReadSize) < c->_maxLength || *(c->_lastConsumerWriteIdx) != -1) {
            c->_consumerWriteDoneLock->release();
            break;
        }
        c->_readerLock->release();
    }
}

consumer::~consumer() {
    pthread_exit(&_thread);
    fclose(_file);
}
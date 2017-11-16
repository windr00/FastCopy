//
// Created by windr on 17-10-31.
//

#include "producer.h"
#include <memory.h>
#include <sys/stat.h>

static producer *_instance;

producer::producer() {
}

producer::~producer() {
    pthread_exit(&_thread);
    fclose(_readFileHandle);
}

producer *producer::getInstance() {
    if (_instance == NULL) {
        _instance = new producer();
    }
}

void *producer::reader(producer *p) {
    printf("producer thread running\n");
    while (!feof(p->_readFileHandle)) {
        for (int i = 0; i < p->_consumerCount; i++) {
            p->_readerLock[i]->lock();
            char temp[p->_bufferLength] = {0};
            *(p->_actualReadSize[i]) = fread(temp, sizeof(unsigned char), p->_bufferLength, p->_readFileHandle);
//            printf("producer read %u for consumer %d\n", *(p->_actualReadSize[i]), i);
            if (feof(p->_readFileHandle)) {
                *(p->_lastWriteConsumerIdx) = i;
                p->_writterLock[i]->release();
                break;
            }
            memcpy(p->_buffers[i], temp, (size_t) p->_bufferLength);
            p->_writterLock[i]->release();
        }
    }
}

producer *producer::setReadBuffer(unsigned char **buffers, size_t **actualReadSize, size_t length, int consumerCount) {
    this->_buffers = buffers;
    this->_consumerCount = consumerCount;
    this->_bufferLength = length;
    this->_actualReadSize = actualReadSize;
}

producer *producer::setSharedLock(sem_lock **readLock, sem_lock **writeLock) {
    _readerLock = readLock;
    _writterLock = writeLock;
}

producer *producer::setFileName(const char *readPath, const char *writePath) {
    this->_readFileName = readPath;
    this->_writeFileName = writePath;
    return this;
}

void producer::start() {
    _readFileHandle = fopen(_readFileName, "rb");
    struct stat statbuff;
    stat(_readFileName, &statbuff);
    _writeFileHandle = fopen(_writeFileName, "wb");
    unsigned char tmp[statbuff.st_size] = {0};
    fwrite(tmp, sizeof(unsigned char), statbuff.st_size, _writeFileHandle);
    fclose(_writeFileHandle);
    pthread_create(&_thread, NULL, (void *(*)(void *)) this->reader, this);
}

producer *producer::setLastWriteConsumerIndex(int *idx) {
    this->_lastWriteConsumerIdx = idx;
}
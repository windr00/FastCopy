//
// Created by windr on 17-10-31.
//

#include "producer.h"
#include <memory.h>

explicit producer::producer() {
    pthread_create(_thread, NULL, this->reader, NULL);
    sem_init(_internalReaderLock, 0, 1);
}

producer::~producer() {
    sem_close(_internalReaderLock);
    pthread_exit(_thread);
    _filehandle.close();
}

producer *producer::GetInstance() {
    if (_instance == nullptr) {
        _instance = new producer();
    }
}

void *producer::reader(void *) {
    sem_wait(_internalReaderLock);
    while (!_filehandle.eof()) {
        sem_wait(_sharedLock);
        for (int i = 0; i < _consumerCount; i++) {
            char temp[_bufferLength] = {0};
            for (int j = 0; j < _bufferLength; j++) {
                _filehandle >> temp[j];
            }
            memcpy(_buffers[i], temp, (size_t) _bufferLength);
        }
    }
}

void producer::setReadBuffer(unsigned char **buffers, int length, int consumerCount) {
    this->_buffers = buffers;
    this->_consumerCount = consumerCount;
    this->_bufferLength = length;
}

void producer::setSharedLock(sem_t *lock) {
    this->_sharedLock = lock;
}

void producer::start() {
    _filehandle.open(_filename);
    sem_post(_internalReaderLock);
}
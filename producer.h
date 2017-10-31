//
// Created by windr on 17-10-31.
//

#ifndef FASTCOPY_PRODUCER_H
#define FASTCOPY_PRODUCER_H

#include <pthread.h>
#include <semaphore.h>
#include <fstream>

class producer {
private:

    static producer *_instance = nullptr;

    pthread_t *_thread;

    const char *_filename;

    int _bufferLength;

    int _consumerCount;

    unsigned char **_buffers;

    sem_t *_sharedLock;

    sem_t *_internalReaderLock;

    std::ifstream _filehandle;

public:

    static producer *GetInstance();

    void start();

    void setFileName(const char *);

    void setSharedLock(sem_t *lock);

    void setReadBuffer(unsigned char **buffers, int length, int consumerCount);

private:


    explicit producer();

    ~producer();

    void *reader(void *);

};


#endif //FASTCOPY_PRODUCER_H

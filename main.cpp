#include <iostream>
#include "producer.h"
#include "consumer.h"


int main() {
    int consumerCount = 2;
    size_t length = 2;
    const char *readFile = "/home/windr/Desktop/test.txt";
    const char *writeFile = "/home/windr/Desktop/test-cpy.txt";
    auto **readLocks = new sem_lock *[consumerCount];
    auto **writeLocks = new sem_lock *[consumerCount];
    auto *writeDoneLock = new sem_lock(0);
    auto **consumers = new consumer *[consumerCount];
    auto **buffers = new unsigned char *[consumerCount];
    auto **actualReadLength = new size_t *[consumerCount];
    auto *lastConsumerWriteLock = new int(-1);
    auto *fileLock = new sem_lock(1);
    for (int i = 0; i < consumerCount; i++) {
        readLocks[i] = new sem_lock(1);
        writeLocks[i] = new sem_lock(0);
        buffers[i] = new unsigned char[length];
        actualReadLength[i] = new size_t(0);
        consumers[i] = new consumer(writeLocks[i],
                                    fileLock,
                                    readLocks[i],
                                    writeDoneLock,
                                    writeFile,
                                    buffers[i],
                                    i, consumerCount,
                                    lastConsumerWriteLock,
                                    actualReadLength[i],
                                    length);
    }
    for (int i = 0; i < consumerCount; i++) {
        consumers[i]->start();
    }

    producer::getInstance()
            ->setFileName(readFile, writeFile)
            ->setSharedLock(readLocks, writeLocks)
            ->setReadBuffer(buffers, actualReadLength, length, consumerCount)
            ->setLastWriteConsumerIndex(lastConsumerWriteLock)
            ->start();

    writeDoneLock->lock();
    return 0;
}
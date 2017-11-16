#include <iostream>
#include "producer.h"
#include "consumer.h"

void printHelp() {
    printf("usage: <origin file> <new file>");
}


int main(int argc, const char **argv) {
    int consumerCount = 2;
    size_t length = 4096;
    const char *readFile = NULL;
    const char *writeFile = NULL;

    if (argc != 3) {
        printHelp();
        return -1;
    }

    for (int i = 1; i < argc; i++) {
        if (readFile == NULL) {
            readFile = argv[i];
        } else {
            writeFile = argv[i];
        }
    }
    if (readFile == NULL || writeFile == NULL) {
        printHelp();
        return -1;
    }

    auto **readLocks = new sem_lock *[consumerCount];
    auto **writeLocks = new sem_lock *[consumerCount];
    auto *writeDoneLock = new sem_lock(0);
    auto **consumers = new consumer *[consumerCount];
    auto **buffers = new unsigned char *[consumerCount];
    auto **actualReadLength = new size_t *[consumerCount];
    auto *lastConsumerWriteLock = new int(-1);
    for (int i = 0; i < consumerCount; i++) {
        readLocks[i] = new sem_lock(1);
        writeLocks[i] = new sem_lock(0);
        buffers[i] = new unsigned char[length];
        actualReadLength[i] = new size_t(0);
        consumers[i] = new consumer(writeLocks[i],
                                    readLocks[i],
                                    writeDoneLock,
                                    writeFile,
                                    buffers[i],
                                    i, consumerCount,
                                    lastConsumerWriteLock,
                                    actualReadLength[i],
                                    length);
    }
    producer::getInstance()
            ->setFileName(readFile)
            ->setSharedLock(readLocks, writeLocks)
            ->setReadBuffer(buffers, actualReadLength, length, consumerCount)
            ->setLastWriteConsumerIndex(lastConsumerWriteLock)
            ->start();
    for (int i = 0; i < consumerCount; i++) {
        consumers[i]->start();
    }
    writeDoneLock->lock();
    return 0;
}
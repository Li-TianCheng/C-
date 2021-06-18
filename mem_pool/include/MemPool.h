//
// Created by ltc on 2021/3/6.
//

#ifndef MEMPOOL_MEMPOOL_H
#define MEMPOOL_MEMPOOL_H

#include "ManageChunk.h"
#include "../../my_pthread/include/Mutex.h"
#include <vector>
#include <unordered_set>
#include <iostream>

using std::vector;

class MemPool {
public:
    explicit MemPool(int num);
    void* allocate(size_t size);
    void deallocate(void* ptr, size_t size);
    ~MemPool() = default;
    MemPool(const MemPool&) = delete;
    MemPool(MemPool&&) = delete;
    MemPool& operator=(const MemPool&) = delete;
    MemPool& operator=(MemPool&&) = delete;
private:
    vector<ManageChunk> mem;
    vector<Mutex> mutex;
    int num;
};


#endif //MEMPOOL_MEMPOOL_H

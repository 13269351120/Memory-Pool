#ifndef FIXED_SIZE_MEMORY_POOL_H
#define FIXED_SIZE_MEMORY_POOL_H
#include <stdlib.h>

typedef unsigned short USHORT;

class MemoryBlock;


const int MEMPOOL_ALIGNMENT = 8;

class MemoryPool
{
    public:
        MemoryPool(USHORT nUnitSize, USHORT nInitSize = 1024, USHORT nGrowSize = 256 );

        ~MemoryPool();

        void* Alloc();

        void Free(void* p);

    private:
        MemoryBlock* headBlock;
        //
        USHORT m_nUnitSize;
        USHORT m_nInitSize;
        USHORT m_nGrowSize;
};


#endif
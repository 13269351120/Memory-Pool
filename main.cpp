#include <iostream>
#include <cassert>
#include <ctime>
#include <vector>

//#include "MemoryPool.hpp"
#include "StackAlloc.hpp"

#define ELEMS 1000000

#define REPS 100

int main()
{
    clock_t start;

    //使用 STL 默认分配器
    StackAlloc<int, std::allocator<int>> stackDefault;
    start = clock();
    for (int j = 0; j < REPS; j++)
    {
        assert(stackDefault.empty());
        for (int i = 0; i < ELEMS; i++)
            stackDefault.push(i);
        for (int i = 0; i < ELEMS; i++)
            stackDefault.pop();
    }
    std::cout << "Default Allocator Time: " ;
    std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n" ;

    //使用 内存池 
   // StackAlloc<int, MemoryPool<int>> stackPool;
   // start = clock();
   // for (int j = 0; j < ERPS; j++)
   // {
   //     for (int i = 0; i < ELEMS; i++)
   //         stackPool.push(i);
   //     for (int i = 0; i < ELEMS; i++)
   //         stackPool.pop();
   // }
   // std::cout << "MemoryPool Allocator Time: " ;
   // std::cout << (((double)clock() - start) / CLOCK_PER_SEC) << "\n\n" 

    return 0;
}

# Memory-Pool
本文可以分为4个部分：`内存池的简介及工作原理`，`固定大小线程非安全内存池的实现`，`线程安全内存池的实现`，`总结回顾`。

本文主要参考：
* 《C++应用程序性能优化》
* https://github.com/DavidLiRemini/MemoryPool
* https://github.com/lenonk/memorypool

本文主要是为了个人学习理解内存池而作，并且结合了自己的理解，如果对您的学习理解有帮助可以`star`，欢迎帮忙分享，如果存在理解上的错误，欢迎吐槽和指正。

本项目并无商业用途，如有侵权请告知，谢谢各位大佬~

### 内存池简介及工作原理：
内存池是池化技术中的一种形式。通常我们在编写程序的时候回使用` new` `delete` 这些关键字来向操作系统申请内存，而这样造成的后果就是每次申请内存和释放内存的时候，都需要和操作系统的`系统调用`打交道，从堆中分配所需的内存。如果这样的操作太过频繁，就会找成大量的**内存碎片**进而降低内存的分配性能，甚至出现内存分配失败的情况。

而内存池就是为了解决这个问题而产生的一种技术。从内存分配的概念上看，内存申请无非就是向内存分配方索要一个指针，当向操作系统申请内存时，操作系统需要进行复杂的内存管理调度之后，才能正确的分配出一个相应的指针。而这个分配的过程中，我们还面临着分配失败的风险。

#### 内存池的思想：
**集中分配**：每一次进行内存分配，就会消耗一次分配内存的时间，设这个时间为 T，那么进行 n 次分配总共消耗的时间就是 nT；如果我们一开始就确定好我们可能需要多少内存，那么在最初的时候就分配好这样的一块内存区域，当我们需要内存的时候，直接从这块已经分配好的内存中使用即可，那么总共需要的分配时间仅仅只有 T。当 n 越大时，节约的时间就越多。

**固定分配**：内存池实现的是固定大小的内存的分配，而且往往是比较小的内存分配。比较小的内存在`malloc`的时候，在侯捷老师的课上知道每一次`malloc`分配出来的内存，都会带有上下两个**cookie**，每个占4个字节，用作标识内存是否被使用，标记大小以帮助回收等作用。而且会额外塞入若干个**padding**使内存对齐。所以小内存的分配空间利用率比较低，尤其适合使用内存池技术来提高内存利用率，同时比较容易控制页边界和内存字节对齐，可以减少内存碎片和提高内存locality来提高效率。

总之内存池解决了两个问题：
* 频繁的系统调用
* 内存碎片

#### 内存池的分类：
* 从线程安全角度来分：可以分为单线程内存池和多线程内存池。
* 从分配单元大小来分：可以分为固定内存池和可变内存池。


#### 内存池工作原理：
以固定内存池为例子，分析内存池的工作原理。

固定内存池由一系列固定大小的内存块组成，每个内存块又包含了固定数量和大小的内存单元，内存块与块之间用链表的形式串连起来，重要的信息存储在内个内存块头部。

**pics：guding**

**分配**：只需要简单的遍历所有的内存池块头信息，快速定位到还有空闲单元的那个内存块，然后返回单元地址，更新内存池块头部信息即可，如果没有空闲的内存块了，那么就向操作系统申请一块新的内存块切分成若干个小块后，返回即可。
**释放**：将释放的内存定位到当初取出来的内存块后，将其标记为空闲后，更新内存池块头部信息即可。

---

### 固定大小线程非安全内存池的实现
#### 总体思路
对于一个固定大小的内存池，需要控制和维护的内容：
* 每个分配出去的单元的大小
* 每个内存块的大小（这里还区分初始化时第一个内存块『相对较大』 和 内存不够时再次申请的内存块『相对较小』）
* 第一块内存块的指针


---

main.cpp:
在·main.cpp·中，这是一个性能测试的程序，包含ctime头文件，使用两次`clock`获取时间差，并且除以`CLOCKS_PER_SEC`就可以获得运行的秒数。

---
StackAlloc.hpp
StackAlloc 是一个链表形式的栈，接受两个模板参数，第一个参数是栈中的元素类型，第二个参数就是栈使用的内存分配器。
因此，这个内存分配器的模板参数就是整个比较过程中唯一的变量，使用默认分配器的模板参数为 std::allocator<int>，
  
  

而使用内存池的模板参数为 MemoryPool<int>。

std::allocator 是 C++标准库中提供的默认分配器，他的特点就在于我们在 使用 new 来申请内存构造新对象的时候，势必要调用类对象的默认构造函数，而使用 std::allocator 则可以将内存分配和对象的构造这两部分逻辑给分离开来，使得分配的内存是原始、未构造的。然后调用construct进行对象初始化。


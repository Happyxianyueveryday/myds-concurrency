# myds-concurrency

本项目的最新更新以及补充内容已经迁移至：https://github.com/Happyxianyueveryday/vessel-concurrency ，敬请移步至该链接。

基于C++的相对线程安全的典型容器实现。

附注：cpp的stl本身大多数容器不支持并发，本项目是根据其他项目时的需求设计的，从零开始构造的线程安全容器，包含支持并发的一些典型容器。其中部分在设计原理上参照了java.util.concurrent并发容器库的实现，部分则是自己根据实际需求设计的新容器（例如ConcurrentLinkedSet），并且在锁的粒度以及一致性上提供了多种选择。

## 目录

```
root/ 
  // 1. 锁与信号量
  SpinLock/ -- 自旋锁 √
    SpinLock -- 非公平自旋锁 √
    TicketSpinLock -- 公平自旋锁（返回排队号） √
    TicketLocalSpinLock -- 公平自旋锁（thread_local版本，不返回排队号）√
    MCSLock -- 公平自旋锁
    ReentrantSpinLock -- 可重入自旋锁
    
  ReadWriteMutex/ -- 读写锁 √
    rwmutex -- 读优先的读写锁 √
    wrmutex -- 写优先的读写锁 √
    fair_rwmutex -- 读写公平的读写锁 √
  Semaphore -- 信号量
    
  // 2. 线程安全容器部分
  Stack/ -- 栈
    ConcurrentArrayStack -- 数组栈（互斥锁或三种策略的读写锁）√
    ConcurrentLinkedStack -- 链表栈（互斥锁或三种策略的读写锁）√
  Queue/ --队列
    ConcurrentArrayQueue -- 环形数组单向队列（互斥锁或三种策略的读写锁）√
    ConcurrentLinkedQueue -- 链表单向队列（互斥锁或三种策略的读写锁）√
  BlockingStack/ -- 阻塞栈
    ConcurrentBlockingStack -- 链表阻塞栈 （条件变量，可重入锁）√
  BlockingQueue/ -- 阻塞队列
    LinkedBlockingQueue -- 链表阻塞队列（条件变量，可重入锁）√
    ArrayBlockingQueue -- 环形数组阻塞队列 √ 
    LockFreeRingBuffer -- 无锁并发环形缓冲区 
  Vector/ -- 数组
    CopyOnWriteArrayList -- 写时复制数组容器（可重入锁）√
  Set/ -- 集合
    ConcurrentLinkedSet -- 基于链表的集合（交替锁）√
  HashSet/ -- 哈希集合
    ConcurrentStrongHashSet -- 强一致性哈希集合（分段锁+互斥锁） √
    ConcurrentWeakHashSet -- 弱一致性哈希集合（分段锁+无锁读，或分段锁+读写锁）√  
 ```


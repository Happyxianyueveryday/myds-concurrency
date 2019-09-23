# myds-concurrency

基于C++的相对线程安全的典型容器实现。

附注：cpp的stl本身大多数容器不支持并发，本项目是根据其他项目时的需求设计的，包含支持并发的一些典型容器。其中部分在设计原理上参照了java.util.concurrent并发容器库的实现，部分则是自己根据实际需求设计的新容器（例如ConcurrentLinkedSet），并且在锁的粒度以及一致性上提供了多种选择。

## 目录

```
root/ 
  // 1. 基本锁机制部分
  SpinningMutex -- 自旋锁
  ReadWriteMutex/ -- 读写锁 √
    rwmutex -- 读优先的读写锁 √
    wrmutex -- 写优先的读写锁 √
    fair_rwmutex -- 读写公平的读写锁 √
    
  // 2. 线程安全容器部分
  Stack/ -- 栈
    ConcurrentArrayStack -- 数组栈（互斥锁或三种策略的读写锁）√
    ConcurrentLinkedStack -- 链表栈（互斥锁或三种策略的读写锁）√
  BlockingStack/ -- 阻塞栈
    ConcurrentBlockingStack -- 链表阻塞栈 （条件变量，可重入锁）√
  Queue/ --队列
    ConcurrentArrayQueue -- 环形数组单向队列（互斥锁或三种策略的读写锁）√
    ConcurrentLinkedQueue -- 链表单向队列（互斥锁或三种策略的读写锁）√
  BlockingQueue/ -- 阻塞队列
    LinkedBlockingQueue -- 链表阻塞队列（条件变量，可重入锁）√
    ArrayBlockingQueue -- 环形数组阻塞队列 √ 
    LockFreeRingBuffer -- 无锁并发环形缓冲区 
  Vector/ -- 数组
    CopyOnWriteArrayList -- 写时复制数组容器（可重入锁）
  Set/ -- 集合
    ConcurrentLinkedSet -- 基于链表的集合（交替锁）√
  HashSet/ -- 哈希集合
    ConcurrentStrongHashSet -- 强一致性哈希集合（分段锁+互斥锁） √
    ConcurrentWeakHashSet -- 弱一致性哈希集合（分段锁+HashEntry，以及分段锁+读写锁）√ 
 ```


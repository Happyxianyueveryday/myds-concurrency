# myds-concurrency

基于C++的线程安全的典型容器实现。

附注：因为C++的stl库并不是线程安全的，而在自己的另一个关于cpp网络编程框架中需要使用各种各样的线程安全的容器，因此自己手写实现了一套非常基础的支持并发的典型容器，在设计原理上参照了java.util.concurrent并发容器库的实现，并且在锁的粒度上提供了多种选择。

## 目录

```
root/ 
  // 1. 基本锁机制部分
  ReadWriteMutex/ -- 读写锁 √
    rwmutex -- 读优先的读写锁 √
    wrmutex -- 写优先的读写锁 √
    fair_rwmutex -- 读写公平的读写锁 √
    
  // 2. 线程安全容器部分
  Stack/ -- 栈
    ConcurrentArrayStack -- 线程安全的数组栈（互斥锁或三种策略的读写锁）√
    ConcurrentLinkedStack -- 线程安全的链表栈（互斥锁或三种策略的读写锁）√
    ConcurrentLockFreeStack -- 无锁线程安全的链表栈 
  BlockingStack/ -- 阻塞栈
    ConcurrentBlockingStack -- 阻塞栈 （条件变量，可重入锁）√
  Queue/ --队列
    ConcurrentArrayQueue -- 线程安全的数组单向队列（互斥锁或三种策略的读写锁）√
    ConcurrentLinkedQueue -- 线程安全的链表单向队列（互斥锁或三种策略的读写锁）√
    ConcurrentLockFreeQueue -- 无锁线程安全的链表队列 
  BlockingQueue/ -- 阻塞队列
    LinkedBlockingQueue -- 链表阻塞队列（条件变量，可重入锁）√
    ArrayBlockingQueue -- 
    DelayQueue -- 
  Vector/ -- 数组
    CopyOnWriteArrayList -- 线程安全的动态数组（写时复制容器，可重入锁）
  Set/ -- 集合
    ConcurrentLinkedSet -- 基于链表的集合（交替锁）
    ConcurrentSkipList -- 基于跳表的集合
  HashSet/HashMap/ -- 哈希表集合
    ConcurrentHashset -- 线程安全的哈希集合（分段锁） 
    ConcurrentHashMap -- 线程安全的哈希映射（分段锁）
    
  // 3. 线程池
 ```


# myds-concurrency

线程安全的典型容器实现。

并发编程始终是编程中极具挑战的部分，本人水平有限且测试不足，发现错误欢迎在issue中提出，感激不尽！！！

## 文件目录结构

```
root/ 
  // 1. 基本锁机制部分
  SpinningMutex/ -- 自旋锁 
  ReadWriteMutex/ -- 读写锁 √
    rwmutex -- 读优先的读写锁 √
    wrmutex -- 写优先的读写锁 √
    fair_rwmutex -- 读写公平的读写锁 √
    
  // 2. 线程安全容器部分
  List/ 
    ConcurrentLinkedList -- 线程安全的链表（交替锁） 
  Stack/ 
    ConcurrentArrayStack -- 线程安全的数组栈（互斥锁或读写锁）√
    ConcurrentLinkedStack -- 线程安全的链表栈（互斥锁或读写锁）√
    ConcurrentBlockingStack -- 阻塞栈 √
  Queue/
    ConcurrentArrayQueue -- 线程安全的数组单向队列（互斥锁或读写锁）√
    ConcurrentLinkedQueue -- 线程安全的链表单向队列（互斥锁或读写锁）√
    ConcurrentLinkedDeque -- 线程安全的双向队列 
    ConcurrentBlockingQueue -- 阻塞队列 √
  Vector/
    CopyOnWriteArrayList -- 线程安全的动态数组（写时复制容器，可重入锁）
  HashSet/
    ConcurrentHashset -- 线程安全的哈希集合（分段锁） 
  HashMap/
    ConcurrentHashMap -- 线程安全的哈希映射（分段锁）
 ```


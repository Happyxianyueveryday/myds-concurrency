# myds-concurrency

线程安全的典型容器实现。

附注：因为cpp的stl库并不是线程安全的，而在自己的另一个关于cpp网络编程框架中需要使用各种各样的线程安全的容器，因此自己手写实现了一套非常基础的支持并发的典型容器，在设计原理上部分参照了java.util.concurrent并发容器库的实现。

## 文件目录结构

```
root/ 
  // 1. 基本锁机制部分
  SpinningMutex -- 自旋锁 
  ReadWriteMutex/ -- 读写锁 √
    rwmutex -- 读优先的读写锁 √
    wrmutex -- 写优先的读写锁 √
    fair_rwmutex -- 读写公平的读写锁 √
    
  // 2. 线程安全容器部分
  List/ 
    ConcurrentLinkedList -- 线程安全的链表（交替锁） √
    ConcurrentOptLinkedList -- 线程安全的链表（交替锁+乐观锁策略） √
  Stack/ 
    ConcurrentArrayStack -- 线程安全的数组栈（互斥锁或读写锁）√
    ConcurrentLinkedStack -- 线程安全的链表栈（互斥锁或读写锁）√
    ConcurrentBlockingStack -- 阻塞栈 （条件变量，可重入锁）√
    ConcurrentLinkedStack(Java) -- java的线程安全链表栈（CAS）预计9.15上午完成
  Queue/
    ConcurrentArrayQueue -- 线程安全的数组单向队列（互斥锁或读写锁）√
    ConcurrentLinkedQueue -- 线程安全的链表单向队列（互斥锁或读写锁）√
    ConcurrentBlockingQueue -- 阻塞队列（条件变量，可重入锁）√
    ConcurrentLinkedQueue(Java) -- java的线程安全链表队列（CAS）预计9.15上午完成
  Vector/
    CopyOnWriteArrayList -- 线程安全的动态数组（写时复制容器，可重入锁）
  HashSet/
    ConcurrentHashset -- 线程安全的哈希集合（分段锁） √ 9.15上午
    
  // 3. 线程池
 ```


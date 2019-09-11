# myds-concurrency

线程安全的典型容器实现。

并发编程始终是编程中极具挑战的部分，本人水平有限，发现错误欢迎在issue中提出，感激不尽！！

## 文件目录结构

```
root/ 
  ReadWriteMutex/ -- 读写锁 √
    rwmutex -- 读优先的读写锁 √
    wrmutex -- 写优先的读写锁 √
    fair_rwmutex -- 读写公平的读写锁 √
  List/ -- 链表抽象类 
    ConcurrentLinkedList -- 线程安全的链表（交替锁） √
  Stack/ -- 栈抽象类 
    ConcurrentArrayStack/ -- 线程安全的数组栈（互斥锁或读写锁） √
    ConcurrentLinkedStack/ -- 线程安全的链表栈（互斥锁或读写锁） √
  Queue/ -- 队列抽象类  
    ConcurrentArrayQueue/ -- 线程安全的数组队列（互斥锁或读写锁） √
    ConcurrentLinkedQueue/ -- 线程安全的链表队列（互斥锁或读写锁）√
    ConcurrentBlockingQueue -- 阻塞队列 √
  HashMap/ -- 哈希表抽象类
    ConcurrentHashmap -- 线程安全的哈希表（分段锁） √
 ```


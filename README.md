# myds-concurrency

线程安全的典型容器实现。

## 文件目录结构

```
root/ 
  SpinMutex/ -- 自旋锁
  ReadWriteMutex/ -- 读写锁 √
    rwmutex -- 读优先的读写锁 √
    fair_rwmutex -- 读写公平的读写锁 √
  List/ -- 链表抽象类 
    ConcurrentStaticList -- 基于交替锁的线程安全的静态链表 
    ConcurrentLinkedList -- 基于交替锁的线程安全的链表 
  Stack/ -- 栈抽象类 
    ConcurrentArrayStack/ -- 线程安全的数组栈 √
      ConcurrentArrayStack（互斥锁） -- 基于互斥锁的线程安全数组栈 √
      ConcurrentArrayStack（读写锁） -- 基于读写锁的线程安全数组栈（支持读优先和读写公平） √
    ConcurrentLinkedStack/ -- 线程安全的链表栈 √
      ConcurrentLinkedStack（互斥锁）-- 基于互斥锁的线程安全数组栈 √
      ConcurrentLinkedStack（读写锁）-- 基于读写锁的线程安全数组栈（支持读优先和读写公平） √
  Queue/ -- 队列抽象类  
    ConcurrentArrayQueue/ -- 线程安全的数组队列 
    ConcurrentLinkedQueue/ -- 线程安全的链表队列 
    BlockingQueue -- 阻塞队列
  HashSet/ -- 哈希表 
    ConcurrentArrayHashSet -- 基于分段锁的开放定址法线程安全哈希表 
    ConcurrentZipHashSet -- 基于分段锁的拉链法线程安全哈希表 
 ```


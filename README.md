# myds-concurrency

线程安全的典型容器实现。

## 文件目录结构

```
root/ 
  rwmutex -- 读写锁实现
  List/ -- 链表抽象类 
    ConcurrentStaticList -- 线程安全的静态链表 
    ConcurrentLinkedList -- 线程安全的链表 
  Stack/ -- 栈抽象类 
    ConcurrentArrayStack/ -- 线程安全的数组栈 √
      ConcurrentArrayStack（互斥锁） -- 互斥锁实现的线程安全数组栈 √
      ConcurrentArrayStack（读写锁） -- 读写锁实现的线程安全数组栈（支持读优先和写优先） √
    ConcurrentLinkedStack -- 线程安全的链表栈 √
      ConcurrentLinkedStack（互斥锁）-- 互斥锁实现的线程安全数组栈 √
      ConcurrentLinkedStack（读写锁）-- 读写锁实现的线程安全数组栈（支持读优先和写优先） √
  Queue/ -- 队列抽象类  
    ConcurrentArrayQueue -- 线程安全的数组队列 
    ConcurrentLinkedQueue -- 线程安全的链表队列 
    BlockingQueue -- 阻塞队列
  Vector/ -- 动态数组抽象类
    ConcurrentVector -- 线程安全的Vector
  HashSet/ -- 哈希表 
    ConcurrentArrayHashSet -- 基于开放定址法的线程安全哈希表 
    ConcurrentZipHashSet -- 基于拉链法的线程安全哈希表 
  HashMap -- 哈希映射
    ConcurrentArrayHashSet -- 基于开放定址法的线程安全哈希映射 
    ConcurrentZipHashSet -- 基于拉链法的线程安全哈希映射
 ```


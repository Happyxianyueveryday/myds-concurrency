## ConcurrentStrongHashSet: 强一致性并发哈希集合

### 0. 备注
2019.10.11 测试和项目中发现问题，重新下线修改。

### 1. 总体结构
![avatar](https://img-blog.csdnimg.cn/20181116015408480.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTMyNTY4MTY=,size_16,color_FFFFFF,t_70)

一个ConcurrentStrongHashSet中包含多个Segment，每个Segment实际上都是独立的普通的单线程哈希表。

### 2. ConcurrrentStrongHashSet结构

一个ConcurrentStrongHashSet默认情况下包含16个Segment（用户也可以通过参数curr_level来调整Segment的数量）。该容器保证的并发级别是：

+ 假设ConcurrentStrongHashSet包含curr_level个Segment，则最多可以支持curr_level个线程（包括读线程和写线程）并发执行。

对于哈希表的插入，删除和查找操作，ConcurrentStrongHashSet通过一次哈希，定位到需要操作的Segment，后续的任务交由Segment内部完成。

### 3. Segment结构
一个Segment就是一个完整的普通哈希表结构，因此每个Segment可以像普通的哈希表一样进行扩容。

本容器中为了保证强一致性的实现方案是：对一个Segment对象的写方法和读方法都进行加互斥锁，这样可以完全保证读线程读到的始终是最新版本的哈希表。

#### a. 插入
+ 获取Segment的互斥锁。
+ 进行二次哈希计算待插入元素应该插入的桶位置下标。
+ 在桶的链表中插入待插入元素。
+ 释放Segment的互斥锁。

#### b. 删除
+ 获取Segment的互斥锁。
+ 进行二次哈希计算待删除元素应该所在的桶位置下标。
+ 从桶的链表中删除待删除元素。
+ 释放Segment的互斥锁。

#### c. 查找
+ 获取Segment的互斥锁。
+ 进行二次哈希计算待查找元素所在的桶位置下标。
+ 在桶的链表中查找待查找元素。
+ 释放Segment的互斥锁。





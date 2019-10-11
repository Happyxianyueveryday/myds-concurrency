## ConcurrentStrongHashSet: 强一致性并发哈希集合

### 1. 总体结构
![avatar](https://img-blog.csdnimg.cn/20181116015408480.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTMyNTY4MTY=,size_16,color_FFFFFF,t_70)

一个ConcurrentStrongHashSet中包含多个Segment，每个Segment实际上都是独立的普通的单线程哈希表。

### 2. ConcurrrentStrongHashSet总体设计结构

一个ConcurrentStrongHashSet默认情况下包含16个Segment（用户也可以通过参数curr_level来调整Segment的数量）。保证
该容器保证的并发级别是：

+ 假设ConcurrentStrongHashSet包含curr_level个Segment，则最多可以支持curr_level个线程（包括读线程和写线程）并发执行。

对于哈希表的插入，删除和查找操作，ConcurrentStrongHashSet通过一次哈希，定位到需要操作的Segment，后续的任务交由Segment内部完成。

### 3. Segment结构和强一致性保证
一个Segment就是一个完整的普通哈希表结构，因此每个Segment可以像普通的哈希表一样进行扩容。

本容器中为了保证强一致性的实现方案是：对一个Segment对象的写方法和读方法都进行加互斥锁，这样可以完全保证读线程进行读操作时读到的始终是最新版本的哈希表。


### 4. 附注：线程安全不是万能的


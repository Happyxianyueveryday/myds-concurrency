## ConcurrentStrongHashSet: 强一致性并发哈希集合

### 1. 总体结构
![avatar](https://img-blog.csdnimg.cn/20181116015408480.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTMyNTY4MTY=,size_16,color_FFFFFF,t_70)

一个ConcurrentStrongHashSet中包含多个Segment，而一个Segment中包含多个Hashentry。

### 2. Segment
一个Segment就是一个完整的普通哈希表结构。其中写方法加了互斥锁，即每个Segment中最多只允许一个写线程进行写操作；而读方法中没有任何的锁，因此一个Segment中可以进行任意数量的读线程进行读操作。

Segment主要需要完成以下的操作：

#### 1. 插入
+ 获取Segment的互斥锁。
+ 进行二次哈希计算待插入元素应该插入的桶位置下标。
+ 调用HashEntry的插入方法，将待插入元素插入到桶的HashEntry对象中。
+ 释放Segment的互斥锁。

#### 2. 删除
+ 获取Segment的互斥锁。
+ 进行二次哈希计算待删除元素应该所在的桶位置下标。
+ 调用HashEntry的删除方法，从桶的HashEntry对象中删除待删除元素。
+ 释放Segment的互斥锁。

#### 3. 查找

### 3. HashEntry
一个Segment中含有一个桶数组，其中每个桶数组中的链表称为一个HashEntry。



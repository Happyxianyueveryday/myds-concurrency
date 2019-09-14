#ifndef CONCURRENTLINKEDLIST_H  
#define CONCURRENTLINKEDLIST_H        
#include "ConcurrentList.h"
#include <iostream>
#include <string>
#include <mutex>

using namespace std;

template<typename T>
class ListNode
{
	public:
    // 1. methods
    ListNode(T _val=T()):val(_val), next(nullptr)
    { 
    }
    
    inline void lock()
    {
    	mut.lock();
	}
    
    inline void unlock()
    {
    	mut.unlock();
	}
    
    // 2. domains
    T val;             // 结点存储的值 
    ListNode<T> *next; // 指向下一个表结点的指针 
    
    private:
    mutex mut;         // 结点的互斥量  
};

/**
 * ConcurrentLinkedList: 基于交替锁的线程安全的单向链表类
 * note: 使用交替锁实现线程安全时，需要十分小心各个方法中遍历访问链表结点的顺序，禁止反向加锁，即遍历时每个结点只能加锁一次解锁一次 
 */
template<typename T>
class ConcurrentLinkedList: public ConcurrentList<T>
{
    public:
    // 1. constructor
    ConcurrentLinkedList();                             // 默认构造函数

    // 2. copy/move controller
    ConcurrentLinkedList(const ConcurrentLinkedList &other) = delete;                 		  // 禁用拷贝构造函数
    ConcurrentLinkedList &operator= (const ConcurrentLinkedList &other) = delete;             // 禁用拷贝赋值运算符
    ConcurrentLinkedList(ConcurrentLinkedList &&other) = delete;                      		  // 禁用移动构造函数
	ConcurrentLinkedList &operator= (ConcurrentLinkedList &&other) = delete;                  // 禁用移动赋值运算符 
    
    // 3. methods
    bool get(int index, T &val) override;    // 链表指定位置获取元素    
    bool insert(int index, T val) override;  // 链表指定位置插入元素 
    bool remove(int index, T &val) override; // 链表指定位置删除元素   
    int length() override;                   // 获取链表的长度 
    string toString() override;              // 输出当前链表 

    // 4. destructor
    ~ConcurrentLinkedList();

    private:
    ListNode<T> *head;
    
    // 5. private methods
    void safe_lock(ListNode<T> *node);
    void safe_unlock(ListNode<T> *node);
};

/**
 * ConcurrentLinkedList: 默认构造函数
 */
template<typename T>
ConcurrentLinkedList<T>::ConcurrentLinkedList() 
{
    head=new ListNode<T>();    // 创建链表头部哑结点
}

/**
 * get: 链表指定位置获取元素
 * param index: 要获取的元素的位置索引（从0开始索引）
 * param val: 所获取的元素值
 * return: 若位置合法获取元素值成功，则返回true；否则返回false
 * note: 获取指定位置的链表元素时，在遍历时始终只需要锁定一个结点，获取到下一个结点互斥锁，移动指针到下一个结点，释放当前结点的互斥锁即可  
 */      
template<typename T>
bool ConcurrentLinkedList<T>::get(int index, T &val) 
{
	if(index<0)     // 检查输入下标值的合法性 
	return false;
	
	safe_lock(head);                // 头部哑结点上锁 
	ListNode<T> *now=head;          // 链表游标指针 
	
	for(int i=0;i<=index&&now;i++)  // 逐渐移动游标指针到目标位置上 
	{ 
		ListNode<T> *temp=now;    
		safe_lock(now->next);  // 尝试获取下一个结点的互斥锁 
		now=now->next;         // 获取后继结点的互斥锁成功后，移动指针到下一个结点  // debug note: 禁止先解锁当前结点后再移动，因为一旦解锁后，now->next就可能被修改，这时now=now->next就不再准确了 
		safe_unlock(temp);     // 解锁之前指向的结点 
	}
	if(!now)   // 因为now提前到达nullptr而退出循环，则下标不合法 
	return false;    
	else       // 下标合法，返回所求结果       
	{
		val=now->val;
		safe_unlock(now);
		return true;
	}
}

/**
 * insert: 链表指定位置插入元素
 * param index: 要插入元素的位置索引
 * param val: 要插入的元素值
 * return: 若插入成功，则返回true；否则返回false
 * note: 插入操作首先按照查找元素相同的方式，交替锁定一个结点查找到待插入位置的前驱结点now；然后执行插入过程，进行插入之前首先必须锁定结点now和now->next，防止插入过程中这两个结点被修改，然后进行插入过程 
 */
template<typename T>
bool ConcurrentLinkedList<T>::insert(int index, T val)
{
    if(index<0)     // 检查输入下标值的合法性 
	return false;
	
	safe_lock(head);          // 头部哑结点上锁 
	ListNode<T> *now=head;    // 链表游标指针 
	
	for(int i=0;i<index&&now;i++)    // 查找待插入位置的前驱结点 
	{ 
		ListNode<T> *temp=now;
		safe_lock(now->next);
		now=now->next;
		safe_unlock(temp);
	}
	if(!now)   // 因为now提前到达nullptr而退出循环，则下标不合法 
	return false; 
	else       // 下标合法，进行插入过程  
	{
		// 结点now此时已经加锁 
		safe_lock(now->next);         // debug warning: 禁止使用两个语句进行加锁，例如：ListNode<T> *next=now->next; safe_lock(now->next); 这相当于在获得互斥锁之前就取得了now->next的值，两个语句之间可能并发执行了其他线程导致now->next被修改 
		ListNode<T> *newnode=new ListNode<T>(val);  // 执行插入过程，新建的结点无需加锁，因为两侧结点均已经加锁，插入过程中别的线程不可能修改新结点 
		newnode->next=now->next;   
		now->next=newnode;
		safe_unlock(now); 
		safe_unlock(newnode->next);   // debug warning: 禁止写成safe_lock(now->next)，now->next已经改变了，会导致锁被释放两次 
		return true;
	}
}

/**
 * remove: 链表指定位置删除元素
 * param index: 待删除的位置下标
 * param val: 所删除的值
 * note: 删除操作首先按照查找元素相同的方式，查找到待删除位置的前驱结点now；然后执行删除过程，删除过程之前首先需要锁定结点now，now->next，now->next->next三个结点，接着才能执行删除结点操作，删除结束后，解锁剩下的两个结点 
 */ 
template<typename T>
bool ConcurrentLinkedList<T>::remove(int index, T &val)
{
    if(index<0)
    return false;

	// 查找待删除结点的前驱结点的方法和查找给定下标的结点的方法一致 
	safe_lock(head);          // 获取头部哑结点的互斥锁 
    ListNode<T> *now=head;    
    for(int i=0;i<index&&now->next;i++)   // 查找待删除位置的前驱位置 
    {
    	ListNode<T> *temp=now;
    	safe_lock(now->next); // 尝试获取下一个结点的互斥锁  
        now=now->next;        // 移动游标指针 
        safe_unlock(temp);    // 释放上一个结点的互斥锁 
    }
    if(!now->next)   // 若还没有移动到指定的次数，now->next就为空，这时待删除位置的前驱结点就是now，相当于待删除结点为空，不合法 
    {
    	safe_unlock(now);   // 释放结点now的锁 
    	return false;
	}
	else
	{
		// 结点now此时已经加锁
		safe_lock(now->next);
		safe_lock(now->next->next);
		ListNode<T> *temp=now->next;
		now->next=now->next->next;
		val=temp->val;
		delete temp;
		safe_unlock(now);
		safe_unlock(now->next);       // debug warning: 删除待删除结点后，要解锁的两个结点是now和now->next，不是now->next->next！ 
		return true; 
	}
}

/**
 * size: 获取链表的长度/元素个数
 */
template<typename T>
int ConcurrentLinkedList<T>::length()
{
	int count=0;
    safe_lock(head->next);                 // 获取首结点的互斥锁 
	ListNode<T> *now=head->next;           // 用于遍历的游标指针 
	
	while(now)
	{
		count+=1;	
		ListNode<T> *temp=now;
		safe_lock(now->next);              // 获取下一个结点的互斥锁 
		now=now->next;
		safe_unlock(temp);                 // 释放上一个结点的互斥锁 
	}
	safe_unlock(now);
	return count;
}

/**
 * ~toString: 输出当前链表 
 */
template<typename T>
string ConcurrentLinkedList<T>::toString()
{
	string res; 
	safe_lock(head->next);                 // 获取首结点的互斥锁 
	ListNode<T> *now=head->next;           // 用于遍历的游标指针 
	
	while(now)
	{
		res=res+((now==head->next)?"":"->")+to_string(now->val);	
		ListNode<T> *temp=now;
		safe_lock(now->next);              // 获取下一个结点的互斥锁 
		now=now->next;
		safe_unlock(temp);                 // 释放上一个结点的互斥锁 
	}
	safe_unlock(now);
	return res;
}

/**
 * safe_lock: 带空指针安全检查的加锁方法  
 */
template<typename T>
void ConcurrentLinkedList<T>::safe_lock(ListNode<T> *node)
{
	if(node)
	node->lock();
}

/**
 * safe_unlock: 带空指针安全检查的解锁方法 
 */
template<typename T>
void ConcurrentLinkedList<T>::safe_unlock(ListNode<T> *node)
{
	if(node)
	node->unlock();  
}

/**
 * ~ConcurrentLinkedList: 析构函数
 * note: 析构函数同样可以加锁，但是实际上没有必要，析构时已经没有线程还在调用线程安全链表的方法了 
 */
template<typename T>
ConcurrentLinkedList<T>::~ConcurrentLinkedList() 
{
    ListNode<T> *now=head;

    while(now)
    {
        ListNode<T> *temp=now;
        now=now->next;
        delete temp;
    }
}

#endif

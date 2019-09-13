#ifndef CONCURRENTBLOCKINGQUEUE_H 
#define CONCURRENTBLOCKINGQUEUE_H
#include "BlockingQueue.h"  
#include <iostream> 
#include <string>
#include <condition_variable> 
#include <mutex>
#define INF INT_MAX

using namespace std;  

template<typename T> 
struct ListNode
{
    // 1. methods
    ListNode(T _val):val(_val), next(NULL)
    {
    }
    
    // 2. domains
    T val; 
    ListNode<T> *next;  
};

template<typename T> 
class ConcurrentBlockingQueue: public BlockingQueue<T>
{
    public:   
    // 1. public constructor
    ConcurrentBlockingQueue<T>(int _max_size=INF);

    // 2. copy_controller
    ConcurrentBlockingQueue(const ConcurrentBlockingQueue<T> &other);                           // 拷贝构造函数
    ConcurrentBlockingQueue<T> &operator= (const ConcurrentBlockingQueue<T> &other) = delete;   // 拷贝赋值运算符
    ConcurrentBlockingQueue(ConcurrentBlockingQueue<T> &&other);                                // 移动构造函数 
    ConcurrentBlockingQueue<T> &operator= (ConcurrentBlockingQueue<T> &&other) = delete;        // 移动赋值运算符 

    // 3. methods
    void push(T val) override;            // 队列入队操作
    void pop(T &val) override;            // 队列出队操作
    void frontval(T &val) override;       // 获取队首元素
    int length() override;                // 计算队列长度，即队列中的元素个数

    // 4. destructor
    ~ConcurrentBlockingQueue();

	private:
    ListNode<T> *front;      // 阻塞队列队首元素指针
    ListNode<T> *end;        // 阻塞队列队尾元素指针
    int size;                // 阻塞队列当前元素个数
    int max_size;            // 阻塞队列中最大元素个数 
    
    mutex mut;          // 控制互斥的可重入锁，使用可重入锁的原因主要是考虑用户可能递归调用阻塞队列的某些方法 
	condition_variable push_var;  // 生产者入队的等待条件变量
	condition_variable pop_var;   // 消费者出队的等待条件变量 
};

/**
 * ConcurrentBlockingQueue: 阻塞队列构造函数
 * param max_size: 指定阻塞队列中的最大元素数量，若不指定则为无限大 
 */
template<typename T> 
ConcurrentBlockingQueue<T>::ConcurrentBlockingQueue(int _max_size)
{
    front=new ListNode<T>(0);   // 链表头部哑结点
    end=front;
    size=0;
    max_size=_max_size;
}

/**
 * ConcurrentBlockingQueue: 拷贝构造函数 
 */
template<typename T> 
ConcurrentBlockingQueue<T>::ConcurrentBlockingQueue(const ConcurrentBlockingQueue<T> &other)
{
	lock_guard<mutex> other_guard(other.mut);    // 调用拷贝构造函数时当前对象尚未构造完成，因此只需要获得拷贝源对象的可重入互斥锁	
	
    front=new ListNode<T>(0);
    ListNode<T> *now=front;
    ListNode<T> *othnow=other.front->next; 
    while(othnow)
    {
    	now->next=new ListNode<T>(othnow->val);
    	now=now->next;
    	othnow=othnow->next;
	}
	end=now;
	size=other.size;
	max_size=other.max_size;
}

/**
 * ConcurrentBlockingQueue: 移动构造函数 
 */
template<typename T> 
ConcurrentBlockingQueue<T>::ConcurrentBlockingQueue(ConcurrentBlockingQueue<T> &&other)
{	
	lock_guard<mutex> other_guard(other.mut);
	
	front=other.front; 
	end=other.end; 
	size=other.size;
	max_size=other.max_size;
	
	other.front=new ListNode<T>(0);   // 链表头部哑结点
    other.end=front;
    other.size=0;
}   

/**
 * push: 阻塞版入队操作
 * param val: 需要入队的值 
 */
template<typename T> 
void ConcurrentBlockingQueue<T>::push(T val)
{
	unique_lock<mutex> push_lock(mut);
	push_var.wait(push_lock, [this]() {return (size<max_size); });    // 栈已满则进行让权等待，直到栈未满时再入栈 
	
    ListNode<T> *newnode=new (nothrow)ListNode<T>(val);
    end->next=newnode;
    end=end->next;
    size+=1;
    
    pop_var.notify_one();       // 入栈成功后如果存在等待的出栈线程，则激活这些线程 
}

/** 
 * pop: 阻塞版出队操作
 * param val: 实际出队的队首元素值 
 */
template<typename T> 
void ConcurrentBlockingQueue<T>::pop(T &val)
{
	unique_lock<mutex> pop_lock(mut);
	pop_var.wait(pop_lock, [this]() {return (size!=0);});   // 栈已空则进行让权等待，直到栈未空时才出栈 
	
    ListNode<T> *temp=front->next;
    front->next=temp->next;
    val=temp->val;
    if(temp==end)   // 注意，删除最后一个元素时end指针也要手动更新 
    end=front;
    delete temp;
    size-=1;
    
    push_var.notify_one();     // 出栈成功后如果存在等待的入栈线程，则激活这些线程 
}

/**
 * frontval: 阻塞版取队首元素操作
 * param val: 实际取得的队首元素值 
 */
template<typename T> 
void ConcurrentBlockingQueue<T>::frontval(T &val)
{
	unique_lock<mutex> top_lock(mut);
	pop_var.wait(top_lock, [this]() {return (size!=0);});   // 若栈为空则进行让权等待，直到栈未空时才取栈顶元素 
    val=front->next->val;
}

template<typename T> 
int ConcurrentBlockingQueue<T>::length() 
{
	lock_guard<mutex> length_guard(mut);
    return size;
}

template<typename T> 
ConcurrentBlockingQueue<T>::~ConcurrentBlockingQueue()
{
	lock_guard<mutex> guard(mut);
    ListNode<T> *now=front;
    while(now)
    {
        ListNode<T> *temp=now;
        now=now->next;
        delete temp;
    }
}

#endif

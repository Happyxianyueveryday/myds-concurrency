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
    ConcurrentBlockingQueue(const ConcurrentBlockingQueue<T> &other);                           // �������캯��
    ConcurrentBlockingQueue<T> &operator= (const ConcurrentBlockingQueue<T> &other) = delete;   // ������ֵ�����
    ConcurrentBlockingQueue(ConcurrentBlockingQueue<T> &&other);                                // �ƶ����캯�� 
    ConcurrentBlockingQueue<T> &operator= (ConcurrentBlockingQueue<T> &&other) = delete;        // �ƶ���ֵ����� 

    // 3. methods
    void push(T val) override;            // ������Ӳ���
    void pop(T &val) override;            // ���г��Ӳ���
    void frontval(T &val) override;       // ��ȡ����Ԫ��
    int length() override;                // ������г��ȣ��������е�Ԫ�ظ���
    bool empty() override;                // �ж϶����Ƿ�Ϊ��
	bool full() override;                 // �ж϶����Ƿ�Ϊ�� 

    // 4. destructor
    ~ConcurrentBlockingQueue();

	private:
    ListNode<T> *front;      // �������ж���Ԫ��ָ��
    ListNode<T> *end;        // �������ж�βԪ��ָ��
    int size;                // �������е�ǰԪ�ظ���
    int max_size;            // �������������Ԫ�ظ��� 
    
    recursive_mutex mut;          // ���ƻ���Ŀ���������ʹ�ÿ���������ԭ����Ҫ�ǿ����û����ܵݹ�����������е�ĳЩ���� 
	condition_variable_any push_var;  // ��������ӵĵȴ���������
	condition_variable_any pop_var;   // �����߳��ӵĵȴ��������� 
};

/**
 * ConcurrentBlockingQueue: �������й��캯��
 * param max_size: ָ�����������е����Ԫ������������ָ����Ϊ���޴� 
 */
template<typename T> 
ConcurrentBlockingQueue<T>::ConcurrentBlockingQueue(int _max_size)
{
    front=new ListNode<T>(0);   // ����ͷ���ƽ��
    end=front;
    size=0;
    max_size=_max_size;
}

/**
 * ConcurrentBlockingQueue: �������캯�� 
 */
template<typename T> 
ConcurrentBlockingQueue<T>::ConcurrentBlockingQueue(const ConcurrentBlockingQueue<T> &other)
{
	lock_guard<recursive_mutex> other_guard(other.mut);    // ���ÿ������캯��ʱ��ǰ������δ������ɣ����ֻ��Ҫ��ÿ���Դ����Ŀ����뻥����	
	
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
 * ConcurrentBlockingQueue: �ƶ����캯�� 
 */
template<typename T> 
ConcurrentBlockingQueue<T>::ConcurrentBlockingQueue(ConcurrentBlockingQueue<T> &&other)
{	
	lock_guard<recursive_mutex> other_guard(other.mut);
	
	front=other.front; 
	end=other.end; 
	size=other.size;
	max_size=other.max_size;
	
	other.front=new ListNode<T>(0);   // ����ͷ���ƽ��
    other.end=front;
    other.size=0;
}   

/**
 * push: ��������Ӳ���
 * param val: ��Ҫ��ӵ�ֵ 
 */
template<typename T> 
void ConcurrentBlockingQueue<T>::push(T val)
{
	unique_lock<recursive_mutex> push_lock(mut);
	push_var.wait(push_lock, [this]() {return !full();});    // ջ�����������Ȩ�ȴ���ֱ��ջδ��ʱ����ջ 
	
    ListNode<T> *newnode=new (nothrow)ListNode<T>(val);
    end->next=newnode;
    end=end->next;
    size+=1;
    
    pop_var.notify_one();       // ��ջ�ɹ���������ڵȴ��ĳ�ջ�̣߳��򼤻���Щ�߳� 
}

/** 
 * pop: ��������Ӳ���
 * param val: ʵ�ʳ��ӵĶ���Ԫ��ֵ 
 */
template<typename T> 
void ConcurrentBlockingQueue<T>::pop(T &val)
{
	unique_lock<recursive_mutex> pop_lock(mut);
	pop_var.wait(pop_lock, [this]() {return !empty();});   // ջ�ѿ��������Ȩ�ȴ���ֱ��ջδ��ʱ�ų�ջ����Ҫע���Ա����pop���ֵ����˻���ĳ�Ա����empty����������±���ʹ�ÿ�������������ͬһ���̶߳�μ�����ֱ�ӵ������� 
	
    ListNode<T> *temp=front->next;
    front->next=temp->next;
    val=temp->val;
    if(temp==end)   // ע�⣬ɾ�����һ��Ԫ��ʱendָ��ҲҪ�ֶ����� 
    end=front;
    delete temp;
    size-=1;
    
    push_var.notify_one();     // ��ջ�ɹ���������ڵȴ�����ջ�̣߳��򼤻���Щ�߳� 
}

/**
 * frontval: ������ȡ����Ԫ�ز���
 * param val: ʵ��ȡ�õĶ���Ԫ��ֵ 
 */
template<typename T> 
void ConcurrentBlockingQueue<T>::frontval(T &val)
{
	unique_lock<recursive_mutex> top_lock(mut);
	pop_var.wait(top_lock, [this]() {return !empty();});   // ��ջΪ���������Ȩ�ȴ���ֱ��ջδ��ʱ��ȡջ��Ԫ�� 
    val=front->next->val;
}

/** 
 * length: ���ص�ǰʱ�̶��еĳ��� 
 * return: ��ǰʱ�̶��еĳ��� 
 */
template<typename T> 
int ConcurrentBlockingQueue<T>::length() 
{
	lock_guard<recursive_mutex> length_guard(mut);
    return size;
}

/**
 * empty: �ж϶��е�ǰʱ���Ƿ�Ϊ�� 
 * return: ����ǰ����Ϊ���򷵻�true�����򷵻�false 
 */
template<typename T>
bool ConcurrentBlockingQueue<T>::empty()
{
	lock_guard<recursive_mutex> empty_guard(mut);
	if(!size)
	return true;
	else
	return false; 
}

/**
 * full: �жϵ�ǰ�����Ƿ�Ϊ�� 
 * return: ����ǰ����Ϊ���򷵻�true�����򷵻�false 
 */
template<typename T>
bool ConcurrentBlockingQueue<T>::full()
{
	lock_guard<recursive_mutex> full_guard(mut);
	if(size==max_size)
	return true;
	else
	return false;
}

/**
 * ~ConcurrentBlockingQueue: �������� 
 */
template<typename T> 
ConcurrentBlockingQueue<T>::~ConcurrentBlockingQueue()
{
	lock_guard<recursive_mutex> guard(mut);
    ListNode<T> *now=front;
    while(now)
    {
        ListNode<T> *temp=now;
        now=now->next;
        delete temp;
    }
}

#endif

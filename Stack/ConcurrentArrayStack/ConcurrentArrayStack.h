#ifndef CONCURRENT_ARRAY_LIST_H
#define CONCURRENT_ARRAY_LIST_H 
#include <iostream>
#include <string>
#include "ConcurrentStack.h"  
#include <mutex>

using namespace std;  

/**
 * ConcurrentArrayStack: 线程安全的数组栈
 */
template<typename T>
class ConcurrentArrayStack: public ConcurrentStack<T>
{
    public:   
    // 1. public constructor
    ConcurrentArrayStack(int _max_size);

    // 2. copy/move_controller
    ConcurrentArrayStack(const ConcurrentArrayStack<T> &other);                           // 拷贝构造函数 
    ConcurrentArrayStack<T> &operator= (const ConcurrentArrayStack<T> &other) = delete;   // 禁用拷贝赋值运算符 
    ConcurrentArrayStack(ConcurrentArrayStack<T> &&other);                                // 移动构造函数 
    ConcurrentArrayStack<T> &operator= (ConcurrentArrayStack<T> &&other) = delete;        // 禁用移动赋值运算符 

    // 3. methods
    bool push(T val);       // 入栈操作，该操作是线程安全的 
    bool pop(T &val);		// 出栈操作，该操作是线程安全的 
    bool top(T &val);       // 获取栈顶元素操作，该操作线程安全，但是仅有在取得的瞬间是有效的，过一段时间后可能栈被修改，这时取得的栈顶元素就不是此刻的栈顶元素了。因此必须将取栈顶操作和使用栈顶元素操作合并为一个原子操作 
    int length();           // 计算栈中的元素数量，该操作线程安全，但是仅有在取得的瞬间是有效的，因此必须将计算栈元素数量操作和使用元素数量的代码合并为一个原子操作  

    // 4. destructor
    ~ConcurrentArrayStack();

    int size;         // 当前栈中元素个数
    int max_size;     // 当前栈的最大可容纳元素个数
    T *data;          // 栈数组存储
    int pos;          // 栈顶指针位置
    mutex mut;        // 互斥访问信号量 
};

/**
 * ConcurrentArrayStack: 构造函数，初始化栈 
 * note: 构造过程整体必须互斥，因此使用类的信号量成员创建一个lock_guard对象保证互斥 
 * note: 若输入的_max_size不合法，则默认创建最大大小为10的栈 
 */
template<typename T>
ConcurrentArrayStack<T>::ConcurrentArrayStack(int _max_size)
{
	lock_guard<mutex> guard(mut);     
    size=0;
    max_size=(_max_size>=0)?_max_size:10;
    data=new T [max_size];
    pos=-1;
}

/**
 * ConcurrentArrayStack: 拷贝构造函数 
 * note: 构造过程整体必须互斥，因此使用类的信号量成员创建一个lock_guard对象保证互斥 
 */
template<typename T>
ConcurrentArrayStack<T>::ConcurrentArrayStack(const ConcurrentArrayStack<T> &other)
{
	lock_guard<mutex> guard(mut);   
    size=other.size;
    max_size=other.max_size;
    data=new T [max_size];
    pos=other.pos;

    for(int i=0;i<pos;i++)
    {
        data[i]=other.data[i];
    }
}

/**
 * ConcurrentArrayStack: 移动构造函数 
 * note: 构造过程整体必须互斥，因此使用类的信号量成员创建一个lock_guard对象保证互斥 
 */
template<typename T>
ConcurrentArrayStack<T>::ConcurrentArrayStack(ConcurrentArrayStack &&other)
{
	lock_guard<mutex> guard(mut);   
	size=other.size;
    max_size=other.max_size;
    data=other.data;
    pos=other.pos;
    
    other.size=0;
    other.data=new T [other.max_size];
    other.pos=-1;
}

/** 
 * push: 向栈中入栈一个元素
 * note: 入栈的全部过程必须互斥地进行 
 */
template<typename T>
bool ConcurrentArrayStack<T>::push(T val)  
{
    if(size>=max_size)   // 只读操作无需互斥进行 
    return false;
    else                 // 入栈需修改类成员，该操作需要互斥进行 
    {
    	lock_guard<mutex> guard(mut);    
        pos+=1;
        data[pos]=val;
        size+=1;
        return true;
    }
}

template<typename T>
bool ConcurrentArrayStack<T>::pop(T &val)
{
    if(pos==-1)  		// 或者使用条件size==0，只读操作无需互斥进行 
    return false;
    else                // 出栈需修改类成员，该操作需要互斥进行 
    { 
    	lock_guard<mutex> guard(mut); 
        val=data[pos];
        pos-=1;
        size-=1;
        return true;
    }
}

template<typename T>
bool ConcurrentArrayStack<T>::top(T &val)
{
    if(pos==-1)  // 或者使用条件size==0
    return false;
    else
    {
    	val=data[pos];
    	return true;
	}
}

template<typename T>
int ConcurrentArrayStack<T>::length()
{
    return size;
}

template<typename T>
ConcurrentArrayStack<T>::~ConcurrentArrayStack()
{
	lock_guard<mutex> guard(mut); 
    delete data;
}

#endif

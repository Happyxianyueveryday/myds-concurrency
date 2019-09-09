#ifndef CONCURRENT_ARRAY_LIST_H
#define CONCURRENT_ARRAY_LIST_H 
#include <iostream>
#include <string>
#include "ConcurrentStack.h"  
#include <mutex>

using namespace std;  

/**
 * ConcurrentArrayStack: �̰߳�ȫ������ջ
 */
template<typename T>
class ConcurrentArrayStack: public ConcurrentStack<T>
{
    public:   
    // 1. public constructor
    ConcurrentArrayStack(int _max_size);

    // 2. copy/move_controller
    ConcurrentArrayStack(const ConcurrentArrayStack<T> &other);                           // �������캯�� 
    ConcurrentArrayStack<T> &operator= (const ConcurrentArrayStack<T> &other) = delete;   // ���ÿ�����ֵ����� 
    ConcurrentArrayStack(ConcurrentArrayStack<T> &&other);                                // �ƶ����캯�� 
    ConcurrentArrayStack<T> &operator= (ConcurrentArrayStack<T> &&other) = delete;        // �����ƶ���ֵ����� 

    // 3. methods
    bool push(T val);       // ��ջ�������ò������̰߳�ȫ�� 
    bool pop(T &val);		// ��ջ�������ò������̰߳�ȫ�� 
    bool top(T &val);       // ��ȡջ��Ԫ�ز������ò����̰߳�ȫ�����ǽ�����ȡ�õ�˲������Ч�ģ���һ��ʱ������ջ���޸ģ���ʱȡ�õ�ջ��Ԫ�ؾͲ��Ǵ˿̵�ջ��Ԫ���ˡ���˱��뽫ȡջ��������ʹ��ջ��Ԫ�ز����ϲ�Ϊһ��ԭ�Ӳ��� 
    int length();           // ����ջ�е�Ԫ���������ò����̰߳�ȫ�����ǽ�����ȡ�õ�˲������Ч�ģ���˱��뽫����ջԪ������������ʹ��Ԫ�������Ĵ���ϲ�Ϊһ��ԭ�Ӳ���  

    // 4. destructor
    ~ConcurrentArrayStack();

    int size;         // ��ǰջ��Ԫ�ظ���
    int max_size;     // ��ǰջ����������Ԫ�ظ���
    T *data;          // ջ����洢
    int pos;          // ջ��ָ��λ��
    mutex mut;        // ��������ź��� 
};

/**
 * ConcurrentArrayStack: ���캯������ʼ��ջ 
 * note: �������������뻥�⣬���ʹ������ź�����Ա����һ��lock_guard����֤���� 
 * note: �������_max_size���Ϸ�����Ĭ�ϴ�������СΪ10��ջ 
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
 * ConcurrentArrayStack: �������캯�� 
 * note: �������������뻥�⣬���ʹ������ź�����Ա����һ��lock_guard����֤���� 
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
 * ConcurrentArrayStack: �ƶ����캯�� 
 * note: �������������뻥�⣬���ʹ������ź�����Ա����һ��lock_guard����֤���� 
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
 * push: ��ջ����ջһ��Ԫ��
 * note: ��ջ��ȫ�����̱��뻥��ؽ��� 
 */
template<typename T>
bool ConcurrentArrayStack<T>::push(T val)  
{
    if(size>=max_size)   // ֻ���������軥����� 
    return false;
    else                 // ��ջ���޸����Ա���ò�����Ҫ������� 
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
    if(pos==-1)  		// ����ʹ������size==0��ֻ���������軥����� 
    return false;
    else                // ��ջ���޸����Ա���ò�����Ҫ������� 
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
    if(pos==-1)  // ����ʹ������size==0
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

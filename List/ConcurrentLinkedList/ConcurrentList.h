#ifndef CONCURRENTLIST_H     
#define CONCURRENTLIST_H   
#include <string>       

using namespace std;

template<typename T> 
class ConcurrentList
{
	public:
	virtual bool get(int index, T &val) = 0;   		  // 链表指定位置获取元素  
    virtual bool insert(int index, T val) = 0;        // 链表指定位置插入元素 
    virtual bool remove(int index, T &val) = 0;       // 链表指定位置删除元素
    virtual int length() = 0;                   	  // 获取链表的长度
    virtual string toString() = 0;              	  // 输出当前链表     
	virtual ~ConcurrentList() {};                     // 虚析构函数  
};

#endif 

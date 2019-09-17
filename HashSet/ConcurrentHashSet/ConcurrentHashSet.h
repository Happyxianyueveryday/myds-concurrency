#ifndef CONCURRENTHASHSET_H 
#define CONCURRENTHASHSET_H
#include "ConcurrentSet.h"
#include "Segment.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template<typename T>
class ConcurrentHashSet:ConcurrentSet<T>
{
	public:
	// 1. constructor
	ConcurrentHashSet();
	ConcurrentHashSet(const vector<T> &data);
	
	// 2. copy/move_controller
    ConcurrentHashSet(const ConcurrentHashSet &other) = delete;                   // 禁用拷贝构造函数
    ConcurrentHashSet &operator= (const ConcurrentHashSet &other) = delete;       // 禁用拷贝赋值运算符
    ConcurrentHashSet(ConcurrentHashSet &&other) = delete;                        // 禁用移动构造函数
	ConcurrentHashSet &operator= (ConcurrentHashSet &&other) = delete;            // 禁用移动赋值运算符

    // 3. methods
    void insert(T value) override;              // 向哈希表中插入指定值
    void remove(T value) override;              // 从哈希表中移除指定值
    bool contain(T value) override;       		// 判断哈希表中是否包含指定值
    vector<T> values() override;        		// 获得哈希表中所有值的列表 
    int length() override;                      // 获取哈希表的元素个数 
	string toString() override; 	            // 输出哈希表
	~ConcurrentHashSet();                       // 析构函数 
    
    private:
    // 4. domains
    Segment<T> **segments;                      // Segment段数组 
    const unsigned int capacity;                // Segment段的数量（固定为16） 
    
    // 5. private functions
    unsigned int hash(const T &value) const;  // 根据hashCode计算具体存放位置 
    unsigned int prehash(unsigned int code) const;  // 预处理哈希算法，Wang/Jenkins hash  
};

template<typename T>
ConcurrentHashSet<T>::ConcurrentHashSet():capacity(16)
{
	segments=new Segment<T>* [capacity];
	for(int i=0;i<capacity;i++)
	{
		segments[i]=nullptr;
	}
}

template<typename T>
ConcurrentHashSet<T>::ConcurrentHashSet(const vector<T> &data):capacity(16)
{
	segments=new Segment<T>* [capacity];
	for(int i=0;i<capacity;i++)
	{
		segments[i]=nullptr;
	}
	for(int i=0;i<data.size();i++)
	{
		this->insert(data[i]);
	}
}

template<typename T>
void ConcurrentHashSet<T>::insert(T value)
{
	int index=hash(value);
	if(!segments[index])
	segments[index]=new Segment<T>();
	segments[index]->insert(value);
}

template<typename T>
void ConcurrentHashSet<T>::remove(T value)
{
	int index=hash(value);
	if(!segments[index])
	return;
	else
	segments[index]->remove(value);
}

template<typename T>
bool ConcurrentHashSet<T>::contain(T value)
{
	int index=hash(value);
	if(segments[index])
	return segments[index]->contain(value);
	else
	return false;
}

template<typename T>
vector<T> ConcurrentHashSet<T>::values()
{
	vector<T> res;
	for(int i=0;i<capacity;i++)
	{
		if(segments[i])
		{
			vector<T> temp=segments[i]->values();
			for(int k=0;k<temp.size();k++)
			{
				res.push_back(temp[k]);
			}
		}
	}
	return res;
}

template<typename T>
int ConcurrentHashSet<T>::length()
{
	int add=0;
	for(int i=0;i<capacity;i++)
	{
		if(segments[i])
		add+=segments[i]->length();
	}
	return add;
}

template<typename T>
string ConcurrentHashSet<T>::toString()
{
	string res;
	for(int i=0;i<capacity;i++)
	{
		if(segments[i])
		res=res+segments[i]->toString()+"\n";
	}
	return res;
}

template<typename T>
ConcurrentHashSet<T>::~ConcurrentHashSet()
{
	for(int i=0;i<capacity;i++)
	{
		if(segments[i])
		delete segments[i];
	} 
	delete [] segments;
}

template<typename T>
unsigned int ConcurrentHashSet<T>::hash(const T &value) const
{
	unsigned int hashCode=prehash(value.hashCode());
	return hashCode%capacity; 
}

template<typename T>
unsigned int ConcurrentHashSet<T>::prehash(unsigned int h) const
{
	h += (h << 15) ^ 0xffffcd7d; 
	h ^= (h >> 10);
	h += (h << 3); h ^= (h >> 6);
	h += (h << 2) + (h << 14); 
	return h ^ (h >> 16);
}


#endif

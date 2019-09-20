#ifndef CONCURRENT_STRONG_HASHSET_H       
#define CONCURRENT_STRONG_HASHSET_H 
#include "ConcurrentSet.h"
#include "Segment.h" 
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template<typename T>
class ConcurrentStrongHashSet:ConcurrentSet<T>       
{
	public:
	// 1. constructor
	ConcurrentStrongHashSet(string _mode, unsigned int _curr_level=16);
	ConcurrentStrongHashSet(const vector<T> &data, string _mode, unsigned int _curr_level=16);
	
	// 2. copy/move_controller
    ConcurrentStrongHashSet(const ConcurrentStrongHashSet &other) = delete;                   // 禁用拷贝构造函数
    ConcurrentStrongHashSet &operator= (const ConcurrentStrongHashSet &other) = delete;       // 禁用拷贝赋值运算符
    ConcurrentStrongHashSet(ConcurrentStrongHashSet &&other) = delete;                        // 禁用移动构造函数
	ConcurrentStrongHashSet &operator= (ConcurrentStrongHashSet &&other) = delete;            // 禁用移动赋值运算符

    // 3. methods
    void insert(T value) override;              // 向哈希表中插入指定值
    void remove(T value) override;              // 从哈希表中移除指定值
    bool contain(T value) override;       		// 判断哈希表中是否包含指定值
    vector<T> values() override;        		// 获得哈希表中所有值的列表 
    int length() override;                      // 获取哈希表的元素个数 
	string toString() override; 	            // 输出哈希表
	~ConcurrentStrongHashSet();                 // 析构函数 
    
    private:
    // 4. domains
    Segment<T> **segments;                      // Segment段数组 
    const unsigned int capacity;                // Segment段的数量（固定为16） 
    string mode;                                // 读写优先模式：read_first-读优先，write-first-写优先，fair-读写相对平等 
    unsigned int curr_level;                    // 写时并发等级：实际上就是Segment的个数，Segment的个数等于最多的并发写线程个数 
    
    // 5. private functions
    unsigned int jenkins_prehash(unsigned int code) const;     // 预处理哈希算法：Wang/Jenkins hash算法
    unsigned int first_hash(unsigned int code) const;          // 第一次哈希：确定需要加入的Segment位置下标 
};

template<typename T>
ConcurrentStrongHashSet<T>::ConcurrentStrongHashSet(string _mode, unsigned int _curr_level):capacity((_curr_level>0)?_curr_level:16)
{
	if(_mode=="write_first"||_mode=="read_first"||_mode=="fair")
	mode=_mode;
	else
	mode="fair";
	segments=new Segment<T>* [capacity];
	
	for(int i=0;i<capacity;i++)
	{
		segments[i]=nullptr;
	}
}

template<typename T>
ConcurrentStrongHashSet<T>::ConcurrentStrongHashSet(const vector<T> &data, string _mode, unsigned int _curr_level):capacity((_curr_level>0)?_curr_level:16)
{
	if(_mode=="write_first"||_mode=="read_first"||_mode=="fair")
	mode=_mode;
	else
	mode="fair";
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
void ConcurrentStrongHashSet<T>::insert(T value)
{
	int index=first_hash(value.hashCode());
	if(!segments[index])
	segments[index]=new Segment<T>(mode);
	segments[index]->insert(value);
}

template<typename T>
void ConcurrentStrongHashSet<T>::remove(T value)
{
	int index=first_hash(value.hashCode());
	if(!segments[index])
	return;
	else
	segments[index]->remove(value);
}

template<typename T>
bool ConcurrentStrongHashSet<T>::contain(T value)
{
	int index=first_hash(value.hashCode());
	if(segments[index])
	return segments[index]->contain(value);
	else
	return false;
}

template<typename T>
vector<T> ConcurrentStrongHashSet<T>::values()
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
int ConcurrentStrongHashSet<T>::length()
{
	int add=0;
	for(int i=0;i<capacity;i++)
	{
		if(segments[i])
		add+=segments[i]->length();
	}
	return add;
}

/**
 * toString: 输出哈希表
 * note: 输出一个Segment之前必须获得该Segment的互斥锁 
 */ 
template<typename T>
string ConcurrentStrongHashSet<T>::toString()
{
	string res;
	for(int i=0;i<capacity;i++)
	{
		if(segments[i])
		res=res+segments[i]->toString()+"\n";
	}
	return res;
}

/** 
 * first_hash: 第一次哈希（一段哈希） 
 * note 1: 当Segment的数量即capacity的值为2的幂时，始终有：code%capacity==code&(capacity)，从而减少了运算量 
 * note 2: 第一次哈希的目标是确定要操作的Segment的位置下标 
 * note 3: 两次哈希的算法不一样，第一次哈希的过程较为特殊，但是两次哈希之前都推荐做jenkins预处理 
 */ 
template<typename T>
unsigned int ConcurrentStrongHashSet<T>::first_hash(unsigned int code) const
{
	return code&(capacity-1);
}

/**
 * jenkins_prehash: 哈希预处理
 * note: 两次哈希之前都需要进行哈希预处理，预处理的目的是充分混杂，从而优化哈希的效果 
 */
template<typename T>
unsigned int ConcurrentStrongHashSet<T>::jenkins_prehash(unsigned int h) const
{
	h += (h << 15) ^ 0xffffcd7d; 
	h ^= (h >> 10);
	h += (h << 3); h ^= (h >> 6);
	h += (h << 2) + (h << 14); 
	return h ^ (h >> 16);
}

/**
 * ~ConcurrentStrongHashSet: 析构函数 
 */ 
template<typename T>
ConcurrentStrongHashSet<T>::~ConcurrentStrongHashSet()
{
	for(int i=0;i<capacity;i++) 
	{
		if(segments[i])
		delete segments[i];
	} 
	delete [] segments;
}


#endif

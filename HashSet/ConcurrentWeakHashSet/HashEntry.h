#ifndef HASHENTRY_H           
#define HASHENTRY_H 
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stack>
#include <mutex>

using namespace std;

template<typename T>
struct ListNode
{
	ListNode(T _val, shared_ptr<ListNode<T>> _next):val(_val), next(_next)
	{
	}
	const T val;
	const shared_ptr<ListNode<T>> next;
};

template<typename T>
class HashEntry
{
	public: 
	// 1. constructor
	HashEntry();
	
	// 2. copy/move controller
	HashEntry(const HashEntry &other) = delete; 
	HashEntry &operator= (const HashEntry &other) = delete;
	HashEntry(HashEntry &&other) = delete;
	HashEntry &operator= (HashEntry &&other) = delete;
	
	// 3. methods
	bool insert(T val);
	bool remove(T val);
	bool contain(T val) const;
	string toString() const;
	vector<T> values() const;
	
	// 4. destructor
	~HashEntry();
	
	private:
	// 5. domains
	shared_ptr<ListNode<T>> head;
};

/**
 * HashEntry: 默认构造函数 
 */ 
template<typename T>
HashEntry<T>::HashEntry()
{
	// head默认初始化为空指针对象
	head=nullptr;	
}

/**
 * insert: 向HashEntry插入元素
 * note: 首先检查当前HashEntry中是否已经存在待插入元素，若不存在，将待插入元素插入HashEntry链表的头部，本操作为写操作，必须加互斥锁 
 */
template<typename T>
bool HashEntry<T>::insert(T val) 
{ 
	// 1. 首先遍历HashEntry，判断HashEntry中是否已经存在该元素 
	shared_ptr<ListNode<T>> now=head;
	bool flag=false;
	while(now)
	{
		if(now->val.hashCode()==val.hashCode()&&now->val.equals(val))
		{
			flag=true;
			break;
		}
		else
		now=now->next;  
	}
	// 2. 已经存在该元素则无需重复插入，直接返回
	if(flag)
	return false;
	// 3. 固定在链表头部插入新的元素 
	shared_ptr<ListNode<T>> newnode=make_shared<ListNode<T>>(val, head);
	head=newnode;
	return true;
}
 
/**
 * remove: 从HashEntry中移除元素 
 * note: 从HashEntry中移除元素首先需要复制/深拷贝待删除结点前的所有结点，然后将新复制的链表和待删除结点后的链表部分连接起来，本操作为写操作，必须加互斥锁 
 */
template<typename T>
bool HashEntry<T>::remove(T val)
{
	shared_ptr<ListNode<T>> now=head;
	shared_ptr<ListNode<T>> new_head;
	stack<T> sta;  
	while(now)
	{
		if(now->val.hashCode()==val.hashCode()&&now->val.equals(val))   // 找到了对应的结点，则进行删除操作 
		{
			// 拷贝待删除结点之前的所有结点，因为结点的next字段为const的，因此该操作必须倒序进行 
			shared_ptr<ListNode<T>> temp=now->next;
			while(sta.size())    // 进行倒序复制链表结点，需要注意这里的复制技巧 
			{
				new_head=make_shared<ListNode<T>>(sta.top(), temp);
				temp=new_head; 
				sta.pop();
			}
			// 重置首结点为新复制的链表片段的首结点，然后连接拷贝的链表和待删除结点之后的链表 
			head=new_head;
			return true;
		}
		sta.push(now->val);
		now=now->next;
	}
	return false;
}

/**
 * contain: 判断某个元素是否在HashEntry中 
 * note: 直接遍历一遍HashEntry即可得到结果，该方法为读操作，无需加互斥锁 
 */ 
template<typename T>
bool HashEntry<T>::contain(T val) const
{
	shared_ptr<ListNode<T>> now=head;
	while(now)
	{
		if(now->val.hashCode()==val.hashCode()&&now->val.equals(val))
		return true;
		now=now->next;
	}
	return false;
}

/**
 * toString: 字符串形式输出HashEntry 
 * note: 直接遍历一遍HashEntry即可得到结果，该方法为读操作，无需加互斥锁 
 */ 
template<typename T>
string HashEntry<T>::toString() const
{
	shared_ptr<ListNode<T>> now=head;
	string res="*";
	while(now)
	{
		res=res+"->"+now->val.toString();
		now=now->next;	
	}
	return res;
}

/**
 * values: 获取HashEntry存储的值列表 
 * note: 直接遍历一遍HashEntry即可得到结果，该方法为读操作，无需加互斥锁 
 */  
template<typename T>
vector<T> HashEntry<T>::values() const
{
	shared_ptr<ListNode<T>> now=head;
	vector<T> res;
	while(now)
    {
    	res.push_back(now->val);
		now=now->next;
	}
	return res;
}

template<typename T>
HashEntry<T>::~HashEntry()
{
	head=nullptr;
}

#endif

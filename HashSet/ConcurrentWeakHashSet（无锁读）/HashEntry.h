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
 * HashEntry: Ĭ�Ϲ��캯�� 
 */ 
template<typename T>
HashEntry<T>::HashEntry()
{
	// headĬ�ϳ�ʼ��Ϊ��ָ����� 
}

/**
 * insert: ��HashEntry����Ԫ��
 * note: ���ȼ�鵱ǰHashEntry���Ƿ��Ѿ����ڴ�����Ԫ�أ��������ڣ���������Ԫ�ز���HashEntry�����ͷ����������Ϊд����������ӻ����� 
 */
template<typename T>
bool HashEntry<T>::insert(T val) 
{ 
	// 1. ���ȱ���HashEntry���ж�HashEntry���Ƿ��Ѿ����ڸ�Ԫ�� 
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
	// 2. �Ѿ����ڸ�Ԫ���������ظ����룬ֱ�ӷ���
	if(flag)
	return false;
	// 3. �̶�������ͷ�������µ�Ԫ�� 
	shared_ptr<ListNode<T>> newnode=make_shared<ListNode<T>>(val, head);
	head=newnode;
	return true;
}
 
/**
 * remove: ��HashEntry���Ƴ�Ԫ�� 
 * note: ��HashEntry���Ƴ�Ԫ��������Ҫ����/�����ɾ�����ǰ�����н�㣬Ȼ���¸��Ƶ�����ʹ�ɾ�����������������������������Ϊд����������ӻ����� 
 */
template<typename T> 
bool HashEntry<T>::remove(T val)
{
	shared_ptr<ListNode<T>> now=head;             
	shared_ptr<ListNode<T>> new_head;         
	stack<T> sta;    
	while(now)   
	{
		if(now->val.hashCode()==val.hashCode()&&now->val.equals(val))   // �ҵ��˶�Ӧ�Ľ�㣬�����ɾ������ 
		{
			// ������ɾ�����֮ǰ�����н�㣬��Ϊ����next�ֶ�Ϊconst�ģ���˸ò������뵹����� 
			new_head=now->next;  // 1
   			while(sta.size())    // ���е�����������
			{
				new_head=make_shared<ListNode<T>>(sta.top(), new_head);  
				sta.pop();   
			}  
			// �����׽��Ϊ�¸��Ƶ�����Ƭ�ε��׽�㣬Ȼ�����ӿ���������ʹ�ɾ�����֮�������        
			head=new_head;       // 2
			return true;
		}
		sta.push(now->val);
		now=now->next;  
	}
	return false;
}

/**
 * contain: �ж�ĳ��Ԫ���Ƿ���HashEntry�� 
 * note: ֱ�ӱ���һ��HashEntry���ɵõ�������÷���Ϊ������������ӻ����� 
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
 * toString: �ַ�����ʽ���HashEntry 
 * note: ֱ�ӱ���һ��HashEntry���ɵõ�������÷���Ϊ������������ӻ����� 
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
 * values: ��ȡHashEntry�洢��ֵ�б� 
 * note: ֱ�ӱ���һ��HashEntry���ɵõ�������÷���Ϊ������������ӻ����� 
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

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
    T val;             // ���洢��ֵ 
    ListNode<T> *next; // ָ����һ�������ָ�� 
    
    private:
    mutex mut;         // ���Ļ�����  
};

/**
 * ConcurrentLinkedList: ���ڽ��������̰߳�ȫ�ĵ���������
 * note: ʹ�ý�����ʵ���̰߳�ȫʱ����Ҫʮ��С�ĸ��������б��������������˳�򣬽�ֹ���������������ʱÿ�����ֻ�ܼ���һ�ν���һ�� 
 */
template<typename T>
class ConcurrentLinkedList: public ConcurrentList<T>
{
    public:
    // 1. constructor
    ConcurrentLinkedList();                             // Ĭ�Ϲ��캯��

    // 2. copy/move controller
    ConcurrentLinkedList(const ConcurrentLinkedList &other) = delete;                 		  // ���ÿ������캯��
    ConcurrentLinkedList &operator= (const ConcurrentLinkedList &other) = delete;             // ���ÿ�����ֵ�����
    ConcurrentLinkedList(ConcurrentLinkedList &&other) = delete;                      		  // �����ƶ����캯��
	ConcurrentLinkedList &operator= (ConcurrentLinkedList &&other) = delete;                  // �����ƶ���ֵ����� 
    
    // 3. methods
    bool get(int index, T &val) override;    // ����ָ��λ�û�ȡԪ��    
    bool insert(int index, T val) override;  // ����ָ��λ�ò���Ԫ�� 
    bool remove(int index, T &val) override; // ����ָ��λ��ɾ��Ԫ��   
    int length() override;                   // ��ȡ����ĳ��� 
    string toString() override;              // �����ǰ���� 

    // 4. destructor
    ~ConcurrentLinkedList();

    private:
    ListNode<T> *head;
    
    // 5. private methods
    void safe_lock(ListNode<T> *node);
    void safe_unlock(ListNode<T> *node);
};

/**
 * ConcurrentLinkedList: Ĭ�Ϲ��캯��
 */
template<typename T>
ConcurrentLinkedList<T>::ConcurrentLinkedList() 
{
    head=new ListNode<T>();    // ��������ͷ���ƽ��
}

/**
 * get: ����ָ��λ�û�ȡԪ��
 * param index: Ҫ��ȡ��Ԫ�ص�λ����������0��ʼ������
 * param val: ����ȡ��Ԫ��ֵ
 * return: ��λ�úϷ���ȡԪ��ֵ�ɹ����򷵻�true�����򷵻�false
 * note: ��ȡָ��λ�õ�����Ԫ��ʱ���ڱ���ʱʼ��ֻ��Ҫ����һ����㣬��ȡ����һ����㻥�������ƶ�ָ�뵽��һ����㣬�ͷŵ�ǰ���Ļ���������  
 */      
template<typename T>
bool ConcurrentLinkedList<T>::get(int index, T &val) 
{
	if(index<0)     // ��������±�ֵ�ĺϷ��� 
	return false;
	
	safe_lock(head);                // ͷ���ƽ������ 
	ListNode<T> *now=head;          // �����α�ָ�� 
	
	for(int i=0;i<=index&&now;i++)  // ���ƶ��α�ָ�뵽Ŀ��λ���� 
	{ 
		ListNode<T> *temp=now;    
		safe_lock(now->next);  // ���Ի�ȡ��һ�����Ļ����� 
		now=now->next;         // ��ȡ��̽��Ļ������ɹ����ƶ�ָ�뵽��һ�����  // debug note: ��ֹ�Ƚ�����ǰ�������ƶ�����Ϊһ��������now->next�Ϳ��ܱ��޸ģ���ʱnow=now->next�Ͳ���׼ȷ�� 
		safe_unlock(temp);     // ����֮ǰָ��Ľ�� 
	}
	if(!now)   // ��Ϊnow��ǰ����nullptr���˳�ѭ�������±겻�Ϸ� 
	return false;    
	else       // �±�Ϸ�������������       
	{
		val=now->val;
		safe_unlock(now);
		return true;
	}
}

/**
 * insert: ����ָ��λ�ò���Ԫ��
 * param index: Ҫ����Ԫ�ص�λ������
 * param val: Ҫ�����Ԫ��ֵ
 * return: ������ɹ����򷵻�true�����򷵻�false
 * note: ����������Ȱ��ղ���Ԫ����ͬ�ķ�ʽ����������һ�������ҵ�������λ�õ�ǰ�����now��Ȼ��ִ�в�����̣����в���֮ǰ���ȱ����������now��now->next����ֹ�����������������㱻�޸ģ�Ȼ����в������ 
 */
template<typename T>
bool ConcurrentLinkedList<T>::insert(int index, T val)
{
    if(index<0)     // ��������±�ֵ�ĺϷ��� 
	return false;
	
	safe_lock(head);          // ͷ���ƽ������ 
	ListNode<T> *now=head;    // �����α�ָ�� 
	
	for(int i=0;i<index&&now;i++)    // ���Ҵ�����λ�õ�ǰ����� 
	{ 
		ListNode<T> *temp=now;
		safe_lock(now->next);
		now=now->next;
		safe_unlock(temp);
	}
	if(!now)   // ��Ϊnow��ǰ����nullptr���˳�ѭ�������±겻�Ϸ� 
	return false; 
	else       // �±�Ϸ������в������  
	{
		// ���now��ʱ�Ѿ����� 
		safe_lock(now->next);         // debug warning: ��ֹʹ�����������м��������磺ListNode<T> *next=now->next; safe_lock(now->next); ���൱���ڻ�û�����֮ǰ��ȡ����now->next��ֵ���������֮����ܲ���ִ���������̵߳���now->next���޸� 
		ListNode<T> *newnode=new ListNode<T>(val);  // ִ�в�����̣��½��Ľ�������������Ϊ��������Ѿ���������������б���̲߳������޸��½�� 
		newnode->next=now->next;   
		now->next=newnode;
		safe_unlock(now); 
		safe_unlock(newnode->next);   // debug warning: ��ֹд��safe_lock(now->next)��now->next�Ѿ��ı��ˣ��ᵼ�������ͷ����� 
		return true;
	}
}

/**
 * remove: ����ָ��λ��ɾ��Ԫ��
 * param index: ��ɾ����λ���±�
 * param val: ��ɾ����ֵ
 * note: ɾ���������Ȱ��ղ���Ԫ����ͬ�ķ�ʽ�����ҵ���ɾ��λ�õ�ǰ�����now��Ȼ��ִ��ɾ�����̣�ɾ������֮ǰ������Ҫ�������now��now->next��now->next->next������㣬���Ų���ִ��ɾ����������ɾ�������󣬽���ʣ�µ�������� 
 */ 
template<typename T>
bool ConcurrentLinkedList<T>::remove(int index, T &val)
{
    if(index<0)
    return false;

	// ���Ҵ�ɾ������ǰ�����ķ����Ͳ��Ҹ����±�Ľ��ķ���һ�� 
	safe_lock(head);          // ��ȡͷ���ƽ��Ļ����� 
    ListNode<T> *now=head;    
    for(int i=0;i<index&&now->next;i++)   // ���Ҵ�ɾ��λ�õ�ǰ��λ�� 
    {
    	ListNode<T> *temp=now;
    	safe_lock(now->next); // ���Ի�ȡ��һ�����Ļ�����  
        now=now->next;        // �ƶ��α�ָ�� 
        safe_unlock(temp);    // �ͷ���һ�����Ļ����� 
    }
    if(!now->next)   // ����û���ƶ���ָ���Ĵ�����now->next��Ϊ�գ���ʱ��ɾ��λ�õ�ǰ��������now���൱�ڴ�ɾ�����Ϊ�գ����Ϸ� 
    {
    	safe_unlock(now);   // �ͷŽ��now���� 
    	return false;
	}
	else
	{
		// ���now��ʱ�Ѿ�����
		safe_lock(now->next);
		safe_lock(now->next->next);
		ListNode<T> *temp=now->next;
		now->next=now->next->next;
		val=temp->val;
		delete temp;
		safe_unlock(now);
		safe_unlock(now->next);       // debug warning: ɾ����ɾ������Ҫ���������������now��now->next������now->next->next�� 
		return true; 
	}
}

/**
 * size: ��ȡ����ĳ���/Ԫ�ظ���
 */
template<typename T>
int ConcurrentLinkedList<T>::length()
{
	int count=0;
    safe_lock(head->next);                 // ��ȡ�׽��Ļ����� 
	ListNode<T> *now=head->next;           // ���ڱ������α�ָ�� 
	
	while(now)
	{
		count+=1;	
		ListNode<T> *temp=now;
		safe_lock(now->next);              // ��ȡ��һ�����Ļ����� 
		now=now->next;
		safe_unlock(temp);                 // �ͷ���һ�����Ļ����� 
	}
	safe_unlock(now);
	return count;
}

/**
 * ~toString: �����ǰ���� 
 */
template<typename T>
string ConcurrentLinkedList<T>::toString()
{
	string res; 
	safe_lock(head->next);                 // ��ȡ�׽��Ļ����� 
	ListNode<T> *now=head->next;           // ���ڱ������α�ָ�� 
	
	while(now)
	{
		res=res+((now==head->next)?"":"->")+to_string(now->val);	
		ListNode<T> *temp=now;
		safe_lock(now->next);              // ��ȡ��һ�����Ļ����� 
		now=now->next;
		safe_unlock(temp);                 // �ͷ���һ�����Ļ����� 
	}
	safe_unlock(now);
	return res;
}

/**
 * safe_lock: ����ָ�밲ȫ���ļ�������  
 */
template<typename T>
void ConcurrentLinkedList<T>::safe_lock(ListNode<T> *node)
{
	if(node)
	node->lock();
}

/**
 * safe_unlock: ����ָ�밲ȫ���Ľ������� 
 */
template<typename T>
void ConcurrentLinkedList<T>::safe_unlock(ListNode<T> *node)
{
	if(node)
	node->unlock();  
}

/**
 * ~ConcurrentLinkedList: ��������
 * note: ��������ͬ�����Լ���������ʵ����û�б�Ҫ������ʱ�Ѿ�û���̻߳��ڵ����̰߳�ȫ����ķ����� 
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

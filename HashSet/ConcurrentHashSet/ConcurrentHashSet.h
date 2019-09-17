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
    ConcurrentHashSet(const ConcurrentHashSet &other) = delete;                   // ���ÿ������캯��
    ConcurrentHashSet &operator= (const ConcurrentHashSet &other) = delete;       // ���ÿ�����ֵ�����
    ConcurrentHashSet(ConcurrentHashSet &&other) = delete;                        // �����ƶ����캯��
	ConcurrentHashSet &operator= (ConcurrentHashSet &&other) = delete;            // �����ƶ���ֵ�����

    // 3. methods
    void insert(T value) override;              // ���ϣ���в���ָ��ֵ
    void remove(T value) override;              // �ӹ�ϣ�����Ƴ�ָ��ֵ
    bool contain(T value) override;       		// �жϹ�ϣ�����Ƿ����ָ��ֵ
    vector<T> values() override;        		// ��ù�ϣ��������ֵ���б� 
    int length() override;                      // ��ȡ��ϣ���Ԫ�ظ��� 
	string toString() override; 	            // �����ϣ��
	~ConcurrentHashSet();                       // �������� 
    
    private:
    // 4. domains
    Segment<T> **segments;                      // Segment������ 
    const unsigned int capacity;                // Segment�ε��������̶�Ϊ16�� 
    
    // 5. private functions
    unsigned int hash(const T &value) const;  // ����hashCode���������λ�� 
    unsigned int prehash(unsigned int code) const;  // Ԥ�����ϣ�㷨��Wang/Jenkins hash  
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

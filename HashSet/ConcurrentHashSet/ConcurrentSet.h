#ifndef CONCURRENTSET_H    
#define CONCURRENTSET_H 
#include <iostream>    
#include <string> 
#include <vector>

using namespace std;

/** 
 * ConcurrentSet: ��ϣ���Ͻӿڳ������  
 */
template <typename T>
class ConcurrentSet
{    
	public:    
	virtual void insert(T value) = 0;               // ���ϣ���в���ָ��ֵ        
	virtual void remove(T value) = 0;               // �ӹ�ϣ�����Ƴ�ָ��ֵ    
	virtual bool contain(T value) = 0;        		// �жϹ�ϣ�����Ƿ����ָ��ֵ    
	virtual vector<T> values() = 0;           		// ��ù�ϣ��������ֵ���б�  
	virtual int length() = 0;                 		// ��ȡ��ϣ���ĳ��� 
	virtual string toString() = 0;            		// �����ϣ��   
	virtual ~ConcurrentSet() {}                     // ��ϣ�������������������
};

#endif
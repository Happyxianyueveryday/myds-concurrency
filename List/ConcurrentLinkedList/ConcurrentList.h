#ifndef CONCURRENTLIST_H     
#define CONCURRENTLIST_H   
#include <string>       

using namespace std;

template<typename T> 
class ConcurrentList
{
	public:
	virtual bool get(int index, T &val) = 0;   		  // ����ָ��λ�û�ȡԪ��  
    virtual bool insert(int index, T val) = 0;        // ����ָ��λ�ò���Ԫ�� 
    virtual bool remove(int index, T &val) = 0;       // ����ָ��λ��ɾ��Ԫ��
    virtual int length() = 0;                   	  // ��ȡ����ĳ���
    virtual string toString() = 0;              	  // �����ǰ����     
	virtual ~ConcurrentList() {};                     // ����������  
};

#endif 

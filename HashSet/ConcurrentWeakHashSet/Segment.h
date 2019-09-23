#ifndef SEGMENT_H                           
#define SEGMENT_H
#include "HashEntry.h" 
#include <iostream> 
#include <string>
#include <vector>
#include <mutex>  

using namespace std;

/**
 * Segment: ������ʵ�ֵĹ�ϣ���� 
 */
template<typename T>      
class Segment 
{
    public:
    // 1. constructor
    Segment();                                // ���캯��
    Segment(const vector<T> &data);           // �����鹹���ϣ��

    // 2. copy/move_controller
    Segment(Segment &other) = delete;                   // ���ÿ������캯��������� 
    Segment &operator= (Segment &other) = delete;       // ���ÿ�����ֵ������������ 
    Segment(Segment &&other) = delete;                  // �����ƶ����캯�� ��ǳ����/�ƶ���
	Segment &operator= (Segment &&other) = delete;      // �����ƶ���ֵ�������ǳ����/�ƶ��� 

    // 3. methods
    void insert(T value);              // ���ϣ���в���ָ��ֵ
    void remove(T value);              // �ӹ�ϣ�����Ƴ�ָ��ֵ
    bool contain(T value) const;             // �жϹ�ϣ�����Ƿ����ָ��ֵ
    vector<T> values() const;                // ��ù�ϣ��������ֵ���б�
    int length() const;                      // ��ȡ��ϣ����Ԫ�ظ��� 
	string toString() const;                 // �����ϣ��

    // 4. destructor
    ~Segment();                                         // ��ϣ�������������������

    private:
    // 5. domains
    HashEntry<T> **data;          // ��ϣ������ṹ
    unsigned int capacity;        // ��ϣ��Ͱ��������������ĳ��ȣ����������У���ȡ��Ԫ�صĹ�ϣֵ�󽫹�ϣֵģͰ�������Ϳ��Եõ�����Ĵ�ŵ�Ͱ���±�λ��
    double load_factor;           // ��ϣ����������
    unsigned int threshold;       // ��ϣ��������ֵ����ֵ=Ͱ����*�������ӣ�ͨ������������ϣ�������ݲ����ǣ���ִ��insert��������Ԫ�غ󣬼���ϣ���е�Ԫ�ظ���size�Ƿ������ֵthreshold�������ڣ������resize�����������ݣ���������ݷ�����μ�resize����
    unsigned int size;            // ��ǰ��ϣ���е�Ԫ�ظ���
    mutex mut; 

    // 6. private functions
    void resize();       // ��ϣ�����ݷ���
    unsigned int jenkins_prehash(unsigned int h) const; 
    unsigned int second_hash(unsigned int code) const;      // �ڶ��ι�ϣ 
};

/**
 * Segment: Ĭ�Ϲ��캯��
 */
template<typename T>
Segment<T>::Segment()
{
	capacity=16;    
	load_factor=0.75;
	threshold=capacity*load_factor;
	size=0;
	data=new HashEntry<T>* [capacity];
	for(int i=0;i<capacity;i++)
	{
		data[i]=nullptr;
	}
}

/**
 * Segment: �����鹹���ϣ���� 
 * param data: ���ڹ����ԭʼ�������� 
 */
template<typename T>
Segment<T>::Segment(const vector<T> &_data)
{
	capacity=16;    
	load_factor=0.75;
	threshold=capacity*load_factor;
	size=0;
	data=new HashEntry<T>* [capacity];
	for(int i=0;i<capacity;i++)
	{
		data[i]=nullptr;
	}
	for(int i=0;i<_data.size();i++)
	{
		this->insert(_data[i]);
	}
}

/**
 * insert: ���ϣ�����в���Ԫ��ֵ
 * note: �������Ԫ��ֵ�ڹ�ϣ�������Ѿ������ظ����򲻻���в���
 */
template<typename T>
void Segment<T>::insert(T value) 
{
	lock_guard<mutex> guard(mut);
	
	int index=second_hash(value.hashCode());    // �ڶ��ι�ϣ�����������Ԫ��Ӧ�������HashEntry�±� 
	if(!data[index])
	data[index]=new HashEntry<T>();
	if(data[index]->insert(value))
	size+=1;
	if(size>threshold) 
	resize();
	
	return;
}

/**
 * remove: �ӹ�ϣ������ɾ��ָ��ֵ��Ԫ��
 * note: ����ϣ�����в����ڸ�ֵ����ʵ���ϲ������ɾ�� 
 */
template<typename T>
void Segment<T>::remove(T value) 
{
	lock_guard<mutex> guard(mut);
	
	int index=second_hash(value.hashCode());
	if(data[index]&&data[index]->remove(value))
	size-=1;
	
	return;
}

/**
 * contain: �жϸ���ֵ�Ƿ��ڹ�ϣ����
 * note: ������ֵ�ڹ�ϣ�����򷵻�true�����򷵻�false��������������м��� 
 */
template<typename T>
bool Segment<T>::contain(T value) const
{
	int index=second_hash(value.hashCode());
	
	if(!data[index])
	return false;
	else
	return data[index]->contain(value);
}

/**
 * values: ��ȡ��ϣ�����е�����ֵ���б�
 * return: ��ϣ�����е�����Ԫ�ص��б� 
 */
template<typename T>
vector<T> Segment<T>::values() const
{
	vector<T> res;
	for(int i=0;i<capacity;i++)
	{
		if(data[i])
		{
			vector<T> temp=data[i]->values();
			for(int k=0;k<temp.size();k++)
			{
				res.push_back(temp[k]); 
			}
		}
	}
	return res;
}

/**
 * length: ��ȡ��ϣ����Ԫ�ظ��� 
 */
template<typename T>
int Segment<T>::length() const
{
	return size;
}

/**
 * toString: ���ɹ�ϣ��������ַ��� 
 */
template<typename T>
string Segment<T>::toString() const
{
	string res;
	for(int i=0;i<capacity;i++)
	{
		if(data[i])
		res=res+data[i]->toString()+"\n";
	}
	return res;
}

/**
 * resize: ��ϣ���ݷ���
 * note 1: ��ʱ������ϣ���ݣ�
 *         ��ֻҪ��ִ�в�������󣬹�ϣ���е�Ԫ������size����Ͱ������capacity����һ����������ϵ��(load_factor)ʱ�ͷ�����ϣ���ݣ�����������Ϊ: size>capacity*load_factor 
 * note 2: ��ϣ���ݾ����㷨���裺 
 *         ��1. ��Ͱ������(capacity)����Ϊԭ�ȵ���������: capacity=2*capacity
 *			   2. ���¼�����ֵ����: threshoud=capacity*load_factor������load_factorΪ�������ӣ��̶�Ϊ0.75 
 *			   3. �����µ�Ͱ����
 *			   4. ����Ͱ�����е�����������֮ǰ��ͬ�㷨���¼����ϣֵ�Ͳ���λ�ã���ʹ��ǳ�����ƶ�����Ͱ���飬��: index=(element->hashCode())%capacity 
 *			   5. �ͷž�Ͱ���飬�µ�Ͱ������Ϊ��ǰ��ϣ����Ͱ���� 
 */
template<typename T>
void Segment<T>::resize()  
{
	
}  

/** 
 * second_hash: �ڶ��ι�ϣ�����ι�ϣ�� 
 * note 1: ��Ͱ��������capacity��ֵΪ2����ʱ��ʼ���У�code%capacity==code&(capacity)���Ӷ������������� 
 * note 2: �ڶ��ι�ϣ��Ŀ����ȷ��Ҫ������Ͱ��λ���±� 
 * note 3: ���ι�ϣ���㷨��һ�����ڶ��ι�ϣ�Ĺ��̸���������ͨ�ķ��̰߳�ȫ�Ĺ�ϣ�����������ι�ϣ֮ǰ���Ƽ���jenkinsԤ���� 
 */ 
template<typename T>
unsigned int Segment<T>::second_hash(unsigned int code) const
{
	return jenkins_prehash(code)&(capacity-1);
}

/**
 * jenkins_prehash: ��ϣԤ����
 * note: ���ι�ϣ֮ǰ����Ҫ���й�ϣԤ������Ԥ������Ŀ���ǳ�ֻ��ӣ��Ӷ��Ż���ϣ��Ч�� 
 */
template<typename T>
unsigned int Segment<T>::jenkins_prehash(unsigned int h) const
{
	h += (h << 15) ^ 0xffffcd7d; 
	h ^= (h >> 10);
	h += (h << 3); h ^= (h >> 6);
	h += (h << 2) + (h << 14); 
	return h ^ (h >> 16);
}
   
/**
 * ~Segment: ���������� 
 */
template<typename T>
Segment<T>::~Segment()
{
	for(int i=0;i<capacity;i++)
	{
		delete data[i];
	}
	delete [] data;
}

#endif
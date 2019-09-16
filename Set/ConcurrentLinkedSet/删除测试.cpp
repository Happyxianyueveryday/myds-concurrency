#include "ConcurrentLinkedSet.h"         
#include "Hashable.h"            
#include <string> 
#include <thread>  

using namespace std;

mutex mut; 

void insert_func1(ConcurrentLinkedSet<Hashable> &lis, int id)
{
	lis.remove(Hashable(1));
} 

void insert_func2(ConcurrentLinkedSet<Hashable> &lis, int id)
{
	lis.remove(Hashable(3));
} 

void insert_func3(ConcurrentLinkedSet<Hashable> &lis, int id)
{
	lis.remove(Hashable(5));
}

void insert_func4(ConcurrentLinkedSet<Hashable> &lis, int id) 
{
	lis.remove(Hashable(7));
}

void insert_func5(ConcurrentLinkedSet<Hashable> &lis, int id)
{
	lis.remove(Hashable(9));
}

void insert_func6(ConcurrentLinkedSet<Hashable> &lis, int id)
{
	lis.remove(Hashable(21)); 
}

void insert_func7(ConcurrentLinkedSet<Hashable> &lis, int id)
{
	lis.remove(Hashable(23));
}

void insert_func8(ConcurrentLinkedSet<Hashable> &lis, int id)
{
	lis.remove(Hashable(25));
}

void insert_func9(ConcurrentLinkedSet<Hashable> &lis, int id)
{
	lis.remove(Hashable(27));
}

int main(void)
{
	ConcurrentLinkedSet<Hashable> lis;
	
	for(int i=0;i<100;i++)
	{
		lis.add(i);
	}
	
	thread th8(insert_func8, ref(lis), 1);    // ɾ�������в�ָ���߳�ִ��˳�򣬳�ֲ���ִ�У����ÿ������ʱ��Ϊִ��˳��һ�������ܵõ��Ľ��Ҳ��һ�������Ǿ����ܹ��������У�������̰߳�ȫ����������˳��Ŀ���Ӧ�����û����� 
	th8.join();
	
	thread th2(insert_func2, ref(lis), 2);
	thread th3(insert_func3, ref(lis), 3);
	thread th4(insert_func4, ref(lis), 4);
	thread th1(insert_func1, ref(lis), 5); 
	thread th5(insert_func5, ref(lis), 6);
	thread th6(insert_func6, ref(lis), 7); 
	thread th7(insert_func7, ref(lis), 8); 
	thread th9(insert_func9, ref(lis), 9); 
	
	th2.join();
	th3.join();
	th4.join();
	th1.join();
	th5.join();
	th6.join();
	th7.join();
	th9.join();
	
	cout<<lis.toString()<<endl;
}

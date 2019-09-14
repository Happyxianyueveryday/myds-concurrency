#include "ConcurrentLinkedList.h"      
#include <string> 
#include <thread>  

using namespace std;

mutex mut;  

void insert_func1(ConcurrentLinkedList<int> &lis, int id)
{
	lis.insert(1, 111);
} 

void insert_func2(ConcurrentLinkedList<int> &lis, int id)
{
	lis.insert(3, 222);
} 

void insert_func3(ConcurrentLinkedList<int> &lis, int id)
{
	lis.insert(5, 333);
}

void insert_func4(ConcurrentLinkedList<int> &lis, int id) 
{
	lis.insert(7, 444);
}

void insert_func5(ConcurrentLinkedList<int> &lis, int id)
{
	lis.insert(9, 555);  
}

void insert_func6(ConcurrentLinkedList<int> &lis, int id)
{
	lis.insert(21, 666);
}

void insert_func7(ConcurrentLinkedList<int> &lis, int id)
{
	lis.insert(23, 777);
}

void insert_func8(ConcurrentLinkedList<int> &lis, int id)
{
	lis.insert(25, 888);
}

void insert_func9(ConcurrentLinkedList<int> &lis, int id)
{
	lis.insert(27, 999);
}

int main(void)
{
	ConcurrentLinkedList<int> lis;
	
	for(int i=0;i<100;i++)
	{
		lis.insert(i, i);
	}
	
	thread th8(insert_func8, ref(lis), 1);
	thread th2(insert_func2, ref(lis), 2);
	thread th3(insert_func3, ref(lis), 3);
	thread th4(insert_func4, ref(lis), 4);
	thread th1(insert_func1, ref(lis), 5); 
	thread th5(insert_func5, ref(lis), 6);
	thread th6(insert_func6, ref(lis), 7); 
	thread th7(insert_func7, ref(lis), 8); 
	thread th9(insert_func9, ref(lis), 9); 
	
	
	th8.join();
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

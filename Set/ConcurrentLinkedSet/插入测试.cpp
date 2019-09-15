#include "ConcurrentLinkedSet.h"  
#include "Hashable.h"
#include <string> 
#include <thread>  

using namespace std;

mutex mut;  

void insert_func1(ConcurrentLinkedSet<Hashable> &lis, int id)
{
	lis.add(Hashable(111));
} 

void insert_func2(ConcurrentLinkedSet<Hashable> &lis, int id)
{
	lis.add(Hashable(222));
} 

void insert_func3(ConcurrentLinkedSet<Hashable> &lis, int id)
{
	lis.add(Hashable(333));
}

void insert_func4(ConcurrentLinkedSet<Hashable> &lis, int id) 
{
	lis.add(Hashable(444));
}

void insert_func5(ConcurrentLinkedSet<Hashable> &lis, int id)
{
	lis.add(Hashable(555));
}

void insert_func6(ConcurrentLinkedSet<Hashable> &lis, int id)
{
	lis.add(Hashable(666));
}

void insert_func7(ConcurrentLinkedSet<Hashable> &lis, int id)
{
	lis.add(Hashable(777));
}

void insert_func8(ConcurrentLinkedSet<Hashable> &lis, int id)
{
	lis.add(Hashable(888));
}

void insert_func9(ConcurrentLinkedSet<Hashable> &lis, int id)
{
	lis.add(Hashable(999));
}

int main(void)
{
	ConcurrentLinkedSet<Hashable> lis;
	
	for(int i=0;i<100;i++)
	{
		lis.add(Hashable(i));
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

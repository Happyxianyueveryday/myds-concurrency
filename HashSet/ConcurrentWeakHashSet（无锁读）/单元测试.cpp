#include "ConcurrentWeakHashSet.h"                            
#include "Hashable.h"             
#include <iostream>     
#include <cstdlib> 
#include <thread>
using namespace std;

void insert_func(ConcurrentWeakHashSet<Hashable> &hashset, int id)
{
	srand(id);
	for(int i=0;i<10;i++)
	{
		hashset.insert(i);
	}
}

void remove_func(ConcurrentWeakHashSet<Hashable> &hashset, int id)
{
	srand(id);
	for(int i=0;i<10;i++)
	{
		hashset.remove(rand()%10);
	}
}
 
int main(void)
{
	ConcurrentWeakHashSet<Hashable> hashset;
	
	thread th1(insert_func, ref(hashset), 1);
	thread th2(remove_func, ref(hashset), 7); 
	
	th1.join();
	th2.join();
	
	cout<<hashset.toString()<<endl;
}

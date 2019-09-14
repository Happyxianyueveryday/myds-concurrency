#include "ConcurrentLinkedList.h"            
#include <string> 
#include <thread>   
#include <atomic>

using namespace std;

mutex mut; 

void get_func(ConcurrentLinkedList<int> &lis,int id)
{
	srand(id);
	for(int i=0;i<rand()%1000;i++)
	{
		int val;
		string str;
		
		int pos=rand()%1000;
		lis.get(pos, val);
	}
} 

void print_func(ConcurrentLinkedList<int> &lis, int id)
{
	srand(id);
	for(int i=0;i<rand()%6;i++)
	{
		string str="线程4打印: \n"+lis.toString()+"\n";
		cout<<str;
	}
}

void insert_func(ConcurrentLinkedList<int> &lis, int id)
{
	srand(id);
	for(int i=0;i<rand()%3000;i++)
	{
		int pos=rand()%500;
		lis.insert(pos, 999);
	}
} 

void delete_func(ConcurrentLinkedList<int> &lis, int id)
{
	srand(id);
	for(int i=0;i<rand()%1000;i++)
	{
		int val;
		int pos=rand()%200;
		lis.remove(pos, val); 
	}
}

int length_func(ConcurrentLinkedList<int> &lis, int id)
{
	srand(id);
	for(int i=0;i<rand()%7;i++)
	{
		cout<<("线程5打印: "+to_string(lis.length())+"\n");
	}
}

int main(void)
{
	ConcurrentLinkedList<int> lis;
	
	for(int i=0;i<100;i++)
	{
		lis.insert(i, i);
	}
	
	for(int i=0;i<100;i++)
	{
		lis.insert(rand()%100, i+100); 
	}
	
	// 1. 访问线程 
	thread th1(get_func, ref(lis), 1);
	thread th2(get_func, ref(lis), 2);
	thread th3(get_func, ref(lis), 3);
	
	// 2. 打印线程 
	thread th4(print_func, ref(lis), 4); 
	
	// 3. 长度计算线程 
	thread th5(length_func, ref(lis), 5);
	
	// 4. 删除线程 
	thread th6(delete_func, ref(lis), 6);
	thread th7(delete_func, ref(lis), 7);
	thread th8(delete_func, ref(lis), 8);
	
	// 5. 插入线程 
	thread th11(insert_func, ref(lis), 11);
	thread th12(insert_func, ref(lis), 12);
	thread th13(insert_func, ref(lis), 13);
	thread th14(insert_func, ref(lis), 14);
	thread th15(insert_func, ref(lis), 15);
	thread th16(insert_func, ref(lis), 16);
	thread th17(insert_func, ref(lis), 17);
	thread th18(insert_func, ref(lis), 18);
	thread th19(insert_func, ref(lis), 19);
	thread th20(insert_func, ref(lis), 20);
	
	
	th1.join();
	th2.join();
	th3.join(); 
	
	th4.join();
	
	th5.join();
	
	th6.join();
	th7.join();
	th8.join(); 
	
	th11.join();
	th12.join();
	th13.join();
	th14.join();
	th15.join();
	th16.join();
	th17.join();
	th18.join();
	th19.join();
	th20.join();
	
	cout<<"主线程打印: \n"+lis.toString()<<endl;
}

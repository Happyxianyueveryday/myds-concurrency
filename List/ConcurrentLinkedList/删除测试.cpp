#include "ConcurrentLinkedList.h"               
#include <string> 
#include <thread>  

using namespace std;

mutex mut; 

void insert_func1(ConcurrentLinkedList<int> &lis, int id)
{
	int val;
	lis.remove(1, val);
	string str="移除位置"+to_string(1)+"的值"+to_string(val)+"\n";
	cout<<str;
} 

void insert_func2(ConcurrentLinkedList<int> &lis, int id)
{
	int val;
	lis.remove(3, val);
	string str="移除位置"+to_string(3)+"的值"+to_string(val)+"\n";
	cout<<str;
} 

void insert_func3(ConcurrentLinkedList<int> &lis, int id)
{
	int val;
	lis.remove(5, val);
	string str="移除位置"+to_string(5)+"的值"+to_string(val)+"\n";
	cout<<str;
}

void insert_func4(ConcurrentLinkedList<int> &lis, int id) 
{
	int val;
	lis.remove(7, val);
	string str="移除位置"+to_string(7)+"的值"+to_string(val)+"\n";
	cout<<str;
}

void insert_func5(ConcurrentLinkedList<int> &lis, int id)
{
	int val;
	lis.remove(9, val);  
	string str="移除位置"+to_string(9)+"的值"+to_string(val)+"\n";
	cout<<str;
}

void insert_func6(ConcurrentLinkedList<int> &lis, int id)
{
	int val;
	lis.remove(21, val);
	string str="移除位置"+to_string(21)+"的值"+to_string(val)+"\n";
	cout<<str;
}

void insert_func7(ConcurrentLinkedList<int> &lis, int id)
{
	int val;
	lis.remove(23, val);
	string str="移除位置"+to_string(23)+"的值"+to_string(val)+"\n";
	cout<<str;
}

void insert_func8(ConcurrentLinkedList<int> &lis, int id)
{
	int val;
	lis.remove(25, val);
	string str="移除位置"+to_string(25)+"的值"+to_string(val)+"\n";
	cout<<str;
}

void insert_func9(ConcurrentLinkedList<int> &lis, int id)
{
	int val;
	lis.remove(27, val);
	string str="移除位置"+to_string(27)+"的值"+to_string(val)+"\n";
	cout<<str;
}

int main(void)
{
	ConcurrentLinkedList<int> lis;
	
	for(int i=0;i<100;i++)
	{
		lis.insert(i, i);
	}
	
	thread th8(insert_func8, ref(lis), 1);    // 删除测试中不指定线程执行顺序，充分并发执行，因此每次运行时因为执行顺序不一样，可能得到的结果也不一样。但是绝对能够正常运行，这就是线程安全容器，至于顺序的控制应该由用户控制 
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

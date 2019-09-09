#include "ConcurrentArrayStack.h" 
#include <iostream>   
#include <thread>

using namespace std; 
 
mutex mut;    // �����̻߳����� 

void func1(ConcurrentArrayStack<int> &sta)
{
	for(int i=0;i<10;i++)
	{
		int val;
		sta.push(1);
		sta.push(1);
		cout<<"�߳�1��ջֵ1\n";
		cout<<"�߳�1��ջֵ1\n";
		
		lock_guard<mutex> guard(mut);
		int size=sta.length(); 
		if(size)
		{
			string str="����ջ�Ĵ�СΪ"+to_string(size)+"\n";
			cout<<str;
			sta.top(val);
			string res="�߳�1��ջֵ"+to_string(val)+"\n";
			cout<<res; 
			sta.pop(val);
		}
	}
} 

void func2(ConcurrentArrayStack<int> &sta)
{ 
	for(int i=0;i<10;i++)
	{
		int val;
		sta.push(2);
		sta.push(2);
		cout<<"�߳�2��ջֵ2\n";
		cout<<"�߳�2��ջֵ2\n";
		
		lock_guard<mutex> guard(mut);
		int size=sta.length(); 
		if(size)
		{
			string str="����ջ�Ĵ�СΪ"+to_string(size)+"\n";
			cout<<str;
			sta.top(val);
			string res="�߳�2��ջֵ"+to_string(val)+"\n";
			cout<<res; 
			sta.pop(val);
		}
	}
} 

void func3(ConcurrentArrayStack<int> &sta)
{
	for(int i=0;i<10;i++)
	{
		int val;
		sta.push(3);
		sta.push(3);
		cout<<"�߳�3��ջֵ3\n";
		cout<<"�߳�3��ջֵ3\n";
		
		lock_guard<mutex> guard(mut);
		int size=sta.length(); 
		if(size)
		{
			string str="����ջ�Ĵ�СΪ"+to_string(size)+"\n";
			cout<<str;
			sta.top(val);
			string res="�߳�3��ջֵ"+to_string(val)+"\n";
			cout<<res; 
			sta.pop(val);
		}
	}
} 

int main(void)
{
	ConcurrentArrayStack<int> sta1(100);
	
	thread thr1(func1, ref(sta1));
	thread thr2(func2, ref(sta1));
	thread thr3(func3, ref(sta1));
	
	thr1.join();
	thr2.join();
	thr3.join();
	
	while(sta1.length())
	{
		int val;
		sta1.pop(val); 
		cout<<val<<" ";
	}
	cout<<endl;
}

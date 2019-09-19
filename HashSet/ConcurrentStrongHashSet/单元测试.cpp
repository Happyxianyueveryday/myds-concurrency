#include "ConcurrentStrongHashSet.h"                   
#include "Hashable.h"             
#include <iostream> 
#include <cstdlib> 
using namespace std;
 
int main(void)
{
	ConcurrentStrongHashSet<Hashable> hashset("fair");
	srand(8);
	vector<Hashable> dev;
	for(int i=0;i<5;i++)
	{
		Hashable temp(rand());
		hashset.insert(temp);
		dev.push_back(temp);
	} 
	cout<<hashset.length()<<endl;
	cout<<"hashset = "<<endl;
	cout<<hashset.toString()<<endl;
	
	cout<<"--------------------------------------------"<<endl;
	
	for(int i=0;i<5;i++)
	{
		cout<<hashset.contain(dev[i])<<endl;
	}
	cout<<hashset.contain(Hashable(2))<<endl;
	
	cout<<"--------------------------------------------"<<endl;
	
	vector<Hashable> res=hashset.values();
	for(int i=0;i<res.size();i++)
	{
		cout<<res[i].toString()<<" ";
	}
	cout<<endl;
	
	cout<<"--------------------------------------------"<<endl;
	
	ConcurrentStrongHashSet<Hashable> hashset2("read_first", dev);
	cout<<"hashset2 = "<<endl;
	cout<<hashset2.toString()<<endl;
	
	cout<<"--------------------------------------------"<<endl;
	
	for(int i=0;i<5;i++)
	{
		hashset.remove(dev[i]);
	}
	hashset.remove(Hashable(2));
	cout<<hashset.length()<<endl;
	cout<<"hashset = "<<endl;
	cout<<hashset.toString()<<endl;
	
	cout<<"--------------------------------------------"<<endl;
	
	for(int i=0;i<5;i++)
	{
		hashset.insert(rand());
	}
	cout<<hashset.length()<<endl;
	cout<<"hashset = "<<endl;
	cout<<hashset.toString()<<endl;
	
	cout<<"--------------------------------------------"<<endl;
	
	for(int i=0;i<100;i++)
	{
		hashset.insert(rand());
	}
	cout<<hashset.length()<<endl;
	cout<<"hashset = "<<endl;
	cout<<hashset.toString()<<endl;
	
	cout<<"--------------------------------------------"<<endl;
	cout<<"下面是扩容测试"<<endl;
	
	vector<Hashable> data; 
	for(int i=0;i<10;i++)    // 10个元素，未发生哈希扩容操作 
	{
		data.push_back(Hashable(rand())); 
	}
	
	ConcurrentStrongHashSet<Hashable> hashset5("write_first", data);
	
	cout<<hashset5.length()<<endl;
	cout<<"hashset5 = "<<endl;
	cout<<hashset5.toString()<<endl;
	
	for(int i=0;i<5;i++)    // 再随机插入5个整数，发生哈希扩容操作 
	{
		hashset5.insert(Hashable(rand()));
	} 
	
	cout<<hashset5.length()<<endl;
	cout<<"hashset5 = "<<endl; 
	cout<<hashset5.toString()<<endl;
	
	for(int i=0;i<100;i++)   // 大范围随机测试 
	{
		hashset5.insert(Hashable(rand()));
	}
	cout<<hashset5.length()<<endl;
	cout<<"hashset5 = "<<endl; 
	cout<<hashset5.toString()<<endl; 
}

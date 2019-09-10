#ifndef RWMUTEX
#define RWMUTEX
#include <string>
#include <mutex>   

using namespace std;   

/**
 * rwmutex: 基于互斥量实现的读优先的读写锁
 * note 1: 读写锁：允许多个线程同时进行读操作，但只允许一个线程同时进行写操作，且读写操作之间不能同时进行的一种互斥锁 
 * note 2: 读优先：当有读线程时，写线程将会被延迟执行；且只要存在一个读线程，后到的读线程都会优先执行，而后到的写线程会被延迟 
 * note 3：读优先的实现核心在于：当前时间段内，第一个到达的读进程为写线程互斥量解锁，最后一个到达的读线程为写线程互斥量解锁 
 *         更具体的解释就是：write_lock只能管理写线程的写操作互斥，而read_lock不仅可以管理读线程，还可以为写线程的互斥量write_mut上锁，以延迟后续到达的写线程，这就是读优先的核心机制和精华所在 
 */   
class rwmutex
{
	public: 
	// 1. constructor
	rwmutex();   		   // 默认构造函数 
	
	// 2. copy/move controller = delete
	rwmutex(const rwmutex &other) = delete;               // 读写锁禁用拷贝构造函数 
	rwmutex &operator= (const rwmutex &other) = delete;
	rwmutex(rwmutex &&other) = delete;
	rwmutex &operator= (rwmutex &&other) = delete;
	
	// 3. methods
	void read_lock();      // 读者线程加锁，读取操作前加锁 
	void read_unlock();    // 读者线程解锁，读取操作后解锁 
	void write_lock();     // 写者线程加锁，写入操作前加锁 
	void write_unlock();   // 写者线程加锁，写入操作后解锁 
	
	private:
	// 4. domains 
	mutex count_mut;       // 负责读者计数器count修改的互斥锁 
	int count;             // 读者数量的计数器 
	mutex write_mut;       // 负责写者间互斥写的互斥锁 
};

/**
 * rwmutex: 初始化独写锁 
 */  
rwmutex::rwmutex()
{
	count=0;    // 设置读者计数器为0 
}

/**
 * read_lock: 读者线程加锁 
 * note: 读者线程加锁包括以下两个重要步骤： 
 *       1. 互斥地将读者计数器count增加1 
 *       2. 互斥地判断当前读者数量是否为1，若为1则当前线程为第一个读线程，则加锁write_mut，终止写者的写操作
 */
void rwmutex::read_lock()
{
	count_mut.lock();   // 互斥访问读者计数器count，P操作 
	count+=1;           // 将读者数量增加1 
	if(count==1)        // 若读者数量增加1后值为1，则为第一个读者，根据读优先原则，首先加锁写者的互斥锁，使得写者的写操作被延迟到读者后进行 
	write_mut.lock();   // 加锁写者的互斥锁       
	count_mut.unlock(); // 互斥访问读者计数器count，V操作 
} 

/**
 * read_unlock: 读者线程解锁 
 * note: 读者线程解锁包括以下两个重要步骤：
 *		 1. 互斥地将读者计数器count减少1 
 *       2. 互斥地判断当前读者计数器是否为0，若为0则说明所有读者已经执行完读操作，这时解锁写者的互斥锁，使得被延迟的写者能够继续进行写操作 
 */  
void rwmutex::read_unlock()
{
	count_mut.lock();   // 互斥访问读者计数器count，P操作
	count-=1;           // 减小读者计数器count 
	if(count==0)        // 若读者数量减小后为0，则所有的读者已经进行完了读操作，这时释放写者的互斥锁，允许写者进行写操作 
	write_mut.unlock(); // 解锁写者的互斥锁 
	count_mut.unlock(); // 互斥访问读者计数器count，V操作 
} 

/**
 * write_lock: 写者加锁 
 */
void rwmutex::write_lock()
{
	write_mut.lock();
} 

/**
 * write_unlock: 写者解锁 
 */
void rwmutex::write_unlock()
{
	write_mut.unlock();
}

 

#endif

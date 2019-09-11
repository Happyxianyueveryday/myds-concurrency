#include<iostream>
#include<assert.h>
#include<thread>
#include<mutex>
#include<atomic>
#define MAX_HASH_SIZE  10
#define SEGMENT_SIZE 3
#define INVALID_INDEX -1
using namespace std;

int simplehash(long int n);
void threadfunc(void *ptr,int id);

class CHashMap
{
	
	private:
		struct Bucket
		{	
			int key;
			int data;
			bool used;
			std::atomic<bool> atomic_used;
			int _first_delta;
			int _next_delta;
		};
		
		struct Segment
		{
			struct Bucket *start_bucket;
			struct Bucket *last_bucket;
			std::mutex _lock;    // SegmentµÄ»¥³âËø 
		};
		
		struct Bucket buckarray[MAX_HASH_SIZE];
		struct Segment segments[MAX_HASH_SIZE];
		struct Segment& getSegment(int hash);
		struct Bucket* getBucket(int hash);
		struct Bucket* getFreeBucketInCacheLine(const struct Segment& segment, struct Bucket* const start_bucket);
		struct Bucket* getNearestFreeBucket(struct Segment &segment, struct Bucket* bucket);
		struct Bucket* displaceTheFreeBucket(struct Segment &segment, 
							struct Bucket *start_bucket, 
							struct Bucket *free_bucket);
	public:
		CHashMap();
		int add(int key ,int data);
		int remove(int key );
		bool contains(int key);
		int get(int key);
		void printContent();
		
};

CHashMap::CHashMap()
{
	for(int i = 0;i < MAX_HASH_SIZE;i++)
	{
		buckarray[i].used = false;
		buckarray[i].atomic_used.store(false);
		buckarray[i]._first_delta = i;
		if(i == MAX_HASH_SIZE-1)
		{
			buckarray[i]._next_delta = -1;
		}
		else	
		{
			buckarray[i]._next_delta = i+1;
		}
		buckarray[i].key = -1;
		buckarray[i].data = -1;
		segments[i].start_bucket = &buckarray[i];
		if(i+SEGMENT_SIZE > MAX_HASH_SIZE-1)
		{
			segments[i].last_bucket = &buckarray[MAX_HASH_SIZE-1];
		}
		else
		{
			segments[i].last_bucket = &buckarray[i+SEGMENT_SIZE-1];
		}
	}
}
CHashMap::Bucket* CHashMap::displaceTheFreeBucket(struct Segment& segment, 
							struct Bucket *start_bucket, 
							struct  Bucket *free_bucket)
{ 
	struct Bucket *temp = free_bucket-(SEGMENT_SIZE-1);
	int free_index = free_bucket->_first_delta;
	bool moved = false;
	while(free_bucket - temp < SEGMENT_SIZE)
	{	
		int key = temp->key;
		int hash = simplehash(key);
		if(free_index - hash  > SEGMENT_SIZE-1)
		{
			temp++;
			continue;
		}
		else
		{	
			int tempkey = free_bucket->key;
			int tempdata = free_bucket->data;
			free_bucket->used = true;
			free_bucket->key = temp->key;
			free_bucket->data = temp->data;
			free_bucket = temp;
			free_bucket->key = tempkey;
			free_bucket->data = tempdata;
			
			moved = true;
			break;
		}
	}
	if(!moved)
	{
		return NULL;
	}
	int current_distance = free_bucket - start_bucket;
	if(current_distance > SEGMENT_SIZE -1)
	{
		return displaceTheFreeBucket(segment, start_bucket, free_bucket);
	}
}
CHashMap::Bucket* CHashMap::getNearestFreeBucket(struct Segment& segment, struct Bucket *bucket)
{
	cout<<"temp bucket called"<<endl;
	struct Bucket *tempbucket = bucket;
	bool expected  = false;
	while(tempbucket->_first_delta <MAX_HASH_SIZE)
	{
		cout<<"index = "<< tempbucket - buckarray<<" before atomic set value ="<<tempbucket->atomic_used.load()<<endl;
		if(tempbucket->atomic_used.compare_exchange_strong(expected, true,memory_order_release, memory_order_relaxed))
		{
			cout<<"atomic set check "<<tempbucket->atomic_used.load()<<endl;
			return tempbucket;
		}
		else if(tempbucket->atomic_used.load() ==false)
		{
			cout<<"failed spuriously "<<tempbucket->atomic_used.load()<<endl;
		}
		tempbucket++;
		expected = false;
	}
	return NULL;
}
CHashMap::Bucket* CHashMap::getFreeBucketInCacheLine( const Segment& segment,  struct Bucket* const start_bucket)
{
	struct Bucket *bucket = start_bucket;
	//cout<<"start bucket index "<<bucket->_first_delta<<endl;
	bool expected = false;
	while(bucket <= segment.last_bucket)
	{
		cout<<"start bucket index "<<bucket->_first_delta<<" used "<<bucket->atomic_used.load()<<endl;
		if(bucket->atomic_used.compare_exchange_strong(expected, true,memory_order_release, memory_order_relaxed))
			return bucket;
		else if(bucket->atomic_used.load() == false)
		{
			cout<<"failed spuriously"<<endl;
		}
		//if(bucket->used == false) return bucket;
		bucket++;
		expected = false;
	}
	return NULL;
}
CHashMap::Segment& CHashMap::getSegment(int hash)
{
	return segments[hash];
}
CHashMap::Bucket* CHashMap::getBucket(int hash)
{
	return &buckarray[hash];
}
int CHashMap::add(int key, int data)
{
	struct Segment& segment = getSegment(simplehash(key));
	struct Bucket* start_bucket = getBucket(simplehash(key));

	segment._lock.lock();
	if(contains(key))
	{
		cout<<"key already present"<<endl;
		segment._lock.unlock();
		return 0;
	}
	
	struct Bucket* free_bucket = getFreeBucketInCacheLine(segment, start_bucket);
	if(free_bucket != NULL)
	{
		cout<<"found a free bucket in cacheline"<<endl;
		free_bucket->used = true;
		free_bucket->key = key;
		free_bucket->data = data;
		segment._lock.unlock();
		cout<<" added key and data"<<endl;
		return 0;
	}
	cout<<"no free bucket in cacheline"<<endl;
	free_bucket = getNearestFreeBucket(segment, start_bucket);
	if(free_bucket == NULL)
	{
		cout<<"could not find a free bucked"<<endl;
		segment._lock.unlock();
		return -1;
	}
	free_bucket->key = key;
	free_bucket->used = true;
	free_bucket->data = data;
	int distance_to_free = free_bucket - start_bucket;
	cout<<"distance to free bucket "<< distance_to_free<<endl;
	if(distance_to_free > SEGMENT_SIZE-1)	
	{
		cout<<"free bucket is too far"<<endl;
		free_bucket = displaceTheFreeBucket(segment, start_bucket, free_bucket);
	}
	cout<<"done add"<<endl;
	segment._lock.unlock();
}

int CHashMap::remove(int key)
{
	int hash = simplehash(key);
	struct Segment& segment = getSegment(hash);
	
	segment._lock.lock();
	struct Bucket* bucket = segment.start_bucket;
	while(bucket != segment.last_bucket)
	{
		if(bucket->key == key)
		{
			bucket->used  = false;
			segment._lock.unlock();
			return bucket->data;
		}
		bucket++;
	}
	segment._lock.unlock();
	return -1;
}

bool CHashMap::contains(int key)
{
	//hopscotch
	struct Segment& segment = getSegment(simplehash(key));
	struct Bucket* bucket = segment.start_bucket;
	struct Bucket *last_bucket = segment.last_bucket;
	while(bucket <= last_bucket)
	{	
		if(bucket->key == key)
			return true;
		else 
			bucket++;

	}
	return false;
}

int CHashMap::get(int key)
{
	int hash = simplehash(key);
	struct Segment& segment = getSegment(hash);
	struct Bucket *bucket = segment.start_bucket;
	while(bucket <= segment.last_bucket){
		if(bucket->key == key)
			return bucket->data;
		bucket++;
	}
	return -1;
}

void CHashMap::printContent()
{
	cout<<"=================================================="<<endl;
	cout<<"Content of the hash table"<<endl;
	cout<<"=================================================="<<endl;
	
	for(int i =0;i < MAX_HASH_SIZE;i++)
	{
		cout<<i<<"   "<<buckarray[i].key<<"         "<<buckarray[i].data<<endl;
	}	
}



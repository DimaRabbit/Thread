#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>

using namespace std;


class Data 
{
	
public:
	mutex mtx;
	int a;
	int b;
	Data(int _a,int _b): a(_a), b(_b) {}
};


void Swap_lock(Data& data1, Data& data2) {
	
	data1.mtx.lock();
	data2.mtx.lock();
	swap(data1.a, data2.a);
	swap(data1.b, data2.b);

	data1.mtx.unlock();
	data2.mtx.unlock();
}
void Swap_scoped(Data& data1, Data& data2) {

	scoped_lock lock(data1.mtx, data2.mtx);

	swap(data1.a, data2.a);
	swap(data1.b, data2.b);

}
void Swap_unique(Data& data1, Data& data2) {
	unique_lock<mutex>lock1(data1.mtx, defer_lock);
	unique_lock<mutex>lock2(data2.mtx, defer_lock);

	lock(lock1, lock2);

	swap(data1.a, data2.a);
	swap(data1.b, data2.b);

}

int main() 
{
	Data data1(1, 2);
	Data data2(3, 4);

	thread t1(Swap_lock, ref(data1),ref(data2));
	t1.join();
	cout << "Data After Swap_lock -: " << data1.a<<", " << data1.b << " Data2: " << data2.a << ", " << data2.b << endl;
	thread t2(Swap_scoped, ref(data1), ref(data2));
	t2.join();
	cout << "After Swap_scoped Data: " << data1.a << ", " << data1.b << " Data2: " << data2.a << ", " << data2.b << endl;
	thread t3(Swap_unique, ref(data1), ref(data2));
	t3.join();

	cout << "Swap_unique Data: " << data1.a << ", " << data1.b << " Data2: " << data2.a << ", " << data2.b << endl;
	
	
	



	return 0;
}
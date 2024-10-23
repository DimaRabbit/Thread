#include<iostream>
#include<mutex>
#include<thread>
#include<chrono>
#include<atomic>
using namespace std;

mutex mtx;


atomic<int>client_counter = 0;

atomic<bool> continue_processing = true;
memory_order order = memory_order_seq_cst;

void client(int max_client) 
{
	while (client_counter < max_client) 
	{
	
		this_thread::sleep_for(chrono::seconds(1));
		
		if (client_counter < max_client) {
			client_counter.fetch_add(1,order);
			cout << "New lient add. Client in the queue: " << client_counter << endl;
		
		}
	}
}


void operator_window() 
{
	while (continue_processing || client_counter > 0) 
	{
		this_thread::sleep_for(chrono::seconds(2));
		
		if(client_counter > 0)
		{
			client_counter.fetch_sub(1,order);
			cout << "The operator served the client. Left in the queue: " << client_counter << endl;
		}
		if(client_counter==0&& !continue_processing) {
			break;
		
		}
	
	}

}

int main() 
{
	int max_clients = 0;
	int order_choise;

	cout << "Enter number clients: ";
	cin >> max_clients;

	cout << "Select memory order: \n";
	cout << "1. memory_order_relaxed\n";
	cout << "2. memory_order_acquire\n";
	cout << "3. memory_order_release\n";
	cout << "4. memory_order_acq_rel\n";
	cout << "5. memory_order_seq_cst\n";
	cin >> order_choise;

	switch (order_choise) 
	{
	case 1: 
		order = memory_order_relaxed;
		break;
	case 2:
		order = memory_order_acquire;
		break;
	case 3:
		order = memory_order_release;
		break;
	case 4:
		order = memory_order_acq_rel;
		break;
	case 5:
		order = memory_order_seq_cst;
		break;
	default:
		order = memory_order_seq_cst;
		cout << "Invalid choise. Default to memory_order_seq_cst!";
		break;
	}

	thread t1(client, max_clients);

	thread t2(operator_window);

	t1.join();
	continue_processing = false;
	t2.join();

	cout << "All clients are served. The program is completed!" << endl;

	return 0;
}
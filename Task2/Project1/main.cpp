#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <windows.h>  
std::mutex mtx; 


void set_cursor_position(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void progress_bar(int thread_num, int progress_length, int delay_ms, int console_line) {
    auto thread_id = std::this_thread::get_id();
    std::string progress = "";

    
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < progress_length; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms)); 

        progress += '#';  

       
        {
            std::lock_guard<std::mutex> lock(mtx);
            set_cursor_position(0, console_line); 
            std::cout << "Thread " << thread_num << " (ID: " << thread_id << "): ["
                << progress << std::string(progress_length - progress.size(), ' ') << "]" << std::flush;
        }
    }

   
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_time = end_time - start_time;

    {
        std::lock_guard<std::mutex> lock(mtx);
        set_cursor_position(0, console_line); 
        std::cout << "Thread " << thread_num << " (ID: " << thread_id << "): Completed in "
            << total_time.count() << " seconds" << std::endl;
    }
}

int main() {
    const int num_threads = 7;      
    const int progress_length = 20; 
    const int delay_ms = 100;       

    std::vector<std::thread> threads;

    
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(progress_bar, i + 1, progress_length, delay_ms, i + 1);
    }

    
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
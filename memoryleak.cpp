#include <iostream>
#include <thread>
#include <vector>

constexpr int CHUNK_SIZE = 100000;
constexpr int NUM_THREADS = 4;

void allocateMemory() {
	while (true) {
		int* chunk = new int[CHUNK_SIZE];
		std::cout << "Allocated memory at address: " << chunk << std::endl;			
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

int main() {
	std::vector<std::thread> threads;
	
	for (int i = 0; i < NUM_THREADS; ++i) {
		threads.emplace_back(allocateMemory);	
	}	

	for (auto& thread : threads) {
		thread.join();
	}
	
	return 0;	
}	

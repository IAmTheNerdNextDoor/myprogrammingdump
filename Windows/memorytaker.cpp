#include <iostream>
#include <cstring>
#include <chrono>
#include <thread>
#include <random>
#include <vector>

int main() {
		long long int allocatedMemory = 0;
		int memoryIncrement = 1000;
		int fallbackIncrement = 10;
		
		bool fallbackAttempted = false;
		
		std::random_device rd;
		std::mt19937_64 gen(rd());
		std::uniform_int_distribution<int>dis(0,255);
	
	while (true) {
		allocatedMemory += memoryIncrement;
		long long int size = allocatedMemory * 1024 * 1024;
		
		std::vector<char> memory(size);
		char* memoryPtr = memory.data();
		if (memoryPtr == nullptr) {
			std::cout << "Normal memory allocation limit reached. Allocated up to " << allocatedMemory << " MB of memory. Switching to Failsafe." << std::endl;
			if(!fallbackAttempted) {	
				allocatedMemory -= memoryIncrement;
				memoryIncrement = fallbackIncrement;
				fallbackAttempted = true;
				continue;
			} else {
				std::cout << "Unable to allocate memory. Exiting." << std::endl;
				break;
			}
		}
		
	for (long long int i = 0; i < size; ++i) {
		memoryPtr[i] = static_cast<char>(dis(gen));
	}
	
	std::cout << "Allocated " << allocatedMemory << " MB of memory with garbage." << std::endl;
		
	std::this_thread::sleep_for(std::chrono::seconds(0));
		

	}
	
	
	return 0;
}

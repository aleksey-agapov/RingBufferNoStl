//============================================================================
// Name        : RingBufferTest.cpp
// Author      : Agapov Aleksey
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <thread>

#include "RingBuffer.h"

#define max_count 100 /* test sequence */

using namespace std;

int main() {
	std::cout << "Start!" << std::endl;

	ring::RingBuffer<int> ringBuff;

	auto writer = [&]() { int count = 0; std::cout << "Write THREAD\n";  while (true) { std::this_thread::sleep_for (std::chrono::milliseconds(1));   if (ringBuff.Add(count))  { std::cout << "\nWrite:" << count; if (count++ >= max_count) {break;}} else {std::cout << " Write failde!";} } };

	auto reader = [&]() { int count = 0; std::cout << "Read  THREAD\n";  while (true) { std::this_thread::sleep_for (std::chrono::milliseconds(200));  if (ringBuff.Take(count)) { std::cout << "\nRead: " << count; if (count == max_count)  {break;}} else {std::cout << " Read failde!"; } } };

	std::thread f1(writer);
	std::thread f2(reader);

	f1.join();
	f2.join();
	std::cout << std::endl << "Finish!" << std::endl;
	return 0;
}

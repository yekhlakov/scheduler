#include <iostream>
#include "maxy/control/scheduler.h"

int main ()
{
	maxy::control::scheduler s(std::chrono::milliseconds(100));
	long long counter = 0;

	auto c = 0;

	std::cout << "QUEUING FIRST\n";
	s.add (4, [] () { std::cout << "FIRST\n"; });
	std::cout << "QUEUING SECOND\n";
	s.add (1, [] () { std::cout << "SECOND\n"; });

	for (c = 0; c < 2; c++)
	{
		std::cout << "WAITING...\n";
		std::this_thread::sleep_for (std::chrono::seconds (1));
	}

	std::cout << "QUEUING THIRD\n";
	s.add (2, [&counter] () { counter++; std::cout << "THIRD, COUNTER=" << counter << "\n";});

	std::cout << "QUEUING FOURTH\n";
	s.add (1, [&] () { counter++; std::cout << "FOURTH, COUNTER=" << counter << "\n"; });

	for (c = 0; c < 4; c++)
	{
		std::cout << "WAITING...\n";
		std::this_thread::sleep_for (std::chrono::seconds (1));
	}

	std::cout << "QUEUING TWO TASKS AT EQUAL TIMESTAMPS\n";
	auto now_plus_n = s.now () + std::chrono::seconds(2);

	s.add (now_plus_n, [] { std::cout << "AAAAA\n"; });
	s.add (now_plus_n, [] { std::cout << "BBBBB\n"; });

	for (c = 0; c < 4; c++)
	{
		std::cout << "WAITING...\n";
		std::this_thread::sleep_for (std::chrono::seconds (1));
	}
}

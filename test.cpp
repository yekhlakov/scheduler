#include <iostream>
#include "maxy/control/scheduler.h"

int main ()
{
	maxy::control::scheduler s(std::chrono::milliseconds(100));
	long long counter = 0;

	std::cout << "QUEUING FIRST\n";
	s.add (4, [] () { std::cout << "FIRST\n"; });
	std::cout << "QUEUING SECOND\n";
	s.add (1, [] () { std::cout << "SECOND\n"; });

	std::cout << "WAITING 2 SECONDS\n";
	std::this_thread::sleep_for (std::chrono::seconds (2));

	std::cout << "QUEUING THIRD\n";
	s.add (2, [&counter] () { counter++; std::cout << "THIRD, COUNTER=" << counter << "\n";});

	std::cout << "QUEUING FOURTH\n";
	s.add (1, [&] () { counter++; std::cout << "FOURTH, COUNTER=" << counter << "\n"; });

	std::cout << "WAITING 6 seconds\n";
	std::this_thread::sleep_for (std::chrono::seconds(6));
}

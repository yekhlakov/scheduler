#pragma once
#ifndef __MAXY_CONTROL_SCHEDULER__
#define __MAXY_CONTROL_SCHEDULER__

#include <map>
#include <chrono>
#include <thread>
#include <mutex>
#include <functional>

namespace maxy
{
	namespace control
	{
		class scheduler
		{
			// task type
			using Task_fn = std::function<void ()>;
			// time interval type
			using Time_t = std::chrono::duration<long long, std::micro>;
			// time point type
			using Time_point_t = std::chrono::time_point<std::chrono::high_resolution_clock>;
			// type of the core container
			using Map_type = std::map<Time_point_t, Task_fn>;

			// The task container
			Map_type schedule;

			// Time interval between queue checks (a task may be executed late by this duration at most)
			Time_t wakeup_interval;

			// termination flag
			volatile bool keep_running = true;

			// worker thread
			std::thread thread;

			// mutex for thread safety
			std::mutex lock;

			// Get iterator pointing to a task ready to be executed (or end() if no such task is found)
			Map_type::iterator get_next_task ()
			{
				std::lock_guard<std::mutex> g (lock);

				auto b = schedule.begin ();
				if (b == schedule.end ())
				{
					return b;
				}

				auto e = schedule.lower_bound (now ());
				if (b != e)
				{
					return b;
				}

				return e;
			}

			// Get current time point
			Time_point_t now ()
			{
				return std::chrono::high_resolution_clock::now ();
			}

			// Remove a task pointed at by given iterator
			void remove_task (Map_type::iterator elem)
			{
				std::lock_guard<std::mutex> g (lock);
				schedule.erase (elem);
			}

		public:

			// Default wakeup interval is 100 ms
			scheduler () : scheduler {std::chrono::milliseconds (100)} {};

			// Constructor accepts wakeup interval
			scheduler (Time_t wu) : wakeup_interval {wu}
			{
				thread = std::thread ([this] ()
					{
						while (keep_running)
						{
							auto next = get_next_task ();

							if (next != schedule.end ())
							{
								// execute task without locking
								next->second ();

								remove_task (next);
							}
							else
							{
								// no more elements - just sleep
								std::this_thread::sleep_for (wakeup_interval);
							}
						}
					});
			}

			// Add a task to be executed at certain time point
			void add (Time_point_t tm, Task_fn task)
			{
				std::lock_guard<std::mutex> g (lock);
				schedule[tm] = task;
			}

			// Add a task to be executed after given timeout from now
			void add (Time_t timeout, Task_fn task)
			{
				add (now () + timeout, task);
			}

			// Add a task to be executed in given amount of seconds
			void add (long long seconds, Task_fn task)
			{
				add (now () + std::chrono::seconds (seconds), task);
			}

			// Destructor just stops the worker thread
			~scheduler ()
			{
				keep_running = false;

				thread.join ();
			}
		};
	}
}
#endif // __MAXY_CONTROL_SCHEDULER__

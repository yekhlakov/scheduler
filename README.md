# scheduler
task scheduler lets you run tasks when it is needed

# usage
scheduler is simple to use:

```
// create scheduler
scheduler s();

// the scheduler will run the lambda in 666 seconds
s.add(666, [] () { std::cout << "LOL"; });
```
a task may be scheduled to run at specified moment in time, or after specified timeout (or right away if zero timestamp is specified).

as usual, `test.cpp` provides more usage examples.

scheduler runs in separate thread and is thread-safe.

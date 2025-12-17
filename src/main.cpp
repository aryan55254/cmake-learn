#include <iostream>
#include <thread>
#include <vector>

int counter = 0; // Shared resource, no atomic, no mutex!

void worker()
{
    for (int i = 0; i < 10000; ++i)
    {
        counter++; // DATA RACE HERE
    }
}

int main()
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i)
    {
        threads.emplace_back(worker);
    }

    for (auto &t : threads)
        t.join();

    std::cout << "Final Counter: " << counter << std::endl;
    return 0;
}
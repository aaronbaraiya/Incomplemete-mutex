#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>

class Customer : public std::thread {
public:
    Customer(std::mutex& operatorsMutex, std::condition_variable& standByMessage,
             std::string name)
        : operatorsMutex(operatorsMutex), standByMessage(standByMessage) {
        this->name = name;
        this->start();
    }

    void run() {
        // simulate the time required for the phone call (between one and six seconds)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 6);
        int duration = dis(gen);

        std::unique_lock<std::mutex> lock(operatorsMutex);
        std::cout << name << " is waiting to speak to the operator..." << std::endl;

        standByMessage.wait(lock);

        operatorsMutex.unlock();
        std::cout << name << " is getting the connection to the operator ..." << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(duration));

        std::cout << name << "'s phone call with the operator ending." << std::endl;

        operatorsMutex.lock();
        std::cout << "Available operators=" << availableOperators << std::endl;
        operatorsMutex.unlock();
    }

private:
    std::mutex& operatorsMutex;
    std::condition_variable& standByMessage;
    std::string name;
    int availableOperators;
};

int main() {
    std::mutex operatorsMutex;
    std::condition_variable standByMessage;
    Customer customer1(operatorsMutex, standByMessage, "Customer1");
    Customer customer2(operatorsMutex, standByMessage, "Customer2");

    // Other code to start the simulation
    // ...
    return 0;
}

#include <mutex>
#include <functional>
class DiningPhilosophers {
private:
    std::mutex forks[5];
public:
    DiningPhilosophers() {      
    }
    void wantsToEat(int philosopher,
                    std::function<void()> pickLeftFork,
                    std::function<void()> pickRightFork,
                    std::function<void()> eat,
                    std::function<void()> putLeftFork,
                    std::function<void()> putRightFork) {
        int leftFork = philosopher;
        int rightFork = (philosopher + 1) % 5;
        if (philosopher % 2 == 0) {
            forks[leftFork].lock();
            forks[rightFork].lock();
        } else {
            forks[rightFork].lock();
            forks[leftFork].lock();
        }
        pickLeftFork();
        pickRightFork();
        eat();
        putLeftFork();
        putRightFork();
        forks[leftFork].unlock();
        forks[rightFork].unlock();
    }
};

#include <iostream>
class Reactor {
   private:
    std::unordered_map<int, reactorFunc> fdFunctions;
    std::atomic<bool> running;

   public:
    Reactor();
    ~Reactor();
    typedef void *(*reactorFunc)(int fd);
    // starts new reactor and returns pointer to it
    void *startReactor();
    // adds fd to Reactor (for reading) ; returns 0 on success.
    int addFdToReactor(void *reactor, int fd, reactorFunc func);
    // removes fd from reactor
     int removeFdFromReactor(void *reactor, int fd);
    // stops reactor
    int stopReactor(void *reactor);
};

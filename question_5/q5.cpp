#include "q5.hpp"
Reactor::Reactor() : running(false) {}
Reactor::~Reactor() {
    // Stop the reactor
    stopReactor();
}
// Starts new reactor and returns pointer to it
void *Reactor::startReactor() {
    // Dynamically allocate a new reactor instance
    reactor *newReactor = new reactor();
    return newReactor;
}
int Reactor::addFdToReactor(void *reactor, int fd, reactorFunc func) {
    // Cast the void pointer to a reactor pointer
    reactor *r = static_cast<reactor *>(reactor);
    // Add the file descriptor and function to the reactor
    r->fdFunctions[fd] = func;
    return 0;
}

int Reactor::removeFdFromReactor(void *reactor, int fd) {
    // Cast the void pointer to a reactor pointer
    reactor *r = static_cast<reactor *>(reactor);
    // Remove the file descriptor from the reactor
    r->fdFunctions.erase(fd);
    return 0;
}
int Reactor::stopReactor(void *reactor) {
    running = false
}
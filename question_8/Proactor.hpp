#include <pthread.h>

#include <mutex>
#include <thread>
class Proactor {
   private:
    std::mutex mtx;
    pthread_t server_thread;

   public:
    typedef void* (*proactorFunc)(int sockfd)
        // starts new proactor and returns proactor thread id.
        void startProactor(int sockfd, proactorFunc threadFunc);
    // stops proactor by threadid
    void stopProactor();

    // destructor
    ~Proactor();
};

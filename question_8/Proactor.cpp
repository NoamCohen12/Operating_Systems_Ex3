#include "Proactor.hpp"



void Proactor::startProactor(int sockfd, proactorFunc threadFunc) {
    thread server = thread(sockfdt, [](){
    // accept clients and create a thread for each client
    for(;;){
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        
        int client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd < 0) {
            perror("accept");
            break;
        }
        cout << "New connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << endl;
        thread client_thread = thread(threadFunc, client_fd);
        client_thread.detach();
    }
    server_thread = move(server);
    });
    
}


void Proactor::stopProactor() {
    server_thread.join();
    
    
}

Proactor::~Proactor() {
    stopProactor();
}

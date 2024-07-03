/*
** selectserver.cpp -- a cheezy multiperson chat server
*/

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <sstream>  // Include this for istringstream
#include <string>
#include <vector>

#include "Kosaraju.hpp"
#include "Reactor.hpp"
#define PORT "9034"  // port we're listening on
using namespace std;
vector<list<int>> sharedGraph;  // Shared graph for all clients

Reactor reactor;

string graph_user_commands(string input_user) {
    string ans;
    string command_of_user;
    istringstream iss(input_user);
    iss >> command_of_user;

    int n, m;

    if (command_of_user.empty()) {
        ans += "No command received.\n";
    } else if (command_of_user == "Newgraph") {
        iss >> n >> m;
        if (n <= 0 || m < 0) {
            ans += "Invalid graph parameters. \n";
        } else {
            sharedGraph = Newgraph(iss, n, m);
            ans += "Graph created:\n";
            for (int i = 0; i < n; i++) {
                ans += to_string(i + 1) + ": ";
                for (int neighbor : sharedGraph[i]) {
                    ans += to_string(neighbor + 1) + " ";
                }
                ans += "\n";
            }
        }
    } else if (command_of_user == "Kosaraju") {
        if (!sharedGraph.empty()) {
            ans += kosaraju(sharedGraph.size(), sharedGraph);
        } else {
            ans += "No graph found for Kosaraju.\n";
        }
    } else if (command_of_user == "Newedge") {
        int u, v;
        iss >> u >> v;
        if (!sharedGraph.empty()) {
            addEdge(u, v, sharedGraph);
            ans += "Edge added from " + to_string(u) + " to " + to_string(v) + "\n";
        } else {
            ans += "No graph found for adding edge.\n";
        }
    } else if (command_of_user == "Removeedge") {
        int u, v;
        iss >> u >> v;
        if (!sharedGraph.empty()) {
            removeEdge(u, v, sharedGraph);
            ans += "Edge removed from " + to_string(u) + " to " + to_string(v) + "\n";
        } else {
            ans += "No graph found for removing edge.\n";
        }
    } else {
        ans += "Unknown command.\n";
    }
    return ans;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}
// Callback function for handling data reception on the new connections
void *handle_client_data(int fd) {
    char buf[2048];
    int nbytes;

    // Receive data from the client
    nbytes = recv(fd, buf, sizeof(buf), 0);
    if (nbytes <= 0) {
        if (nbytes == 0) {
            cout << "selectserver: socket " << fd << " hung up" << endl;
        } else {
            perror("recv");
        }
        // Remove the file descriptor from the reactor
        reactor.removeFdFromReactor(fd);
        close(fd);
        return nullptr;
    } else {
        buf[nbytes] = '\0';

        // Process the received data
        string res = graph_user_commands(buf);

        // Send the result back to the client
        send(fd, res.c_str(), res.size(), 0);
    }
    return nullptr;
}

// Callback function for handling new connections
void *handle_new_connection(int fd) {
    char remoteIP[INET6_ADDRSTRLEN];

    int newfd;
    struct sockaddr_storage their_addr;
    socklen_t sin_size = sizeof(their_addr);

    // Accept a new connection
    newfd = accept(fd, (struct sockaddr *)&their_addr, &sin_size);
    if (newfd == -1) {
        perror("accept");
        return nullptr;
    }

    // Convert the IP address to a string
    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), remoteIP, INET6_ADDRSTRLEN);
    cout << "selectserver: got connection from " << remoteIP << endl;

    // Add the newfd to the reactor with handle_client_data as the callback
    reactor.addFdToReactor(newfd, handle_client_data);

    return nullptr;
}

int main() {
    int listener;                        // Listening socket descriptor
    struct sockaddr_storage remoteaddr;  // Client address
    socklen_t addrlen;

    char remoteIP[INET6_ADDRSTRLEN];

    int yes = 1;  // For setsockopt() SO_REUSEADDR, below
    int rv;

    struct addrinfo hints, *ai, *p;

    // Get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        cerr << "selectserver: " << gai_strerror(rv) << endl;
        exit(1);
    }

    for (p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }

        // Lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    // If we got here, it means we didn't get bound
    if (p == NULL) {
        cerr << "selectserver: failed to bind" << endl;
        exit(2);
    }

    freeaddrinfo(ai);  // All done with this

    // Listen
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }

    // Add the listener to the reactor with handle_new_connection as the callback
    reactor.addFdToReactor(listener, handle_new_connection);
    // Run the reactor's event loop
    reactor.run();

    delete &reactor;

    return 0;
}

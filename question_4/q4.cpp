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
#include <sstream>  // Include this for istringstream
#include <string>
#include <vector>
#include <list>

#include "Kosaraju.hpp"

#define PORT "9034"  // port we're listening on

using namespace std;

vector<list<int>> sharedGraph; // Shared graph for all clients

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

int main() {
    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    int fdmax;        // maximum file descriptor number

    int listener;                        // listening socket descriptor
    int newfd;                           // newly accept()ed socket descriptor
    struct sockaddr_storage clientAddress;  // client address
    socklen_t addrlen;

    char buf[256];  // buffer for client data
    int nbytes;

    char remoteIP[INET6_ADDRSTRLEN];

    int yes = 1;  // for setsockopt() SO_REUSEADDR, below
    int i, j, rv;

    struct addrinfo hints, *ai, *p;

    FD_ZERO(&master);  // clear the master and temp sets
    FD_ZERO(&read_fds);

    // get us a socket and bind it
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

        // lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    // if we got here, it means we didn't get bound
    if (p == NULL) {
        cerr << "selectserver: failed to bind" << endl;
        exit(2);
    }

    freeaddrinfo(ai);  // all done with this

    // listen
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }

    // add the listener to the master set
    FD_SET(listener, &master);

    // keep track of the biggest file descriptor
    fdmax = listener;  // so far, it's this one

    // main loop
    for (;;) {
        read_fds = master;  // copy it
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }

        // run through the existing connections looking for data to read
        for (i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) {  // we got one!!
                if (i == listener) {
                    // handle new connections
                    addrlen = sizeof clientAddress;
                    newfd = accept(listener,
                                   (struct sockaddr *)&clientAddress,
                                   &addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        FD_SET(newfd, &master);  // add to master set
                        if (newfd > fdmax) {     // keep track of the max
                            fdmax = newfd;
                        }
                        cout << "selectserver: new connection from "
                             << inet_ntop(clientAddress.ss_family,
                                          get_in_addr((struct sockaddr *)&clientAddress),
                                          remoteIP, INET6_ADDRSTRLEN)
                             << " on socket " << newfd << endl;
                    }
                } else {
                    // handle data from a client
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                        // got error or connection closed by client
                        if (nbytes == 0) {
                            // connection closed
                            cout << "selectserver: socket " << i << " hung up" << endl;
                        } else {
                            perror("recv");
                        }
                        close(i);            // bye!
                        FD_CLR(i, &master);  // remove from master set
                    } else {
                        // we got some data from a client
                        buf[nbytes] = '\0'; // Null-terminate the input
                        string client_input = string(buf);
                        cout << "Received from client: " << client_input << endl;

                        string ans = graph_user_commands(client_input);
                        cout << "Response to client: " << ans << endl;
                        // send the response to all clients
                        for (j = 0; j <= fdmax; j++) {
                            // send to everyone!
                            if (FD_ISSET(j, &master)) {
                                // except the listener
                                if (j != listener) {
                                    if (send(j, ans.c_str(), ans.size(), 0) == -1) {
                                        perror("send");
                                    }
                                }
                            }
                        }
                    }
                }  // END handle data from client
            }  // END got new incoming connection
        }  // END looping through file descriptors
    }  // END for(;;)--and you thought it would never end!

    return 0;
}

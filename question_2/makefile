.PHONY: all clean

CC = g++
CFLAGS =  -g -O2 -pg

all: adj_Deque vecList_Deque generate_input base_adj_List base_adj_Deque vecList_List base_vecList_Deque adj_List base_vecList_List

generate_input: generate_input.cpp
	$(CC) $(CFLAGS) generate_input.cpp -o generate_input

# for kosaraju with deque
adj_Deque: adj_Deque.cpp
	$(CC) $(CFLAGS) adj_Deque.cpp -o adj_Deque

vecList_Deque: vecList_Deque.cpp
	$(CC) $(CFLAGS) vecList_Deque.cpp -o vecList_Deque

# for kosaraju with list
vecList_List: vecList_List.cpp
	$(CC) $(CFLAGS) vecList_List.cpp -o vecList_List

adj_List: adj_List.cpp
	$(CC) $(CFLAGS) adj_List.cpp -o adj_List




# for simple input check 
base_adj_List: base_adj_List.cpp
	$(CC) $(CFLAGS) base_adj_List.cpp -o base_adj_List

base_adj_Deque: base_adj_Deque.cpp
	$(CC) $(CFLAGS) base_adj_Deque.cpp -o base_adj_Deque

base_vecList_Deque: base_vecList_Deque.cpp
	$(CC) $(CFLAGS) base_vecList_Deque.cpp -o base_vecList_Deque

base_vecList_List: base_vecList_List.cpp
	$(CC) $(CFLAGS) base_vecList_List.cpp -o base_vecList_List

clean:
	rm -f gmon.out adj_Deque vecList_Deque generate_input base_adj_List base_adj_Deque vecList_List base_vecList_Deque adj_List base_vecList_List
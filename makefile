main: main.cpp
		g++ -g main.cpp -o main

server: server.cpp
		g++ -g server.cpp -o server

client: client.cpp
		g++ -g client.cpp -o client

clean:
	rm -rf *.o main client server
# echo "files cleaned"
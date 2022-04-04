#define SOCKET_NAME "/tmp/resol.sock"
#define BUFFER_SIZE 1000

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
  struct sockaddr_un addr;
  int i;
  int ret;
  int data_socket;
  // char buffer[BUFFER_SIZE];
  char *buffer;

  /* Create local socket. */

  data_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (data_socket == -1)
  {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  /*
   * For portability clear the whole structure, since some
   * implementations have additional (nonstandard) fields in
   * the structure.
   */

  memset(&addr, 0, sizeof(struct sockaddr_un));

  /* Connect socket to socket address */

  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

  ret = connect(data_socket, (const struct sockaddr *)&addr,
                sizeof(struct sockaddr_un));
  if (ret == -1)
  {
    fprintf(stderr, "The server is down.\n");
    exit(EXIT_FAILURE);
  }

  /* Send arguments. */

  cout << "arguments" << endl;
  for (i = 1; i < argc; ++i)
  {
    ret = write(data_socket, argv[i], strlen(argv[i]) + 1);
    if (ret == -1)
    {
      perror("write");
      break;
    }
  }

  /* Request result. */
  cout << "request results" << endl;
  strcpy(buffer, "END");
  ret = write(data_socket, buffer, strlen(buffer) + 1); // request file content
  if (ret == -1)
  {
    perror("write");
    exit(EXIT_FAILURE);
  }

  /* Receive result. */

  cout << "receive results" << endl;
  ret = read(data_socket, buffer, BUFFER_SIZE);
  if (ret == -1)
  {
    perror("read");
    exit(EXIT_FAILURE);
  }

  /* Ensure buffer is 0-terminated. */

  buffer[BUFFER_SIZE - 1] = 0;

  printf("Result = %s\n", buffer);

  // search lines that match keyword

  istringstream fileContent(buffer);
  string line;
  string output;
  while (getline(fileContent, line))
  {
    int pos = 0;
    int index;
    bool match;
    match = line.find(argv[0], pos) != string::npos;

    if (match)
    {
      output += line + "\n";
    }
  }

  /* send output to server */
  ret = write(data_socket, &buffer, strlen(buffer) + 1);
  if (ret == -1)
  {
    perror("write");
    exit(EXIT_FAILURE);
  }

  /* Ensure buffer is 0-terminated. */

  buffer[BUFFER_SIZE - 1] = 0;

  /* Close socket. */

  close(data_socket);

  exit(EXIT_SUCCESS);
}

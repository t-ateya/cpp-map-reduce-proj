#define SOCKET_NAME "/tmp/resol.sock"
#define BUFFER_SIZE 1000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

string read_from_file(char *sf);

int main(int argc, char *argv[])
{
  struct sockaddr_un addr;
  int down_flag = 0;
  int ret;
  int listen_socket;
  int data_socket;
  int result;

  // char buffer[BUFFER_SIZE];
  char *buffer;
  string response;
  char *query;

  /*
   * In case the program exited inadvertently on the last run,
   * remove the socket.
   */

  unlink(SOCKET_NAME);

  /* Create local socket. */

  listen_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (listen_socket == -1)
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

  /* Bind socket to socket name. */

  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

  ret = bind(listen_socket, (const struct sockaddr *)&addr,
             sizeof(struct sockaddr_un));
  if (ret == -1)
  {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  /*
   * Prepare for accepting connections. The backlog size is set
   * to 20. So while one request is being processed other requests
   * can be waiting.
   */

  ret = listen(listen_socket, 20);
  if (ret == -1)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  /* This is the main loop for handling connections. */

  for (;;)
  {

    /* Wait for incoming connection. */

    data_socket = accept(listen_socket, NULL, NULL);
    if (data_socket == -1)
    {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    result = 0;
    for (;;)
    {

      /* Wait for next data packet. */

      ret = read(data_socket, &buffer, BUFFER_SIZE);

      if (ret == -1)
      {
        perror("read");
        exit(EXIT_FAILURE);
      }

      /* Ensure buffer is 0-terminated. */

      buffer[BUFFER_SIZE - 1] = 0;

      /* Handle commands. */

      if (!strncmp(buffer, "DOWN", BUFFER_SIZE))
      {
        down_flag = 1;
        break;
      }

      if (!strncmp(buffer, "END", BUFFER_SIZE))
      {
        break;
      }

      /* Add received summand. */
      cout << "query: " << buffer << endl;

      cout << "reading file" << endl;
      response = read_from_file(buffer);
      cout << response << endl;
    }

    /* Send result. */

    char *cstr = new char[response.length() + 1];
    strcpy(cstr, response.c_str());

    sprintf(buffer, "%s", cstr);
    // cout << "Response: " << response << endl;

    // sprintf(buffer, "%s", result);
    ret = write(data_socket, &buffer, BUFFER_SIZE);

    if (ret == -1)
    {
      perror("write");
      exit(EXIT_FAILURE);
    }

    /* read processed data from client */
    ret = read(data_socket, &buffer, BUFFER_SIZE);
    if (ret == -1)
    {
      perror("read");
      exit(EXIT_FAILURE);
    }

    cout << endl
         << "Lines that match the keyword are:" << endl;
    cout << buffer << endl;

    /* Close socket. */

    close(data_socket);

    /* Quit on DOWN command. */

    if (down_flag)
    {
      break;
    }
  }

  close(listen_socket);

  /* Unlink the socket. */

  unlink(SOCKET_NAME);

  exit(EXIT_SUCCESS);
}

string read_from_file(char *search_phrase)
{
  fstream file;
  std::string output;

  file.open("test.txt", std::ios::in);

  if (file.is_open())
  {
    std::string content;
    while (getline(file, content))
    {
      output += content + "\n";
    }
    file.close();
  }

  return output;
}

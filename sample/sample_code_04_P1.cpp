#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <fstream>

// Compile this file using:
//$ g++ -o write.out sample_code_04_P1.cpp -lrt
// Then run it using:
//$ ./write.out

#define SHMPATH "/mysm007"

using namespace std;

const char *read_from_file();

int main(int argc, char **argv)
{

    int fd = shm_open(SHMPATH, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("shm_open");
        return 1;
    }

    const char *my_str = read_from_file(); // this should be the content of our input file
    size_t shm_size = strlen(my_str) + 1;

    ftruncate(fd, shm_size);

    void *shmp = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shmp == MAP_FAILED)
    {
        perror("Could not mmap");
        return 1;
    }

    memcpy(shmp, my_str, shm_size);

    printf("Contents of region: %s\n", (char *)shmp);

    munmap(shmp, shm_size);
    // shm_unlink(SHMPATH);

    return 0;
}

const char *read_from_file()
{
    fstream file;
    std::string output;

    file.open("../test.txt", std::ios::in);

    if (file.is_open())
    {
        std::string content;
        while (getline(file, content))
        {
            output += content + "\n";
        }
        file.close();
    }

    char *result = new char[output.length() + 1];
    strcpy(result, output.c_str());

    return result;
}

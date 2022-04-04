#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <pthread.h>
#include <string>
#include <sstream>

#define SHMPATH "/mysm007"
#define BUFFER_SIZE 1024

// Compile this file using
//$ g++ -o read.out sample_code_04_P2.cpp -lrt
// Then, run it using
//$ ./read.out

using namespace std;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int containWordIgnoreCase(const char *line, const char *word, size_t n)
{
    size_t m = strlen(word);

    char *lower_line = (char *)calloc(n + 1, sizeof(char));
    for (size_t i = 0; i < n; ++i)
    {
        lower_line[i] = tolower((unsigned char)line[i]);
    }
    char *lower_word = (char *)calloc(m + 1, sizeof(char));
    for (size_t i = 0; i < m; ++i)
    {
        lower_word[i] = tolower((unsigned char)word[i]);
    }

    // printf("%s, %s\n", lower_line, lower_word);
    char *ret = strstr(lower_line, lower_word);

    // printf("%s\n", ret);
    int return_val = 0;
    if (ret != NULL)
        if (strlen(ret) == m || !isalnum(ret[m]))
        {
            if (ret == lower_line)
                return_val = 1;
            else
            {
                ret--;
                if (!isalnum(ret[0]))
                    return_val = 1;
            }
        }
    free(lower_line);
    free(lower_word);

    return return_val;
}

typedef struct
{
    vector<string> v_tmp;
    const char *target;
    int begin;
    int end;
    vector<string> res_vec;
} MY_ARGS;

void *find_and_output(void *args)
{

    MY_ARGS *my_args = (MY_ARGS *)args;
    vector<string> v_tmp = my_args->v_tmp;
    const char *target = my_args->target;
    int first = my_args->begin;
    int last = my_args->end;

    int i = first;
    int counter = 0;
    for (; i < last; i++)
    {
        const char *tmp_line = v_tmp[i].c_str();
        if (containWordIgnoreCase(tmp_line, target, strlen(tmp_line)))
            my_args->res_vec.push_back(v_tmp[i]);
    }

    return NULL;
}

int main(int argc, char **argv)
{

    vector<string> v3;

    int fd = shm_open(SHMPATH, O_RDWR, 0);
    if (fd == -1)
    {
        perror("shm_open");
        return 1;
    }

    size_t shm_size = BUFFER_SIZE;
    ftruncate(fd, shm_size);

    /* Read from shared memory */
    void *fileContent = mmap(NULL, shm_size, PROT_READ, MAP_SHARED, fd, 0);
    if (fileContent == MAP_FAILED)
    {
        perror("Could not mmap");
        return 1;
    }

    printf("Contents of region: %s\n", (char *)fileContent);

    // find target word in filecontent using threads
    char *temp;

    // TODO: Read lines and push each line to a vector data structure
    // while (getline(fileContent, temp, '\n'))
    // {
    // }

    // TODO: Continue with sample 3 code to search content using 4 threads

    // TODO: Write the results found in the shared memory again

    // TODO: Ensure that the sample cod part 1 is listening for result from this thread

    munmap(fileContent, shm_size);
    shm_unlink(SHMPATH);

    return 0;
}
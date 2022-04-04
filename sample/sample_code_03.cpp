#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <vector>
#include <pthread.h>

using namespace std;

// Compile this file using
//$ g++ -o sample3.out -pthread sample_code_03.cpp -lrt
// Then, run it using
//$ ./read.out

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

int main(int argc, char *argv[])
{
    vector<string> v3;

    const char *target = "world";

    v3.push_back("This a worlds;");
    v3.push_back("Hello, my World!");
    v3.push_back("This sentence is meaningless");
    v3.push_back("This world is meaningless");
    v3.push_back("This sentence is change world");
    v3.push_back("World sentence is meaningless");
    v3.push_back("Hello, myWorlD");
    v3.push_back("This sentence is also meaningless");
    v3.push_back("wORld and WorLDs");
    v3.push_back("This is the last meaningless sentence.");

    int how_many = v3.size();

    pthread_t th1;
    pthread_t th2;
    pthread_t th3;
    pthread_t th4;

    MY_ARGS args1 = {v3, target, 0, how_many / 4};
    MY_ARGS args2 = {v3, target, how_many / 4, how_many / 2};
    MY_ARGS args3 = {v3, target, how_many / 2, (3 * how_many) / 4};
    MY_ARGS args4 = {v3, target, (3 * how_many) / 4, how_many};

    pthread_create(&th1, NULL, find_and_output, &args1);
    pthread_create(&th2, NULL, find_and_output, &args2);
    pthread_create(&th3, NULL, find_and_output, &args3);
    pthread_create(&th4, NULL, find_and_output, &args4);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);
    pthread_join(th4, NULL);

    // vector<string> result;

    for (vector<string>::iterator t = args1.res_vec.begin(); t != args1.res_vec.end(); t++)
    {
        cout << *t << endl;
    }

    for (vector<string>::iterator t = args2.res_vec.begin(); t != args2.res_vec.end(); t++)
    {
        cout << *t << endl;
    }

    for (vector<string>::iterator t = args3.res_vec.begin(); t != args3.res_vec.end(); t++)
    {
        cout << *t << endl;
    }

    for (vector<string>::iterator t = args4.res_vec.begin(); t != args4.res_vec.end(); t++)
    {
        cout << *t << endl;
    }

    return 0;
}

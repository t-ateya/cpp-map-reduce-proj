#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include <pthread.h>

using namespace std;

int containWordIgnoreCase(const char *line, const char *word, size_t n) {
    size_t m = strlen(word);

    char *lower_line = (char*) calloc(n+1, sizeof(char));
    for (size_t i = 0; i < n; ++i) {
       lower_line[i] = tolower((unsigned char)line[i]);
    }
    char *lower_word = (char*) calloc(m+1, sizeof(char));
    for (size_t i = 0; i < m; ++i) {
       lower_word[i] = tolower((unsigned char)word[i]);
    } 

    //printf("%s, %s\n", lower_line, lower_word);
    char * ret = strstr (lower_line, lower_word);
    //printf("%s\n", ret);
    int return_val  = 0;
    while( ret != NULL )
    {
        //The matched substring is the end of the sentence
        //Or the character after the substring is neither a letter nor a number -> Punctuation Marks or blank space
        if ( strlen(ret)==m || !isalnum(ret[m]) )
        {
            //the sentence begins with the matched substring
            if(ret == lower_line)
            {
              	return_val = 1;
            }
            //Otherwise, the character before the substring is neither a letter nor a number 
            else{
                ret--;
                if (!isalnum(ret[0]))
                    return_val = 1;
            }
        }

        ret = strstr (ret+m, lower_word);
    }

    free(lower_line);
    free(lower_word);

    return return_val;
}

typedef struct{
    vector<string> v_tmp;
    const char * target;
    int begin;
    int end;
    //int result;
} MY_ARGS;

void * find_and_output(void* args){
 
    MY_ARGS* my_args = (MY_ARGS*) args;
    vector<string> v_tmp = my_args -> v_tmp;
    const char * target = my_args -> target;
    int first = my_args -> begin;
    int last = my_args -> end;
 
    int i = first;
    int counter = 0;
    for (; i<last; i++){
        const char * tmp_line = v_tmp[i].c_str();
        if (containWordIgnoreCase(tmp_line, target, strlen(tmp_line)))
            cout<<v_tmp[i]<<endl;
    }
 
    return NULL;
}


int main(int argc, char* argv[]) {
 
    vector<string> v3;

    const char * target = "world";

    v3.push_back("This a worlds;");
    v3.push_back("Hello, my World!");
    v3.push_back("wORld and WorLDs");
    v3.push_back("This sentence is meaningless");
    v3.push_back("Hello, myWorlD");
    v3.push_back("This sentence is also meaningless");
    v3.push_back("worlds and world");
    v3.push_back("This is the last meaningless sentence.");

    int how_many = v3.size();

    pthread_t th1;
    pthread_t th2;

    MY_ARGS args1 = {v3, target, 0, how_many/2};
    MY_ARGS args2 = {v3, target, how_many/2, how_many};

    pthread_create(&th1, NULL, find_and_output, &args1);
    pthread_create(&th2, NULL, find_and_output, &args2);
 
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    return 0;
}

//coding exercise 1
//Try a bigger vector v3
//Create 4 threads to finish the task


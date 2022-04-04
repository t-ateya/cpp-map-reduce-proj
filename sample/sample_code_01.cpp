#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

using namespace std;

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
    while (ret != NULL)
    {
        // The matched substring is the end of the sentence
        // Or the character after the substring is neither a letter nor a number -> Punctuation Marks or blank space
        if (strlen(ret) == m || !isalnum(ret[m]))
        {
            // the sentence begins with the matched substring
            if (ret == lower_line)
            {
                return_val = 1;
            }
            // Otherwise, the character before the substring is neither a letter nor a number
            else
            {
                ret--;
                if (!isalnum(ret[0]))
                    return_val = 1;
            }
        }

        ret = strstr(ret + m, lower_word);
    }

    free(lower_line);
    free(lower_word);

    return return_val;
}

int main(int argc, char *argv[])
{

    vector<string> v3;
    const char *target = "world";

    v3.push_back("This a worlds;");
    v3.push_back("Hello, my World!");
    v3.push_back("wORld and WorLDs");
    v3.push_back("This sentence is meaningless");
    v3.push_back("Hello, myWorlD");
    v3.push_back("This sentence is also meaningless");
    v3.push_back("worlds and world");
    v3.push_back("This is the last meaningless sentence.");

    for (int i = 0; i < 7; i++)
    {
        const char *tmp_line = v3[i].c_str();
        if (containWordIgnoreCase(tmp_line, target, strlen(tmp_line)))
            cout << v3[i] << endl;
    }

    return 0;
}
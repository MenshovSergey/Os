#include <string.h>

#include "helpers.h"

#define BUFFER_SIZE 1024

int push(char *buf, int to_move, int count)
{   
    memmove(buf, buf + to_move, count - to_move);
    return count - to_move;
}

int main(int argc, char *argv[])
{   

    char *args[argc + 1];
    int length_argc = argc - 1;
    int i;
    for (i = 1; i < argc; ++i)
    {
        args[i - 1] = argv[i];
    }

    char buffer[BUFFER_SIZE];
    char arg[BUFFER_SIZE];
    ssize_t count;
    int size = 0;

    while ((count = read_until(STDIN_FILENO, buffer + size, BUFFER_SIZE - size, '\n')) > 0)
    {   
        int word_count = count + size;
        while (1)
        {
            int flag = 0;
            for (i = 0; i < word_count; ++i)
            {
                if (buffer[i] == '\n')
                {
                    memcpy(arg, buffer, i);
                    arg[i] = 0;
                    args[length_argc] = arg;
                    args[length_argc + 1] = NULL;
                    int res = spawn(args[0], args);
                    if (!res)
                    {   
                        arg[i] = '\n';
                        ssize_t writed = write_(STDOUT_FILENO, arg, i + 1);
                        if (writed < 0)
                        {
                            exit(EXIT_FAILURE);
                        }
                    }
                    word_count = push(buffer, i + 1, word_count);
                    flag = 1;
                    break;
                }
            }
            if (!flag)
            {
                break;
            }
        }
        size = word_count;
    }
    if (count < 0)
    {
        exit(EXIT_FAILURE);
    }

    if (size > 0)
    {
        memcpy(arg, buffer, size);
        arg[size] = 0;
        args[length_argc] = arg;
        args[length_argc + 1] = NULL;
        int res = spawn(args[0], args);
        if (!res)
        {
            ssize_t writed = write_(STDOUT_FILENO, arg, i + 1);
            if (writed < 0)
            {
                exit(EXIT_FAILURE);
            }
        }
    }
    return 0;
}
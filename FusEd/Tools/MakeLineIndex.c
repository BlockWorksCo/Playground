

#include <stdio.h>




int main(int argc, char* argv[])
{
    char*   fileName    = argv[1];
    int     fd          = open(fileName, "r");
    
    if(fd != -1)
    {
        char    block[4096];
        off_t   blockPosition   = lseek(fd, 0, SEEK_CUR);
        ssize_t readResult  = read(fd, &block[0], sizeof(block));
        for(uint32_t i=0; i<sizeof(block); i++)
        {
            if(block[i] == '\n')
            {
                
            }
        }
        return 0;
    }
    else
    {
        return -1;
    }
}


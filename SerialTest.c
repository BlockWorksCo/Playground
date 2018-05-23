
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdint.h>
#include <stdbool.h>

/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.

We need these:
intr = ^C; quit = ^\; erase = ^?; kill = ^U; eof = ^D; eol = <undef>; eol2 = <undef>; swtch = <undef>; start = ^Q; stop = ^S; susp = ^Z; rprnt = ^R; werase = ^W; lnext = ^V; discard = ^O;
min = 1; time = 5;
-parenb -parodd -cmspar cs8 -hupcl -cstopb cread clocal -crtscts
-ignbrk -brkint -ignpar parmrk -inpck -istrip -inlcr -igncr -icrnl ixon -ixoff -iuclc -ixany -imaxbel -iutf8
-opost -olcuc -ocrnl -onlcr -onocr -onlret -ofill -ofdel nl0 cr0 tab0 bs0 vt0 ff0
isig -icanon -iexten -echo -echoe -echok -echonl -noflsh -xcase -tostop -echoprt -echoctl -echoke -flusho -extproc

 */

int open_port(void)
{
    int fd;                                   /* File descriptor for the port */

    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd == -1)
    {
        /* Could not open the port */
        fprintf(stderr, "open_port: Unable to open port - %s\n",
                strerror(errno));
    }

    return (fd);
}

void main()
{
    int mainfd = 0;                                          /* File descriptor */
    struct termios options;

    mainfd = open_port();

    fcntl(mainfd, F_SETFL, FNDELAY);                  /* Configure port reading */
    /* Get the current options for the port */
    tcgetattr(mainfd, &options);

    /* Enable the receiver and set local mode */
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB; /* Mask the character size to 8 bits, no parity */
    options.c_cflag &= ~CSTOPB;

    options.c_cflag &= ~IGNBRK;
    options.c_cflag &= ~BRKINT;
    options.c_cflag &= ~IGNPAR;
    options.c_cflag |= PARMRK; // break translate.
    options.c_cflag &= ~INPCK;
    options.c_cflag &= ~INPCK;

    options.c_cflag &= ~CSIZE;
    options.c_cflag |=  CS8;                              /* Select 8 data bits */
    options.c_cflag &= ~CRTSCTS;               /* Disable hardware flow control */

    /* Enable data to be processed as raw input */
    //options.c_lflag &= ~(ICANON | ECHO | ISIG);

    /* Set the new options for the port */
    cfsetispeed(&options, B57600);                 /* Set the baud rates to 9600 */
    cfsetospeed(&options, B57600);
    tcsetattr(mainfd, TCSANOW, &options);

    while (1)
    {
        uint8_t breakBuffer[3]  = {0};
        int     bytesRead       = read(mainfd, &breakBuffer[0], sizeof(breakBuffer));
        bool    breakFlag       = false;

        for(int i=0; i<bytesRead; i++)
        {
            printf("raw (%02x).\n", breakBuffer[i] );
        }

        if(bytesRead == 3)
        {
            if((breakBuffer[0] == 0xff) && (breakBuffer[1] == 0x00) && (breakBuffer[2] == 0x00))
            {
                printf("<BREAK>\n");
                bytesRead   = 0;
                breakFlag   = true;
            }
        }

        if(bytesRead == 2)
        {
            if((breakBuffer[0] == 0xff) && (breakBuffer[1] == 0xff))
            {
                printf("<ESCAPED FF>\n");
                bytesRead   = 1;
            }
        }

        for(int i=0; i<bytesRead; i++)
        {
            printf("Processed %c (%02x).\n", breakBuffer[i], breakBuffer[i] );
        }

        usleep(2000);
    }

    /* Close the serial port */
    close(mainfd);
}


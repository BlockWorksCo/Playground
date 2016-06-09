//
// Purpose of this shell is:
//  - Abstract the details of the debugger from the GUI (portable interface, not shell/stdio related).
//  - Provide isolation of the test script (multiple scripts shall not interfere with each other).
//  - Test reliability not linked to GUI.
//  - Provide watchdog type facilities for the test script (maybe this is better done outside?).
//  - Abstract the details of the interpreter (potentially use different Python API for old/new versions).
//  - Potentially run scripts simultaeously that need different interpreters/environments.
//  - Provide GUI reconnection facilities.
//  - Provide remote debug facility, potentially without a shell/stdio.
//




#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "Common.h"
#include "Shell.h"




//
//
//
void* ShellMain(void* args)
{
    //
    //
    //
    while(true)
    {
        char*   line            = NULL;
        size_t  size            = 128;

        if (getline(&line, &size, stdin) > 0)
        {
            char* eol   = strchr( line, '\n');
            if( eol != NULL )
            {
                *eol    = 0;
            }

            ProcessRequest(line);
        }

    }
}



#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>






#define ASSERT(test, failText)      {if(test == 0) {failureType=(char*)"ASSERT";failureDetails=(char*)failText;successFlag=false;return;} }

//
//
//
void One(char*& className, char*& testName, bool& successFlag, char*& failureType, char*& failureDetails)
{    
    className        = (char*)"Global";
    testName         = (char*)"One";
    successFlag      = (char*)true;
    failureType      = (char*)"*generic fail*";
    failureDetails   = (char*)"*failure details*";

    ASSERT( 1 == 1, "one is not one");
}



//
//
//
void Two(char*& className, char*& testName, bool& successFlag, char*& failureType, char*& failureDetails)
{    
    className        = (char*)"Global";
    testName         = (char*)"Two";
    successFlag      = (char*)true;
    failureType      = (char*)"*generic fail*";
    failureDetails   = (char*)"*failure details*";

    ASSERT( 2 == 1, "two is not one");
}



//
//
//
void Three(char*& className, char*& testName, bool& successFlag, char*& failureType, char*& failureDetails)
{    
    className        = (char*)"Global";
    testName         = (char*)"Three";
    successFlag      = (char*)true;
    failureType      = (char*)"*generic fail*";
    failureDetails   = (char*)"*failure details*";

    ASSERT( 13 == 3, "thirteen is not three");
}
















//
//
//
typedef void (*testFn)(char*&, char*&, bool&, char*&, char*&);



//
//
//
int RunTests(testFn testFns[], int numberOfTests)
{
    char*   className;
    char*   testName;
    bool    successFlag;
    char*   failureType;
    char*   failureDetails;


    printf("<testsuite tests=\"%d\">\n", numberOfTests);

        for(int testNumber=0; testNumber<numberOfTests; testNumber++)
        {
            testFns[testNumber]( className, testName, successFlag, failureType, failureDetails );

            if(successFlag == true)
            {
                printf("\t<testcase classname=\"%s\" name=\"%s\"/>\n", className, testName);
            }
            else
            {
                printf("\t<testcase classname=\"%s\" name=\"%s\">\n", className, testName);
                printf("\t<failure type=\"%s\"> %s </failure>", failureType, failureDetails);
                printf("</testcase>\n");            
            }            
        }

    printf("</testsuite>\n");
}


//
//
//
int main()
{
    testFn  tests[]     =
    {
        One,
        Two,
        Three,
    };    

    RunTests( tests, sizeof(tests)/sizeof(tests[0]) );
    return 0;
}





#include <stdio.h>
#include <pthread.h>




int main()
{
    pthread_mutex_t     mutex;
    pthread_mutexattr_t attr;

    pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );
    pthread_mutex_init( &mutex, &attr );
    //pthread_mutex_init( &mutex, 0 );

    printf("1\n");
    pthread_mutex_lock( &mutex );
    printf("2\n");
    pthread_mutex_lock( &mutex );
    printf("3\n");
    pthread_mutex_unlock( &mutex );
    printf("4\n");
    pthread_mutex_unlock( &mutex );
    printf("5\n");
}


#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
//#define DEBUG_LOG(msg,...)
#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    // DEBUG_LOG(thread_func_args);
    usleep(thread_func_args->wait_to_obtain_ms*1000); //waiting to obtain mutex
    pthread_mutex_lock(thread_func_args->mutex);//obtaining mutex
    usleep(thread_func_args->wait_to_obtain_ms*1000);//waiting to release
    pthread_mutex_unlock(thread_func_args->mutex);
    thread_func_args->thread_complete_success=true;
    return (void *)thread_func_args;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    //allocate memory of sixe thread data  for the thread data
    struct thread_data* thread_data= (struct thread_data*) malloc(sizeof(struct thread_data));
    /*check if nulll return false */
    if (thread_data==NULL)
    {
        return false;
    }
    
    //assign values to the thread data
    thread_data->mutex=mutex;
    thread_data->wait_to_obtain_ms=wait_to_obtain_ms;
    thread_data->wait_to_release_ms=wait_to_release_ms;
    thread_data->thread_complete_success=false;
    //create thread
    int result = pthread_create(thread,NULL,threadfunc,(void*)thread_data);
    //if returns a non-zero ==fail //free thread memory &return false else return true
    if (result!=0)
    {
        free(thread_data);
        return false;
    }
    
    return true;
}


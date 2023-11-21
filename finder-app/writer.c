
/********************************************************************
 * Write a C application “writer” ( finder-app/writer.c) which can be used as an alternative to the “writer.sh” test script created in assignment1 and using File IO as described in LSP chapter 2.  See the Assignment 1 requirements for the writer.sh test script and these additional instructions:
 * One difference from the write.sh instructions in Assignment 1: 
 * You do not need to make your "writer" utility create directories which do not exist. You can assume the directory is created by the caller.
 * Setup syslog logging for your utility using the LOG_USER facility.
 * Use the syslog capability to write a message “Writing <string> to <file>” where <string> is the text string written to file (second argument) and <file> is the file created by the script.
 * This should be written with LOG_DEBUG level.
 * Use the syslog capability to log any unexpected errors with LOG_ERR level.
 * 
*/


/*include stdlib stdio syslog*/
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>

/* add argc and argv arrays to deal with arguments passed to the executable */
int main(int argc,char* argv[]){
    /*set the log mask to allow LOG_DEBUG AND LOG_ERROR */
    setlogmask(LOG_UPTO(LOG_DEBUG));
    /*Open syslog connectionwith LOG_USER facility */
    openlog("writer",LOG_PID|LOG_NDELAY,LOG_USER);
    /*check if the user provided a file path and content */
    if (argc<3)
    {
        syslog(LOG_ERR,"Error: Missing arguments Please provide a file path and content.\n");
        return 1;
    }

    /* get file file path and content*/
    char *file_path= argv[1];
    char*content= argv[2]; 
    /*open file with write mode*/
    FILE *file =fopen(file_path,"w");
    /*check if file was opened successfully*/
    if(file==NULL){
        syslog(LOG_ERR,"Error: couldn't open the file %s.\n",file_path);
        return 1;
    }
    /* write content to file*/
    fprintf(file,"%s\n",content);
    /*close file*/
    fclose(file);
    /*log debug file */
    syslog(LOG_DEBUG,"Successfully wrote file %s\n",file_path);
    /*close syslog*/
    closelog();
    return 0;
}

/* writer.c
 * Author: Nathan Johnson
 * Writes the specified string to the specified file.
 * Usage: writer <writefile> <writestr> where
 * <writefile>: Full path of the file to write, created if does not exist,
 * overwritten if existing.  The directory must exist.
 * <writestr>: The string to write.
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <syslog.h>

int main(int argc, char *argv[])
{
    char *writefile = NULL;
    char *writestring = NULL;
    FILE *fp = NULL;
    int error = 0;

    /* Setup logging. */
    openlog(NULL, 0, LOG_USER);

    /* Check that both arguments are provided (ignore extra arguments) */
    if(argc >= 3)
    {
        /* Populate local variables from command line arguments. */
        writefile = argv[1];
        writestring = argv[2];

        /* Log the write to be attempted. */
        syslog(LOG_DEBUG, "Writing %s to %s", writefile, writestring);

        /* Attempt to open the file. */
        fp = fopen(argv[1], "w");
        if(fp != NULL)
        {
            /* Attempt to print the string to the file. */
            if(fprintf(fp, "%s\n", argv[2]) > 0)
            {
                fclose(fp);
                return 0;
            }
            else
            {
                /* File write failed, log error and exit. */
                error = errno;
                printf("File write failed: %s\n", strerror(error));
                syslog(LOG_ERR, "File open failed: %s", strerror(error));
                fclose(fp);
                return 1;
            }
        }
        {
            /* File open attempt failed, log error and exit. */
            error = errno;
            printf("File open failed: %s\n", strerror(error));
            syslog(LOG_ERR, "File open failed: %s", strerror(error));
            return 1;
        }
    }
    else
    {
        /* Insufficient arguments provided, log error and exit. */
        syslog(LOG_ERR, "Missing argument(s)");
        printf("%s", "Missing arguments: usage writer <writefile> <writestr>\n");
        return 1;
    }
}

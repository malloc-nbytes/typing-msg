#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>

// Restore cursor exit
void
cleanup(int signum)
{
        printf("\033[?25h");
        fflush(stdout);
        exit(0);
}

int
main(int argc, char **argv)
{
        if (argc <= 1) {
                printf("Usage: starting-soon <msg>\n");
                exit(1);
        }

        putchar('\n');

        signal(SIGINT, cleanup);

        // Hide the terminal cursor
        printf("\033[?25l");
        fflush(stdout);

        char *msg = argv[1];
        size_t n = strlen(msg);

        while (1) {
                for (size_t i = 0; i <= n; i++) {
                        printf("\r\033[2K"); // Clear line
                        for (size_t j = 0; j < i; j++) {
                                putchar(msg[j]);
                        }
                        putchar('_');
                        fflush(stdout);
                        usleep(100000);
                }

                struct timespec start, current;
                clock_gettime(CLOCK_MONOTONIC, &start);
                double elapsed = 0;
                int blink_state = 1;

                while (elapsed < 5.0) {
                        printf("\r\033[2K");
                        printf("%s", msg);
                        if (blink_state) {
                                putchar('_');
                        }
                        fflush(stdout);

                        usleep(500000);
                        clock_gettime(CLOCK_MONOTONIC, &current);
                        elapsed = (current.tv_sec - start.tv_sec) +
                                (current.tv_nsec - start.tv_nsec) / 1e9;
                        blink_state = !blink_state;
                }

                printf("\r\033[2K");
                fflush(stdout);
        }

        printf("\033[?25h");
        fflush(stdout);
        return 0;
}

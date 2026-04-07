#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <spawn.h>
#include <sys/wait.h>

extern char **environ;

// Helper to spawn gpio-rp1 commands
static int run_cmd(char *const argv[]) {
    pid_t pid;
    int status;
    if (posix_spawnp(&pid, argv[0], NULL, NULL, argv, environ) != 0) {
        perror("posix_spawnp");
        return -1;
    }
    if (waitpid(pid, &status, 0) == -1)
    {
        perror("waitpid");
        return -1;
    }
    return status;
}

bool gpio_set_output(int pin) {
    char p[8];
    snprintf(p, sizeof(p), "%d", pin);
    char *cmd[] = { "gpio-rp1", "set", p, "op", "pn", "dl", NULL };
    return run_cmd(cmd) == 0;
}

bool gpio_high(int pin) {
    char p[8];
    snprintf(p, sizeof(p), "%d", pin);
    char *cmd[] = { "gpio-rp1", "set", p, "dh", NULL };
    return run_cmd(cmd) == 0;
}

bool gpio_low(int pin) {
    char p[8];
    snprintf(p, sizeof(p), "%d", pin);
    char *cmd[] = { "gpio-rp1", "set", p, "dl", NULL };
    return run_cmd(cmd) == 0;
}

int main()
{
    int pin = 20;  // GPIO20 (physical header pin 38 on Pi 5)

    if (!gpio_set_output(pin))
    {
        printf("Failed to configure pin\n");
        return 1;
    }

    while (1)
    {
        gpio_high(pin);
        usleep(500000);
        gpio_low(pin);
        usleep(500000);
    }
}

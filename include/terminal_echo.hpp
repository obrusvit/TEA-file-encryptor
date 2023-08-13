#ifndef GUARD_ENCRYPTOR_SIMPLE_TERMINAL_ECHO
#define GUARD_ENCRYPTOR_SIMPLE_TERMINAL_ECHO

#include <termios.h>
#include <unistd.h>

/**
 * Disables terminal echo for password input.
 * Works for Unix only.
 * https://stackoverflow.com/a/1455007/4825420
 */
inline void SetStdinEcho(bool enable = true)
{
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (!enable)
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;

    (void)tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

#endif


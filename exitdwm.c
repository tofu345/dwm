#include <stdio.h>
#include <string.h>

void exitdwm ()
{
#if			    \
   defined S_RESTART_DWM || \
   defined S_EXIT_DWM	 || \
   defined S_REBOOT	 || \
   defined S_SHUTDOWN	 || \
   defined S_HIBERNATE
#error (conflicting macro names)
#endif

#define S_RESTART_DWM "Restart Dwm "
#define S_EXIT_DWM "Exit Dwm "
#define S_SHUTDOWN "Shutdown "
#define S_REBOOT "Reboot "
#define S_HIBERNATE "Hibernate "

    FILE * exit_menu =
        popen("echo \""
              S_RESTART_DWM "\n"
              S_EXIT_DWM "\n"
              S_SHUTDOWN "\n"
              S_REBOOT "\n"
              S_HIBERNATE "\" | rofi -dmenu -i -p ''", "r");

    char exit_action[16];
    if (exit_menu == NULL || fscanf(exit_menu, "%15[a-zA-Z -]", exit_action) == EOF)
    {
        fputs("Error. Failure in exit_dwm.", stderr);
        goto close;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"

    /* unused-result warning in here get ignored during compilation */
    if (strcmp (exit_action, S_RESTART_DWM) == 0)
        quit(&(const Arg){1});
    else if (strcmp (exit_action, S_EXIT_DWM) == 0)
        quit(&(const Arg){0});
    else if (strcmp(exit_action, S_SHUTDOWN) == 0)
        system("doas loginctl poweroff");
    else if (strcmp(exit_action, S_REBOOT) == 0)
        system("doas loginctl reboot");
    else if (strcmp(exit_action, S_HIBERNATE) == 0)
        system("doas loginctl hibernate");

#pragma GCC diagnostic pop

close:
    pclose(exit_menu);

#undef S_RESTART_DWM
#undef S_EXIT_DWM
#undef S_SHUTDOWN
#undef S_REBOOT
#undef S_HIBERNATE
}

/***************************************************************************

		rv40_syswrch.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-06-27 14:15:35

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

void
_ttywrch(int ch)
{
    printf("%c",ch);
}

void
_sys_exit(int return_code)
{
		printf("call system exit.\n");
    while(1);
}

char *
_sys_command_string(char *cmd, int len)
{
    return "[cmd]";
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Helper: escape and quote a string
void quote_arg(char *dst, const char *src)
{
    strcat(dst, " \"");
    strcat(dst, src);
    strcat(dst, "\"");
}

// Converts Unix path (e.g. /cygdrive/c/foo) to Windows path (e.g. C:\foo)
void cygwin_to_win_path(const char *src, char *dst, int dst_len)
{
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "cygpath -w \"%s\"", src);
    FILE *fp = _popen(cmd, "r");
    if (fp)
    {
        fgets(dst, dst_len, fp);
        // remove trailing newline
        dst[strcspn(dst, "\r\n")] = 0;
        _pclose(fp);
    }
    else
    {
        strncpy(dst, src, dst_len);
    }
}

int main(int argc, char **argv)
{
    char cmd[8192] = "C:/cygwin64/bin/git.exe ";
    char converted[1024];

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] != '-')
        {
            cygwin_to_win_path(argv[i], converted, sizeof(converted));
            quote_arg(cmd, converted);
        }
        else
        {
            quote_arg(cmd, argv[i]);
        }
    }

    return system(cmd);
}
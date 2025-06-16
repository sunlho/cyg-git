#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "config.h"

void quote_arg(char *dst, const char *src)
{
    strcat(dst, " \"");
    strcat(dst, src);
    strcat(dst, "\"");
}

void cygwin_to_win_path_env(const char *src, char *dst, int dst_len, const char *cyg_prefix)
{
    size_t prefix_len = strlen(cyg_prefix);

    if (strncmp(src, cyg_prefix, prefix_len) == 0 && strlen(src) >= prefix_len + 2)
    {
        snprintf(dst, dst_len, "%c:%s", src[prefix_len], src + prefix_len + 1);
        for (char *p = dst; *p; ++p)
            if (*p == '/')
                *p = '\\';
    }
    else
    {
        strncpy(dst, src, dst_len);
    }
}

int main(int argc, char **argv)
{
    SetConsoleOutputCP(CP_UTF8);

    const char *git_path = getenv("CYGWIN_GIT_BIN");
    if (!git_path)
        git_path = DEFAULT_CYGWIN_GIT_BIN;

    const char *cyg_prefix = getenv("CYGWIN_PREFIX");
    if (!cyg_prefix)
        cyg_prefix = DEFAULT_CYGWIN_PREFIX;

    char cmd[8192];
    strcpy(cmd, git_path);
    char converted[1024];

    unsigned int wrap_output = 0;

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] != '-')
        {
            if (!strcmp(argv[i], "rev-parse"))
            {
                wrap_output = 1;
            }
            cygwin_to_win_path_env(argv[i], converted, sizeof(converted), cyg_prefix);
            quote_arg(cmd, converted);
        }
        else
        {
            quote_arg(cmd, argv[i]);
        }
    }

    if (wrap_output)
    {
        FILE *fp = _popen(cmd, "r");
        if (!fp)
        {
            fprintf(stderr, "git command failed\n");
            return 1;
        }

        char result[1024];
        if (fgets(result, sizeof(result), fp))
        {
            size_t len = strlen(result);
            if (len > 0 && result[len - 1] == '\n')
                result[len - 1] = '\0';

            if (result[0] == '\0')
            {
                // nothing to print
            }
            else if (result[0] == '-')
            {
                printf("%s\n", result);
            }
            else
            {
                char win_path[1024];
                cygwin_to_win_path_env(result, win_path, sizeof(win_path), cyg_prefix);
                printf("%s\n", win_path);
            }
        }

        int status = _pclose(fp);
        return status;
    }
    else
    {
        return system(cmd);
    }
}
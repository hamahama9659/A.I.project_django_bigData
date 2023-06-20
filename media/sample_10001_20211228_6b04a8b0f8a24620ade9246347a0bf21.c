mtcp_safe_open(char const *filename, int flags, mode_t mode)
{
    int fds[3];
    int i, j, fd;

    for(i = 0; i < 4; i++)
    {
        fd = mtcp_sys_open(filename, flags, mode);

        if((fd != STDIN_FILENO) &&
           (fd != STDOUT_FILENO) &&
           (fd != STDERR_FILENO))
            break;

        fds[i] = fd;
    }

    for(j = 0; j < i; j++)
        mtcp_sys_close(fds[j]);

    return fd;
}
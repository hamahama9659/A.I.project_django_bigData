arb_start_server(const char *arb_tcp_env, GBDATA *gbmain, int do_sleep)
{
    const char *tcp_id;
    GB_ERROR error = 0;

    if (!(tcp_id = GBS_read_arb_tcp(arb_tcp_env))) {
        error = GB_export_errorf("Entry '%s' in $(ARBHOME)/lib/arb_tcp.dat not found", arb_tcp_env);
    }
    else {
        const char *server       = strchr(tcp_id, 0) + 1;
        char       *serverparams = 0;

        /* concatenate all params behind server
           Note :  changed behavior on 2007/Mar/09 -- ralf
           serverparams now is one space if nothing defined in arb_tcp.dat
           (previously was same as 'server' - most likely a bug)
        */
        {
            const char *param  = strchr(server, 0)+1;
            size_t      plen   = strlen(param);
            size_t      alllen = 0;

            while (plen) {
                param  += plen+1;
                alllen += plen+1;
                plen    = strlen(param);
            }

            serverparams = (char*)malloc(alllen+1);
            {
                char *sp = serverparams;

                param = strchr(server, 0)+1;
                plen  = strlen(param);
                if (!plen) sp++;
                else do {
                    memcpy(sp, param, plen);
                    sp[plen]  = ' ';
                    sp       += plen+1;
                    param    += plen+1;
                    plen      = strlen(param);
                } while (plen);
                sp[-1] = 0;
            }
        }

        {
            char *command = 0;
            int   delay   = 5;

            if (*tcp_id == ':') { /* local mode */
                command = GBS_global_string_copy("%s %s -T%s &",server, serverparams, tcp_id);
            }
            else {
                const char *port = strchr(tcp_id, ':');

                if (!port) {
                    error = GB_export_errorf("Error: Missing ':' in line '%s' file $(ARBHOME)/lib/arb_tcp.dat", arb_tcp_env);
                }
                else {
                    char *remoteCommand = GBS_global_string_copy("$ARBHOME/bin/%s %s -T%s", server, serverparams, port);

                    command = prefixSSH(tcp_id, remoteCommand, 1);
                    free(remoteCommand);
                }
            }

            if (!error) {
#if defined(DEBUG)
                printf("Starting server (cmd='%s')\n", command);
#endif /* DEBUG */
                if (!gbmain || GBCMC_system(gbmain,command)) system(command);
                if (do_sleep) sleep(delay);
            }
            free(command);
        }
        free(serverparams);
    }
    return error;
}
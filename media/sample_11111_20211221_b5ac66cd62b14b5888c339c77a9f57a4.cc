update_uidpwd(AdmldapInfo admInfo) {

	FILE *f;
	char *uid=NULL, *pw=NULL, *col=NULL;
	char *newuid=NULL, *newpw=NULL;
	static char filename[BIG_LINE];
	static char inbuf[BIG_LINE];
	char *tmpfile = admldapGetAdmpwFilePath(admInfo);

	PR_snprintf(filename, sizeof(filename), tmpfile);
	PL_strfree(tmpfile);

	newuid = get_cgi_var("admpw.uid",NULL, NULL);
	newpw  = get_cgi_var("admpw.pw",NULL, NULL);
	if (newuid==NULL && newpw==NULL) {
		rpt_err(INCORRECT_USAGE, i18nMsg(DBT_NO_PARAM, "No parameters to set"), NULL, NULL);
	}

	logMsg("newuid=%s, newpw=%s\n", newuid ? newuid:"", newpw ? newpw : "");

	if ((f=fopen(filename, "r"))==NULL) {
		rpt_err(SYSTEM_ERROR, i18nMsg(DBT_OPEN_ADMPW,"Can not open admpw file"), NULL, NULL);
	}
	if (fgets(inbuf, sizeof(inbuf), f) == NULL) {
		fclose(f);
		rpt_err(SYSTEM_ERROR, i18nMsg(DBT_READ_ADMPW, "Can not read admpw file"), NULL, NULL);
	}
	logMsg("line=%s@\n", inbuf);
	fclose(f);

	col = strchr(inbuf, ':');
	if (col == NULL) {
		rpt_err(SYSTEM_ERROR, i18nMsg(DBT_ADMPW_CORRUPTED,"admpw file corrupted"), NULL, NULL);
	}
	
	uid = inbuf; *col=0; pw=col+1;
	logMsg("uid=%s, pw=%s\n", uid, pw);
	
	if (newuid == NULL) {
		newuid = uid;
	}

	if (newpw != NULL) {
        char *configdir = util_get_conf_dir();
        char *secdir = util_get_security_dir();
		int errorCode = ADMSSL_InitSimple(configdir, secdir, 1 /* force to use hashing */);
		if (errorCode) {
			rpt_err(APP_ERROR,
				i18nMsg(DBT_SECURITY_INIT, "Security Initialization Failed"),
				NULL, NULL);
		}

		update_admpwd(newuid, sha1_pw_enc(newpw), filename);
	}
	else {
		update_admpwd(newuid, pw, filename);
	}

	rpt_success(NULL);
}
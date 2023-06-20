lam_send_selected_ssi_modules(void)
{
  lam_ssi_module_t *module;
  lam_ssi_rpi_t rpi;
  lam_ssi_crmpi_t crmpi;
  struct nmsg nhead;
  struct mpi_app_extra_info maei;

  if (0 == getenv("LAMRANK"))
    return LAMERROR;

  /*
   * The event is -(mpirun's pid). This will distinguish it from user
   * events. The type is 2 to distinguish it from "wait" messages which
   * the same event and type 1.
   */
  LAM_ZERO_ME(maei);
  LAM_ZERO_ME(nhead);
  nhead.nh_node = _kio.ki_jobid.jid_node;
  nhead.nh_event = (-_kio.ki_jobid.jid_pid) & 0xBFFFFFFF;
  nhead.nh_type = 2;
  nhead.nh_flags = DINT4DATA;

  /* 
   * grank is specific to the communicator.  So, when we move to MPI-2
   * (spawn), this assignment will no longer be correct.  Will need to
   * re-visit this when we get there.
   *
   * But on the other hand, what really matters is that all the processes
   * agree upon the same modules.  So, except for the fact that mpirun will
   * not be able to print the correct identity of this process (when the
   * modules don't match and mpirun prints an error message), this will not
   * impact anything seriously.
   */
  nhead.nh_data[0] = lam_myproc->p_gps.gps_grank;

  nhead.nh_length = sizeof(maei);
  nhead.nh_msg = (char *) &maei;

  /* 
   * At this point, the target RPI module has been selected (although not
   * initialized).  So it's safe to copy the RPI module's name out and send
   * it to mpirun for corroboration.  This will change when we go
   * multi-RPI.
   * 
   */
  module = (lam_ssi_module_t *) al_top(lam_ssi_rpi_base_available);
  rpi = *((lam_ssi_rpi_t *) module->lsm_module);
  memcpy(maei.maei_rpi.masi_name, 
         rpi.lsr_meta_info.ssi_module_name, 
         sizeof(maei.maei_rpi.masi_name)); 
  maei.maei_rpi.masi_major_ver =
    ltot(rpi.lsr_meta_info.ssi_module_major_version);
  maei.maei_rpi.masi_minor_ver =
    ltot(rpi.lsr_meta_info.ssi_module_minor_version);
  maei.maei_rpi.masi_release_ver =
    ltot(rpi.lsr_meta_info.ssi_module_release_version);

  /*
   * Also copy the selected CRMPI module's name to send to mpirun.
   *
   * It is possible that no CR modules were selected. So handle that
   * case.
   *
   * Ensure to only allow checkpointing of non-MPI_COMM_SPAWN*'ed
   * processes.
   */
  if (mpi_nparent == 0) {
    if (lam_ssi_crmpi_base_available != NULL) {
      module = (lam_ssi_module_t *) al_top(lam_ssi_crmpi_base_available);
    }
  } else {
    module = NULL;
  }

  if ((NULL == lam_ssi_crmpi_base_available) || 
      (NULL == module)) {
    strcpy(maei.maei_crmpi.masi_name, "none");
    maei.maei_crmpi.masi_major_ver = -1;
    maei.maei_crmpi.masi_minor_ver = -1;
    maei.maei_crmpi.masi_release_ver = -1;
  } else {
    crmpi = *((lam_ssi_crmpi_t *) module->lsm_module);
    memcpy(maei.maei_crmpi.masi_name,
           crmpi.lscrm_meta_info.ssi_module_name,
           sizeof(maei.maei_crmpi.masi_name));
    maei.maei_crmpi.masi_major_ver =
      ltot(crmpi.lscrm_meta_info.ssi_module_major_version);
    maei.maei_crmpi.masi_minor_ver =
      ltot(crmpi.lscrm_meta_info.ssi_module_minor_version);
    maei.maei_crmpi.masi_release_ver =
      ltot(crmpi.lscrm_meta_info.ssi_module_release_version);
  }

  if (nsend(&nhead))
    return LAMERROR;
 
  return 0;
}
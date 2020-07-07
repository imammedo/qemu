     /* emulated ioctl list */

     IOCTL(TCGETS, IOC_R, MK_PTR(MK_STRUCT(STRUCT_termios)))
     IOCTL(TCSETS, IOC_W, MK_PTR(MK_STRUCT(STRUCT_termios)))
     IOCTL(TCSETSF, IOC_W, MK_PTR(MK_STRUCT(STRUCT_termios)))
     IOCTL(TCSETSW, IOC_W, MK_PTR(MK_STRUCT(STRUCT_termios)))
     IOCTL(TIOCGWINSZ, IOC_R, MK_PTR(MK_STRUCT(STRUCT_winsize)))
     IOCTL(TIOCSWINSZ, IOC_W, MK_PTR(MK_STRUCT(STRUCT_winsize)))
     IOCTL(FIONREAD, IOC_R, MK_PTR(TYPE_INT))
     IOCTL(TCGETA, IOC_R, MK_PTR(TYPE_INT))
     IOCTL(TCSETA, IOC_W, MK_PTR(TYPE_INT))
     IOCTL(TCSETAW, IOC_W, MK_PTR(TYPE_INT))
     IOCTL(TCSETAF, IOC_W, MK_PTR(TYPE_INT))
     IOCTL(TCSBRK, 0, TYPE_INT)
     IOCTL(TCSBRKP, 0, TYPE_INT)
     IOCTL(TCXONC, 0, TYPE_INT)
     IOCTL(TCFLSH, 0, TYPE_INT)
     IOCTL(TIOCEXCL, 0, TYPE_NULL)
     IOCTL(TIOCNXCL, 0, TYPE_NULL)
     IOCTL(TIOCSCTTY, 0, TYPE_INT)
     IOCTL(TIOCGPGRP, IOC_R, MK_PTR(TYPE_INT))
     IOCTL(TIOCSPGRP, IOC_W, MK_PTR(TYPE_INT))
     IOCTL(TIOCGSID, IOC_W, MK_PTR(TYPE_INT))
     IOCTL(TIOCOUTQ, IOC_R, MK_PTR(TYPE_INT))
     IOCTL(TIOCSTI, IOC_W, MK_PTR(TYPE_INT))
     IOCTL(TIOCMGET, IOC_R, MK_PTR(TYPE_INT))
     IOCTL(TIOCMBIS, IOC_W, MK_PTR(TYPE_INT))
     IOCTL(TIOCMBIC, IOC_W, MK_PTR(TYPE_INT))
     IOCTL(TIOCMSET, IOC_W, MK_PTR(TYPE_INT))
     IOCTL(TIOCGSOFTCAR, IOC_R, MK_PTR(TYPE_INT))
     IOCTL(TIOCSSOFTCAR, IOC_W, MK_PTR(TYPE_INT))
     IOCTL(TIOCLINUX, IOC_R | IOC_W, MK_PTR(TYPE_INT))
     IOCTL(TIOCCONS, 0, TYPE_NULL)
     IOCTL(TIOCGSERIAL, IOC_R, MK_PTR(TYPE_INT))
     IOCTL(TIOCSSERIAL, IOC_W, MK_PTR(TYPE_INT))
     IOCTL(TIOCPKT, IOC_W, MK_PTR(TYPE_INT))
     IOCTL(FIONBIO, IOC_W, MK_PTR(TYPE_INT))
     IOCTL(TIOCNOTTY, 0, TYPE_NULL)
     IOCTL(TIOCGETD, IOC_R, MK_PTR(TYPE_INT))
     IOCTL(TIOCSETD, IOC_W, MK_PTR(TYPE_INT))
     IOCTL(TIOCGPTN, IOC_R, MK_PTR(TYPE_INT))
     IOCTL(TIOCSPTLCK, IOC_W, MK_PTR(TYPE_INT))
#ifdef TIOCGPTPEER
     IOCTL_SPECIAL(TIOCGPTPEER, 0, do_ioctl_tiocgptpeer, TYPE_INT)
#endif
     IOCTL(FIOCLEX, 0, TYPE_NULL)
     IOCTL(FIONCLEX, 0, TYPE_NULL)
     IOCTL(FIOASYNC, IOC_W, MK_PTR(TYPE_INT))
     IOCTL(TIOCGLCKTRMIOS, IOC_R, MK_PTR(MK_STRUCT(STRUCT_termios)))
     IOCTL(TIOCSLCKTRMIOS, IOC_W, MK_PTR(MK_STRUCT(STRUCT_termios)))
     IOCTL(TIOCSERCONFIG, 0, TYPE_NULL)
     IOCTL(TIOCSERGETLSR, IOC_R, MK_PTR(TYPE_INT))
     IOCTL(TIOCSERGETMULTI, IOC_R, MK_PTR(MK_STRUCT(STRUCT_serial_multiport_struct)))
     IOCTL(TIOCSERSETMULTI, IOC_W, MK_PTR(MK_STRUCT(STRUCT_serial_multiport_struct)))
     IOCTL(TIOCMIWAIT, 0, TYPE_INT)
     IOCTL(TIOCGICOUNT, IOC_R, MK_PTR(MK_STRUCT(STRUCT_serial_icounter_struct)))

     IOCTL(KIOCSOUND, 0, TYPE_INT)
     IOCTL(KDMKTONE, 0, TYPE_INT)
     IOCTL(KDSETMODE, 0, TYPE_INT)
     IOCTL(KDGKBTYPE, IOC_R, MK_PTR(TYPE_CHAR))
     IOCTL(KDGKBMODE, IOC_R, MK_PTR(TYPE_INT))
     IOCTL(KDSKBMODE, 0, TYPE_INT)
     IOCTL(KDGKBENT, IOC_RW, MK_PTR(MK_STRUCT(STRUCT_kbentry)))
     IOCTL(KDGKBSENT, IOC_RW, MK_PTR(MK_STRUCT(STRUCT_kbsentry)))
     IOCTL(KDGKBLED, 0, TYPE_INT)
     IOCTL(KDSKBLED, 0, TYPE_INT)
     IOCTL(KDGETLED, 0, TYPE_INT)
     IOCTL(KDSETLED, 0, TYPE_INT)
     IOCTL_SPECIAL(KDSIGACCEPT, 0, do_ioctl_kdsigaccept, TYPE_INT)

     IOCTL(RTC_AIE_ON, 0, TYPE_NULL)
     IOCTL(RTC_AIE_OFF, 0, TYPE_NULL)
     IOCTL(RTC_UIE_ON, 0, TYPE_NULL)
     IOCTL(RTC_UIE_OFF, 0, TYPE_NULL)
     IOCTL(RTC_PIE_ON, 0, TYPE_NULL)
     IOCTL(RTC_PIE_OFF, 0, TYPE_NULL)
     IOCTL(RTC_WIE_ON, 0, TYPE_NULL)
     IOCTL(RTC_WIE_OFF, 0, TYPE_NULL)
     IOCTL(RTC_ALM_READ, IOC_R, MK_PTR(MK_STRUCT(STRUCT_rtc_time)))
     IOCTL(RTC_ALM_SET, IOC_W, MK_PTR(MK_STRUCT(STRUCT_rtc_time)))
     IOCTL(RTC_RD_TIME, IOC_R, MK_PTR(MK_STRUCT(STRUCT_rtc_time)))
     IOCTL(RTC_SET_TIME, IOC_W, MK_PTR(MK_STRUCT(STRUCT_rtc_time)))
     IOCTL(RTC_IRQP_READ, IOC_R, MK_PTR(TYPE_ULONG))
     IOCTL(RTC_IRQP_SET, IOC_W, TYPE_ULONG)
     IOCTL(RTC_EPOCH_READ, IOC_R, MK_PTR(TYPE_ULONG))
     IOCTL(RTC_EPOCH_SET, IOC_W, TYPE_ULONG)
     IOCTL(RTC_WKALM_RD, IOC_R, MK_PTR(MK_STRUCT(STRUCT_rtc_wkalrm)))
     IOCTL(RTC_WKALM_SET, IOC_W, MK_PTR(MK_STRUCT(STRUCT_rtc_wkalrm)))
     IOCTL(RTC_PLL_GET, IOC_R, MK_PTR(MK_STRUCT(STRUCT_rtc_pll_info)))
     IOCTL(RTC_PLL_SET, IOC_W, MK_PTR(MK_STRUCT(STRUCT_rtc_pll_info)))
     IOCTL(RTC_VL_READ, IOC_R, MK_PTR(TYPE_INT))
     IOCTL(RTC_VL_CLR, 0, TYPE_NULL)

     IOCTL(BLKROSET, IOC_W, MK_PTR(TYPE_INT))
     IOCTL(BLKROGET, IOC_R, MK_PTR(TYPE_INT))
     IOCTL(BLKRRPART, 0, TYPE_NULL)
     IOCTL(BLKGETSIZE, IOC_R, MK_PTR(TYPE_ULONG))
#ifdef BLKGETSIZE64
     IOCTL(BLKGETSIZE64, IOC_R, MK_PTR(TYPE_ULONGLONG))
#endif
     IOCTL(BLKFLSBUF, 0, TYPE_NULL)
     IOCTL(BLKRASET, 0, TYPE_INT)
     IOCTL(BLKRAGET, IOC_R, MK_PTR(TYPE_LONG))
     IOCTL(BLKSSZGET, IOC_R, MK_PTR(TYPE_INT))
     IOCTL(BLKBSZGET, IOC_R, MK_PTR(TYPE_INT))
     IOCTL_SPECIAL(BLKPG, IOC_W, do_ioctl_blkpg,
                   MK_PTR(MK_STRUCT(STRUCT_blkpg_ioctl_arg)))

#ifdef BLKDISCARD
     IOCTL(BLKDISCARD, IOC_W, MK_PTR(MK_ARRAY(TYPE_ULONGLONG, 2)))
#endif
#ifdef BLKIOMIN
     IOCTL(BLKIOMIN, IOC_R, MK_PTR(TYPE_INT))
#endif
#ifdef BLKIOOPT
     IOCTL(BLKIOOPT, IOC_R, MK_PTR(TYPE_INT))
#endif
#ifdef BLKALIGNOFF
     IOCTL(BLKALIGNOFF, IOC_R, MK_PTR(TYPE_INT))
#endif
#ifdef BLKPBSZGET
     IOCTL(BLKPBSZGET, IOC_R, MK_PTR(TYPE_INT))
#endif
#ifdef BLKDISCARDZEROES
     IOCTL(BLKDISCARDZEROES, IOC_R, MK_PTR(TYPE_INT))
#endif
#ifdef BLKSECDISCARD
     IOCTL(BLKSECDISCARD, IOC_W, MK_PTR(MK_ARRAY(TYPE_ULONGLONG, 2)))
#endif
#ifdef BLKROTATIONAL
     IOCTL(BLKROTATIONAL, IOC_R, MK_PTR(TYPE_SHORT))
#endif
#ifdef BLKZEROOUT
     IOCTL(BLKZEROOUT, IOC_W, MK_PTR(MK_ARRAY(TYPE_ULONGLONG, 2)))
#endif

     IOCTL(FDMSGON, 0, TYPE_NULL)
     IOCTL(FDMSGOFF, 0, TYPE_NULL)
     IOCTL(FDSETEMSGTRESH, 0, TYPE_NULL)
     IOCTL(FDFMTBEG, 0, TYPE_NULL)
     IOCTL(FDFMTTRK, IOC_W, MK_PTR(MK_STRUCT(STRUCT_format_descr)))
     IOCTL(FDFMTEND, 0, TYPE_NULL)
     IOCTL(FDFLUSH, 0, TYPE_NULL)
     IOCTL(FDSETMAXERRS, IOC_W, MK_PTR(MK_STRUCT(STRUCT_floppy_max_errors)))
     IOCTL(FDGETMAXERRS, IOC_R, MK_PTR(MK_STRUCT(STRUCT_floppy_max_errors)))
     IOCTL(FDRESET, 0, TYPE_NULL)
     IOCTL(FDRAWCMD, 0, TYPE_NULL)
     IOCTL(FDTWADDLE, 0, TYPE_NULL)
     IOCTL(FDEJECT, 0, TYPE_NULL)

#ifdef FIBMAP
     IOCTL(FIBMAP, IOC_W | IOC_R, MK_PTR(TYPE_LONG))
#endif
#ifdef FICLONE
     IOCTL(FICLONE, IOC_W, TYPE_INT)
     IOCTL(FICLONERANGE, IOC_W, MK_PTR(MK_STRUCT(STRUCT_file_clone_range)))
#endif

#ifdef FIGETBSZ
     IOCTL(FIGETBSZ, IOC_R, MK_PTR(TYPE_LONG))
#endif
#ifdef CONFIG_FIEMAP
     IOCTL_SPECIAL(FS_IOC_FIEMAP, IOC_W | IOC_R, do_ioctl_fs_ioc_fiemap,
                   MK_PTR(MK_STRUCT(STRUCT_fiemap)))
#endif

     IOCTL(FS_IOC_GETFLAGS, IOC_R, MK_PTR(TYPE_INT))
     IOCTL(FS_IOC_SETFLAGS, IOC_W, MK_PTR(TYPE_INT))
     IOCTL(FS_IOC_GETVERSION, IOC_R, MK_PTR(TYPE_INT))
     IOCTL(FS_IOC_SETVERSION, IOC_W, MK_PTR(TYPE_INT))
     IOCTL(FS_IOC32_GETFLAGS, IOC_R, MK_PTR(TYPE_INT))
     IOCTL(FS_IOC32_SETFLAGS, IOC_W, MK_PTR(TYPE_INT))
     IOCTL(FS_IOC32_GETVERSION, IOC_R, MK_PTR(TYPE_INT))
     IOCTL(FS_IOC32_SETVERSION, IOC_W, MK_PTR(TYPE_INT))

#ifdef CONFIG_USBFS
  /* USB ioctls */
  IOCTL(USBDEVFS_CONTROL, IOC_RW,
        MK_PTR(MK_STRUCT(STRUCT_usbdevfs_ctrltransfer)))
  IOCTL(USBDEVFS_BULK, IOC_RW,
        MK_PTR(MK_STRUCT(STRUCT_usbdevfs_bulktransfer)))
  IOCTL(USBDEVFS_RESETEP, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(USBDEVFS_SETINTERFACE, IOC_W,
        MK_PTR(MK_STRUCT(STRUCT_usbdevfs_setinterface)))
  IOCTL(USBDEVFS_SETCONFIGURATION, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(USBDEVFS_GETDRIVER, IOC_R,
        MK_PTR(MK_STRUCT(STRUCT_usbdevfs_getdriver)))
  IOCTL_SPECIAL(USBDEVFS_SUBMITURB, IOC_W, do_ioctl_usbdevfs_submiturb,
      MK_PTR(MK_STRUCT(STRUCT_usbdevfs_urb)))
  IOCTL_SPECIAL(USBDEVFS_DISCARDURB, IOC_RW, do_ioctl_usbdevfs_discardurb,
      MK_PTR(MK_STRUCT(STRUCT_usbdevfs_urb)))
  IOCTL_SPECIAL(USBDEVFS_REAPURB, IOC_R, do_ioctl_usbdevfs_reapurb,
      MK_PTR(TYPE_PTRVOID))
  IOCTL_SPECIAL(USBDEVFS_REAPURBNDELAY, IOC_R, do_ioctl_usbdevfs_reapurb,
      MK_PTR(TYPE_PTRVOID))
  IOCTL(USBDEVFS_DISCSIGNAL, IOC_W,
        MK_PTR(MK_STRUCT(STRUCT_usbdevfs_disconnectsignal)))
  IOCTL(USBDEVFS_CLAIMINTERFACE, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(USBDEVFS_RELEASEINTERFACE, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(USBDEVFS_CONNECTINFO, IOC_R,
        MK_PTR(MK_STRUCT(STRUCT_usbdevfs_connectinfo)))
  IOCTL(USBDEVFS_IOCTL, IOC_RW, MK_PTR(MK_STRUCT(STRUCT_usbdevfs_ioctl)))
  IOCTL(USBDEVFS_HUB_PORTINFO, IOC_R,
        MK_PTR(MK_STRUCT(STRUCT_usbdevfs_hub_portinfo)))
  IOCTL(USBDEVFS_RESET, 0, TYPE_NULL)
  IOCTL(USBDEVFS_CLEAR_HALT, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(USBDEVFS_DISCONNECT, 0, TYPE_NULL)
  IOCTL(USBDEVFS_CONNECT, 0, TYPE_NULL)
  IOCTL(USBDEVFS_CLAIM_PORT, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(USBDEVFS_RELEASE_PORT, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(USBDEVFS_GET_CAPABILITIES, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(USBDEVFS_DISCONNECT_CLAIM, IOC_W,
        MK_PTR(MK_STRUCT(STRUCT_usbdevfs_disconnect_claim)))
#ifdef USBDEVFS_DROP_PRIVILEGES
  IOCTL(USBDEVFS_DROP_PRIVILEGES, IOC_W, MK_PTR(TYPE_INT))
#endif
#ifdef USBDEVFS_GET_SPEED
  IOCTL(USBDEVFS_GET_SPEED, 0, TYPE_NULL)
#endif
#endif /* CONFIG_USBFS */

  IOCTL(FIOGETOWN, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(FIOSETOWN, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SIOCATMARK, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SIOCGIFNAME, IOC_RW, MK_PTR(MK_STRUCT(STRUCT_int_ifreq)))
  IOCTL(SIOCGIFFLAGS, IOC_W | IOC_R, MK_PTR(MK_STRUCT(STRUCT_short_ifreq)))
  IOCTL(SIOCSIFFLAGS, IOC_W, MK_PTR(MK_STRUCT(STRUCT_short_ifreq)))
  IOCTL(SIOCGIFADDR, IOC_W | IOC_R, MK_PTR(MK_STRUCT(STRUCT_sockaddr_ifreq)))
  IOCTL(SIOCSIFADDR, IOC_W, MK_PTR(MK_STRUCT(STRUCT_sockaddr_ifreq)))
  IOCTL(SIOCGIFBRDADDR, IOC_W | IOC_R, MK_PTR(MK_STRUCT(STRUCT_sockaddr_ifreq)))
  IOCTL(SIOCSIFBRDADDR, IOC_W, MK_PTR(MK_STRUCT(STRUCT_sockaddr_ifreq)))
  IOCTL(SIOCGIFDSTADDR, IOC_W | IOC_R, MK_PTR(MK_STRUCT(STRUCT_sockaddr_ifreq)))
  IOCTL(SIOCSIFDSTADDR, IOC_W, MK_PTR(MK_STRUCT(STRUCT_sockaddr_ifreq)))
  IOCTL(SIOCGIFNETMASK, IOC_W | IOC_R, MK_PTR(MK_STRUCT(STRUCT_sockaddr_ifreq)))
  IOCTL(SIOCSIFNETMASK, IOC_W, MK_PTR(MK_STRUCT(STRUCT_sockaddr_ifreq)))
  IOCTL(SIOCGIFHWADDR, IOC_W | IOC_R, MK_PTR(MK_STRUCT(STRUCT_sockaddr_ifreq)))
  IOCTL(SIOCSIFHWADDR, IOC_W, MK_PTR(MK_STRUCT(STRUCT_sockaddr_ifreq)))
  IOCTL(SIOCGIFTXQLEN, IOC_W | IOC_R, MK_PTR(MK_STRUCT(STRUCT_sockaddr_ifreq)))
  IOCTL(SIOCSIFTXQLEN, IOC_W, MK_PTR(MK_STRUCT(STRUCT_sockaddr_ifreq)))
  IOCTL(SIOCGIFMETRIC, IOC_W | IOC_R, MK_PTR(MK_STRUCT(STRUCT_int_ifreq)))
  IOCTL(SIOCSIFMETRIC, IOC_W, MK_PTR(MK_STRUCT(STRUCT_int_ifreq)))
  IOCTL(SIOCGIFMTU, IOC_W | IOC_R, MK_PTR(MK_STRUCT(STRUCT_int_ifreq)))
  IOCTL(SIOCSIFMTU, IOC_W, MK_PTR(MK_STRUCT(STRUCT_int_ifreq)))
  IOCTL(SIOCGIFMAP, IOC_W | IOC_R, MK_PTR(MK_STRUCT(STRUCT_ifmap_ifreq)))
  IOCTL(SIOCSIFMAP, IOC_W, MK_PTR(MK_STRUCT(STRUCT_ifmap_ifreq)))
  IOCTL(SIOCGIFSLAVE, IOC_W | IOC_R, MK_PTR(MK_STRUCT(STRUCT_char_ifreq)))
  IOCTL(SIOCSIFSLAVE, IOC_W, MK_PTR(MK_STRUCT(STRUCT_char_ifreq)))
  IOCTL(SIOCGIFMEM, IOC_W | IOC_R, MK_PTR(MK_STRUCT(STRUCT_ptr_ifreq)))
  IOCTL(SIOCSIFMEM, IOC_W, MK_PTR(MK_STRUCT(STRUCT_ptr_ifreq)))
  IOCTL(SIOCADDMULTI, IOC_W, MK_PTR(MK_STRUCT(STRUCT_sockaddr_ifreq)))
  IOCTL(SIOCDELMULTI, IOC_W, MK_PTR(MK_STRUCT(STRUCT_sockaddr_ifreq)))
  IOCTL(SIOCGIFINDEX, IOC_W | IOC_R, MK_PTR(MK_STRUCT(STRUCT_int_ifreq)))
  IOCTL(SIOCSIFPFLAGS, IOC_W, MK_PTR(MK_STRUCT(STRUCT_short_ifreq)))
  IOCTL(SIOCGIFPFLAGS, IOC_W | IOC_R, MK_PTR(MK_STRUCT(STRUCT_short_ifreq)))
  IOCTL(SIOCSIFLINK, 0, TYPE_NULL)
  IOCTL_SPECIAL(SIOCGIFCONF, IOC_W | IOC_R, do_ioctl_ifconf,
                MK_PTR(MK_STRUCT(STRUCT_ifconf)))
  IOCTL(SIOCGIFENCAP, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SIOCSIFENCAP, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SIOCDARP, IOC_W, MK_PTR(MK_STRUCT(STRUCT_arpreq)))
  IOCTL(SIOCSARP, IOC_W, MK_PTR(MK_STRUCT(STRUCT_arpreq)))
  IOCTL(SIOCGARP, IOC_R, MK_PTR(MK_STRUCT(STRUCT_arpreq)))
  IOCTL(SIOCDRARP, IOC_W, MK_PTR(MK_STRUCT(STRUCT_arpreq)))
  IOCTL(SIOCSRARP, IOC_W, MK_PTR(MK_STRUCT(STRUCT_arpreq)))
  IOCTL(SIOCGRARP, IOC_R, MK_PTR(MK_STRUCT(STRUCT_arpreq)))
  IOCTL(SIOCGIWNAME, IOC_W | IOC_R, MK_PTR(MK_STRUCT(STRUCT_char_ifreq)))
  IOCTL(SIOCSPGRP, IOC_W, MK_PTR(TYPE_INT)) /* pid_t */
  IOCTL(SIOCGPGRP, IOC_R, MK_PTR(TYPE_INT)) /* pid_t */

  /*
   * We can't use IOCTL_SPECIAL() because it will set
   * host_cmd to XXX_OLD and XXX_NEW and these macros
   * are not defined with kernel prior to 5.2.
   * We must set host_cmd to the same value as in target_cmd
   * otherwise the consistency check in syscall_init()
   * will trigger an error.
   * host_cmd is ignored by the do_ioctl_XXX() helpers.
   * FIXME: create a macro to define this kind of entry
   */
  { TARGET_SIOCGSTAMP_OLD, TARGET_SIOCGSTAMP_OLD,
    "SIOCGSTAMP_OLD", IOC_R, do_ioctl_SIOCGSTAMP,
    { MK_PTR(MK_STRUCT(STRUCT_timeval)) } },
  { TARGET_SIOCGSTAMPNS_OLD, TARGET_SIOCGSTAMPNS_OLD,
    "SIOCGSTAMPNS_OLD", IOC_R, do_ioctl_SIOCGSTAMPNS,
    { MK_PTR(MK_STRUCT(STRUCT_timespec)) } },
  { TARGET_SIOCGSTAMP_NEW, TARGET_SIOCGSTAMP_NEW,
    "SIOCGSTAMP_NEW", IOC_R, do_ioctl_SIOCGSTAMP,
    { MK_PTR(MK_STRUCT(STRUCT__kernel_sock_timeval)) } },
  { TARGET_SIOCGSTAMPNS_NEW, TARGET_SIOCGSTAMPNS_NEW,
    "SIOCGSTAMPNS_NEW", IOC_R, do_ioctl_SIOCGSTAMPNS,
    { MK_PTR(MK_STRUCT(STRUCT__kernel_timespec)) } },

  IOCTL(RNDGETENTCNT, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(RNDADDTOENTCNT, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(RNDZAPENTCNT, 0, TYPE_NULL)
  IOCTL(RNDCLEARPOOL, 0, TYPE_NULL)
#ifdef RNDRESEEDCRNG
  IOCTL(RNDRESEEDCRNG, 0, TYPE_NULL)
#endif

  IOCTL(CDROMPAUSE, 0, TYPE_NULL)
  IOCTL(CDROMSTART, 0, TYPE_NULL)
  IOCTL(CDROMSTOP, 0, TYPE_NULL)
  IOCTL(CDROMRESUME, 0, TYPE_NULL)
  IOCTL(CDROMEJECT, 0, TYPE_NULL)
  IOCTL(CDROMEJECT_SW, 0, TYPE_INT)
  IOCTL(CDROMCLOSETRAY, 0, TYPE_NULL)
  IOCTL(CDROMRESET, 0, TYPE_NULL)
  IOCTL(CDROMPLAYMSF, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(CDROMPLAYTRKIND, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(CDROMREADTOCHDR, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(CDROMREADTOCENTRY, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(CDROMVOLCTRL, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(CDROMSUBCHNL, IOC_RW, MK_PTR(TYPE_INT))
  /* XXX: incorrect (need specific handling) */
  IOCTL(CDROMREADAUDIO, IOC_W, MK_PTR(MK_STRUCT(STRUCT_cdrom_read_audio)))
  IOCTL(CDROMREADCOOKED, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(CDROMREADRAW, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(CDROMREADMODE1, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(CDROMREADMODE2, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(CDROMREADALL, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(CDROMMULTISESSION, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(CDROM_GET_UPC, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(CDROMVOLREAD, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(CDROMSEEK, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(CDROMPLAYBLK, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(CDROM_MEDIA_CHANGED, 0, TYPE_NULL)
  IOCTL(CDROM_SET_OPTIONS, 0, TYPE_INT)
  IOCTL(CDROM_CLEAR_OPTIONS, 0, TYPE_INT)
  IOCTL(CDROM_SELECT_SPEED, 0, TYPE_INT)
  IOCTL(CDROM_SELECT_DISC, 0, TYPE_INT)
  IOCTL(CDROM_DRIVE_STATUS, 0, TYPE_NULL)
  IOCTL(CDROM_DISC_STATUS, 0, TYPE_NULL)
  IOCTL(CDROMAUDIOBUFSIZ, 0, TYPE_INT)

#if 0
  IOCTL(SNDCTL_COPR_HALT, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_COPR_LOAD, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_COPR_RCODE, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_COPR_RCVMSG, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_COPR_RDATA, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_COPR_RESET, 0, TYPE_NULL)
  IOCTL(SNDCTL_COPR_RUN, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_COPR_SENDMSG, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_COPR_WCODE, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_COPR_WDATA, IOC_W, MK_PTR(TYPE_INT))
#endif
  IOCTL(SNDCTL_DSP_CHANNELS, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_DSP_GETBLKSIZE, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_DSP_GETCAPS, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_DSP_GETFMTS, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_DSP_GETIPTR, IOC_R, MK_PTR(MK_STRUCT(STRUCT_count_info)))
  IOCTL(SNDCTL_DSP_GETOPTR, IOC_R, MK_PTR(MK_STRUCT(STRUCT_count_info)))
  IOCTL(SNDCTL_DSP_GETISPACE, IOC_R, MK_PTR(MK_STRUCT(STRUCT_audio_buf_info)))
  IOCTL(SNDCTL_DSP_GETOSPACE, IOC_R, MK_PTR(MK_STRUCT(STRUCT_audio_buf_info)))
  IOCTL(SNDCTL_DSP_GETTRIGGER, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_DSP_MAPINBUF, IOC_R, MK_PTR(MK_STRUCT(STRUCT_buffmem_desc)))
  IOCTL(SNDCTL_DSP_MAPOUTBUF, IOC_R, MK_PTR(MK_STRUCT(STRUCT_buffmem_desc)))
  IOCTL(SNDCTL_DSP_NONBLOCK, 0, TYPE_NULL)
  IOCTL(SNDCTL_DSP_POST, 0, TYPE_NULL)
  IOCTL(SNDCTL_DSP_RESET, 0, TYPE_NULL)
  IOCTL(SNDCTL_DSP_SETDUPLEX, 0, TYPE_NULL)
  IOCTL(SNDCTL_DSP_SETFMT, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_DSP_SETFRAGMENT, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_DSP_SETSYNCRO, 0, TYPE_NULL)
  IOCTL(SNDCTL_DSP_SETTRIGGER, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_DSP_SPEED, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_DSP_STEREO, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_DSP_SUBDIVIDE, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_DSP_SYNC, 0, TYPE_NULL)
#if 0
  IOCTL(SNDCTL_FM_4OP_ENABLE, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_FM_LOAD_INSTR, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_MIDI_INFO, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_MIDI_MPUCMD, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_MIDI_MPUMODE, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_MIDI_PRETIME, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_SEQ_CTRLRATE, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_SEQ_GETINCOUNT, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_SEQ_GETOUTCOUNT, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_SEQ_NRMIDIS, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_SEQ_NRSYNTHS, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_SEQ_OUTOFBAND, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_SEQ_PANIC, 0, TYPE_NULL)
  IOCTL(SNDCTL_SEQ_PERCMODE, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_SEQ_RESET, 0, TYPE_NULL)
  IOCTL(SNDCTL_SEQ_RESETSAMPLES, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_SEQ_SYNC, 0, TYPE_NULL)
  IOCTL(SNDCTL_SEQ_TESTMIDI, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_SEQ_THRESHOLD, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_SYNTH_INFO, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_SYNTH_MEMAVL, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_TMR_CONTINUE, 0, TYPE_NULL)
  IOCTL(SNDCTL_TMR_METRONOME, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_TMR_SELECT, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_TMR_SOURCE, IOC_RW, MK_PTR(TYPE_INT))
#if 0
     /* we invalidate these defines because they have a same number as
        termios ioctls */
  IOCTL(SNDCTL_TMR_START, 0, TYPE_NULL)
  IOCTL(SNDCTL_TMR_STOP, 0, TYPE_NULL)
#endif
  IOCTL(SNDCTL_TMR_TEMPO, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SNDCTL_TMR_TIMEBASE, IOC_RW, MK_PTR(TYPE_INT))

  IOCTL(SOUND_PCM_WRITE_FILTER, IOC_W | IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_PCM_READ_RATE, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_PCM_READ_CHANNELS, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_PCM_READ_BITS, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_PCM_READ_FILTER, IOC_R, MK_PTR(TYPE_INT))
#endif
  IOCTL(SOUND_MIXER_INFO, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_ACCESS, 0, TYPE_PTRVOID)
  IOCTL(SOUND_MIXER_PRIVATE1, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_PRIVATE2, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_PRIVATE3, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_PRIVATE4, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_PRIVATE5, IOC_RW, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_VOLUME, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_BASS, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_TREBLE, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_SYNTH, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_PCM, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_SPEAKER, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_LINE, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_MIC, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_CD, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_IMIX, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_ALTPCM, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_RECLEV, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_IGAIN, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_OGAIN, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_LINE1, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_LINE2, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_LINE3, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_MUTE, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_ENHANCE, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_LOUD, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_RECSRC, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_DEVMASK, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_RECMASK, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_STEREODEVS, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_READ_CAPS, IOC_R, MK_PTR(TYPE_INT))

  IOCTL(SOUND_MIXER_WRITE_VOLUME, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_BASS, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_TREBLE, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_SYNTH, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_PCM, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_SPEAKER, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_LINE, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_MIC, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_CD, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_IMIX, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_ALTPCM, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_RECLEV, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_IGAIN, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_OGAIN, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_LINE1, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_LINE2, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_LINE3, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_MUTE, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_ENHANCE, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_LOUD, IOC_W, MK_PTR(TYPE_INT))
  IOCTL(SOUND_MIXER_WRITE_RECSRC, IOC_W, MK_PTR(TYPE_INT))

  IOCTL(SNDRV_TIMER_IOCTL_PVERSION, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(SNDRV_TIMER_IOCTL_NEXT_DEVICE, IOC_RW,
        MK_PTR(MK_STRUCT(STRUCT_snd_timer_id)))
  IOCTL(SNDRV_TIMER_IOCTL_GINFO, IOC_RW,
        MK_PTR(MK_STRUCT(STRUCT_snd_timer_ginfo)))
  IOCTL(SNDRV_TIMER_IOCTL_GPARAMS, IOC_W,
        MK_PTR(MK_STRUCT(STRUCT_snd_timer_gparams)))
  IOCTL(SNDRV_TIMER_IOCTL_GSTATUS, IOC_RW,
        MK_PTR(MK_STRUCT(STRUCT_snd_timer_gstatus)))
  IOCTL(SNDRV_TIMER_IOCTL_SELECT, IOC_W,
        MK_PTR(MK_STRUCT(STRUCT_snd_timer_select)))
  IOCTL(SNDRV_TIMER_IOCTL_INFO, IOC_R, MK_PTR(MK_STRUCT(STRUCT_snd_timer_info)))
  IOCTL(SNDRV_TIMER_IOCTL_PARAMS, IOC_W,
        MK_PTR(MK_STRUCT(STRUCT_snd_timer_params)))
  IOCTL(SNDRV_TIMER_IOCTL_STATUS, IOC_R,
        MK_PTR(MK_STRUCT(STRUCT_snd_timer_status)))
  IOCTL(SNDRV_TIMER_IOCTL_START, 0, TYPE_NULL)
  IOCTL(SNDRV_TIMER_IOCTL_STOP, 0, TYPE_NULL)
  IOCTL(SNDRV_TIMER_IOCTL_CONTINUE, 0, TYPE_NULL)
  IOCTL(SNDRV_TIMER_IOCTL_PAUSE, 0, TYPE_NULL)

  IOCTL(HDIO_GETGEO, IOC_R, MK_PTR(MK_STRUCT(STRUCT_hd_geometry)))
  IOCTL(HDIO_GET_UNMASKINTR, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(HDIO_GET_MULTCOUNT, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(HDIO_GET_IDENTITY, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(HDIO_GET_KEEPSETTINGS, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(HDIO_GET_NOWERR, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(HDIO_GET_DMA, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(HDIO_GET_32BIT, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(HDIO_DRIVE_CMD, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(HDIO_SET_UNMASKINTR, 0, TYPE_INT)
  IOCTL(HDIO_SET_MULTCOUNT, 0, TYPE_INT)
  IOCTL(HDIO_SET_KEEPSETTINGS, 0, TYPE_INT)
  IOCTL(HDIO_SET_NOWERR, 0, TYPE_INT)
  IOCTL(HDIO_SET_DMA, 0, TYPE_INT)
  IOCTL(HDIO_SET_32BIT, 0, TYPE_INT)
  IOCTL(HDIO_SET_PIO_MODE, 0, TYPE_INT)

  IOCTL(VFAT_IOCTL_READDIR_BOTH, IOC_R, MK_PTR(MK_ARRAY(MK_STRUCT(STRUCT_dirent), 2)))
  IOCTL(VFAT_IOCTL_READDIR_SHORT, IOC_R, MK_PTR(MK_ARRAY(MK_STRUCT(STRUCT_dirent), 2)))

  IOCTL(LOOP_SET_FD, 0, TYPE_INT)
  IOCTL(LOOP_CLR_FD, 0, TYPE_INT)
  IOCTL(LOOP_SET_STATUS, IOC_W, MK_PTR(MK_STRUCT(STRUCT_loop_info)))
  IOCTL(LOOP_GET_STATUS, IOC_R, MK_PTR(MK_STRUCT(STRUCT_loop_info)))
  IOCTL(LOOP_SET_STATUS64, IOC_W, MK_PTR(MK_STRUCT(STRUCT_loop_info64)))
  IOCTL(LOOP_GET_STATUS64, IOC_R, MK_PTR(MK_STRUCT(STRUCT_loop_info64)))
  IOCTL(LOOP_CHANGE_FD, 0, TYPE_INT)

  IOCTL(LOOP_CTL_ADD, 0, TYPE_INT)
  IOCTL(LOOP_CTL_REMOVE, 0, TYPE_INT)
  IOCTL(LOOP_CTL_GET_FREE, 0, TYPE_NULL)

  IOCTL(MTIOCTOP, IOC_W, MK_PTR(MK_STRUCT(STRUCT_mtop)))
  IOCTL(MTIOCGET, IOC_R, MK_PTR(MK_STRUCT(STRUCT_mtget)))
  IOCTL(MTIOCPOS, IOC_R, MK_PTR(MK_STRUCT(STRUCT_mtpos)))

  IOCTL(FBIOGET_FSCREENINFO, IOC_R, MK_PTR(MK_STRUCT(STRUCT_fb_fix_screeninfo)))
  IOCTL(FBIOGET_VSCREENINFO, IOC_R, MK_PTR(MK_STRUCT(STRUCT_fb_var_screeninfo)))
  IOCTL(FBIOPUT_VSCREENINFO, IOC_W, MK_PTR(MK_STRUCT(STRUCT_fb_var_screeninfo)))
  IOCTL(FBIOGETCMAP,        IOC_RW, MK_PTR(MK_STRUCT(STRUCT_fb_cmap)))
  IOCTL(FBIOPUTCMAP,        IOC_RW, MK_PTR(MK_STRUCT(STRUCT_fb_cmap)))
  IOCTL(FBIOPAN_DISPLAY,    IOC_RW, MK_PTR(MK_STRUCT(STRUCT_fb_var_screeninfo)))
  IOCTL(FBIOGET_CON2FBMAP,  IOC_RW, MK_PTR(MK_STRUCT(STRUCT_fb_con2fbmap)))
  IOCTL(FBIOPUT_CON2FBMAP,  IOC_RW, MK_PTR(MK_STRUCT(STRUCT_fb_con2fbmap)))

  IOCTL(VT_OPENQRY, IOC_R, MK_PTR(TYPE_INT))
  IOCTL(VT_GETSTATE, IOC_R, MK_PTR(MK_STRUCT(STRUCT_vt_stat)))
  IOCTL(VT_ACTIVATE, 0, TYPE_INT)
  IOCTL(VT_WAITACTIVE, 0, TYPE_INT)
  IOCTL(VT_LOCKSWITCH, 0, TYPE_INT)
  IOCTL(VT_UNLOCKSWITCH, 0, TYPE_INT)
  IOCTL(VT_GETMODE, IOC_RW, MK_PTR(MK_STRUCT(STRUCT_vt_mode)))
  IOCTL(VT_SETMODE, IOC_RW, MK_PTR(MK_STRUCT(STRUCT_vt_mode)))
  IOCTL(VT_RELDISP, 0, TYPE_INT)
  IOCTL(VT_DISALLOCATE, 0, TYPE_INT)

  IOCTL(DM_VERSION, IOC_RW, MK_PTR(MK_STRUCT(STRUCT_dm_ioctl)))
  IOCTL_SPECIAL(DM_REMOVE_ALL,   IOC_RW, do_ioctl_dm,
                MK_PTR(MK_STRUCT(STRUCT_dm_ioctl)))
  IOCTL_SPECIAL(DM_LIST_DEVICES, IOC_RW, do_ioctl_dm,
                MK_PTR(MK_STRUCT(STRUCT_dm_ioctl)))
  IOCTL_SPECIAL(DM_DEV_CREATE,   IOC_RW, do_ioctl_dm,
                MK_PTR(MK_STRUCT(STRUCT_dm_ioctl)))
  IOCTL_SPECIAL(DM_DEV_REMOVE,   IOC_RW, do_ioctl_dm,
                MK_PTR(MK_STRUCT(STRUCT_dm_ioctl)))
  IOCTL_SPECIAL(DM_DEV_RENAME,   IOC_RW, do_ioctl_dm,
                MK_PTR(MK_STRUCT(STRUCT_dm_ioctl)))
  IOCTL_SPECIAL(DM_DEV_SUSPEND,  IOC_RW, do_ioctl_dm,
                MK_PTR(MK_STRUCT(STRUCT_dm_ioctl)))
  IOCTL_SPECIAL(DM_DEV_STATUS,   IOC_RW, do_ioctl_dm,
                MK_PTR(MK_STRUCT(STRUCT_dm_ioctl)))
  IOCTL_SPECIAL(DM_DEV_WAIT,     IOC_RW, do_ioctl_dm,
                MK_PTR(MK_STRUCT(STRUCT_dm_ioctl)))
  IOCTL_SPECIAL(DM_TABLE_LOAD,   IOC_RW, do_ioctl_dm,
                MK_PTR(MK_STRUCT(STRUCT_dm_ioctl)))
  IOCTL_SPECIAL(DM_TABLE_CLEAR,  IOC_RW, do_ioctl_dm,
                MK_PTR(MK_STRUCT(STRUCT_dm_ioctl)))
  IOCTL_SPECIAL(DM_TABLE_DEPS,   IOC_RW, do_ioctl_dm,
                MK_PTR(MK_STRUCT(STRUCT_dm_ioctl)))
  IOCTL_SPECIAL(DM_TABLE_STATUS, IOC_RW, do_ioctl_dm,
                MK_PTR(MK_STRUCT(STRUCT_dm_ioctl)))
  IOCTL_SPECIAL(DM_LIST_VERSIONS,IOC_RW, do_ioctl_dm,
                MK_PTR(MK_STRUCT(STRUCT_dm_ioctl)))
  IOCTL_SPECIAL(DM_TARGET_MSG,   IOC_RW, do_ioctl_dm,
                MK_PTR(MK_STRUCT(STRUCT_dm_ioctl)))
  IOCTL_SPECIAL(DM_DEV_SET_GEOMETRY, IOC_RW, do_ioctl_dm,
                MK_PTR(MK_STRUCT(STRUCT_dm_ioctl)))
  IOCTL_SPECIAL(SIOCADDRT, IOC_W, do_ioctl_rt,
                MK_PTR(MK_STRUCT(STRUCT_rtentry)))
  IOCTL_SPECIAL(SIOCDELRT, IOC_W, do_ioctl_rt,
                MK_PTR(MK_STRUCT(STRUCT_rtentry)))

#ifdef HAVE_DRM_H
  IOCTL_SPECIAL(DRM_IOCTL_VERSION, IOC_RW, do_ioctl_drm,
                MK_PTR(MK_STRUCT(STRUCT_drm_version)))
#endif

#ifdef TARGET_TIOCSTART
  IOCTL_IGNORE(TIOCSTART)
  IOCTL_IGNORE(TIOCSTOP)
#endif

#ifdef CONFIG_KCOV
  IOCTL(KCOV_ENABLE, 0, TYPE_NULL)
  IOCTL(KCOV_DISABLE, 0, TYPE_NULL)
  IOCTL(KCOV_INIT_TRACE, IOC_R, TYPE_ULONG)
#endif

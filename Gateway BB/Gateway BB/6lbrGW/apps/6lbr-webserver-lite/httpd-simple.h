/* 
 * Copyright (c) 01/2018, Blueberry Tech srl.
 * All rights reserved.

/**
 * \file
 *         A simple webserver
 */

#ifndef __HTTPD_SIMPLE_H__
#define __HTTPD_SIMPLE_H__

#include "contiki-net.h"

/* The current internal border router webserver ignores the requested file name */
/* and needs no per-connection output buffer, so save some RAM */
#ifndef WEBSERVER_CONF_CFS_PATHLEN
#define HTTPD_PATHLEN 20
#else /* WEBSERVER_CONF_CFS_CONNS */
#define HTTPD_PATHLEN WEBSERVER_CONF_CFS_PATHLEN
#endif /* WEBSERVER_CONF_CFS_CONNS */

struct httpd_state;
typedef char (*httpd_simple_script_t) (struct httpd_state * s);

struct httpd_state {
  struct timer timer;
  struct psock sin, sout;
  struct pt outputpt;
  char inputbuf[HTTPD_PATHLEN + 24];
#if CONTIKI_TARGET_NATIVE
  char outputbuf[UIP_TCP_MSS];
  int fd;
  int len;
  int to_send;
#endif
  char filename[HTTPD_PATHLEN];
  httpd_simple_script_t script;
  char state;
};

void httpd_init(void);
void httpd_appcall(void *state);

httpd_simple_script_t httpd_simple_get_script(const char *name);

#define SEND_STRING(s, str) PSOCK_SEND(s, (uint8_t *)str, strlen(str))

#endif /* __HTTPD_SIMPLE_H__ */

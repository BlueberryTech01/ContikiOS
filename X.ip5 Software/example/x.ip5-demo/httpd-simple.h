/*
 * Copyright (c) 11/2017, Blueberry Tech srl.
 * All rights reserved.
/**
 * \addtogroup x.ip5-web-demo
 *
 * \file
 *         Header file for the HTTPD of the cc26xx web demo example.
/*---------------------------------------------------------------------------*/
#ifndef HTTPD_SIMPLE_H_
#define HTTPD_SIMPLE_H_
/*---------------------------------------------------------------------------*/
#include "contiki-net.h"
#include "sys/process.h"
#include "x.ip5-demo.h"
/*---------------------------------------------------------------------------*/
/* Ideally a multiple of TCP_MSS */
#ifdef HTTPD_SIMPLE_CONF_MAIN_BUF_SIZE
#define HTTPD_SIMPLE_MAIN_BUF_SIZE HTTPD_SIMPLE_CONF_MAIN_BUF_SIZE
#else
#define HTTPD_SIMPLE_MAIN_BUF_SIZE UIP_TCP_MSS
#endif
/*---------------------------------------------------------------------------*/
#define HTTPD_PATHLEN  16
#define HTTPD_INBUF_LEN (HTTPD_PATHLEN + 10)

#define TMP_BUF_SIZE   (UIP_TCP_MSS + 1)
/*---------------------------------------------------------------------------*/
/* POST request handlers */
#define HTTPD_SIMPLE_POST_HANDLER_OK      1
#define HTTPD_SIMPLE_POST_HANDLER_UNKNOWN 0
#define HTTPD_SIMPLE_POST_HANDLER_ERROR   0xFFFFFFFF

/**
 * \brief Datatype for a handler which can process incoming POST requests
 * \param key The configuration key to be updated
 * \param key_len The length of the key argument
 * \param val The new configuration value for key
 * \param val_len The length of the value argument
 *
 * \return 1: HTTPD_SIMPLE_POST_HANDLER_OK if the function can handle the
 * request, HTTPD_SIMPLE_POST_HANDLER_UNKNOWN if it does not know how to handle
 * it. HTTPD_SIMPLE_POST_HANDLER_ERROR if it does know how to handle it but
 * the request was malformed.
 */
typedef struct httpd_simple_post_handler {
  struct httpd_simple_post_handler *next;
  int (*handler)(char *key, int key_len, char *val, int val_len);
} httpd_simple_post_handler_t;

/* Declare a handler */
#define HTTPD_SIMPLE_POST_HANDLER(name, fp) \
  httpd_simple_post_handler_t name##_handler = { NULL, fp }

/**
 * \brief Register a handler for POST requests
 * \param h A pointer to the handler structure
 */
void httpd_simple_register_post_handler(httpd_simple_post_handler_t *h);
/*---------------------------------------------------------------------------*/
/*
 * An event generated by the HTTPD when a new configuration request has been
 * received
 */
extern process_event_t httpd_simple_event_new_config;
/*---------------------------------------------------------------------------*/
PROCESS_NAME(httpd_simple_process);
/*---------------------------------------------------------------------------*/
#endif /* HTTPD_SIMPLE_H_ */

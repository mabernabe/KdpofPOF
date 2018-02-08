/*
 * Copyright (c) 2007, Takahide Matsutsuka.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 */

/*
 * \file
 * 	contiki-conf.h
 * 	A set of configurations of contiki for PC-6001 family. 
 * \author
 * 	Takahide Matsutsuka <markn@markn.org>
 */

#ifndef __CONTIKI_CONF_H__
#define __CONTIKI_CONF_H__

#include "z80def.h"
#include "sys/cc.h"
#include <ctype.h>
#include <string.h>
#include "strcasecmp.h"
#include "log.h"
#include <imp_define.h>
#include <imp_io.h>

/* Time type. */
typedef unsigned long clock_time_t;

/* Defines tick counts for a second. */
#define CLOCK_CONF_SECOND		1000

#define rtimer_arch_now()	clock_time()

/* Memory filesystem RAM size. */
#define CFS_RAM_CONF_SIZE		4096

/* Logging.. */
#define LOG_CONF_ENABLED		0

#undef MALLOC_TEST

//#define IO_TELNET
#define TELNETD_CONF_NUMLINES  		50

/* uIP configuration */
#define UIP_CONF_MAX_CONNECTIONS	4
#define UIP_CONF_MAX_LISTENPORTS	4
#define UIP_CONF_BUFFER_SIZE		1540
#define UIP_CONF_BYTE_ORDER		LITTLE_ENDIAN
#define UIP_CONF_TCP_SPLIT		0
#define UIP_CONF_LOGGING		0
#define UIP_CONF_ARPTAB_SIZE		32
#define UIP_CONF_LLH_LEN 		15
//This is due to the extra field for the port

#define UIP_CONF_UDP			1
#define UIP_CONF_UDP_CHECKSUMS	        0

// #define UIP_CONF_BROADCAST

/*---------------------------------------------------------------------------*/
/* Application specific configurations. */

/* Command shell */
#define SHELL_GUI_CONF_XSIZE		26
#define SHELL_GUI_CONF_YSIZE		10

/* Text editor */
#define EDITOR_CONF_WIDTH		26
#define EDITOR_CONF_HEIGHT		 8

/* Process list */
#define PROCESSLIST_CONF_HEIGHT	12

/* File dialog */
#define FILES_CONF_HEIGHT		 6

/* Shell */
//#define SHELL_CONF_WITH_PROGRAM_HANDLER	 1

/* Telnet */
#define TELNET_CONF_WINDOW_WIDTH        30
#define TELNET_CONF_WINDOW_HEIGHT       13
//#define TELNET_CONF_TEXTAREA_HEIGHT      5 // TELNET_WINDOW_HEIGHT - 8
//#define TELNET_CONF_ENTRY_WIDTH         22 // TELNET_WINDOW_WIDTH - 8

/* Telnetd */
#define TELNETD_CONF_GUI                 0
#define SHELL_CONF_WITH_PROGRAM_HANDLER              0

/* Web server */
#define WEBSERVER_CONF_NANO 1
#undef WEBSERVER_CONF_LOG_ENABLED
#define HTTPD_CONF_CGI		0
#define HTTPD_CONF_SCRIPT	0
#define HTTPD_CONF_STATISTICS	0

/* unused yet */
#define VNC_CONF_REFRESH_ROWS    8

#define WWW_CONF_WEBPAGE_WIDTH 76
#define WWW_CONF_WEBPAGE_HEIGHT 30

#endif /* __CONTIKI_CONF_H__ */

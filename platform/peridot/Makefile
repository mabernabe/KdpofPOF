#
# Makefile for PC-6001 using z80/SDCC
# @author Takahide Matsutsuka <markn@markn.org>
#
# $Id: Makefile,v 1.7 2009/12/16 06:47:18 matsutsuka Exp $
#

CONTIKI = ../..
TARGET = peridot

APPS 			= program-handler
PLATFORM_APPS 	= is-alive dhcp ethernet_driver webserver-mini igmp telnetd shell
CONTIKI_TARGET_MAIN = contiki-server-main.c

all: main.ihx


include $(CONTIKI)/Makefile.include

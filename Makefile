###############################################################################
# Makefile 
# Jared Spaulding
# COSC 3750, SP 2020
# Homework 5
# Mar. 3, 2020
# 
# Makefile to complile wyls.c
###############################################################################

CC=gcc
CFLAGS=-ggdb -Wall
RM=/bin/rm -f

.PHONY: clean

wyls: wyls.c
	$(CC) $(CFLAGS) $? -o wyls -I $(CURDIR)

clean: 
	$(RM) wyls

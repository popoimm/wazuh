/* Copyright (C) 2009 Trend Micro Inc.
 * All rights reserved.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */

#ifndef _EXECD_H
#define _EXECD_H

#ifndef ARGV0
#define ARGV0 "ossec-execd"
#endif

/* Add/delete arguments for the commands */
#define ADD_ENTRY       "add"
#define DELETE_ENTRY    "delete"

/* Maximum number of active responses active */
#define MAX_AR      64

/* Maximum number of command arguments */
#define MAX_ARGS    32

/* Execd select timeout -- in seconds */
#define EXECD_TIMEOUT   90

extern int repeated_offenders_timeout[];
extern char ** wcom_ca_store;
extern int pending_upg;

/** Function prototypes **/

void WinExecdRun(char *exec_msg);
int ReadExecConfig(void);
char *GetCommandbyName(const char *name, int *timeout) __attribute__((nonnull));
void ExecCmd(char *const *cmd) __attribute__((nonnull));
void ExecCmd_Win32(char *cmd);
int ExecdConfig(const char *cfgfile) __attribute__((nonnull));
int WinExecd_Start(void);
void WinTimeoutRun(int timeout);

size_t wcom_open(const char *file_path, const char *mode, char *output);
size_t wcom_write(const char *file_path, char *buffer, size_t length, char *output);
size_t wcom_close(const char *file_path, char *output);
size_t wcom_sha1(const char *file_path, char *output);
size_t wcom_unmerge(const char *file_path, char *output);
size_t wcom_uncompress(const char * source, const char * target, char * output);
size_t wcom_upgrade(const char * package, const char * installer, char * output);
size_t wcom_upgrade_result(char *output);
size_t wcom_restart(char *output);
size_t wcom_dispatch(char *command, size_t length, char *output);

#ifndef WIN32
// Com request thread dispatcher
void * wcom_main(void * arg);
#endif

/* Timeout data structure */
typedef struct _timeout_data {
    time_t time_of_addition;
    int time_to_block;
    char **command;
} timeout_data;

void FreeTimeoutEntry(timeout_data *timeout_entry);

#endif

/*
 * Copyright (C) 2005-2008 William Pitcock, et al.
 * Rights to this code are as documented in doc/LICENSE.
 *
 * Config file parser.
 *
 */
/*
 * This file was adapted to fit opensn0w-core.
 */
 
#ifndef CONFPARSE_H
#define CONFPARSE_H

typedef struct _configfile config_file_t;
typedef struct _configentry config_file_entry_t;

struct _configfile
{
	char *filename;
	config_file_entry_t *entries;
	config_file_t *next;
	int curline;
	char *mem;
};

struct _configentry
{
	config_file_t *fileptr;

	int varlinenum;
	char *varname;
	char *vardata;
	int sectlinenum; /* line containing closing brace */

	config_file_entry_t *entries;
	config_file_entry_t *prevlevel;
	config_file_entry_t *next;
};

/* confp.c */
extern void config_file_free(config_file_t *cfptr);
extern config_file_t *config_file_load(const char *filename);
void config_check(config_file_t * config);

#endif

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs ts=8 sw=8 noexpandtab
 */

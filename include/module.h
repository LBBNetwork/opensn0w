/*
 * libmowgli: A collection of useful routines for programming.
 * module.h: Loadable modules.
 *
 * Copyright (c) 2007 William Pitcock <nenolod -at- sacredspiral.co.uk>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice is present in all copies.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

//
// Adapted to fit opensn0w.
//

#ifndef __sn0w_MODULE_H__
#define __sn0w_MODULE_H__

typedef void * sn0w_module_t;
void loadmodules(config_file_entry_t * entry);
extern sn0w_module_t sn0w_module_open(const char *path);
extern void * sn0w_module_symbol(sn0w_module_t module, const char *symbol);
extern void sn0w_module_close(sn0w_module_t module);


typedef enum {
	MODULE_UNLOAD_INTENT_PERM,
	MODULE_UNLOAD_INTENT_RELOAD
} module_unload_intent_t;

typedef enum {
	MODULE_UNLOAD_CAPABILITY_OK,
	MODULE_UNLOAD_CAPABILITY_NEVER,
	MODULE_UNLOAD_CAPABILITY_RELOAD_ONLY
} module_unload_capability_t;

typedef struct module_ module_t;
typedef struct module_header_t module_header_t;

typedef void (*module_unload_handler_t)(module_t *, module_unload_intent_t);

/* Module structure. Might be a loaded .so module, or something else that
 * behaves as a module for dependency purposes (perl script, etc).
 */

#ifndef BUFSIZE
#define BUFSIZE		256
#endif

/* macros for linked lists */
#define LIST_FOREACH(n, head) for (n = (head); n; n = n->next)  
#define LIST_FOREACH_NEXT(n, head) for (n = (head); n->next; n = n->next)
#define LIST_FOREACH_PREV(n, tail) for (n = (tail); n; n = n->prev)

#define LIST_LENGTH(list) (list)->count

#define LIST_FOREACH_SAFE(n, tn, head) for (n = (head), tn = n ? n->next : NULL; n != NULL; n = tn, tn = n ? n->next : NULL)

#define ITER_FOREACH(n, head) for (n = (head); n; n = n->next)
#define ITER_FOREACH_NEXT(n, head) for (n = (head); n->next; n = n->next)
#define ITER_FOREACH_PREV(n, tail) for (n = (tail); n; n = n->prev)
#define ITER_FOREACH_SAFE(n, tn, head) for (n = (head), tn = n ? n->next : NULL; n != NULL; n = tn, tn = n ? n->next : NULL)


/* list node struct */
typedef struct sn0w_node_ sn0w_node_t;
typedef struct sn0w_list_ sn0w_list_t;

struct sn0w_node_
{
	struct sn0w_node_ *next, *prev; 
	void *data;                   /* pointer to real structure */
};

/* node list struct */
struct sn0w_list_
{
	sn0w_node_t *head, *tail;
	size_t count;                    /* how many entries in the list */
};

extern void sn0w_node_bootstrap(void);
extern sn0w_node_t *sn0w_node_create(void);
extern void sn0w_node_free(sn0w_node_t *n);
extern void sn0w_node_add(void *data, sn0w_node_t *n, sn0w_list_t *l);
extern void sn0w_node_add_head(void *data, sn0w_node_t *n, sn0w_list_t *l);
extern void sn0w_node_add_before(void *data, sn0w_node_t *n, sn0w_list_t *l, sn0w_node_t *before);
extern void sn0w_node_add_after(void *data, sn0w_node_t *n, sn0w_list_t *l, sn0w_node_t *before);
extern void sn0w_node_insert(void *data, sn0w_node_t *n, sn0w_list_t *l, size_t position);
extern ssize_t sn0w_node_index(sn0w_node_t *n, sn0w_list_t *l);
extern void sn0w_node_delete(sn0w_node_t *n, sn0w_list_t *l);
extern sn0w_node_t *sn0w_node_find(void *data, sn0w_list_t *l);
extern void sn0w_node_move(sn0w_node_t *m, sn0w_list_t *oldlist, sn0w_list_t *newlist);
extern sn0w_node_t *sn0w_node_nth(sn0w_list_t *l, size_t pos);
extern void *sn0w_node_nth_data(sn0w_list_t *l, size_t pos);

typedef int (*sn0w_list_comparator_t)(sn0w_node_t *n, sn0w_node_t *n2, void *opaque);

extern sn0w_list_t *sn0w_list_create(void);
extern void sn0w_list_free(sn0w_list_t *l);
extern void sn0w_list_concat(sn0w_list_t *l, sn0w_list_t *l2);
extern void sn0w_list_reverse(sn0w_list_t *l);
extern void sn0w_list_sort(sn0w_list_t *l, sn0w_list_comparator_t comp, void *opaque);


struct module_ {
	char name[BUFSIZE];
	char modpath[BUFSIZE];
	module_unload_capability_t can_unload;

	unsigned int mflags;

	/* These three are real-module-specific. Either all will be set, or all
	 * will be null.
	 */
	module_header_t *header;
	void *address;
	sn0w_module_t *handle;

	/* If this module is not a loaded .so (the above three are null), and
	 * can_unload is not never, then * this must be set to a working unload
	 * function.
	 */
	module_unload_handler_t unload_handler;

	sn0w_list_t dephost;
	sn0w_list_t deplist;

	sn0w_list_t symlist;		/* MAPIv2 symbol dependencies. */
};

#define MODTYPE_STANDARD	0
#define MODTYPE_CORE		1 /* Can't be unloaded. */
#define MODTYPE_FAIL		0x8000 /* modinit failed */

#define SN0W_MAPI      		0x736e3077
#define SN0W_MAPI_VER		1

#define MAX_CMD_PARC		20

struct module_header_t {
	unsigned int sn0w_mod;
	unsigned int abi_ver;
	unsigned int abi_rev;
	const char *serial;
	const char *name;
	module_unload_capability_t can_unload;
	void (*modinit)(module_t *m);
	void (*deinit)(module_unload_intent_t intent);
	const char *vendor;
	const char *version;
};

/* name is the module name we're searching for.
 * path is the likely full path name, which may be ignored.
 * If it is found, set module to the loaded module_t pointer
 */
typedef struct {
	const char *name;
	const char *path;
	module_t *module;
	int handled;
} hook_module_load_t;

#define DECLARE_MODULE(name, norestart, modinit, deinit, ver, ven) \
	module_header_t _header = { \
		SN0W_MAPI, SN0W_MAPI_VER, \
		SN0W_ABIREV, "unknown", \
		name, norestart, modinit, deinit, ven, ver \
	}

extern void _modinit(module_t *m);
extern void _moddeinit(module_unload_intent_t intent);

extern void modules_init(void);
extern void modules_shutdown(void);
extern module_t *module_load(const char *filespec);
extern void module_load_dir(const char *dirspec);
extern void module_load_dir_match(const char *dirspec, const char *pattern);
extern void *module_locate_symbol(const char *modname, const char *sym);
extern void module_unload(module_t *m, module_unload_intent_t intent);
extern module_t *module_find(const char *name);
extern module_t *module_find_published(const char *name);
extern bool module_request(const char *name);

#define MODULE_TRY_REQUEST_DEPENDENCY(self, modname) \
	if (module_request(modname) == false)				\
	{								\
		(self)->mflags = MODTYPE_FAIL;				\
		return;							\
	}

#define MODULE_TRY_REQUEST_SYMBOL(self, dest, modname, sym) \
	if ((dest = module_locate_symbol(modname, sym)) == NULL)		\
	{									\
		MODULE_TRY_REQUEST_DEPENDENCY(self, modname);			\
		if ((dest = module_locate_symbol(modname, sym)) == NULL)	\
		{								\
			(self)->mflags = MODTYPE_FAIL;				\
			return;							\
		}								\
	}

typedef struct module_dependency_ {
	char *name;
	module_unload_capability_t can_unload;
} module_dependency_t;

#endif

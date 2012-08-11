/*
 * Patch stuff
 */

#ifndef _patch_h_
#define _patch_h_

typedef struct _patch_node {
    struct _patch_node *prev;
    struct _patch_node *next;
    char* name;
    unsigned char* original_bytes;
    unsigned char* patch_bytes;
    int architecture;
    int version;
    int size;
} patch_node_t;

typedef struct _patch_list {
    patch_node_t *head;
    patch_node_t *tail;
    int count;
} patch_list_t;

#define LIST_FOREACH(n, head) for (n = (head); n; n = n->next)  
#define LIST_FOREACH_NEXT(n, head) for (n = (head); n->next; n = n->next)
#define LIST_FOREACH_PREV(n, tail) for (n = (tail); n; n = n->prev)
#define LIST_LENGTH(list) (list)->count
#define LIST_FOREACH_SAFE(n, tn, head) for (n = (head), tn = n ? n->next : NULL; n != NULL; n = tn, tn = n ? n->next : NULL)

patch_node_t* patch_node_create(void);
void patch_node_free(patch_node_t* p);
void patch_node_add(char* name, unsigned char* original, unsigned char *replacement, int size, int architcture, int version, patch_node_t *n, patch_list_t* l);
void patch_node_delete(patch_node_t *n, patch_list_t *l);
patch_node_t* patch_node_find(char* name, patch_list_t *l);
patch_list_t* patch_list_create(void);
void patch_replace(patch_list_t *l, unsigned char *p, int len);

#endif

/*
 * patch.c
 *
 * Base patching routines
 *
 * Sort of based off of libmowgli-2.
 */

#include "core.h"

extern int arch, global_version;

patch_node_t* patch_node_create(void) {
    patch_node_t* p;

    p = malloc(sizeof(patch_node_t));
    if(!p)
        return NULL;

    memset(p, 0, sizeof(patch_node_t));

    return p;
}

void patch_node_free(patch_node_t* p) {
    if(!p)
        return;

    free(p);
}

void patch_node_add(char* name, unsigned char* original, unsigned char *replacement, int size, int architcture, int version, patch_node_t *n, patch_list_t* l) {
    patch_node_t *tn;

    if(!n)
        return;
    if(!l)
        return;

    n->next = n->prev = NULL;
    n->name = name;
    n->original_bytes = original;
    n->patch_bytes = replacement;
    n->size = size;
    n->architecture = architcture;
    n->version = version;
    
    printf("Registering patch \"%s\" with size %d for ARMv%d to patch list %p\n", name, size, architcture, l);

    if(!l->head) {
        l->head = n;
        l->tail = n;
        l->count++;
        return;
    }

    tn = l->tail;
    n->prev = tn;
    n->prev->next = n;
    l->tail = n;
    l->count++;
}

void patch_node_delete(patch_node_t *n, patch_list_t *l) {
    if(!n)
        return;
    if(!l)
        return;

    if(!n->prev)
        l->head = n->next;
    else
        n->prev->next = n->next;
    if(!n->next)
        l->tail = n->prev;
    else
        n->next->prev = n->prev;

    l->count--;
}

patch_node_t* patch_node_find(char* name, patch_list_t *l) {
    patch_node_t *n;

    if(!l)
        return NULL;
    if(!name)
        return NULL;

    LIST_FOREACH(n, l->head) {
        if(!strcmp(name, n->name))
            return n;
    }

    return NULL;
}

patch_list_t* patch_list_create(void) {
    patch_list_t* out = malloc(sizeof(patch_list_t));
    if(!out)
        return NULL;

    memset(out, 0, sizeof(patch_list_t));
    return out;
}

void patch_replace(patch_list_t *l, unsigned char *p, int len) {
    patch_node_t *n, *tn;

    if(!l)
        return;
    if(!p)
        return;

    LIST_FOREACH_SAFE(n, tn, l->head) {
        int i;
        for(i = 0; i < len; i++) {
            unsigned char *c = &p[i];
            /*if(arch == n->architecture || n->architecture == 0)*/ {
                /*if(global_version == n->version || n->version == 0)*/ {
                    if(!memcmp(c, n->original_bytes, n->size)) {
                        DPRINT("Patching %s check at 0x%08x\n", n->name, i);
                        memcpy(c, n->patch_bytes, n->size);
                        continue;
                    }
                }
            }
        }
    }
}

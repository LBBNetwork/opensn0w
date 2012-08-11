/*
 * Config stuff.
 */

#include "core.h"

config_file_t *config;

void loadmodules(config_file_entry_t * entry)
{
    module_t * m;

    while (entry != NULL)
    {
        if (!strcmp(entry->varname, "loadmodule"))
        {
            m = module_load(entry->vardata);
            if (m != NULL)
                printf("[Modules] Loaded module %s\n", m->name);
            else
                printf("[Modules] Failed to load module %s\n", entry->vardata);
        }
        if (entry->next != NULL)
            loadmodules(entry->next);

        entry = entry->next;
    }
}


config_file_entry_t * config_find_entry(config_file_entry_t * start,
                                               const char * name)
{
    config_file_entry_t * e;
    
    e = start;
    
    while(e != NULL)
    {
        if (!strcmp(e->varname, name))
            return e;
        else
            e = e->next;
    }
    
    return NULL;
}

config_file_entry_t * config_find_entry_next(config_file_entry_t * start)
{
    config_file_entry_t * e;
    
    e = start;
    
    if(e != NULL)
    {
        return e->next;
    }
    
    return NULL;
}

config_file_entry_t * config_fatal_find_entry(config_file_entry_t * entries,
                                                     char * name)
{
    config_file_entry_t * entry;
    entry = config_find_entry(entries, name);
    
    if (entry == NULL)
    {
        printf("%s was not found in configuration file. Aborting.\n", name);
        exit(-1);
    }
    
    return entry;
}

#define SAFE_FIND(a,b) \
    s = config_find_entry(e->entries, a); \
    if(s) \
        b = s->vardata;

#define SAFE_FIND_INT(a,b) \
    s = config_find_entry(e->entries, a); \
    if(s) \
        b = (int)strtol(s->vardata, (char **)NULL, 10);

void config_parse(config_file_entry_t * entries, int level)
{
    
    static char* name;
    static char* orig_hex_bytes;
    static char* patch_hex_bytes;
    static char* orig_string;
    static char* patch_string;
    static int architecture;
    static int global_os;
    static char* type;
    static unsigned char* t0, *p0;
    static size_t t0s, p0s;
    
    config_file_entry_t * e, *s;
    
    e = entries;
    
    while (e != NULL)
    {
        if(level == 2) {
            name = e->varname;
            SAFE_FIND("OriginalHexBytes", orig_hex_bytes);
            SAFE_FIND("PatchedHexBytes", patch_hex_bytes);
            SAFE_FIND("OriginalString", orig_string);
            SAFE_FIND("PatchedString", patch_string);
            SAFE_FIND("Type", type);
            SAFE_FIND_INT("GlobalOperatingSystem", global_os);
            SAFE_FIND_INT("Architecture", architecture);
            if(orig_string && patch_string) {
                t0s = strlen(orig_string);
                p0s = strlen(patch_string);
                t0 = (unsigned char*)orig_string;
                p0 = (unsigned char*)patch_string;
                if(t0s != p0s) {
                    printf("Patches must be the same size. Aborting.\n");
                }
            } else if(orig_hex_bytes && patch_hex_bytes) {
                hexToBytes(orig_hex_bytes, &t0, &t0s);
                hexToBytes(patch_hex_bytes, &p0, &p0s);
                if(t0s != p0s) {
                    printf("Patches must be the same size. Aborting.\n");
                }
            }
            if(!strcasecmp(type, "BootLoader"))
                patch_node_add(name, t0, p0, t0s, architecture, global_os, patch_node_create(), iboot_patches);
            else if(!strcasecmp(type, "Kernel"))
                patch_node_add(name, t0, p0, t0s, architecture, global_os, patch_node_create(), kernel_patches);
            else
                printf("Unknown patch type \"%s\".\n", name);
            orig_hex_bytes = patch_hex_bytes = orig_string = patch_string = NULL;
            architecture = global_os = t0s = p0s = 0;
            t0 = p0 = type = NULL;
        }

        if (e->entries != NULL)
            config_parse(e->entries, level+1);
        e = e->next;
    }
}

void config_check(config_file_t * config)
{
    config_file_entry_t * entry, * root;
    root = config_fatal_find_entry(config->entries, "opensn0w");
    if ((entry = config_find_entry(root->entries, "ConfigurationVersion"))) {
        if ((int)strtol(entry->vardata, (char **)NULL, 10) != 100) {
            printf("Configuration version is wrong, expected %d, got %s\n", 100, entry->vardata);
            exit(-1);
        }
    }
    
    root = config_fatal_find_entry(config->entries, "patches");

    iboot_patches = patch_list_create();
    kernel_patches = patch_list_create();
    if(!iboot_patches || !kernel_patches) {
        printf("Failed to create patch list.\n");
        exit(-1);
    }
    
    config_parse(root, 1);
}


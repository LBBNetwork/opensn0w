/*
 * ARM generic subroutine patcher.
 */ 

#include "core.h"

#if 0 /* work in progress */

static char push = 0xB5;
static char push_r7_lr[] = { 0x80, 0xB5 };
static char push_r4_to_r7_lr[] = { 0xF0, 0xB5 };
static char pop_r4_to_r7_lr[] = { 0xF0, 0xBD };

static char* functions[][3] = {
	{ "cmd_go", "jumping into image", push_r7_lr },
	{ "image_load", "image validation failed but untrusted images are permitted", push_r4_to_r7_lr },
	{ NULL, NULL, NULL }
};

void* find_reference(unsigned char* data, unsigned int base, unsigned int size, char* signature) {
	unsigned int i = 0;
	unsigned int reference = 0;
	unsigned int reference2 = 0;
	// First find the string
	unsigned int address = 0;
	for(i = 0; i < size; i++) {
		if(!memcmp(&data[i], signature, strlen(signature))) {
			address = base | i;
			break;
		}
	}
	if(address == 0) return NULL;
    
	// Next find where that string is referenced
    
	for(i = 0; i < size; i++) {
		if(!memcmp(&data[i], &address, 4)) {
			reference = base | i;
			break;
		}
	}
	if(reference == 0) return NULL;
	reference -= 8;
    
	for(i = 0; i < size; i++) {
		if(!memcmp(&data[i], &reference, 4)) {
			reference2 = base | i;
			break;
		}
	}
	if(reference2 == 0) return NULL;
	return (void*)reference2;
}


void* find_top(unsigned char* data, unsigned int base, unsigned int size, unsigned int address) {
	// Find the top of that function
	int i = 0;
	unsigned int function = 0;
	while(i > 0) {
		i--;
		if(data[i] == push) {
			function = base | i;
			break;
		}
	}
	if(function == 0) return NULL;
	
	return (void*)function;
}

int find_offset(char* data, unsigned int base, unsigned int size, char** what) {
	unsigned int i = 0;
	char* signature = (char*)what[1];
	unsigned int reference = 0;
	unsigned int address = 0;
	for(i = 0; i < size; i++) {
		if(!memcmp((char*)(&data[i]), (char*)signature, strlen(signature))) {
			address = base | i;
			break;
		}
	}
    
	if(address == 0) return 0;
    
    
	for(i = 0; i < size; i++) {

		if(!memcmp((const char*)&data[i], (const char*)&address, 4)) {
			reference = base | i;
			break;
		}
	}
	if(reference == 0) return 0;
    
	while(i > 0) {
		i--;
		if(!memcmp((const char*)&data[i], what[2], 2)) {
			break;
		}
	}
    
	return i;
}

unsigned int find_string(unsigned char* data, unsigned int base, unsigned int size, const char* name) {
	// First find the string
	int i = 0;
	unsigned int address = 0;
	for(i = 0; i < size; i++) {
		if(!memcmp((const char*)&data[i], (const char*)name, strlen(name))) {
			address = (unsigned int)&data[i];
			break;
		}
	}
	return address;
}

int find_function(const char* name, unsigned char* target, unsigned char* base, int l) {
	int i = 0;
	int found = 0;
	for(i = 0; i < sizeof(functions); i++) {
		if(!strcmp(functions[i][0], name)) {
			found = (unsigned int)find_offset((char*)target, (unsigned int)base, l, (char**)functions[i]);
			if(found < 0) {
				return 0;
			}
			break;
		}
	}
    
	return found;
}

void* pattern_search(void* addr, int len, int pattern, int mask, int step) {
    char *caddr = (char*)(addr);
    int i;
    if(len <= 0)
        return NULL;
    
    if(step < 0) {
        len = -len;
        len &= ~-(step+1);
    } else {
        len &= ~(step-1);
    }
    
    for(i = 0; i != len; i += step) {
        int x = *(int*)(caddr+i);
        if((x & mask) == pattern) {
            return (void*)(caddr+i);
        }
    }
    return NULL;
}

void* bl_search_down(void* p, int l) {
    return pattern_search(p, l, 0xD000F000, 0xD000F800, 2);
}

void* bl_search_up(void* p, int l) {
    return pattern_search(p, l, 0xD000F000, 0xD000F800, -2);
}

void* resolve_bl32(const void* bl)
{
    typedef unsigned short uint16_t;
    typedef int int32_t;
    union {
        uint16_t value;
        
        struct {
            uint16_t immediate : 10;
            uint16_t s : 1;
            uint16_t : 5;
        };
    } bits = {*(uint16_t*)bl};
    
    union {
        uint16_t value;
        
        struct {
            uint16_t immediate : 11;
            uint16_t j2 : 1;
            uint16_t x : 1;
            uint16_t j1 : 1;
            uint16_t : 2;
        };
    } exts = {((uint16_t*)bl)[1]};
    
    int32_t jump = 0;
    jump |= bits.s << 24;
    jump |= (~(bits.s ^ exts.j1) & 0x1) << 23;
    jump |= (~(bits.s ^ exts.j2) & 0x1) << 22;
    jump |= bits.immediate << 12;
    jump |= exts.immediate << 1;
    jump |= exts.x;
    jump <<= 7;
    jump >>= 7;
    return (void*)((int)jump+4);
}

void patch_image_load(char*p, int l) {
    unsigned int fn_begin, fn_end, fn_size;
    unsigned char cmp_r0_0[] = {0x00, 0x28};
    unsigned int imagebase = 0x4FF00000;
    unsigned char replacement[] = {0x00, 0x20, 0x70, 0x47}; /* mov r0, #0; bx lr */
    unsigned char function_prolog[] = {0xF0, 0xB5L}; /* push {r4-r7,lr}; add r7, sp, #0xc; push {r8,r10,r11} */
    
    static unsigned int cachebase;
    unsigned int candidate = find_function("image_load", (unsigned char*)&p[0], (unsigned char*)0x4FF00000, l);
    
    if(!candidate) {
        candidate = find_function("image_load", (unsigned char*)&p[0], (unsigned char*)0x84000000, l);
        imagebase = 0x84000000;
    }
    
    if(!candidate) {
        candidate = find_function("image_load", (unsigned char*)&p[0], (unsigned char*)0x5FF00000, l);
        imagebase = 0x5FF00000;
    }

    if(!candidate) {
        candidate = find_function("image_load", (unsigned char*)&p[0], (unsigned char*)0x0FF00000, l);
        imagebase = 0x0FF00000;
    }
    
    if(!candidate) {
        candidate = find_function("image_load", (unsigned char*)&p[0], (unsigned char*)0x22000000, l);
        imagebase = 0x22000000;
    }
    
    printf("Opcode:\t\tpush\t{r4-r7,lr}\tat 0x%x\n", candidate|imagebase);
    fn_begin = candidate;
    
    while(memcmp((const char*)&p[candidate], pop_r4_to_r7_lr, 2)) {
        candidate += 2;
    }
    
    printf("Opcode:\t\tpop\t{r4-r7,lr}\tat 0x%x\n", candidate|imagebase);
    fn_end = candidate;
    
    printf("Function size: %d bytes, image base 0x%08x\n", fn_end - fn_begin, imagebase);
    fn_size = fn_end - fn_begin;
    
    candidate = fn_begin;
    while(candidate <= fn_end - 128 /* last 128 bytes are simply epilogue */) {
        unsigned int addr = bl_search_down(&p[candidate], fn_size) - (void*)&p[0];
        uint32_t insn;
        unsigned int base = (unsigned int)(addr+resolve_bl32(&insn)-1);
        
        memcpy(&insn, &p[addr], 4);
        
        if(cachebase != base) {
            if(!memcmp(&p[addr] + 4, cmp_r0_0, 2)) {
                if(!memcmp(&p[base], function_prolog, sizeof(function_prolog))) {
                    memcpy(&p[base], replacement, sizeof(replacement));
                    printf("nuked sub_%08x\n", base);
                    cachebase = base;
                }
              } else if(!memcmp(&p[addr] + 6, cmp_r0_0, 2)) {
                if(!memcmp(&p[base], function_prolog, sizeof(function_prolog))) {
                    memcpy(&p[base], replacement, sizeof(replacement));
                    printf("nuked sub_%08x\n", base);
                    cachebase = base;
                }
            }
        }
        
        fn_size -= 2;
        candidate += 2;
    }
    
}

#endif

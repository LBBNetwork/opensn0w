/*
 * Firmware/patch declerations
 */

#ifndef _firmware_h_
#define _firmware_h_

typedef struct _firmware_item {
	char *key;
	char *iv;
	char *vfkey;
	char *name;
} firmware_item, *firmware_item_t;

// The enum _firmware_images and the image_names array in main.c MUST match //

typedef enum _firmware_images {
	IBSS = 0,
	DEVICETREE,
	BATTERYCHARGING1,
	GLYPHCHARGING,
	BATTERYCHARGING0,
	IBOOT,
	BATTERYLOW0,
	LLB,
	IBEC,
	KERNELCACHE,
	FILESYSTEM,
	APPLELOGO,
	UPDATERAMDISK,
	RESTORERAMDISK,
	GLYPHPLUGIN,
	RECOVERY,
	BATTERYLOW1,
	BATTERYFULL,
	NEEDSERVICE
} firmware_images_t;

typedef enum _modes_t {
	DFU = 0,
	RECOVERYMODE
} modes_t;

typedef struct _firmware {
	firmware_item *item;
	uint8_t items;
} firmware, *firmware_t;

extern patch_list_t *iboot_patches;
extern patch_list_t *devicetree_patches;
extern patch_list_t *kernel_patches;

int fetch_image(const char *path, const char *output);
int patch_file(char *filename);

#endif

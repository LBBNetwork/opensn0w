/*
 * The funnest part
 */

#include "core.h"

extern char *kernelcache;
extern char *bootlogo;
extern char *url;
extern char *plist;
extern char *ramdisk;
extern int iboot;
extern int dry_run;
extern int gp_payload;
extern int nologo;
extern int pwndfu;
extern int pwnrecovery;
extern int autoboot;
extern int download;
extern int use_shatter;
extern int do_jailbreak;
extern int dump_bootrom;
extern volatile int jailbreaking;

firmware Firmware;
char *version = "UNKNOWN";
int userprovided = 0;
Dictionary *firmwarePatches, *patchDict, *info;

static const char *image_names[] = {
        "iBSS",
        "DeviceTree",
        "BatteryCharging1",
        "GlyphCharging",
        "BatteryCharging0",
        "iBoot",
        "BatteryLow0",
        "LLB",
        "iBEC",
        "KernelCache",
        "FileSystem",
        "AppleLogo",
        "UpdateRamdisk",
        "RestoreRamdisk",
        "GlyphPlugin",
        "Recovery",
        "BatteryLow1",
        "BatteryFull",
        "NeedService"
};


/*!
 * \fn int upload_image(firmware_item item, int mode, int patch, int userprovided)
 * \brief Upload image to device based on \a item and \a patch it if necessary.
 *
 * \param item Firmware item to be uploaded.
 * \param mode Notify DFU mode if upload should be finished or not.
 * \param patch Patch file.
 * \param userprovided User provided.
 */

int upload_image(firmware_item item, int mode, int patch, int userprovided)
{
	char path[255];
	struct stat buf;
	irecv_error_t error = IRECV_E_SUCCESS;
	char *buffer;
	char *filename;

	if(!item.name) {
		DPRINT("Failing upload of image as filename is NULL.\n");
		return -1;
	}

	filename = strrchr(item.name, '/');
	filename = strrchr(item.name, '/');

	if (filename == NULL)
		filename = item.name;
	else
		filename++;

	buffer = malloc(strlen(filename) + 10 + strlen(version));
	if (!buffer) {
		ERR("Cannot allocate memory\n");
		return -1;
	}
	memset(buffer, 0, strlen(filename) + 10 + strlen(version));

	snprintf(buffer, strlen(filename) + 10 + strlen(version), "%s_%s",
		 filename, version);


	if (userprovided)
		snprintf(buffer, strlen(filename) + 10 + strlen(version), "%s",
			 filename);

	DPRINT("Checking if %s already exists\n", buffer);

	memset(path, 0, 255);

	if (stat(buffer, &buf) != 0) {
		if (fetch_image(item.name, buffer) < 0) {
			ERR("Unable to upload DFU image\n");
			return -1;
		}
	}

	if (patch && !userprovided)
		patch_file(buffer);

	if (patch && !userprovided) {
		snprintf(buffer, strlen(filename) + 10 + strlen(version),
			 "%s_%s.pwn", filename, version);
	}

	DPRINT("Uploading %s to device\n", buffer);

	if(!dry_run) {
		if (client->mode != kDfuMode)
			error = irecv_send_file(client, buffer, 0);
		else
			error = irecv_send_file(client, buffer, 1);
	}

	if (error != IRECV_E_SUCCESS) {
		ERR("%s\n", irecv_strerror(error));
		return -1;
	}
	free(buffer);
	return 0;
}

void identify_device(void) {	
	STATUS("[*] Device found.\n");
	DPRINT("Device found: name: %s, processor s5l%dxsi\n",
		   device->product, device->chip_id);
	DPRINT("iBoot information: %s\n", client->serial);
}

void fix_recovery_loop(void) {
	irecv_error_t err = IRECV_E_SUCCESS;

	while (poll_device(RECOVERYMODE)) {
		sleep(1);
	}

	err = irecv_get_device(client, &device);
	if (device == NULL || device->index == DEVICE_UNKNOWN) {
		FATAL("[!] USB communications error.\n");
	}

	identify_device();	

	STATUS("[*] Fixing recovery loop...\n");
	irecv_send_command(client, "setenv auto-boot true");
	irecv_send_command(client, "saveenv");
	client = irecv_reconnect(client, 2);

	irecv_send_command(client, "reboot");

	STATUS("[*] Operation completed.\n");

	exit(0);
}

int exploit_device(void) {
	sn0w_node_t *n, *tn;

	LIST_FOREACH_SAFE(n, tn, exploits.head) {
		exploit_info_t *i = (exploit_info_t*)n->data;
		if(i) {
			if(i->compatible_chip == device->chip_id) {
				DPRINT("Executing exploit handler\n");
				if(!dry_run)
					return i->handler();
			}
		}
	}
}

void upload_apple_logo(void) {
	if (bootlogo) {
		userprovided = 1;
		Firmware.item[APPLELOGO].name = bootlogo;
	}
	upload_image(Firmware.item[APPLELOGO], 2, 0, userprovided);
    userprovided = 0;
    
	irecv_send_command(client, "setpicture 0");
	irecv_send_command(client, "bgcolor 0 0 0");
	irecv_send_command(client, "bgcolor 192 192 192");
	client = irecv_reconnect(client, 2);
}

void upload_devicetree(void) {
	upload_image(Firmware.item[DEVICETREE], 1, 1, 0);
	irecv_send_command(client, "devicetree");
	client = irecv_reconnect(client, 5);
}

void upload_kernel(void) {
	if (kernelcache) {
		Firmware.item[KERNELCACHE].name = kernelcache;
		userprovided = 1;
	}
	upload_image(Firmware.item[KERNELCACHE], 3, 1, userprovided);
	userprovided = 0;
}

void upload_ibss(void) {
	upload_image(Firmware.item[IBSS], 0, 1, 0);
	client = irecv_reconnect(client, 5);
}

void upload_ibec(void) {
	upload_image(Firmware.item[IBEC], 0, 1, 0);
	client = irecv_reconnect(client, 10);
    irecv_send_command(client, "go");
}

void upload_iboot(void) {
    upload_image(Firmware.item[IBOOT], 0, 1, 0);
    client = irecv_reconnect(client, 10);
    irecv_send_command(client, "go");
}

int jailbreak(void) {
	irecv_error_t err = IRECV_E_SUCCESS;
	AbstractFile *plistFile;
	Dictionary *bundle;
	Dictionary *temporaryDict;
	StringValue *urlKey = NULL;
	int i, error;
	char *processedname;

	/* obviously jailbreaking */
	jailbreaking = true;

	DPRINT("Initializing libirecovery\n");
	irecv_init();

#ifndef __APPLE__
	irecv_set_debug_level(3);
#endif

	/* If recovery loop fix */
	if(autoboot) {
		fix_recovery_loop();
	}

	if ((plistFile =
		 createAbstractFileFromFile(fopen(plist, "rb"))) != NULL) {
		plist = (char *)malloc(plistFile->getLength(plistFile));
		plistFile->read(plistFile, plist,
				plistFile->getLength(plistFile));
		plistFile->close(plistFile);
		info = createRoot(plist);
	} else if ((pwndfu == false) &&
		   (plistFile =
			createAbstractFileFromFile(fopen(plist, "rb"))) == NULL) {
		FATAL("plist must be specified in this mode!\n\n");
	}

	/* Initialize Firmware structure */
	memset(&Firmware, 0, sizeof(firmware));
	Firmware.items = sizeof(image_names) / sizeof(char *);
	Firmware.item = malloc(Firmware.items * sizeof(firmware_item));

	if (Firmware.item == NULL) {
		FATAL("Unable to allocate memory for decryption keys!\n");
	}

	memset(Firmware.item, 0, Firmware.items * sizeof(firmware_item));

	bundle = (Dictionary *) getValueByKey(info, "FirmwareKeys");
	if (bundle != NULL) {
		for (i = 0; i < Firmware.items; i++) {
			Dictionary *entry = (Dictionary *) getValueByKey(bundle,
									 image_names
									 [i]);
			if (entry != NULL) {
				StringValue *key = NULL, *iv = NULL, *name =
					NULL, *vfkey = NULL;

				key =
					(StringValue *) getValueByKey(entry, "Key");
				iv = (StringValue *) getValueByKey(entry, "IV");
				name =
					(StringValue *) getValueByKey(entry,
								  "FileName");
				vfkey =
					(StringValue *) getValueByKey(entry,
								  "VFDecryptKey");

				if (key)
					Firmware.item[i].key = key->value;

				if (iv)
					Firmware.item[i].iv = iv->value;

				if (name)
					Firmware.item[i].name = name->value;

				if (vfkey)
					Firmware.item[i].vfkey = vfkey->value;

				DPRINT("[plist] (%s %s %s %s) [%s %d]\n",
					   Firmware.item[i].key,
					   Firmware.item[i].iv,
					   Firmware.item[i].name,
					   Firmware.item[i].vfkey,
					   image_names[i], i);

			}
		}
	}

out:

	/* Change working directory to ${PREFIX}/var */
	error = chdir(RUNDIR);
	if(error < 0) {
		FATAL("Failed to change working directory.\n");
	}

	/* Poll for DFU mode */
	while (poll_device(DFU)) {
		sleep(1);
	}

	err = irecv_get_device(client, &device);
	if (device == NULL || device->index == DEVICE_UNKNOWN) {
		FATAL("[!] USB communications error.\n");
	}

	identify_device();

	/* exploit device */
	exploit_device();

	if(pwndfu) {
		STATUS("[!] Operation completed.\n");
		exit(0);
	}

	temporaryDict =
		(Dictionary *) getValueByKey(info, "FirmwareInfo");

	if (temporaryDict != NULL)
		urlKey = (StringValue *) getValueByKey(temporaryDict, "URL");
	if (urlKey != NULL) {
		char *p = NULL, dup[256];
		int len;

		memset(dup, 0, 256);

		if(!gp_payload)
			device->url = urlKey->value;
		if (temporaryDict != NULL)
			urlKey =
				(StringValue *) getValueByKey(temporaryDict, "URL");
		if (urlKey != NULL)
			p = urlKey->value;
		if (!p)
			goto out;

		p = strstr(p, device->product);
		if (!p)
			goto out;

		len = strlen(p);
		if (len <= 0)
			goto out;

		strncpy(dup, p, len - sizeof("Restore.ipsw"));
		version = strdup(dup);

	}


	STATUS("[*] Uploading stage zero (iBSS)...\n");
    upload_ibss();

	STATUS("[*] Uploading stage one (iBEC)...\n");
	upload_ibec();

	if(iboot == true)  {
		STATUS("[*] Uploading stage two (iBoot)...\n");
        upload_iboot();
		exit(0);
	}


	STATUS("[*] Waiting for reset...\n");
	client = irecv_reconnect(client, 2);
	irecv_reset(client);
	irecv_set_interface(client, 0, 0);
	irecv_set_interface(client, 1, 1);

	if (pwnrecovery) {
		FATAL
			("Device has a pwned iBEC uploaded. Do whatever you want \n");
	}

	/* upload logo */

	STATUS("[*] Uploading boot logo...\n");
    upload_apple_logo();

	STATUS("[*] Uploading device tree...\n");
    upload_devicetree();

	STATUS("[*] Uploading kernel...\n");
    upload_kernel();

	STATUS("[*] Booting.\n");
	DPRINT("Booting kernel.\n");
	irecv_send_command(client, "bootx");
	/* Done? */
}

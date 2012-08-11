/*
 * opensn0w File Operations/Device Stuff
 */

#include "core.h"

/*!
 * \fn int is_compatible(void)
 * \brief Check if device is in DFU mode.
 */
int is_compatible(void)
{
	irecv_get_device(client, &device);
	if (device == NULL) {
		DPRINT("Null device, continuing to probe.\n");
		return FALSE;
	}
	DPRINT("Identified device as %s\n", device->product);
	return TRUE;
}

/*!
 * \fn int poll_device(int mode)
 * \brief Poll device for specified \a mode.
 *
 * \param mode Required mode (DFU/iBoot).
 */

int poll_device(int mode)
{
	irecv_error_t err;
	static int ___try;	/* actually a msvc keyword, __try */

	err = irecv_open(&client);
	if (err != IRECV_E_SUCCESS) {
		STATUS("Connect the device in %s mode. [%u]\r",
			   mode_to_string(mode), ___try);
		fflush(stdout);
		___try++;
		return 1;
	}

	switch (client->mode) {
	case kDfuMode2:
	case kDfuMode:
		if (mode == DFU) {
			return 0;
		}
	case kRecoveryMode2:
		if (mode == RECOVERYMODE) {
			return 0;
		}
	default:
		STATUS("Connect the device in %s mode. [%u]\r",
			   mode_to_string(mode), ___try);
		irecv_close(client);
		___try++;
		return 1;
	}

	return 0;
}

/*!
 * \fn int send_command(char *name)
 * \brief Send \a name as an iBoot command to device.
 *
 * \param name iBoot command.
 */

int send_command(char *name)
{
	irecv_error_t err;

	DPRINT("Initializing libirecovery...\n");
	irecv_init();

#ifndef __APPLE__
	irecv_set_debug_level(3);
#endif

	while (1) {
		err = irecv_open(&client);
		if (err != IRECV_E_SUCCESS) {
			STATUS("Connect the device. err %d\n", err);
			sleep(1);
		} else if (err == IRECV_E_SUCCESS) {
			break;
		}
	};

	if (!client) {
		FATAL("Null client!\n");
	}

	STATUS("[*] Sending command \"%s\" to iBoot...\n", name);
	irecv_send_command(client, name);

	exit(err);
	return err;
}

/*!
 * \fn int send_filename(char *name)
 * \brief Send \a name as file to device.
 *
 * \param name iBoot file.
 */

int send_file(char *name)
{
	irecv_error_t err;

	DPRINT("Initializing libirecovery...\n");
	irecv_init();

#ifndef __APPLE__
	irecv_set_debug_level(3);
#endif

	while (1) {
		err = irecv_open(&client);
		if (err != IRECV_E_SUCCESS) {
			STATUS("Connect the device. err %d\n", err);
			sleep(1);
		} else if (err == IRECV_E_SUCCESS) {
			break;
		}
	};

	if (!client) {
		FATAL("Null client!\n");
	}

	if (client->mode != kDfuMode) {
		err = irecv_send_file(client, name, 0);
	} else {
		err = irecv_send_file(client, name, 1);
	}

	if (err != IRECV_E_SUCCESS) {
		FATAL("%s\n", irecv_strerror(err));
	}

	exit(err);
	return err;
}

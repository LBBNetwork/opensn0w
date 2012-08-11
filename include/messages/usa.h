/*
 * English localization for opensn0w-core.
 */ 


#define USAGE_MESSAGE \
			"Usage: %s [OPTIONS]\n" \
			"Jailbreak an iOS device, this includes the iPhone, iPad, iPod touch and\n" \
			"Apple TV 2G.\n\n" \
			"Currently supported devices are: %s\n" \
			"Not really supported devices are: %s\n" \
			"\n" \
			"Options:\n" \
			"   -a [boot-args]     Set device boot-args for boot.\n" \
			"   -A                 Set auto-boot. (Kick out of recovery.)\n" \
			"   -b bootlogo.img3   Use specified bootlogo img3 file during startup.\n" \
			"   -B                 Dump SecureROM to bootrom.bin (works on limera1n devices only.)\n" \
			"   -C [command]       Send command to device.\n" \
			"   -d                 Just pwn dfu mode.\n" \
			"   -D                 Dry run, still requires DFU mode.\n" \
			"   -g                 Use greenpois0n payload.\n" \
			"   -f [file]          Use specified configuration file.\n" \
			"   -h                 Help.\n" \
			"   -i ipsw            Get necessary files from a remote IPSW.\n" \
			"   -I                 Apple TV 2G users, boot kernelcache on disk using iBoot with boot-args injected.\n" \
			"   -j                 Jailbreak.\n" \
			"   -k kernelcache     Boot using specified kernel.\n" \
			"   -n                 Do not display intro banner.\n" \
			"   -p plist           Use firmware plist\n" \
			"   -r ramdisk.dmg     Boot specified ramdisk.\n" \
			"   -R                 Just boot into pwned recovery mode.\n" \
			"   -S [file]          Send file to device.\n" \
			"   -s                 Start iRecovery recovery mode shell.\n" \
			"   -v                 Verbose mode. Useful for debugging.\n" \
			"   -X                 Download all files from plist.\n" \
			"   -Y                 Use the SHAtter exploit, but for god's sake its broken.\n" \
			"\n" \
			"Exit status:\n" \
			"  0  if OK,\n" \
			" -1  if severe issues.\n" \
			"\n" \
			"Report %s bugs to EMAIL\n" \
			"%s homepage: <http://www.opensn0w.com>\n" \
			"For complete documentation, see the UNIX manual.\n\n"


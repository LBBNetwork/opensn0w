/*
 * File patch logic.
 */

#include "core.h"
#include "xpwntool/xpwn/nor_files.h"

/* globals */
extern Dictionary *firmwarePatches, *patchDict, *info;
extern char* version;

/*!
 * \fn Dictionary *get_key_dictionary_from_bundle(char *member)
 * \brief Get \a member from firmware property list
 * 
 * \param member Plist Member
 */
 
Dictionary *get_key_dictionary_from_bundle(char *member)
{
	firmwarePatches = (Dictionary *) getValueByKey(info, "FirmwareKeys");
	patchDict = (Dictionary *) firmwarePatches->values;

	if (!strcasecmp(patchDict->dValue.key, member))
		return patchDict;

	while (patchDict != NULL) {
		if (!strcasecmp(patchDict->dValue.key, member))
			return patchDict;

		patchDict = (Dictionary *) patchDict->dValue.next;
	}

	return NULL;
}

/*!
 * \fn int patch_file(char *filename)
 * \brief Patch firmware binary
 * 
 * \param filename Filename to decrypt and patch.
 */
 void patch_image_load(char*p, int l);
int patch_file(char *filename)
{
	AbstractFile *template = NULL, *inFile, *certificate =
	    NULL, *outFile, *newFile, *outFile2, *newFile2;
	unsigned int *key = NULL;
	unsigned int *iv = NULL;
	char *inData;
	size_t inDataSize;
	char *buffer;
	Dictionary *data;
	char *buf;
	char *dup;
	char *tokenizedname;
	StringValue *keyValue;
	StringValue *ivValue;

	if(!filename)
		return -1;

	dup = strndup(filename, 255);

	template = createAbstractFileFromFile(fopen(filename, "rb"));

	/* open base template */
	if (!template) {
		ERR("Cannot open template.\n");
		return -1;
	}

	DPRINT("getting keys\n");

	tokenizedname = strtok(dup, ".,");

	data = get_key_dictionary_from_bundle(tokenizedname);
	keyValue = (StringValue *) getValueByKey(data, "Key");
	ivValue = (StringValue *) getValueByKey(data, "IV");

	if (keyValue) {
		size_t bytes;
		DPRINT("Key for %s: %s\n", filename, keyValue->value);
		hexToInts(keyValue->value, &key, &bytes);
	}

	if (ivValue) {
		size_t bytes;
		DPRINT("IV for %s: %s\n", filename, ivValue->value);
		hexToInts(ivValue->value, &iv, &bytes);
	}

	/* open file */
	inFile =
	    openAbstractFile2(createAbstractFileFromFile(fopen(filename, "rb")),
			      key, iv);
	if (!inFile) {
		DPRINT("Cannot open %s.\n", filename);
		return -1;
	}

	/* read it */
	DPRINT("reading data from initial abstract\n");
	inDataSize = (size_t) inFile->getLength(inFile);
	inData = (char *)malloc(inDataSize);
	inFile->read(inFile, inData, inDataSize);
	inFile->close(inFile);

	/* zero buffer */
	buffer = malloc(256);
	if (!buffer) {
		ERR("Cannot allocate memory\n");
		return -1;
	}

	/* zero buffer */
	buf = malloc(256);
	if (!buf) {
		ERR("Cannot allocate memory\n");
		return -1;
	}

	snprintf(buffer, strlen(filename) + 8 + strlen(version), "%s.pwn", filename);
	snprintf(buf, strlen(filename) + 8 + strlen(version), "%s.dec", filename);
	unlink(buf);
	unlink(buffer);

	/* open output */
	DPRINT("opening %s (output) as an abstract file\n", filename);

	outFile = createAbstractFileFromFile(fopen(buffer, "wb"));
	if (!outFile) {
		DPRINT("Cannot open outfile\n");
		return -1;
	}

	outFile2 = createAbstractFileFromFile(fopen(buf, "wb"));
	if (!outFile2) {
		DPRINT("Cannot open outfile\n");
		return -1;
	}

	DPRINT("pwned file is %s, will upload later\n", buffer);

	newFile =
	    duplicateAbstractFile2(template, outFile, key, iv, certificate);
	if (!newFile) {
		DPRINT("Cannot open newfile\n");
		return -1;
	}

	newFile2 = outFile2;
	if (!newFile2) {
		DPRINT("Cannot open newfile\n");
		return -1;
	}

	/* pwn it 8) */
	DPRINT("pwning %s\n", filename);

	if (strcasestr(filename, "iBEC") || 
	    strcasestr(filename, "iBSS") ||
	    strcasestr(filename, "iBoot")) {
	        patch_replace(iboot_patches, (unsigned char*)inData, inDataSize - 128);
		//patch_image_load(inData, inDataSize);
	} else if (strcasestr(filename, "kernelcache")) {
       	 	patch_replace(kernel_patches, (unsigned char*)inData, inDataSize - 128);
	}
    
	/* write patched contents */
	DPRINT("writing pwned file\n");
	newFile2->write(newFile2, inData, inDataSize);
	newFile2->close(newFile2);
	newFile->write(newFile, inData, inDataSize);
	newFile->close(newFile);

	free(inData);

#ifndef MSVC_VER
	free(buffer);	/* I know this is strange, and malpractice, not freeing the buffer...
			   but on MSVC, and my Windows 7 installation, the heap handler tends to 
			   destroy the program state here. I have no clue why. -- acfrazier */
			/* addendum: At most, like 1kB will be lost. Does it really matter? */
#endif

	return 0;
}

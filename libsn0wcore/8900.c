/* opensn0w
 * An open-source jailbreaking utility.
 * Brought to you by rms, acfrazier & Maximus
 * Special thanks to iH8sn0w & MuscleNerd
 *
 * This file is from xpwn.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include "dprint.h"
#include "common.h"
#include <xpwn/8900.h>
#include <xpwn/img2.h>

unsigned char key837[] = {0x18, 0x84, 0x58, 0xA6, 0xD1, 0x50, 0x34, 0xDF, 0xE3, 0x86, 0xF2, 0x3B, 0x61, 0xD4, 0x37, 0x74};

void flipApple8900Header(Apple8900Header * header)
{
	FLIPENDIAN(header->magic);
	FLIPENDIANLE(header->unknown1);
	FLIPENDIANLE(header->sizeOfData);
	FLIPENDIANLE(header->footerSignatureOffset);
	FLIPENDIANLE(header->footerCertOffset);
	FLIPENDIANLE(header->footerCertLen);
	FLIPENDIANLE(header->unknown2);
	FLIPENDIANLE(header->epoch);
}

size_t read8900(AbstractFile * file, void *data, size_t len)
{
	Info8900 *info = (Info8900 *) (file->data);
	memcpy(data,
	       (void *)((uint8_t *) info->buffer + (uint32_t) info->offset),
	       len);
	info->offset += (size_t) len;
	return len;
}

size_t write8900(AbstractFile * file, const void *data, size_t len)
{
	Info8900 *info = (Info8900 *) (file->data);

	while ((info->offset + (size_t) len) > info->header.sizeOfData) {
		info->header.sizeOfData = info->offset + (size_t) len;
		info->buffer = realloc(info->buffer, info->header.sizeOfData);
	}

	memcpy((void *)((uint8_t *) info->buffer + (uint32_t) info->offset),
	       data, len);
	info->offset += (size_t) len;

	info->dirty = TRUE;

	return len;
}

int seek8900(AbstractFile * file, off_t offset)
{
	Info8900 *info = (Info8900 *) (file->data);
	info->offset = (size_t) offset;
	return 0;
}

off_t tell8900(AbstractFile * file)
{
	Info8900 *info = (Info8900 *) (file->data);
	return (off_t) info->offset;
}

off_t getLength8900(AbstractFile * file)
{
	Info8900 *info = (Info8900 *) (file->data);
	return info->header.sizeOfData;
}

void close8900(AbstractFile * file)
{
	unsigned char ivec[16];
	SHA_CTX sha_ctx;
	unsigned char md[20];
	unsigned char exploit_data[0x54] = { 0 };
	/*int align; */
	size_t origSize;
	uint32_t cksum;
	Info8900 *info = (Info8900 *) (file->data);

	if (info->dirty) {
		if (info->header.format == 3) {
			/* we need to block-align our data, or AES will break */
			origSize = info->header.sizeOfData;
			/* gotta break abstraction here, because Apple is mean */
			if (((Img2Header *) info->buffer)->signature ==
			    IMG2_SIGNATURE) {
				((Img2Header *) info->buffer)->dataLenPadded =
				    ((Img2Header *) info->
				     buffer)->dataLenPadded % 16 ==
				    0 ? ((Img2Header *) info->buffer)->
				    dataLenPadded
				    : ((((Img2Header *) info->
					 buffer)->dataLenPadded / 16) + 1) * 16;
				info->header.sizeOfData =
				    ((Img2Header *) info->
				     buffer)->dataLenPadded +
				    sizeof(Img2Header);

				cksum =
				    crc32(0, (unsigned char *)info->buffer,
					  0x64);
				FLIPENDIANLE(cksum);
				((Img2Header *) info->buffer)->header_checksum =
				    cksum;
			}

			info->header.sizeOfData =
			    (info->header.sizeOfData) % 16 ==
			    0 ? info->
			    header.sizeOfData : ((info->header.sizeOfData / 16)
						 + 1) * 16;
			if (info->header.sizeOfData != origSize) {
				info->buffer =
				    realloc(info->buffer,
					    info->header.sizeOfData);
				memset((void *)((uint8_t *) info->buffer +
						origSize), 0,
				       info->header.sizeOfData - origSize);
			}
		}

		info->header.footerSignatureOffset = info->header.sizeOfData;
		info->header.footerCertOffset =
		    info->header.footerSignatureOffset + 0x80;

		if (info->header.format == 3) {
			memset(ivec, 0, 16);
			AES_cbc_encrypt(info->buffer, info->buffer,
					info->header.sizeOfData,
					&(info->encryptKey), ivec, AES_ENCRYPT);
		}

		info->file->seek(info->file, sizeof(info->header));
		info->file->write(info->file, info->buffer,
				  info->header.sizeOfData);
		info->file->seek(info->file,
				 sizeof(info->header) +
				 info->header.footerSignatureOffset);
		info->file->write(info->file, info->footerSignature, 0x80);
		info->file->seek(info->file,
				 sizeof(info->header) +
				 info->header.footerCertOffset);

		if (info->exploit) {
			info->footerCertificate[0x8be] = 0x9F;
			info->footerCertificate[0xb08] = 0x55;
		}

		info->file->write(info->file, info->footerCertificate,
				  info->header.footerCertLen);

		if (info->exploit) {
			DPRINT("Adding exploit data.\n");
			info->header.footerCertLen = 0xc5e;
			exploit_data[0x30] = 0x01;
			exploit_data[0x50] = 0xEC;
			exploit_data[0x51] = 0x57;
			exploit_data[0x53] = 0x20;
			info->file->write(info->file, exploit_data,
					  sizeof(exploit_data));
		}

		flipApple8900Header(&(info->header));
		SHA1_Init(&sha_ctx);
		SHA1_Update(&sha_ctx, &(info->header), 0x40);
		SHA1_Final(md, &sha_ctx);

		memset(ivec, 0, 16);
		AES_cbc_encrypt(md,
				(unsigned char *)&(info->
						   header.headerSignature),
				0x10, &(info->encryptKey), ivec, AES_ENCRYPT);

		info->file->seek(info->file, 0);
		info->file->write(info->file, &(info->header),
				  sizeof(info->header));
	}

	free(info->footerCertificate);
	free(info->buffer);
	info->file->close(info->file);
	free(info);
	free(file);
}

void exploit8900(AbstractFile * file)
{
	Info8900 *info = (Info8900 *) (file->data);
	info->exploit = TRUE;
	info->dirty = TRUE;
}

AbstractFile *createAbstractFileFrom8900(AbstractFile * file)
{
	Info8900 *info;
	unsigned char ivec[16];
	AbstractFile *toReturn;

	if (!file) {
		return NULL;
	}

	info = (Info8900 *) malloc(sizeof(Info8900));
	info->file = file;
	file->seek(file, 0);
	file->read(file, &(info->header), sizeof(info->header));
	flipApple8900Header(&(info->header));
	if (info->header.magic != SIGNATURE_8900) {
		free(info);
		return NULL;
	}

	AES_set_encrypt_key(key837, sizeof(key837) * 8,
			    &(info->encryptKey));
	AES_set_decrypt_key(key837, sizeof(key837) * 8,
			    &(info->decryptKey));

	info->buffer = malloc(info->header.sizeOfData);
	file->read(file, info->buffer, info->header.sizeOfData);

	if (info->header.format == 3) {
		memset(ivec, 0, 16);
		AES_cbc_encrypt(info->buffer, info->buffer,
				info->header.sizeOfData, &(info->decryptKey),
				ivec, AES_DECRYPT);
	}

	info->dirty = FALSE;
	info->exploit = FALSE;

	info->offset = 0;

	file->seek(file,
		   sizeof(info->header) + info->header.footerSignatureOffset);
	file->read(file, info->footerSignature, 0x80);

	info->footerCertificate =
	    (unsigned char *)malloc(info->header.footerCertLen);
	file->seek(file, sizeof(info->header) + info->header.footerCertOffset);
	file->read(file, info->footerCertificate, info->header.footerCertLen);

	toReturn = (AbstractFile *) malloc(sizeof(AbstractFile));
	toReturn->data = info;
	toReturn->read = read8900;
	toReturn->write = write8900;
	toReturn->seek = seek8900;
	toReturn->tell = tell8900;
	toReturn->getLength = getLength8900;
	toReturn->close = close8900;
	toReturn->type = AbstractFileType8900;
	return toReturn;
}

void replaceCertificate8900(AbstractFile * file, AbstractFile * certificate)
{
	Info8900 *info = (Info8900 *) (file->data);
	info->header.footerCertLen = certificate->getLength(certificate);
	if (info->footerCertificate != NULL) {
		free(info->footerCertificate);
	}
	info->footerCertificate =
	    (unsigned char *)malloc(info->header.footerCertLen);
	certificate->read(certificate, info->footerCertificate,
			  info->header.footerCertLen);
	info->dirty = TRUE;
}

AbstractFile *duplicate8900File(AbstractFile * file, AbstractFile * backing)
{
	Info8900 *info;
	unsigned char *copyCertificate;
	AbstractFile *toReturn;

	if (!file) {
		return NULL;
	}

	info = (Info8900 *) malloc(sizeof(Info8900));
	memcpy(info, file->data, sizeof(Info8900));

	info->file = backing;
	info->buffer = malloc(1);
	info->header.sizeOfData = 0;
	info->dirty = TRUE;
	info->offset = 0;
	copyCertificate = (unsigned char *)malloc(info->header.footerCertLen);
	memcpy(copyCertificate, info->footerCertificate,
	       info->header.footerCertLen);
	info->footerCertificate = copyCertificate;

	toReturn = (AbstractFile *) malloc(sizeof(AbstractFile));
	toReturn->data = info;
	toReturn->read = read8900;
	toReturn->write = write8900;
	toReturn->seek = seek8900;
	toReturn->tell = tell8900;
	toReturn->getLength = getLength8900;
	toReturn->close = close8900;
	toReturn->type = AbstractFileType8900;
	return toReturn;
}

/**
  * GreenPois0n iRecovery - irecovery.c
  * Copyright (C) 2010-2011 Chronic-Dev Team
  * Copyright (C) 2010-2011 Joshua Hill
  * Copyright (C) 2008-2011 Nicolas Haunold
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
  *
  * Adapted for opensn0w.
  **/

#include "core.h"

#ifdef HAVE_LIBPTHREAD
#include <pthread.h>
#endif

#ifdef HAVE_LIBREADLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

#define FILE_HISTORY_PATH ".opensn0w_irecovery_sh"

static unsigned int quit = 0;

void print_progress_bar(double progress);
int received_cb(irecv_client_t client, const irecv_event_t * event);
int progress_cb(irecv_client_t client, const irecv_event_t * event);
int precommand_cb(irecv_client_t client, const irecv_event_t * event);
int postcommand_cb(irecv_client_t client, const irecv_event_t * event);
void init_shell(irecv_client_t client);

int irecovery_shell_initialize()
{
	irecv_error_t err;

	printf("opensn0w recovery shell\n"
               "=======================\n");

	printf("Initializing libirecovery...\n");
	irecv_init();

#ifndef __APPLE__
	irecv_set_debug_level(3);
#endif

	while ((err = irecv_open(&client)) != IRECV_E_SUCCESS) {
		if (err != IRECV_E_SUCCESS) {
			printf("Connect the device. err %d\n", err);
			sleep(1);
		} else if (err == IRECV_E_SUCCESS) {
			break;
		}
	};

	if (!client) {
		printf("Null client!\n");
		exit(-1);
	}

	/* Check the device */
	printf("iBoot information: %s\n", client->serial);

	printf("Starting shell...\n");

	irecv_reset(client);
	client = irecv_reconnect(client, 2);
	irecv_set_interface(client, 0, 0);
	irecv_set_interface(client, 1, 1);

	init_shell(client);

	client = irecv_reconnect(client, 2);
	exit(0);
}

void load_command_history()
{
#ifdef HAVE_LIBREADLINE
	read_history(FILE_HISTORY_PATH);
#endif
}

void append_command_to_history(char *cmd)
{
#ifdef HAVE_LIBREADLINE
	add_history(cmd);
	write_history(FILE_HISTORY_PATH);
#endif
}

void *recv_thread(void* c) {
	irecv_error_t error = 0;
	irecv_client_t client = c;
	for(;;) {
		error = irecv_receive(client);
		if (error != IRECV_E_SUCCESS) {
			printf("%s\n", irecv_strerror(error));
			return NULL;
		}
	}
	return NULL;
}

void init_shell(irecv_client_t client)
{
	irecv_error_t error = 0;
	char *cmd = NULL;

	load_command_history();
	irecv_event_subscribe(client, IRECV_PROGRESS, &progress_cb, NULL);
	irecv_event_subscribe(client, IRECV_RECEIVED, &received_cb, NULL);
	irecv_event_subscribe(client, IRECV_PRECOMMAND, &precommand_cb, NULL);
	irecv_event_subscribe(client, IRECV_POSTCOMMAND, &postcommand_cb, NULL);
#ifdef HAVE_LIBPTHREAD
	pthread_t thread;
	pthread_attr_t attr;
	struct sched_param parm;

	pthread_attr_init(&attr);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	pthread_create(&thread, &attr, recv_thread, (void*)client);
	parm.sched_priority = 10;
	pthread_setschedparam(thread, SCHED_FIFO, &parm);
#endif

	while (!quit) {
		char *cmd;
#ifndef HAVE_LIBPTHREAD
		error = irecv_receive(client);
		if (error != IRECV_E_SUCCESS) {
			printf("%s\n", irecv_strerror(error));
			break;
		}
#endif
#ifdef HAVE_LIBREADLINE
		cmd = readline("> ");
#else
		printf("> ");
		cmd = malloc(512);
		if(!cmd) {
			abort();
		}
		memset(cmd, 0, 512);
		cmd = fgets(cmd, 512, stdin);
#endif
		if (cmd && *cmd) {
			error = irecv_send_command(client, cmd);
			if (error != IRECV_E_SUCCESS) {
				quit = 1;
			}

			append_command_to_history(cmd);
			free(cmd);
		}
	}
}

int received_cb(irecv_client_t client, const irecv_event_t * event)
{
	if (event->type == IRECV_RECEIVED) {
		int i = 0;
		int size = event->size;
		char *data = (char *)event->data;
		for (i = 0; i < size; i++) {
			printf("%c", data[i]);
		}
	}
	return 0;
}

int precommand_cb(irecv_client_t client, const irecv_event_t * event)
{
	return 0;
}

int postcommand_cb(irecv_client_t client, const irecv_event_t * event)
{
	char *value = NULL;
	char *action = NULL;
	char *command = NULL;
	char *argument = NULL;
	irecv_error_t error = IRECV_E_SUCCESS;

	if (event->type == IRECV_POSTCOMMAND) {
		command = strdup(event->data);
		action = strtok(command, " ");
		if (!strcmp(action, "getenv")) {
			argument = strtok(NULL, " ");
			error = irecv_getenv(client, argument, &value);
			if (error != IRECV_E_SUCCESS) {
				printf("%s\n", irecv_strerror(error));
				free(command);
				return error;
			}
			printf("%s\n", value);
			free(value);
		}

		if (!strcmp(action, "reboot")) {
			quit = 1;
		}
	}

	if (command)
		free(command);
	return 0;
}

int progress_cb(irecv_client_t client, const irecv_event_t * event)
{
	if (event->type == IRECV_PROGRESS) {
		print_progress_bar(event->progress);
	}
	return 0;
}

void print_progress_bar(double progress)
{
	int i = 0;
	if (progress < 0) {
		return;
	}

	if (progress > 100) {
		progress = 100;
	}

	printf("\r[");
	for (i = 0; i < 50; i++) {
		if (i < progress / 2) {
			printf("=");
		} else {
			printf(" ");
		}
	}

	printf("] %3.1f%%", progress);
	fflush(stdout);
	if (progress == 100) {
		printf("\n");
	}
}

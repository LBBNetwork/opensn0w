/*
 * Platform stuff
 */

#include "core.h"

PartialZipProgressCallback callback = NULL;

/*!
 * \fn bool file_exists(const char *fileName)
 * \brief Check if file exists.
 *
 * \param fileName filename.
 */

bool file_exists(const char *fileName)
{
	struct stat buf;
	return !stat(fileName, &buf);
}

/*!
 * \fn char *mode_to_string(int mode)
 * \brief Required \a mode to string.
 *
 * \param mode Required mode (DFU/iBoot)
 */

char *mode_to_string(int mode)
{
	switch (mode) {
	case DFU:
		return "DFU";
	case RECOVERYMODE:
		return "Recovery/iBoot";
	default:
		return "UNKNOWN";
	}
}

/*!
 * \fn size_t writeData(void *ptr, size_t size, size_t mem, FILE * stream)
 * \brief Wrapper for fwrite.
 *
 * \param ptr Buffer for data.
 * \param size Size of written data.
 * \param mem Number of members of written data.
 * \param stream File descriptor structure.
 */

size_t writeData(void *ptr, size_t size, size_t mem, FILE * stream)
{
	size_t written;
	written = fwrite(ptr, size, mem, stream);
	return written;
}

/*!
 * \fn int fetch_image(const char *path, const char *output)
 * \brief Fetch a specified image from the Internet.
 *
 * \param path Filename in zip.
 * \param output Output filename.
 */

int fetch_image(const char *path, const char *output)
{
	DPRINT("Fetching %s...\n", path);
	STATUS("[*] Fetching %s...\n", path);
	if (download_file_from_zip(device->url, path, output, callback) != 0) {
		ERR("Unable to fetch %s\n", path);
		return -1;
	}

	return 0;
}

/*
 * win32 support, from wine
 */

#ifdef _WIN32

#include <psapi.h>

struct pid_close_info {
	DWORD pid;
	BOOL found;
};

static BOOL CALLBACK pid_enum_proc(HWND hwnd, LPARAM lParam)
{
	struct pid_close_info *info = (struct pid_close_info *)lParam;
	DWORD hwnd_pid;

	GetWindowThreadProcessId(hwnd, &hwnd_pid);

	if (hwnd_pid == info->pid) {
		//PostMessageW(hwnd, WM_CLOSE, 0, 0);
		info->found = TRUE;
	}

	return TRUE;
}

static DWORD *enumerate_processes(DWORD * list_count)
{
	DWORD *pid_list, alloc_bytes = 1024 * sizeof(*pid_list), needed_bytes;

	pid_list = HeapAlloc(GetProcessHeap(), 0, alloc_bytes);
	if (!pid_list)
		return NULL;

	for (;;) {
		DWORD *realloc_list;

		if (!EnumProcesses(pid_list, alloc_bytes, &needed_bytes)) {
			HeapFree(GetProcessHeap(), 0, pid_list);
			return NULL;
		}

		/* EnumProcesses can't signal an insufficient buffer condition, so the
		 * only way to possibly determine whether a larger buffer is required
		 * is to see whether the written number of bytes is the same as the
		 * buffer size. If so, the buffer will be reallocated to twice the
		 * size. */
		if (alloc_bytes != needed_bytes)
			break;

		alloc_bytes *= 2;
		realloc_list =
			HeapReAlloc(GetProcessHeap(), 0, pid_list, alloc_bytes);
		if (!realloc_list) {
			HeapFree(GetProcessHeap(), 0, pid_list);
			return NULL;
		}
		pid_list = realloc_list;
	}

	*list_count = needed_bytes / sizeof(*pid_list);
	return pid_list;
}

static BOOL get_process_name_from_pid(DWORD pid, WCHAR * buf, DWORD chars)
{
	HANDLE process;
	HMODULE module;
	DWORD required_size;

	process =
		OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE,
			pid);
	if (!process)
		return FALSE;

	if (!EnumProcessModules
		(process, &module, sizeof(module), &required_size)) {
		CloseHandle(process);
		return FALSE;
	}

	if (!GetModuleBaseNameW(process, module, buf, chars)) {
		CloseHandle(process);
		return FALSE;
	}

	CloseHandle(process);
	return TRUE;
}

bool is_process_running(WCHAR * process_name)
{
	DWORD *pid_list, pid_list_size;
	DWORD index;
	BOOL found_process = FALSE;
	pid_list = enumerate_processes(&pid_list_size);

	if (!pid_list) {
		return FALSE;
	}

	for (index = 0; index < pid_list_size; index++) {
		WCHAR process_name2[MAX_PATH];

		if (get_process_name_from_pid
			(pid_list[index], process_name2, MAX_PATH) == TRUE
			&& !wcscasecmp(process_name2, process_name)) {
			struct pid_close_info info = { pid_list[index] };

			found_process = TRUE;
			EnumWindows(pid_enum_proc, (LPARAM) & info);
		}

	}
	return found_process;
}

#endif

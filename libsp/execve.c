#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libsp.h"


/*
 * stack dump :
 *
 * [  file  ]
 * [argv[0] ]
 * [argv[1] ]
 * [........]
 * [  NULL  ]
 * [*argv[0]]
 * [*argv[1]]
 *
 * envp not supported at the moment
 */

int sp_execve(const char *file, char * const argv[], char * const envp[])
{
	int i;
	char *data, *arg_data;
	unsigned long *arg_pointer;
	unsigned long esp, esp_off;
	unsigned long argc, data_len, file_len;
	struct syscall_regs regs;

	argc = 0;
	
	file_len = strlen(file) + 1;
	data_len = file_len + sizeof(unsigned long);
	while (argv[argc]) {
		data_len += strlen(argv[argc]) + 1 + sizeof(unsigned long);
		argc++;
	}


	SEND_LEN(sizeof(regs) + data_len);
	GET_ESP(&esp);

        data = malloc(data_len);
	if (!data)
		return -1;

	strncpy(data, file, file_len);
	arg_pointer = (unsigned long *)(data + file_len);
	arg_data = (char *)&arg_pointer[argc + 1];

	esp_off = file_len + (argc + 1) * sizeof(unsigned long);
	for (i = 0; i < argc; i++) {
		int len = strlen(argv[i]) + 1;
		strncpy(arg_data, argv[i], len);
		arg_pointer[i] = esp + esp_off;
		esp_off += len;
		arg_data += len;
	}
	arg_pointer[argc] = 0;
		
	
	SET_REGS(&regs, __NR_execve, esp, esp + file_len, 0, 0, 0);
	SEND_REGS(&regs);
	SEND_DATA(data, data_len);
	RECV_REGS(&regs);
	RECV_DATA(data, data_len);

	free(data);

	return regs.eax;
}

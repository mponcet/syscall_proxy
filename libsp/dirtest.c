#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>   
#include <sys/syscall.h>

#define NAME_MAX 255
struct dirent {
        long  d_ino;              /* inode number */
        off_t d_off;              /* offset to this old_linux_dirent */
        unsigned short d_reclen;  /* length of this d_name */
        char  d_name[NAME_MAX+1]; /* filename (null-terminated) */
}__attribute__ ((packed));


int main(int argc, char * argv)
{
	int ret, fd;
	struct dirent dir;

	printf("sizeof(struct dirent): %d\n", sizeof(struct dirent));
	fd=open("/", O_RDONLY);
	if(fd==-1)
	{
		perror("open ");
		return 0;
	}

	ret=syscall(__NR_readdir, fd, &dir, 0);
	do
	{
		printf("%s\n", dir.d_name);
		ret=syscall(__NR_readdir, fd, &dir, 0);
		if(ret==-1)
		{
			perror("readdir ");
			close(fd);
			return 0;	
		}
	}while(ret!=0);
		
	close(fd);
	return 0;
}

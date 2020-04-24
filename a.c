#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
// #include <time.h>
#include <sys/statfs.h>

static  const  char *dirpath = "/home/excel/Documents";
char code[100] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";

void logsytem(char *level, char *cmd, char *desc){
    FILE *fp;
    int flag2 = 1;
    if((fp = fopen ( "/home/excel/fs.log", "r" ) ) == NULL)
        flag2 = 0;
    fclose(fp);
    if(flag2 == 0){
        fp = fopen ( "/home/excel/fs.log", "w+" );
        fclose(fp);
    }
    char log[200],times[50];
    strcpy(log,level);
    // time_t t = time(NULL);
    // struct tm tm = *localtime(&t);
    // sprintf(times,"::%02d%02d%02d-%02d:%02d:%02d::", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    // strcat(log,times);
    strcat(log,cmd);
    strcat(log,"::");
    strcat(log,desc);
    fp = fopen ("/home/excel/fs.log","a");
    fprintf(fp, "%s\n",log);
    fclose(fp);

}

// Enkripsi
void encrypt1 (char str[1000]) {

    printf("%s\n", str);
	
	int i, j;

	for (i = 0; i < strlen(str); i++) {
		for (j = 0; j < strlen(code); j++) {
			if(str[i] == code[j]) {
				str[i] = code[(j+10)%strlen(code)];
				break;
 			}
		}
	}
	printf("hasil enkripsi : %s\n", str);

}

void dencrypt (char str[1000]) {

    printf("%s\n", str);
	
	int i, j;

	for (i = 0; i < strlen(str); i++) {
		for (j = 0; j < strlen(code); j++) {
			if(str[i] == code[j]) {
                if(j-10 < 0) j =+ strlen(code);
				str[i] = code[(j-10)%strlen(code)];
				break;
 			}
		}
	}
	printf("hasil enkripsi : %s\n", str);

}

// Enkripsi Versi 1
int enkripsi1 (char path[1000]) {
    DIR *dp;
    struct dirent *de;
    dp = opendir(path);
    if (dp == NULL) return -errno;
    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        char name[100];
        strcpy(name, de->d_name);
        printf("de->name : %s\n", name);
        if (strcmp(".", de->d_name) != 0 && strcmp("..", de->d_name) != 0 && de->d_type == 4){
            char folder[100];
            sprintf(folder, "%s/%s", path, name);
            enkripsi1(folder);
            encrypt1(name);
            printf("foldername : %s\n", name);
        }
        if (de->d_type == 8) {
            encrypt1(name);
            printf("filename : %s\n", name);       
        }
    }
    closedir(dp);
    return 0;
}

static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
int res;
char fpath[1000];
sprintf(fpath,"%s%s",dirpath,path);
res = lstat(fpath, stbuf);
if (res == -1)
return -errno;
return 0;
}


static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,off_t offset, struct fuse_file_info *fi)
{
char fpath[1000];
if(strcmp(path,"/") == 0)
{
path=dirpath;
sprintf(fpath,"%s",path);
}
else sprintf(fpath, "%s%s",dirpath,path);
int res = 0;
DIR *dp;
struct dirent *de;
(void) offset;
(void) fi;
dp = opendir(fpath);
if (dp == NULL)
return -errno;
while ((de = readdir(dp)) != NULL) {
struct stat st;
memset(&st, 0, sizeof(st));
st.st_ino = de->d_ino;
st.st_mode = de->d_type << 12;
res = (filler(buf, de->d_name, &st, 0));
if(res!=0) break;
}
closedir(dp);
return 0;
}

  

static int xmp_readlink(const char *path, char *buf, size_t size)
{
    int res;

    res = readlink(path, buf, size - 1);
    if(res == -1)
        return -errno;

    buf[res] = '\0';
    return 0;
}


static int xmp_getdir(const char *path, fuse_dirh_t h, fuse_dirfil_t filler)
{
    DIR *dp;
    struct dirent *de;
    int res = 0;

    dp = opendir(path);
    if(dp == NULL)
        return -errno;

    while((de = readdir(dp)) != NULL) {
        res = filler(h, de->d_name, de->d_type);
        if(res != 0)
            break;
    }

    closedir(dp);
    return res;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
    int res;

    res = mknod(path, mode, rdev);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
    int res;
    printf("path %s\n",path);

    res = mkdir(path, mode);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_unlink(const char *path)
{
    int res;

    res = unlink(path);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_rmdir(const char *path)
{
    int res;

    res = rmdir(path);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_symlink(const char *from, const char *to)
{
    int res;

    res = symlink(from, to);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_rename(const char *from, const char *to)
{
    int res;

    res = rename(from, to);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_link(const char *from, const char *to)
{
    int res;

    res = link(from, to);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
    int res;

    res = chmod(path, mode);
    if(res == -1)
        return -errno;
    
    return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
    int res;

    res = lchown(path, uid, gid);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
    int res;
    
    res = truncate(path, size);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_utime(const char *path, struct utimbuf *buf)
{
    int res;
    
    res = utime(path, buf);
    if(res == -1)
        return -errno;

    return 0;
}


static int xmp_open(const char *path, int flags)
{
    int res;

    res = open(path, flags);
    if(res == -1) 
        return -errno;

    close(res);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset)
{
    int fd;
    int res;

    fd = open(path, O_RDONLY);
    if(fd == -1)
        return -errno;

    res = pread(fd, buf, size, offset);
    if(res == -1)
        res = -errno;
    
    close(fd);
    return res;
}

static int xmp_write(const char *path, const char *buf, size_t size,
                     off_t offset)
{
    int fd;
    int res;

    fd = open(path, O_WRONLY);
    if(fd == -1)
        return -errno;

    res = pwrite(fd, buf, size, offset);
    if(res == -1)
        res = -errno;
    
    close(fd);
    return res;
}

static int xmp_statfs(struct fuse_statfs *fst)
{
    struct statfs st;
    int rv = statfs("/",&st);
    if(!rv) {
    	fst->block_size  = st.f_bsize;
    	fst->blocks      = st.f_blocks;
    	fst->blocks_free = st.f_bavail;
    	fst->files       = st.f_files;
    	fst->files_free  = st.f_ffree;
    	fst->namelen     = st.f_namelen;
    }
    return rv;
}

static int xmp_release(const char *path, int flags)
{
    /* Just a stub.  This method is optional and can safely be left
       unimplemented */

    (void) path;
    (void) flags;
    return 0;
}

static int xmp_fsync(const char *path, int isdatasync)
{
    /* Just a stub.  This method is optional and can safely be left
       unimplemented */

    (void) path;
    (void) isdatasync;
    return 0;
}

static struct fuse_operations xmp_oper = {
    .getattr	= xmp_getattr,
    .readlink	= xmp_readlink,
    .getdir	= xmp_getdir,
    .mknod	= xmp_mknod,
    .mkdir	= xmp_mkdir,
    .symlink	= xmp_symlink,
    .unlink	= xmp_unlink,
    .rmdir	= xmp_rmdir,
    .rename	= xmp_rename,
    .link	= xmp_link,
    .chmod	= xmp_chmod,
    .chown	= xmp_chown,
    .truncate	= xmp_truncate,
    .utime	= xmp_utime,
    .open	= xmp_open,
    .read	= xmp_read,
    .write	= xmp_write,
    .statfs	= xmp_statfs,
    .release	= xmp_release,
    .fsync	= xmp_fsync,
    .readdir = xmp_readdir,
    
};
int  main(int  argc, char *argv[])
{
umask(0);
return fuse_main(argc, argv, &xmp_oper, NULL);

}
#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static  const  char *dirpath = "/home/excel/Documents";
char code[100] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";
char* level2 = "INFO";
char* level1 = "WARNING";
char* desc1;
char encv1[10] = "/encv1_";
char encv2[10] = "encv1_";

void logsytem(char *level, char *cmd, char *desc){
    FILE *fp;
    char log[200],times[100];
    strcpy(log,level);
    time_t t = time(NULL);
    struct tm *ptm = localtime(&t);
    
    sprintf(times,"::%02d%02d%02d-%02d:%02d:%02d::", ptm->tm_year + 1900 - 2000, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    strcat(log,times);
    strcat(log,cmd);
    strcat(log,"::");
    strcat(log,desc);
    fp = fopen ("/home/excel/Downloads/fs.log","a");
    fprintf(fp, "%s\n",log);
    fclose(fp);

}

// Enkripsi
void encrypt (char *str) {

	int i, j,flag = 1,flag2 = 1;
    for(i = strlen(str); i >= 0 ; --i){
        if(str[i] == '.'){
            flag2 = 0 ; flag = 0;
        }

    }

	for (i = strlen(str); i >= 0 ; --i) {
		for (j = 0; j < strlen(code); j++) {
			if(str[i] == code[j] && flag == 1) {
				str[i] = code[(j+10)%strlen(code)];
				break;
 			}
            if(str[i] == '.' && flag2 ==0){
                flag = 1 ; 
                i--;
                flag2 = 1;
            }
		}
	}

}

// denkripsi
void dencrypt (char *str) {
	
	int start, i, j,flag = 1,flag2 = 1,flag3 = 0;
    for(start = 0; start < strlen(str) ; start++){
        if(str[start] == 'e' && str[start+1] == 'n' && str[start+2] == 'c' && str[start+3] == 'v' && str[start+4] == '1' && str[start+5] == '_'){
            flag3 = 1;
        }
        if(str[start] == '/' && flag3 == 1){
            break;
        }
    }

    for(i = strlen(str); i >= start ; --i){
        if(str[i] == '.'){
            flag2 = 0 ; flag = 0;
        }
    }

	for (i = strlen(str); i >= start ; --i) {
		for (j = 0; j < strlen(code); j++) {
			if(str[i] == code[j] && flag == 1) {
                if(j - 10 < 0) j+=strlen(code);
				str[i] = code[(j-10)];
				break;
 			}
            if(str[i] == '.' && flag2 == 0){
                flag = 1;
                i--;
                flag2 = 1;
            }
		}
	}
}


static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    char temp3[1000];
    strcpy(temp3,path);
    logsytem(level2,"LS", temp3);
    int res;
    char fpath[1000];
    char temp[1000];
    strcpy(temp,path);
    sprintf(fpath,"%s%s",dirpath,temp);
    if(strstr(fpath,encv1) != NULL && strcmp(".", temp) != 0 && strcmp("..", temp) != 0){
        char *i = strstr(fpath,encv1);
            dencrypt(i);
        sprintf(fpath,"%s%s",dirpath,i);
    }
    res = lstat(fpath, stbuf);
    if (res == -1)
        return -errno;

    return 0;

}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
off_t offset, struct fuse_file_info *fi)
{
    char temp3[1000];
    strcpy(temp3,path);
    logsytem(level2,"CD", temp3);
    char fpath[1000];
    if(strcmp(fpath,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else {
        char temp[1000];
        strcpy(temp,path);
        sprintf(fpath, "%s%s",dirpath,temp);
        if(strstr(fpath,encv1) != NULL && strcmp(".", temp) != 0 && strcmp("..", temp) != 0){
            char *i = strstr(fpath,encv1);
            dencrypt(i);
            sprintf(fpath,"%s%s",dirpath,i);
        }
    }
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
        char temp1[100];
        strcpy(temp1,de->d_name);
        if(strstr(fpath,encv1) != NULL && strcmp(".", de->d_name) != 0 && strcmp("..", de->d_name) != 0){
            encrypt(temp1);
        }
        res = (filler(buf, temp1, &st, 0));
        if(res!=0) break;
    }

    closedir(dp);

    return 0;

}

static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;
    char fpath[1000];
    char temp[1000];
    strcpy(temp,path);
    logsytem(level2,"MKDIR",temp);
    sprintf(fpath,"%s%s",dirpath,temp);
	res = mkdir(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
struct fuse_file_info *fi)
{
    char temp3[1000];
    strcpy(temp3,path);
    logsytem(level2,"CAT", temp3);
    char fpath[1000];
    if(strcmp(fpath,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else {
        char temp[1000];
        strcpy(temp,path);
        sprintf(fpath, "%s%s",dirpath,temp);
    }
    int res = 0;
    int fd = 0 ;

    (void) fi;

    fd = open(fpath, O_RDONLY);
    if (fd == -1)
        return -errno; 

    res = pread(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    close(fd);

    return res;

}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
        char temp3[1000];
    strcpy(temp3,path);
    logsytem(level2,"CREATE", temp3);
	int res;
    char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
	if (S_ISREG(mode)) {
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(fpath, mode);
	else
		res = mknod(fpath, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_unlink(const char *path)
{
        char temp3[1000];
    strcpy(temp3,path);
    logsytem(level1,"REMOVE", temp3);
	int res;
char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
	res = unlink(fpath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rmdir(const char *path)
{
        char temp3[1000];
    strcpy(temp3,path);
    logsytem(level1,"RMDIR", temp3);
	int res;
char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
	res = rmdir(fpath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
        char temp3[1000];
    strcpy(temp3,from);
    strcat(temp3,"::");
    strcat(temp3,to);
char fpath[1000],fpath2[1000];
    sprintf(fpath,"%s%s",dirpath,from);
    sprintf(fpath2,"%s%s",dirpath,to);
    logsytem(level2,"MOVE", temp3);
	int res;

	res = rename(fpath2, fpath2);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
        char temp3[1000];
    strcpy(temp3,path);
    logsytem(level2,"TRUNCATE", temp3);
	int res;
char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
	res = truncate(fpath, size);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
        char temp3[1000];
    strcpy(temp3,path);
    logsytem(level2,"OPEN", temp3);
	int res;
char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
	res = open(fpath, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
        char temp3[1000];
    strcpy(temp3,path);
    logsytem(level2,"WRITE", temp3);
	int fd;
	int res;
char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static struct fuse_operations xmp_oper = {
	.getattr = xmp_getattr,
	.readdir = xmp_readdir,
	.read = xmp_read,
	.mkdir = xmp_mkdir,
	.mknod = xmp_mknod,
	.unlink = xmp_unlink,
	.rmdir = xmp_rmdir,
	.rename = xmp_rename,
	.truncate = xmp_truncate,
	.open = xmp_open,
	.read = xmp_read,
	.write = xmp_write,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
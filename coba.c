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

    // printf("%s\n", str);
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
	// printf("hasil enkripsi : %s\n", str);

}

// denkripsi
void dencrypt (char *str) {

    // printf("%s\n", str);
	
	int start, i, j,flag = 1,flag2 = 1,flag3 = 0;
    printf("str %c %d\n",str[2],strlen(str));
    for(start = 0; start < strlen(str) ; start++){
        printf("%c ",str[start]);
        if(str[start] == 'e' && str[start+1] == 'n' && str[start+2] == 'c' && str[start+3] == 'v' && str[start+4] == '1' && str[start+5] == '_'){
            flag3 = 1;
            printf("start %d ",start);
        }
        if(str[start] == '/' && flag3 == 1){
            // start++;
            printf("startf %d ",start);
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
	printf("hasil denkripsi : %s\n", str);
}

// void ngeread()

static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    char temp3[1000];
    strcpy(temp3,path);
    logsytem(level2,"LS", temp3);
    // printf("apder %s\n",path);
    // char * pointer = strstr(path,encv1);
    // if(pointer != NULL){
    // printf("apinter %s\n",pointer);
        // dencrypt(pointer);
    // printf("apinter33 %s\n",pointer);
    // }  
    int res;
    char fpath[1000];
    char temp[1000];
    strcpy(temp,path);
    // if(strcmp(".", path) != 0 && strcmp("..", path) != 0 )
    // dencrypt(temp);
    sprintf(fpath,"%s%s",dirpath,temp);
    // if(strncmp(fpath,encv1,7) == 0)
     printf("attaa=============");
    if(strstr(fpath,encv1) != NULL ){
        char *i = strstr(fpath,encv1);
        printf("atta=============");
            dencrypt(i);
            printf("attb=============");
        sprintf(fpath,"%s%s",dirpath,i);
    // printf("denktip1 %s %s\n",fpath,i);

    // encrypt(temp1);
    }

     printf("%s\nattbb=============",fpath);
    // printf("atte %s\n",fpath);
    // printf("atribut %s\n",fpath);
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
        // char * pointer = strstr(path,encv1);
        char temp[1000];
        strcpy(temp,path);
        // if(pointer != NULL){
        // printf("pinter %s\n",pointer);
            // dencrypt(pointer);
        // printf("pinter33 %s\n",pointer);
        // }
        // printf("readdir %s\n",temp);
        // printf("fpath %s\n",temp);
        sprintf(fpath, "%s%s",dirpath,temp);
        // if(strncmp(fpath,encv1,7) == 0)
        // if(strstr(fpath,encv1) != NULL ){
        //     // printf("denktip");
        //     char *i = strstr(fpath,encv1);
        //     printf("readdir=============");
        //     dencrypt(i);
        //     printf("readdir=============");
        //     sprintf(fpath,"%s%s",dirpath,i);

        //     // encrypt(temp1);
        // }
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
        // printf("nama %s\n",de->d_name);
        // char * pointer2 = strstr(de->d_name,encv1);
        // if(pointer2 != NULL)  
        // // dencrypt(pointer2);
        // if(strcmp(".", de->d_name) != 0 && strcmp("..", de->d_name) != 0){
        //     // printf("temp = %s\n",de->d_name);
        // continue;
        // }
        // printf("coa %s\n",temp1);
        if(strstr(fpath,encv1) != NULL ){
            // printf("enktip");

            encrypt(temp1);
        }
        // if(strncmp(fpath,encv1,7) == 0){
        // }
        // printf("dname -> %s\nflat %s\n",temp1,fpath);
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
    // char * pointer = strstr(path,encv1);
    // if(pointer != NULL)  
    //     dencrypt(pointer);

    char fpath[1000];
    if(strcmp(fpath,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
        // printf("+++++++++++++++++++");
    }
    else {
        char temp[1000];
        // printf("----------------------");
        strcpy(temp,path);
        // dencrypt(temp);
        sprintf(fpath, "%s%s",dirpath,temp);
        if(strstr(fpath,encv1) != NULL ){
            printf("read=============");
        char *i = strstr(fpath,encv1);
            dencrypt(i);
            printf("read=============");
        sprintf(fpath,"%s%s",dirpath,i);
    // printf("denktip1 %s %s\n",fpath,i);

    // encrypt(temp1);
        }
    }
    // printf("read %s\n",fpath);
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
	/* On Linux this could just be 'mknod(path, mode, rdev)' but this
	   is more portable */
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
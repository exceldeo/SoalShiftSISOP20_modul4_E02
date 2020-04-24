#define FUSE_USE_VERSION 28

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>

static  const  char *dirpath = "/home/excel/Documents";
char code[100] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";

// Enkripsi
void encrypt (char str[1000]) {

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

// Dekripsi
void decrypt (char str[1000]) {

    printf("%s\n", str);
	
	int i, j;

	for (i = 0; i < strlen(str); i++) {
		for (j = 0; j < strlen(code); j++) {
			if(str[i] == code[j]) {
				if(j - 10 < 0)
                    str[i] = code[strlen(code) + (j-10)];
                else
                    str[i] = code[j-10];
				break;
 			}
		}
	}
	printf("hasil dekripsi : %s\n", str);
}
// denkripsi
void dencrypt (char str[1000]) {

    printf("%s\n", str);
	
	int i, j;

	for (i = 0; i < strlen(str); i++) {
		for (j = 0; j < strlen(code); j++) {
			if(str[i] == code[j]) {
                if(j - 10 < 0) j=+strlen(code);
				str[i] = code[(j-10)];
				break;
 			}
		}
	}
	printf("hasil enkripsi : %s\n", str);

}

int versisatu (char path[1000], int status) {

    printf("path : %s\n", path);

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

        char path2[100], path3[100];
        sprintf(path2, "%s/%s", path, name);

        if (strcmp(".", de->d_name) != 0 && strcmp("..", de->d_name) != 0 && de->d_type == 4){
            
            char folder[100];
            sprintf(folder, "%s/%s", path, name);

            if (status == 1) {
                encrypt(name);
                versisatu(folder, 1);
            }
            else {
                decrypt(name);
                versisatu(folder, 0);
            }

            printf("foldername : %s\n", name);
            sprintf(path3, "%s/%s", path, name);
            
            int res = rename(path2, path3);
	        if (res == -1) return -errno;
        }
        
        if (de->d_type == 8) {

            printf("lama : %s\n", path2);

            char *ext = strrchr(name, '.');

            if (ext == NULL) {
                if (status == 1) encrypt(name);
                else decrypt(name);

                printf("filename : %s\n", name);

                sprintf(path3, "%s/%s", path, name);
            }
            else {
                char noext[ext - name];
                strncpy(noext, name, ext - name);
                
                noext[ext - name] = '\0';
                printf("hasil : %s\n", noext);
                
                if (status == 1) encrypt(noext);
                else decrypt(noext);
                
                sprintf(noext, "%s%s", noext, ext);
                printf("%s\n", noext);

                sprintf(path3, "%s/%s", path, noext);
            }

            printf("baru : %s\n", path3);

            int res = rename(path2, path3);
	        if (res == -1) return -errno;
        }
    }

    closedir(dp);
    return 0;

}

// Get file attributes
static  int  xmp_getattr(const char *path, struct stat *stbuf) {

    printf("getattr %s\n", path);

    int res;
    char fpath[1000];
    dencrypt(path);

    sprintf(fpath,"%s%s", dirpath, path);

    // Return information about a file
    res = lstat(fpath, stbuf);
    if (res == -1) return -errno;

    return 0;
}

// Make directory
static int xmp_mkdir(const char *path, mode_t mode) {

    printf("mkdir %s\n", path);

	int res;
    char fpath[1000];
    encrypt(path);

    sprintf(fpath,"%s%s", dirpath, path);
    printf("fpath mkdir %s\n", fpath);

	res = mkdir(fpath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (res == -1)
		return -errno;

	return 0;
}

// Read directory
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {

    int res;
    char fpath[1000];

    printf("readdir %s\n", path);
    // encrypt(path);
    if(strcmp(path,"/") == 0) {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
        else {
        char temp[1000];
        encrypt(path)
        sprintf(fpath, "%s%s",dirpath,temp);
    }

    printf("fpath readdir %s\n", fpath);

    DIR *dp;
    struct dirent *de;

    (void) offset;
    (void) fi;

    dp = opendir(fpath);
    if (dp == NULL) return -errno;

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

// Read data from an open file
static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {

    int res;
    char fpath[1000];

    printf("read %s\n", path);

    if(strcmp(path,"/") == 0) {
        path=dirpath;
        sprintf(fpath, "%s", path);
        printf("masuk read %s\n", path);
    }
    else sprintf(fpath, "%s%s", dirpath, path);

    int fd;
    (void) fi;

    fd = open(fpath, O_RDONLY);
    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);
    if (res == -1) res = -errno;

    close(fd);
    return res;

}

static int xmp_utimens(const char *path, const struct timespec ts[2]) {

    printf("masuk utimens %s\n", path);

	int res;
	struct timeval tv[2];

    char fpath[1000];
    sprintf(fpath,"%s%s", dirpath, path);

    printf("fpath create %s\n", fpath);

	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res = utimes(fpath, tv);
	if (res == -1) return -errno;

	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {

	int res;
    char fpath[1000];

    printf("write %s\n", path);

    if(strcmp(path,"/") == 0) {
        path=dirpath;
        sprintf(fpath, "%s", path);
        printf("masuk write %s\n", path);
    }
    else sprintf(fpath, "%s%s", dirpath, path);

    int fd;
	(void) fi;

	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1) res = -errno;

	close(fd);
	return res;
}

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi) {

    printf("create : %s\n", path);

    int i, n = 0, status = 0;
    (void) fi;

    char temp[1000], nama[100][1000],
         pathfile[100] = "/";
    strcpy(temp, path);

    char *token;
    token = strtok(temp, "/");
    while (token != NULL) {
        if (strncmp("encv1_", token, 6) == 0) {
            printf("enkripsi 1\n");
            status = 1;
        }
        strcpy(nama[n++], token);

        token = strtok(NULL, "/");
    }
    
    if (status == 1) {
        for (i = 0; i < n - 1; i++)
            sprintf(pathfile, "%s%s/", pathfile, nama[i]);
    
        char filename[100];
        strcpy(filename, nama[n-1]);

        printf("n: %d | path: %s | namafile: %s\n", n, pathfile, filename);

        char *ext = strrchr(filename, '.');
        if (ext == NULL) {
            encrypt(filename);
            sprintf(pathfile, "%s%s", pathfile, filename);
        }
        else {
            char noext[ext - filename];
            strncpy(noext, filename, ext - filename);
                
            noext[ext - filename] = '\0';
            printf("hasil : %s\n", noext);

            encrypt(noext);
            sprintf(noext, "%s%s", noext, ext);

            sprintf(pathfile, "%s%s", pathfile, noext);
        }
        printf("pathbaru: %s\n", pathfile);
    }
    else
        strcpy(pathfile, path);

    char fpath[1000];
    sprintf(fpath,"%s%s", dirpath, pathfile);

    printf("fpath create %s\n", fpath);

    int res = creat(fpath, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(res == -1) return -errno;

    close(res);

    return 0;
}

static int xmp_rename(const char *from, const char *to) {

    printf("rename %s to %s\n", from, to);

	int res;
    char fpath[1000];
    char tpath[1000];

    sprintf(fpath,"%s%s", dirpath, from);
    printf("fpath rename %s\n", fpath);
    sprintf(tpath,"%s%s", dirpath, to);
    printf("fpath rename %s\n", tpath);

    if (strncmp("/encv1_", to, 7) == 0) {
        printf("enkripsi versi 1\n");
        versisatu(fpath, 1);
    }

    if (strncmp("/encv1_", from, 7) == 0) {
        printf("dekripsi versi 1\n");
        versisatu(fpath, 0);
    }

	res = rename(fpath, tpath);
	if (res == -1)
		return -errno;

	return 0;
}



static struct fuse_operations xmp_oper = {

    .getattr = xmp_getattr,
    .mkdir = xmp_mkdir,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .rename = xmp_rename,
    .write = xmp_write,
    .create = xmp_create,
    .utimens = xmp_utimens,

}; 

int  main(int  argc, char *argv[]) {

    umask(0);

    // Fungsi main userspace
    return fuse_main(argc, argv, &xmp_oper, NULL);

}
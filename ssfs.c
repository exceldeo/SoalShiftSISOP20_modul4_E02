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
            
            encrypt(name);
            printf("foldername : %s\n", name);
        }
        
        if (de->d_type == 8) {
            encrypt(name);
            printf("filename : %s\n", name);
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
        enkripsi1(fpath);
    }

	res = rename(fpath, tpath);
	if (res == -1)
		return -errno;

    
    // if (strncmp("encv1_", from, 6) == 0) {
    //     printf("dekripsi versi 1\n");
    // }

	return 0;
}

static struct fuse_operations xmp_oper = {

    .getattr = xmp_getattr,
    .mkdir = xmp_mkdir,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .rename = xmp_rename,
    
}; 

int  main(int  argc, char *argv[]) {

    umask(0);

    // Fungsi main userspace
    return fuse_main(argc, argv, &xmp_oper, NULL);

}
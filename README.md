# SoalShiftSISOP20_modul4_E02

### Enkripsi Versi 1

Pada enkripsi versi 1, semua file dan direktori yang berada dalam direktori `encv1_` akan dienkripsi menggunakan caesar cipher dengan key 9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO. Enkripsi tersebut dilakukan dalam fungsi berikut.
```c
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
```
Looping pertama dilakukan untuk mengecek apakah file yang akan dienkripsi memiliki ekstensi. Jika file tersebut memiliki ekstensi maka ekstensinya tidak akan ikut terenkripsi. Sedangkan looping kedua dan ketiga dilakukan untuk mengenkripsi nama file atau nama direktori.

Sementara itu, untuk fungsi dekripsinya dapat dilihat pada kode di bawah ini.
```c
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
```
Lalu, pada xmp_getattr, xmp_readdir, dan xmp_mkdir ditambahkan kode berikut karena pada mount source /home/(namauser)/Documents, nama file dan direktori dalam direktori encv_1 tidak ikut terenkripsi.

```c
    char fpath[1000];
    if(strcmp(fpath,"/") == 0) {
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
```
Untuk enkripsinya sendiri, dipanggil dalam xmp_readdir pada bagian di bawah ini.
```c
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
```

#### Kendala

Masih belum bisa rename pada direktori yang memiliki nama "encv1_" serta belum bisa melakukan touch dalam direktori yang memiliki nama "encv1_".

### Log System

Pembuatan log system akan dilakukan pada direktori home pengguna. Log system ini menyimpan daftar perintah system call yang telah dijalankan. Berikut merupakan kode fungsi untuk menjalankan log system.
```c
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
    fp = fopen ("/home/excel/fs.log","a");
    fprintf(fp, "%s\n",log);
    fclose(fp);

}
```
+ `strcpy(log, level)` menyalin string dari level. Level sendiri terbagi menjadi level WARNING dan level INFO. Pencatatan level WARNING dilakukan ketika system call rmdir dan unlink. Sementara pencatat level INFO dilakukan untuk system call selain rmdir dan unlink.
+ `strcat(log, times)` menconcat string dari log dengan string times dimana times berisi localtime ketika system call terpanggil. Localtime didapatkan dari `struct tm *ptm = localtime(&t)`.
+ `strcat(log, cmd)` menconcat string dari log dengan string cmd dimana cmd berisi system call yang terpanggil.
+ `strcat(log, desc)` menconcat string dari log dengan string desc dimana desc berisi deskripsi tambahan (seperti nama file atau direktori).
+ `fp = fopen ("/home/excel/fs.log","a")` untuk membuka file fs.log dengan mode append agar log yang ingin dicatat dapat ditulis pada akhir file. Pencatatan log dilakukan dengan `fprintf(fp, "%s\n",log)`.

Selanjutnya tinggal menambahkan contohnya `logsytem(level2,"MKDIR",temp)` pada `xmp_mkdir` untuk memanggil fungsi logsystem() ketika system call mkdir dipanggil. Adapun isi parameter `temp` disini yaitu nama direktori yang baru dibuat.

### Enkripsi Versi 2 dan Sinkronisasi

Untuk kedua soal tersebut mohon maaf belum sempat mengerjakan.

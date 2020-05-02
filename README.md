# SoalShiftSISOP20_modul4_E02

## Log System

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

Selanjutnya tinggal menambahkan contohnya `logsytem(level2,"MKDIR",temp)` pada `xmp_mkdir` untuk memanggil fungsi logsystem() ketika system call mkdir dipanggil. Parameter `temp` disini berisi nama direktori yang baru dibuat.

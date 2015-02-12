/*
 *@Author:  Rocky
 *@Date:    2014-12-29
 *@Version: 0.1
 *@Descr:   Add 16B header info for S5PV210 BL1
 *              Use standard IO interface for Support Linux and Windows
 *              This Program use Malloc the max 16KB heap sapce,and not reuse.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MEM16KB     (0x1<<14)
#define HEADSIZE    (0x10)

int main(int argc,char **argv)
{
    FILE *fp;
    int ret;
    unsigned int file_len = 0;
    unsigned int check_sum = 0;
    unsigned char *buf16K;     /*store header info*/
    unsigned char *tmp_count;
    unsigned int rw_len = 0;
    unsigned int count;
    
    if(argc != 3){
        fprintf(stderr,"Please Use mk210header <source file> <destination file>\n");
        return -1;
    }

    /* Read only file to count filesize and checksum */
    fp = fopen(argv[1],"rb");
    if(fp == NULL){
        fprintf(stderr,"fopen %s error:%s!\n",argv[1],strerror(errno));
        return errno;
    }
    printf("the %s file open success!\n",argv[1]);

    /* Get file size */
    ret = fseek(fp,0,SEEK_END);
    if(ret < 0){
        fprintf(stderr,"fseek file %s error:%s\n",argv[1],strerror(errno));
        fclose(fp);
        return errno;
    }
    file_len = ftell(fp);
    file_len = (file_len > MEM16KB) ? MEM16KB - HEADSIZE : file_len;

    /* Allocate heap buf for checksum */
    buf16K = (unsigned char *)malloc(file_len+HEADSIZE);
    memset(buf16K,0,file_len+HEADSIZE);

    /* Fill 1WROD in buf16k offset 0x0,data is file_len */
    memcpy(buf16K,&file_len,4);

    /* Fill 1WORD in buf16k offset 0x8,data is checksum */
    /* Read file data to buf16k offset 0x10 */
    rewind(fp);
    rw_len = fread(buf16K+HEADSIZE,1,file_len,fp);
    if( rw_len != file_len ){
        fprintf(stderr,"fread buf error %s\n",strerror(errno));
        free(buf16K);
        fclose(fp);
        return errno;
    }
    /* Count checksum */
    tmp_count = buf16K+HEADSIZE;
    for(count = 0; count < rw_len; count++){
        check_sum += tmp_count[count];
    }
    memcpy(buf16K+0x8,&check_sum,4);
    fclose(fp);
    
    /* Create Destiation File */
    fp = fopen(argv[2],"wb");
    if(fp == NULL){
        fprintf(stderr,"fopen %s file error:%s!\n",argv[2],strerror(errno));
        return errno;
    }
    rw_len = fwrite(buf16K,1,file_len+HEADSIZE,fp);
    if( rw_len != file_len+HEADSIZE ){
        fprintf(stderr,"fwrite buf error %s\n",strerror(errno));
        free(buf16K);
        fclose(fp);
        return errno;
    }

    printf("the file %s create success!\n",argv[2]);

    free(buf16K);
    fclose(fp);
    
	return 0;
}


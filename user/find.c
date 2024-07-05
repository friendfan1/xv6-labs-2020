#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void todir(char *buf, char *s){

    int i;
    char *p;

    p = buf + strlen(buf);
    *p++ = '/';

    for(i = 0;i < strlen(s); i++){
        *p++ = s[i];
    }
    *p++ = 0;
    
}

void find(char *path, char *target){

    char buf[512];
    int i = 0;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }


    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(i++ < 2)continue;//1. 如果这里耦合性太强的话，看2的解决
        if(de.inum == 0){
            continue;
        }
        else{
            strcpy(buf, path);
            todir(buf, de.name);
            if(stat(buf, &st) < 0){
                fprintf(2, "find: cannot stat %s\n", buf);
            }
            if(st.type == 1){//2.在这里用de.name进行判断，跳过.和..
                find(buf, target);
            }
            else if(st.type == 2){
                if(!strcmp(de.name, target)){
                    fprintf(1, "%s\n", buf);
                }
            }
        }
    }
}

int
main(int argc, char *argv[]){

    if(argc != 3){
        fprintf(2,"Usage :find <path> <name>\n");
    }

    find(argv[1], argv[2]);

    exit(0);

}
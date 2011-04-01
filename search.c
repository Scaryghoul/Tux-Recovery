int ascii_block_search(char *find, char *file)
{
    char blkls[256], string[256],grp[256],new[256],display[256];
    sprintf(blkls,"blkls %s > %s.blkls",file,file);
    sprintf(string,"strings -t d %s.blkls > %s.blkls.str",file,file);
    sprintf(grp,"grep %s %s.blkls.str > %s.out",find, file,file);
    system(blkls);
    system(string);
    system(grp);
    sprintf(new,"%s.out",file);
    sprintf(display,"grep --color=auto %s %s",find,new);
    system(display);
    return 0;
}

int search_all(char *file,char *search)
{
    int deleted;
    char del[256];
    char buf[256],buffer[2048],filename[256],inode[256],type[256];
    FILE *popen();
    FILE *in;
    sprintf(buf,"fls -rF %s |grep %s",file,search);
    in=popen(buf,"r");
    fls_parse(in,filename,inode,&deleted,type);
    printf("\tDeleted\t|\tInode\t|\tFilename/filesystem\n");
    printf("\t-------\t|\t-----\t|\t-------------------\n");

    while(!feof(in))
    {
        if(deleted==0)
            strcpy(del,"no");
        else
            strcpy(del,"yes");
        printf("\t%s\t|\t%s\t|\t%s\n",del,inode,filename);
        clearstr(del);
        fls_parse(in,filename,inode,&deleted,type);
    }
}

int list_all(char *file)
{
    int deleted;
    char del[256];
    char buf[256],buffer[2048],filename[256],inode[256],type[256],command2[256],sector1[256],sector2[256],ssize[256],fstype[256];
    FILE *popen();
    FILE *in;
    fsparse(file,fstype);
    sprintf(buf,"fls -rF %s",file);
    in=popen(buf,"r");
    if(strcmp(fstype,"FAT32")==0 || strcmp(fstype,"FAT16")==0 || strcmp(fstype,"FAT")==0)
        psearch(buf,buf,buf,buf,buf,buf,21);
    if(strcmp(fstype,"NTFS")==0 || strcmp(fstype,"ntfs")==0)
        psearch(buf,buf,buf,buf,buf,buf,22);
    fls_parse(in,filename,inode,&deleted,type);
    sprintf(command2,"istat %s %s",file,inode);
    istat_parse(command2,sector1,sector2,ssize);
    while(!feof(in))
    {
        if(deleted==0)
            strcpy(del,"no");
        else
            strcpy(del,"yes");
        if(strcmp(fstype,"FAT32")==0 || strcmp(fstype,"FAT16")==0 || strcmp(fstype,"FAT")==0)
            psearch(del,inode,ssize,sector1,sector2,filename,1);
        if(strcmp(fstype,"NTFS")==0 || strcmp(fstype,"ntfs")==0)
            psearch(del,inode,ssize,sector1,sector2,filename,2);            
        clearstr(del);
        fls_parse(in,filename,inode,&deleted,type);
        sprintf(command2,"istat %s %s",file,inode);
        istat_parse(command2,sector1,sector2,ssize);
    }
} 

int psearch(char *del,char *inode, char *ssize, char *sector1,char *sector2,char *filename,int fs)
{

    if(fs==1)
        printf("\t%s\t|\t%s\t|\t%s\t|\t%s\t\t|\t%s\t\t|\t%s\n",del,inode,ssize,sector1,sector2,filename);
    else if(fs==2)
        printf("\t%s \t|\t %s \t|\t%s \t\t|\t %s\t\t|\t %s \t|\t %s\n",del,inode,ssize,sector1,sector2,filename);
    else if(fs==21)
    {
        printf("\tDeleted\t|\tInode\t|\tSize\t|\tFirst sector\t|\tLast sector\t|\tFilename\n");
        printf("\t-------\t|\t-----\t|\t----\t|\t------------\t|\t-----------\t|\t--------\n");
    }
    else if(fs=22)
    {
        printf("\tDeleted\t|\tInode\t\t|\tSize\t\t|\tFirst sector\t|\tLast sector\t|\tFilename\n");
        printf("\t-------\t|\t-----\t\t|\t----\t\t|\t------------\t|\t-----------\t|\t--------\n");
    }
    return 0; 
}

int mkdir(char *directory,int check)
{
        FILE *pfile;
        char buf[1024],dbuf[1024],command[256];
        char *p;

    if(check)
    {
        strcpy(dbuf,directory);
        p=strrchr(dbuf,'/');
        if(p)
        {
            *p=' ';
            sscanf(dbuf,"%s",buf);
            pfile=fopen(buf,"r");
            if(pfile==NULL)
            {
                sprintf(command,"mkdir -p %s",buf);
                system(command);
            }
            else
            {
                pclose(pfile);
            }
        }
    }
    else
    {
        if(fopen(buf,"r")==NULL)
        {
            sprintf(buf,"mkdir -p %s",directory);
            system(buf);
        }
    }
    return 0;
}


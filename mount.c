int mount(char *filesystem, char *point, int ro)
{
    char buffer[256],type[256];
    FILE *pfile=fopen(point,"r");
    if(pfile==NULL)
    {
        sprintf(buffer,"sudo mkdir %s",point);
        system(buffer);
    }
    else
    {
        fclose(pfile);
    }
    char buf[256];
    check_fs(filesystem,type);
    if(strcmp(type,"NTFS")==0)
    {
        ntfs_mount(filesystem,point,ro);
        return 0;
    }
    if(ro)
        sprintf(buf,"sudo mount -r %s %s",filesystem,point);
    else
        sprintf(buf,"sudo mount %s %s",filesystem,point);
    system(buf);
    return 0;
}

int ntfs_mount(char *filesystem, char *point, int ro)
{
    FILE *pfile=fopen("/usr/bin/ntfsmount","r");
    if(pfile==NULL)
    {
        printf("You are missing the ntfsprogs package required to read ntfs partitions\n");
        return 1;
    }
    else
        fclose(pfile);
    char buf[256];
    if(ro)
        sprintf(buf,"sudo ntfsmount -r %s %s",filesystem,point);
    else
        sprintf(buf,"sudo ntfsmount %s %s",filesystem,point);
    system(buf);
    return 0;



}

int check_fs(char *filesystem, char *out)
{
    char buf[1024],buffer[1024],command[256],empty[2048];
    sprintf(command,"fsstat %s |grep File",filesystem);
    FILE *popen();
    FILE *in;
    in=popen(command,"r");
    fgets(buf,1023,in);
    sscanf(buf,"%s %s %s %s",empty,empty,empty,out);
    return 0;
}

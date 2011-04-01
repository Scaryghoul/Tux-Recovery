int data_dump(char *source, char *dest, char *bsize)
{
    char buf[256],buffer[256],t;
    sprintf(buf,"sudo dd if=%s of=%s bs=%s",source,dest,bsize);
    system(buf);
    printf("Do you want to validate the files?(y,n): ");
    t=getchar();
    if(t=='y')
        checksum(source,dest);
    return 0;
}

int controlled_dump(char *source, char *dest, char *bsize, char *count, char *skip)
{
    char buf[256],buffer[256],t;
    sprintf(buf,"sudo dd if=%s of=%s bs=%s skip=%s count=%s",source,dest,bsize,skip,count);
    system(buf);
    return 0;
}

int md5check(char *in,char*out)
{
    char md5c1[256],md5c2[256];
    char buf[1024],buf2[1024],md51[256],md52[256];
    FILE *popen();
    FILE *pipe;
    sprintf(md5c1,"sudo md5sum %s",in);
    sprintf(md5c2,"sudo md5sum %s",out);
    //Checking md5sum of the first file
    printf("Md5sums:    \n");
    pipe=popen(md5c1,"r");
    fgets(buf,1023,pipe);
    sscanf(buf,"%s",md51);
    cprintf(in,0,0,1);
    printf(": %s\n",md51);
    pclose(pipe);
    //Checking md5sum of second file
    pipe=popen(md5c2,"r");
    fgets(buf2,1023,pipe);
    sscanf(buf2,"%s",md52);
    cprintf(out,0,0,1);
    printf(": %s\n\n",md52);
    pclose(pipe);
    if(strcmp(md51,md52)==0)
        return 0;
    return 1;

}
int sha1check(char *in, char *out)
{
    char shac1[256],shac2[256];
    char buffer[1024],buffer2[1024],sha1[256],sha2[256];
    FILE *popen();
    FILE *pipe;
    sprintf(shac1,"sudo sha1sum %s",in);
    sprintf(shac2,"sudo sha1sum %s",out);
    //Checking sha1sum of first file
    printf("Sha1sums:   \n");
    pipe=popen(shac1,"r");
    fgets(buffer,1023,pipe);
    sscanf(buffer,"%s",sha1);
    cprintf(in,0,0,1);
    printf(": %s\n",sha1);
    pclose(pipe);
    //Checking sha1sum of second file
    pipe=popen(shac2,"r");
    fgets(buffer2,1023,pipe);
    sscanf(buffer2,"%s",sha2);
    cprintf(out,0,0,1);
    printf(": %s\n\n",sha2);
    pclose(pipe);
    if(strcmp(sha1,sha2)==0)
        return 0;
    return 1;
}
int checksum(char *in, char *out)
{
    if(md5check(in,out)==0 && sha1check(in,out)==0)
        printf("Files are validated as the same\n");
    else
        printf("The files a different\n");
    return 0;

}   

int block_recover(char *command1,char *filesystem)
{
    int deleted;
    char buf[1024],buffer[1024],command2[256],command3[256],inode[256],name[256],sector1[256],sector2[256],fsize[256],type[256];
    int x;
    FILE *popen();
    FILE *in;
    FILE *in2;
    in=popen(command1,"r");
    while(!feof(in))
    {
        if(feof(in)!=0)
            return 0;
        x=fls_parse(in,name,inode,&deleted,type);
        if(x!=0)
        {
            printf("EOF reached, no more results\n");
            pclose(in);
            return 0;
        }
        printf("Type: %s\n",type);
        if(strcmp(type,"d/d")==0)
        {
            mkdir(name,0);
        }
        else
        {
            mkdir(name,1);
        }
        sprintf(command2,"istat %s %s",filesystem,inode);
        x=istat_parse(command2,sector1,sector2,fsize);        
        if(x!=0)
            return 1;
        sprintf(command3,"blkls %s %s-%s > %s",filesystem,sector1,sector2,name);
        system(command3); 
        clearstr(command2);
        clearstr(command3);
        clearstr(command3);
        clearstr(inode);
        clearstr(sector1);
        clearstr(sector2);
        clearstr(name);
    }
    pclose(in);
    return 0;
}

int sparse_unalloc(char *filesystem,char *filename)
{   
    char command1[256];
    sprintf(command1,"fls -rdp %s |grep %s",filesystem,filename);
    block_recover(command1,filesystem);
    return 0;
}


int sparse_all(char *filesystem,char *filename)
{
    char command1[256];
    sprintf(command1,"fls -rp %s |grep %s",filesystem,filename);
    block_recover(command1,filesystem);
    return 0;
}



int all_unalloc(char *filesystem)
{
    char command1[256];
    sprintf(command1,"fls -rdp %s",filesystem);
    block_recover(command1,filesystem);
    return 0;
}

int get_everything(char *filesystem)
{
    char command1[256];
    sprintf(command1,"fls -rp %s",filesystem);
    block_recover(command1,filesystem);
    return 0;
}

int accurate_recover(char *command1,char *filesystem)
{
    printf("Accurate recover\n");
    int deleted;
    char buf[1024],buffer[1024],command2[256],command3[256],inode[256],name[256],sector1[256],sector2[256],fsize[256],type[256],fstype[256];
    int ssize,sloc,find;
    int x;
    char house[256];
    FILE *popen();
    FILE *in;
    FILE *in2;
    in=popen(command1,"r");
    ssize=fsparse(filesystem,fstype);
    if(ssize==0)
        return 0;
    while(!feof(in))
    {
        if(feof(in)!=0)
            return 0;
        x=fls_parse(in,name,inode,&deleted,type);
        if(x!=0)
        {
            printf("EOF reached, no more results\n");
            pclose(in);
            return 0;
        }
        if(strcmp(type,"d/d")==0)
        {
            mkdir(name,0);
        }
        else
        {
            mkdir(name,1);
            sprintf(command2,"istat %s %s",filesystem,inode);
            x=istat_parse(command2,sector1,sector2,fsize);
            if(x!=0)
                return 1;
            sscanf(sector1,"%d",&sloc);
            find=sloc*ssize;
            sprintf(house,"%d",find);
            controlled_dump(filesystem,name,"1",fsize,house);
            system(command3); 
            clearstr(command2);
            clearstr(command3);
            clearstr(command3);
            clearstr(inode);
            clearstr(sector1);
            clearstr(sector2);
            clearstr(name);
        }
    }
    pclose(in);

}

int accurate_all_unalloc(char *filesystem)
{
    char command1[256];
    sprintf(command1,"fls -rdp %s",filesystem);
    accurate_recover(command1,filesystem);
    return 0;
}

int accurate_all(char *filesystem)
{
    char command1[256];
    sprintf(command1,"fls -rp %s",filesystem);
    accurate_recover(command1,filesystem);
    return 0;
}


int sparse_accurate_unalloc(char *filesystem,char *keyword)
{
    char command1[256];
    sprintf(command1,"fls -rdp %s |grep %s",filesystem,keyword);
    accurate_recover(command1,filesystem);
    return 0;
}


int accurate_recover_inode(char *filesystem, char *inode)
{
    char command[256],command2[256],command3[256],sector1[1024],sector2[1024],fsize[1024],name[256],empty[256],type[256],fstype[256];
    FILE *in;
    FILE *popen();
    sprintf(command,"fls -rp %s |grep %s",filesystem,inode);
    int x=1,find,sloc,ssize,deleted;
    in=popen(command,"r");
    x=fls_parse(in,name,empty,&deleted,type);
    if(strcmp(empty,inode)!=0)
    {
        while(strcmp(empty,inode)!=0)
        {
            fls_parse(in,name,empty,&deleted,type);
        }
    }
    ssize=fsparse(filesystem,fstype);
    if(ssize==0)
        return 1;
    if(x!=0)
    {
        printf("Couldn't find inode\n");
        return 1;    
    }
    fls_name(name);
    if(strcmp(type,"d/d")==0)
    {
        mkdir(name);
    }
    char house[256];
    sprintf(command2,"istat %s %s",filesystem,inode);
    x=istat_parse(command2,sector1,sector2,fsize);
    if(x!=0)
        return 1;
    sscanf(sector1,"%d",&sloc);
    find=sloc*ssize;
    sprintf(house,"%d",find);
    controlled_dump(filesystem,name,"1",fsize,house);
//    system(command3); 
    pclose(in);
}

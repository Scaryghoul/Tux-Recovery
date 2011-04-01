/* Colour codes
 * 0  = No attributes
 * 30 = Black
 * 31 = Red
 * 32 = Green
 * 33 = Yellow
 * 34 = Blue
 * 35 = Magenta
 * 36 = Cyan
 * 37 = White
*/


int cprintf(char *arr,int ttextcolour, int new_line,int attr)
{
    printf("%c[%dm",0x1B,attr);
    if(text_colour)
    {
        printf("%c[%dm%s",0x1B,text_colour,arr);
        printf("%c[%dm",0x1B,0);
    }
    else
    {
        printf("%c[%dm%s",0x1B,ttextcolour,arr);
        printf("%c[%dm",0x1B,0);     
    }
    if(new_line)    
        printf("\n");
    return 0;

}

int fls_parse(FILE *pfile, char *filename,char *inode, int *del,char *type)
{
    char buf[1024],empty[2048];
    char deleted[256];
    if(feof(pfile)!=0)
        return -1;
    fgets(buf,1023,pfile);
    if(feof(pfile)!=0)
        return -1;
    int x=strlen(buf);
    if(x<3)
        return -1;
    sscanf(buf,"%s %s %s %s",type,deleted,inode,filename);
    if(strcmp(deleted,"*")==0)
        *del=1;
    else
    {
        sscanf(buf,"%s %s %s",type,inode,filename);
        *del=0;
    }   

    char *p=strchr(inode,':');
    if(p!=NULL)
        *p='\0';
    return 0;
}

int clearstr(char *array)
{
    int test=0;
    int a=strlen(array);
    while(test<a)
    {
        array[test]=NULL;
        test++;
    }
    return 0;
}
int ntfs_istat(char *command,char *sector1,char *sector2,char *fsize)
{
    char buf[1024],empty[2048],buffer[1024],fstype[256],filename[256];
    FILE *popen();
    FILE *pfile;
    pfile=popen(command,"r");
    int size;
    int test=1;
    while(test)
    {
        if(feof(pfile)!=0)
        {
            return 1;
         }
        fgets(buf,1023,pfile);
        if(strlen(buf)>1)
        {
            sscanf(buf,"%s",buffer);
            if(strcmp(buffer,"Type:")==0)
            {
                sscanf(buf,"%s %s",empty,buffer);
                if(strcmp(buffer,"$DATA")==0)
                {
                    sscanf(buf,"%s %s %s %s %s %s %s %d",empty,empty,empty,empty,empty,empty,empty,&size);
                    sscanf(buf,"%s %s %s %s %s %s %s %s",empty,empty,empty,empty,empty,empty,empty,fsize);
                    test=0;
                    if(size==0)
                        return 1;
                }
            }
        }
    }
    int sector=size/1024*2;
    if(size%1024!=0)
        sector++;
    int sec1=sector;
    sector=sector/5;
    fgets(buf,1023,pfile);
    sscanf(buf,"%s",sector1);
    sector=sector-1;
    while(test<sector)
    {
        if(!feof(pfile))
        {
           fgets(buf,1023,pfile);
        }
        test++;
    }
    sector=sec1%8;
    if(sector==0)
        sscanf(buf,"%s %s %s %s %s %s %s %s",empty, empty, empty, empty, empty , empty, empty,sector2);
    
    else if(sector==7)
        sscanf(buf,"%s %s %s %s %s %s %s",empty,empty,empty,empty,empty,empty,sector2);
    else if(sector==6)
        sscanf(buf,"%s %s %s %s %s %s",empty,empty,empty,empty,empty,sector2);
    else if(sector==5)
        sscanf(buf,"%s %s %s %s %s",empty,empty,empty,empty,sector2);
    else if(sector==4)
        sscanf(buf,"%s %s %s %s",empty,empty,empty,sector2);
    else if(sector==3)
        sscanf(buf,"%s %s %s",empty,empty,sector2);
    else if(sector==2)
        sscanf(buf,"%s %s",empty,sector2);
    else if(sector==1)
        sscanf(buf,"%s",sector2);
    else
    {
        printf("We couldn't determine the end of the file\n");
        pclose(pfile);
        return 1;
    }
    pclose(pfile);
    if(strcmp(sector1,"Type:")==0 || strcmp(sector1,"Type:")==0)
        return 1;
    return 0;


}

int fat_istat(char *command,char *sector1,char *sector2,char *fsize)
{
    char buf[1024],empty[2048],buffer[1024],fstype[256],filename[256];
    FILE *popen();
    FILE *pfile;
    pfile=popen(command,"r");
    int size;
    int test=1;
    while(test)
    {
        if(feof(pfile)!=0)
        {
            printf("Size could not be determined\n");
            return 1;
         }
        fgets(buf,1023,pfile);
        if(strlen(buf)>1)
        {
            sscanf(buf,"%s",buffer);
            if(strcmp(buffer,"Size:")==0)
            {
                sscanf(buf,"%s %d",empty,&size);
                sscanf(buf,"%s %s",empty,fsize);
                test=0;
            }
        }
    }
    //Parsing to the section of the file for the sectors
    test=1;
    while(test)
    {
        fgets(buf,1023,pfile);
        if(strlen(buf)>1)
        {
            sscanf(buf,"%s",buffer);
            if(strcmp(buffer,"Sectors:")==0)
                test=0;            
        }
    }
    int sector=size/1024*2;
        if(size%1024!=0)
    sector++;
    int sec1=sector;
    sector=sector/5;
    fgets(buf,1023,pfile);
    sscanf(buf,"%s",sector1);
    sector=sector-1;
    while(test<sector)
    {
        if(!feof(pfile))
        {
           fgets(buf,1023,pfile);
        }
        test++;
    }
    sector=sec1%8;
    if(sector==0)
        sscanf(buf,"%s %s %s %s %s %s %s %s",empty, empty, empty, empty, empty , empty, empty,sector2);
    
    else if(sector==7)
        sscanf(buf,"%s %s %s %s %s %s %s",empty,empty,empty,empty,empty,empty,sector2);
    else if(sector==6)
        sscanf(buf,"%s %s %s %s %s %s",empty,empty,empty,empty,empty,sector2);
    else if(sector==5)
        sscanf(buf,"%s %s %s %s %s",empty,empty,empty,empty,sector2);
    else if(sector==4)
        sscanf(buf,"%s %s %s %s",empty,empty,empty,sector2);
    else if(sector==3)
        sscanf(buf,"%s %s %s",empty,empty,sector2);
    else if(sector==2)
        sscanf(buf,"%s %s",empty,sector2);
    else if(sector==1)
        sscanf(buf,"%s",sector2);
    else
    {
        printf("We couldn't determine the end of the file\n");
        pclose(pfile);
        return 1;
    }
    pclose(pfile);
    return 0;
}
int istat_parse(char *command,char *sector1,char *sector2,char *fsize)
{
    char buf[1024],empty[2048],buffer[1024],fstype[256],filename[256];
    sscanf(command,"%s %s",empty,filename);
    int x;
    fsparse(filename,fstype);
    if(strcmp(fstype,"FAT32")==0 || strcmp(fstype,"FAT16")==0 || strcmp(fstype,"FAT")==0)
    {
        x=fat_istat(command,sector1,sector2,fsize);
    }
    else if(strcmp(fstype,"NTFS")==0 || strcmp(fstype,"ntfs")==0)
    {
        x=ntfs_istat(command,sector1,sector2,fsize);
    }
    else
    {
        printf("couldn't determine operating sytem type\n");
        return 1;
    }
    return x;
}

int fls_name(char *in)
{
    char empty[2048],name[256],command[256];
    char *p;
    char buf[1024]="%s";
    int x=0;
    int i=0,s=0;
    p=strchr(in,'/');
    while(p!=NULL)
    {
        i=0;
        while(in[s]!='/')
        {
            in[s]=' ';
            s++;
        }
        *p=' ';
        x++;
        p=strchr(in,'/');
    }
    sscanf(in,"%s",in);
    return 0;
}



int fsparse(char *filesystem,char *fstype)
{
    int x=1;
    char buf[2048],command[256],bufsize[1024],buffer[1024],em[256];
    int size;
    sprintf(command,"fsstat %s",filesystem);
    FILE *popen();
    FILE *in;
    in=popen(command,"r");
    fgets(buf,1024,in);
    sscanf(buf,"%s %s",bufsize,buffer);
    while(x)
    {
        if(strcmp(bufsize,"File")==0)
        {
            sscanf(buf,"%s %s %s %s",em,em,em,fstype);
            x=0;
        }
        else
        {
            fgets(buf,1024,in);
            sscanf(buf,"%s %s",bufsize,buffer);
        }
        if(feof(in)!=0)
        {
            printf("Can't read FSTYPE\n");
            return 0;
        }
    }
    x=1;
    while(x)
    {
        if(strcmp(bufsize,"Sector")==0 && strcmp(buffer,"Size:")==0)
        {
            sscanf(buf,"%s %s %d",bufsize,buffer,&size);
            x=0;
        }
        else
        {
            fgets(buf,1024,in);
            sscanf(buf,"%s %s",bufsize,buffer);
        }
        if(feof(in)!=0)
        {
            printf("Couldn't find sector size D=\n");
            pclose(in);
            return 0;
        }
    }
    pclose(in); 
    return size;
}



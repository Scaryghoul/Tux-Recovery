int command(int argc,char **argv)
{
    if(argc<2)
    {
        printf("You must enter a command!\n");
        return 1;
    }    
    else
    {
        //Handling drive aquisition
        if(strcmp(argv[1],"-dd")==0)
        {
            if(argc<3)
            {
                printf("You must enter a source for the drive dump\n");
                return 1;
            }
            else
            {
                if(argc<4)
                {
                    printf("You must enter a destination for the drive dump\n");
                    return 1;
                }
                else
                {
                    if(strcmp(argv[3],"/")==0)
                    {
                        printf("You have assigned %s as your destination, this is very dangerous....do you want to proceed?:(y,n) ",argv[3]);
                        char t=getchar();
                        if(t=='y')
                        {
                            if(argc>5)
                            {
                                data_dump(argv[2],argv[3],drive_bsize);
                                return 0;
                            }
                            else
                            {
                                data_dump(argv[2],argv[3],argv[4]);    
                                return 0;
                            }
                        }
                        else
                            return 0;
                    
                    }
                    if(argc<5)
                        data_dump(argv[2],argv[3],drive_bsize);
                    else
                        data_dump(argv[2],argv[3],argv[4]);
                }
            }
        }
//Sparse raw unallocated recovery
        else if(strcmp(argv[1],"-Rsau")==0)
        {
            if(argc<3)
            {
                printf("You must specify an image\n");
                return 1;
            }
            else if(argc<4)
            {
                printf("You must specify a keyword\n");
                return 1;
            }
            else
            {
                sparse_accurate_unalloc(argv[2],argv[3]);

            }
        }
//Handling help printing
        else if(strcmp(argv[1],"-h")==0 || strcmp(argv[1],"--help")==0)
        {
            help();
            return 0;
        }
//Validating files
        else if(strcmp(argv[1],"-v")==0)
        {
            if(argc<3)
            {
                printf("You must enter a filename\n");
                return 1;
            }
            else if(argc<4)
            {
                printf("You must enter a second filename\n");
                return 1;
            }
            else
            {
                checksum(argv[2],argv[3]);
                return 0;
            }
        }

//Handleing mounting the file systems read only.
        else if(strcmp(argv[1],"-mr")==0)
        {
            if(argc<3)
            {
                printf("You must enter a filesystem or device to mount\n");
                return 1;
            }
            else if(argc<4)
            {
                printf("You must specify a mount point\n");
                return 1;
            }
            else
                mount(argv[2],argv[3],1);
        }
//Handling mounting the filesystem with write support
        else if(strcmp(argv[1],"-m")==0)
        {
            if(argc<3)
            {
                printf("You must enter a filesystem or device to mount\n");
                return 1;
            }
            else if(argc<4)
            {
                printf("You must specify a mount point\n");
                return 1;
            }
            else
                mount(argv[2],argv[3],0);
        }

//ascii block searching(nothing else)
        else if(strcmp(argv[1],"-sb")==0)
        {
            if(argc<3)
            {
                printf("You must enter a filename\n");
                return 1;
            }
            else if(argc<4)
            {
                printf("You must enter a search string\n");
                return 1;
            }
            else
                ascii_block_search(argv[3],argv[2]);
        }
    //Handling controlled dumps
        else if(strcmp(argv[1],"-cd")==0)
        {
            if(argc<3)
            {
                printf("You must enter a source file\n");
                return 1;

            }
            else if(argc<4)
            {
                printf("You must enter a destination file\n");
                return 1;
            }
            else if(argc<5)
            {
                printf("You must enter a starting point in bytes\n");
                return 1;
            }
            else if(argc<6)
            {   
                printf("You must enter an amount of times to collect the data\n");
                return 1;
            }
            else if(argc<7)
            {
                controlled_dump(argv[2],argv[3],drive_bsize,argv[5],argv[4]);
                return 0;
            }
            else
            {
                controlled_dump(argv[2],argv[3],argv[6],argv[5],argv[4]);
                return 0;
            }
        }
    //Spare search of unallocated space
    else if(strcmp(argv[1],"-Rsu")==0)
    {
        if(argc<3)
        {
            printf("You must specify an image or source\n");
            return 1;  
        }
        else if(argc<4)
        {
            printf("You must enter a searchword\n");
            return 1;
        }
        else
        {
            sparse_unalloc(argv[2],argv[3]);
            return 0;
        }
    }
    //Recover all unallocated files
    else if(strcmp(argv[1],"-Ru")==0)
    {
        if(argc<3)
        {
            printf("You must specify an image or source\n");
            return 1;
        }
        else
        {
            all_unalloc(argv[2]);
            return 0;
        }
    }
    //Accurate recovery of all files in unallocated space
    else if(strcmp(argv[1],"-Rau")==0)
    {
        if(argc<3)
        {
            printf("You must specify an image or source\n");
            return 1;
        }
        else
        {
            accurate_all_unalloc(argv[2]);
            return 0;
        }

    }
    //Search for a specific filename in an image
    else if(strcmp(argv[1],"-s")==0)
    {
        if(argc<3)
        {
            printf("You must enter a source file\n");
            return 1;
        }
        else if(argc<4)
        {
            printf("You must enter a file to search for\n");
            return 1;
        }
        else
        {
            search_all(argv[2],argv[3]);
            return 0;
        }
    }

    //Listing all files in an image
    else if(strcmp(argv[1],"-l")==0)
    {
        if(argc<3)
        {
            printf("Enter a filesystem to scan\n");
            return 1;
        }
        else
        {
            list_all(argv[2]);
            return 0;
        }
    }
    //Sparse search
    else if(strcmp(argv[1],"-Rs")==0)
    {
        if(argc<3)
        {
            printf("You must specify an image\n");
            return 1;

        }
        else if(argc<4)
        {
            printf("You must specify a keyword\n");
            return 1;
        }
        else
        {
            sparse_all(argv[2],argv[3]);
            return 0;
        }
    }
    //Get all files in tact
    else if(strcmp(argv[1],"-R")==0)
    {
        if(argc<3)
        {
            printf("You must specify an image\n");
            return 1;
        }
        else
        {
            get_everything(argv[2]);
            return 0;
        }
    }
    else if(strcmp(argv[1],"-Ra")==0)
    {
        if(argc<3)
        {
            printf("You must specify an image\n");
        }
        else
        {
            accurate_all(argv[2]);
            return 0;
        }
    }
    //Recover a specific inode
    else if(strcmp(argv[1],"-Ri")==0)
    {
        if(argc<3)
        {
            printf("You must specify an image\n");
            return 1;
        }
        else if(argc<4)
        {
            printf("You must enter an inode\n");
            return 1;
        }
        else
        {
            accurate_recover_inode(argv[2],argv[3]);
            return 0;
        }

    }
    //Else
    else
    {
        printf("You have entered giberish, please attend to your errors\n");
        return 0;
    }    

    }
}

int help()
{
    printf("Welcome to the TuxRecovery help menu!\n");
    printf("Recovery:\n");
    printf("    -dd source destination bsize --- Dumps a drives contents to a destination. bsize is optional, default bsize is 4096\n");
    printf("    -cd source destination starting_point_bytes count --- controlled dump of an image\n");

    printf("Validation:\n");
    printf("    -v file1 file2 --- Validates file with both md5 & sha1sums\n");

    printf("Mounting:\n");
    printf("    -mr filesystem mount_point --- Mounts a filesystem as read only\n");
    printf("    -m filesystem mount_point --- Mounts a filesystem without a write blocker\n");

    printf("Searching:\n");
    printf("    -s sourcefile search_file --- Searches source for a filename, in both unallocated and allocated space\n");
    printf("    -sb sourcefile search_string --- Searches all blocks for an ascii string\n");
    
    printf("Listing:\n");
    printf("    -l filesystem --- List all files in the file system\n");

    printf("File Recovery:\n");
    printf("    -Rs source keyword ---Sparse recovery of all spaces based on a specific keyword\n");
    printf("    -Rsu source keyword --- Sparse recovery of unallocated space based on specific keywords\n");
    printf("    -Rsau source keyword --- Accurate sparse recovery of unallocated space based on specific keywords, slower\n");
    printf("    -R source --- Recover all files in allocated & unallocated space\n");
    printf("    -Ra source --- Accuratly recover all files from all space,slower\n");
    printf("    -Ru source --- Recover all unallocated files in a filesystem\n");
    printf("    -Rau source --- Accurate recovery of all unallocated files, slower\n");
    printf("    -Ri source inode --- Recover a specific inode\n");

    printf("Misc\n");
    printf("    -h --- Display this message\n");
    return 0;


}

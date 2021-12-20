//  The fstat code for getting file size and file permissions is thanks to the linux man page
//  for fstat code and the organization of the struct stat.
//

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "set.h"
#include "io.h"
#include "defines.h"

#define OPTIONS "hvi:o:"

Set flag_set = (Set) 0;

bool read_byte(int infile, uint8_t *x){
	static uint8_t buffer[BLOCK];
	static int index = 0;
	static int end = -1;  // Tracks the index after the last valid byte.

	// Fill buffer if empty.
	if (index == 0) {
		int bytes = read(infile, buffer, BLOCK);  //  should use read_bytes() here
		if (bytes < BLOCK) {
			// Set the end as 1 after the last valid byte.
			end = bytes +1;
		}
	}

	//  Return a byte out of the buffer.
	*x = buffer[index];
	index += 1;

	// Reset the index if it reaches the end of the buffer
	if (index == BLOCK) {
		index = 0;
	}

	// Return true/false depending on if there is a valid byte to return.
	return index != end;
}
/*
int main(int argc, char **argv) {
	uint8_t byte;
	uint8_t nl = '\n';

	while (read_byte(STDIN_FILENO, &byte )==true){
		write(STDOUT_FILENO, &byte, 1);
		write(STDOUT_FILENO, &nl, 1);
	}
	return 0;
}
*/


void print_histogram(uint64_t *table){
	int total_bytes=0;
	for (int i=0;i<ALPHABET;i++){
		if (table[i] != 0) {
			total_bytes += table[i];
			printf("Symbol: %3d %2x %c: %llu\n", i,i,i,table[i]);
		}
	}
	printf("Total bytes read = %d\n", total_bytes);

}

void build_histogram(int fd) {
	uint8_t byte;
	uint8_t nl = '\n';

	uint64_t histogrm_tab[ALPHABET] = {};	// initialize histogram to all zero's

	while (read_byte(fd, &byte)){
		histogrm_tab[byte] += 1;	// increment value for symbol byte
	}
	print_histogram(&histogrm_tab[0]);
}


int main(int argc, char *argv[]) {

    //  template for parsing code using getopt thanks to Eugene
    //  getopt will parse the command line passed.
    //  if no arguments, it will print out the help file else
    //  -h:  Prints out a help message	
    //  -v: Enable prints compressions stats to stderr.  Includes uncompressed, compressed file sizes and the 
    //      %compression reduction size
    //  -i infile:  Specify the input file path containing the file to compress.  stdin if not specified
    //  -o outfile:  Specify the ouput file path to write the huffman encoded file to.  stdout if not specified


    char *help_str = "SYNOPSIS\n"
                     "Creates a huffman encoded file\n"
                     "\n"
                     "USAGE\n"
		     "./encode [-v] [-h] [-i infile] [-o outfile]\n"
                     "\n"
		     "OPTIONS\n"
		     "-v             Compression stats to stderr\n"
		     "-h             Program usage and help.\n"
		     "-i infile      Input containing file to decode (default: stdin)\n"
		     "-o outfile     Output to write compressed huffman encoded file (default: stdout)\n";


    int opterr; //supress printing an error message for badly form args
    int opt = 0;
//    int ALL_flag = 0, STAT_flag =0;  //, HELP_flag=0;

//  Set up command line processing variable below
//
    opterr = 0;
 
    char str[101];    			//max line width to read is 100 characters 
    uint32_t vec, w, wt = 0;
    FILE *outfile = stdout;		//defaults to stdout
    int fdout = fileno(stdout);
    FILE *infile = stdin;		//defaults to stdin
    int fdin = fileno(stdin);
    char *file_name=optarg;
    uint32_t no_cities = 0;

 //
 //  Command line parsing is below.  Keep track of which switches and arguments
 //  where passed.  Change default value for 'directed' graph, input and 
 //  output files if specified.
 //  Display help if requested, and see if the use wants verbose output,
 //  ie, display of all Hamiltonian paths, not just the shortest.
 //
    
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'v':				// Set the Verbose switch - checked on printing
            flag_set = insert_set(0,flag_set);
            break;
        case 'i': 				// Set the input file name
	    flag_set = insert_set(2, flag_set); 
	    file_name = optarg;
	    fdin = open(file_name, O_RDONLY);  // open iput file for read only
	    if (fdin==-1) {
		    printf("Non existent file.  Exiting");
		    exit(-1);
	    }
		
	    break;
        case 'o': 
	    flag_set = insert_set(3, flag_set); 
	    file_name = optarg;
	    outfile= fopen(file_name, "w");  // open output file for write only
	    break;
        case 'h':
            flag_set = insert_set(8, flag_set);
            printf("%s\n", help_str);
            exit(1);
        default: 
	    printf("%s\n", help_str); 
	    exit(1);
        }
    }

    	
	struct stat sb;


   if (fstat(fdin, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

   printf("File type:                ");

   switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:  printf("block device\n");            break;
    case S_IFCHR:  printf("character device\n");        break;
    case S_IFDIR:  printf("directory\n");               break;
    case S_IFIFO:  printf("FIFO/pipe\n");               break;
    case S_IFLNK:  printf("symlink\n");                 break;
    case S_IFREG:  printf("regular file\n");            break;
    case S_IFSOCK: printf("socket\n");                  break;
    default:       printf("unknown?\n");                break;
    }

   printf("I-node number:            %ld\n", (long) sb.st_ino);

   printf("Mode:                     %lo (octal)\n", (unsigned long) sb.st_mode );

   printf("Link count:               %ld\n", (long) sb.st_nlink);
    printf("Ownership:                UID=%ld   GID=%ld\n",
            (long) sb.st_uid, (long) sb.st_gid);

   printf("Preferred I/O block size: %ld bytes\n",
            (long) sb.st_blksize);
    printf("File size:                %lld bytes\n", (uint64_t) sb.st_size);
    printf("Blocks allocated:         %lld\n",
            (long long) sb.st_blocks);

   printf("Last status change:       %s", ctime(&sb.st_ctime));
    printf("Last file access:         %s", ctime(&sb.st_atime));
    printf("Last file modification:   %s", ctime(&sb.st_mtime));

   build_histogram(fdin);
    
    exit(EXIT_SUCCESS);
}

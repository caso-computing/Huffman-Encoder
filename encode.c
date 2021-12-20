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
#include "iohelper.h"
#include "defines.h"
#include "node.h"
#include "pq.h"
#include "node.h"
#include "stats.h"	//  helper file for heap sort from assignment 3
#include "huffman.h"
#include "code.h"
#include "header.h"

#define OPTIONS "hvi:o:"

static uint16_t unique_sym = 0;
static int inindex=0;
//static uint8_t outbuffer[BLOCK];
static uint8_t inbuffer[BLOCK];

Set flag_set = (Set) 0;


// byte_fr_64 is a helper utility to convert types other than uint8 into a byte size chunk.
//  Only used right now to convert the header structure to bytes.
//

uint8_t byte_fr_64(uint64_t num, uint8_t pos){
    uint64_t mask=0x00000000000000ff;
    
    num &= (mask << (pos*8));
    num = (num>>pos*8);
    return (uint8_t) num;
}


/********************************************

// flushbuffer writes and remaining bytes to be written to outfile and then closes the file.
//
void flush_outbuffer(int outfile){
	if (outindex>0 & outindex < BLOCK) {
		write(outfile, outbuffer, outindex);
		close(outfile);
	}
}

// buffer the bytes to be written until you have a full BLOCK to write.
// make sure you flush the output when you are done writing blocks to ensure everything
// is written to the disk.

bool write_byte(int outfile, uint8_t x){
	//static int end = -1;  //tracks the index after the last valid byte

	//empty buffer if full
	outbuffer[outindex] = x;
	outindex += 1;

	if (outindex == 0) {
		int bytes = write(outfile, outbuffer, BLOCK); // should use write_bytes()
		if (bytes < BLOCK){
			fprintf(stderr, "not all bytes were written to outfile");
			exit(-1);
		}
		outindex = 0;  //reset index to start of buffer since we just dumped a BLOCK
	}
	return true;
}
*////////////////////////////////////////////

bool read_byte(int infile, uint8_t *x){
	static int end = -1;  // Tracks the index after the last valid byte.

	// Fill buffer if empty.
	if (inindex == 0) {
		int bytes = read_bytes(infile, inbuffer, BLOCK);  //  should use read_bytes() here
		if (bytes < BLOCK) {
			// Set the end as 1 after the last valid byte.
			end = bytes +1;
		}
	}

	//  Return a byte out of the buffer.
	*x = inbuffer[inindex];
	inindex += 1;

	// Reset the index if it reaches the end of the buffer
	if (inindex == BLOCK) {
		inindex = 0;
	}

	// Return true/false depending on if there is a valid byte to return.
	return inindex != end;
}

void write_header(int fileout, Header head){

  for (uint32_t i=0;i<sizeof(head.magic);i++){
      write_byte(fileout, byte_fr_64((uint64_t)head.magic,i));
  }
  for (uint16_t i=0;i<sizeof(head.permissions);i++){
      write_byte(fileout, byte_fr_64((uint64_t)head.permissions,i));
  }
  for (uint16_t i=0;i<sizeof(head.tree_size);i++){
      write_byte(fileout, byte_fr_64((uint64_t)head.tree_size,i));
  }
  for (uint64_t i=0;i<sizeof(head.file_size);i++){
      write_byte(fileout, byte_fr_64((uint64_t)head.file_size,i));
  }
}

void print_histogram(uint64_t *table){
	int total_bytes=0;
	for (int i=0;i<ALPHABET;i++){
		if (table[i] != 0) {
			total_bytes += table[i];
			fprintf(stderr,"Symbol: %3d %2x %c: %"PRIu64"\n", i,i,i,table[i]);
		}
	}
	fprintf(stderr,"Total bytes read = %d\n", total_bytes);

}

void build_histogram(int fd, uint64_t *table) {
	uint8_t byte;

	while (read_byte(fd, &byte)){
		table[byte] += 1;	// increment value for symbol byte
	}
	for (int i=0;i<ALPHABET;i++){
		if (table[i]>0) {
			unique_sym += 1;
		}
	}

	print_histogram(table);
}

void print_code_table(Code *table){
	for (int i=0;i<ALPHABET;i++){
		if ((table[i].top!=0 )){
			fprintf(stderr, "Symbol: %3d - ",i);
			code_print(&table[i]);
			printf("\n");
		}
	}
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

    uint64_t histogrm_tab[ALPHABET] ;	// initialize histogram to all zero's
    Code code_tab[ALPHABET];		// initialize huffman code table to all zero's too
    
    // Manually initialize both tables above since some implementations of c don't allow you
    // to create zero arrays/tables.
    //
    for (int i=0;i<ALPHABET;i++){
	    histogrm_tab[i] = 0;
	    code_tab[i].top=0;
	    for (int y=0;y<MAX_CODE_SIZE;y++){
		    code_tab[i].bits[y]=0;
	    }
    }

    
    struct stat sb;
    Header head;

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

//  Set up command line processing variable below
//
    opterr = 0;
 
    int fdout = fileno(stdout);
    int fdin = fileno(stdin);
    char *file_name=optarg;

 //
 //  Command line parsing is below.  Keep track of which switches and arguments
 //  where passed.  Change default value for 'directed' graph, input and 
 //  output files if specified.
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
	    fdout= open(file_name, O_WRONLY | O_APPEND | O_CREAT, 0644);  // open output file for write only
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

    	


   if (fstat(fdin, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }


   printf("Mode:                     %lo (octal)\n", (unsigned long) sb.st_mode );
    printf("File size:                %"PRIu64" bytes\n", (uint64_t) sb.st_size);
    printf("Blocks allocated:         %lld\n",
            (long long) sb.st_blocks);

   build_histogram(fdin, &histogrm_tab[0]);
   Node *root_node = build_tree(&histogrm_tab[0]);
   build_codes(root_node, &code_tab[0]);
   print_code_table(&code_tab[0]);

//
//  code to write the header to the output buffer
//  
  head.magic=MAGIC;
  head.permissions=sb.st_mode;
  head.tree_size=(3*unique_sym)-1;
  head.file_size=sb.st_size;
  write_header(fdout,head);

   dump_tree(fdout, root_node);
 //  write_encoded_file(fdin,fdout,&code_tab[0]);
   flush_outbuffer(fdout);

    exit(EXIT_SUCCESS);
}

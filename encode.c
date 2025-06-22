#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;								// Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);						// Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);						// Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);						// Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open src image file %s\n", encInfo->src_image_fname);
	return e_failure;
    }
    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open secret file%s\n", encInfo->secret_fname);
	return e_failure;
    }
    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open stego image file %s\n", encInfo->stego_image_fname);
	return e_failure;
    }
    // No failure return e_success
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo,char *magic_string)
{
    if(open_files(encInfo) == e_success)
    {
	printf("Success : open file functions\n");
	if(check_capacity(encInfo) == e_success)
	{
	    printf("Checking capacity is done\n");
	    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
	    {
		printf("Copy bmp header is successfull\n");
		if(read_magic_string(magic_string) == e_success)
		{
		    encInfo->length = calculate_magic_string_length(magic_string);
		    printf("Magic string length: %d\n", encInfo->length);
		    printf("\n");
		    if(encode_size_of_magic_string(encInfo) == e_success)
		    {
			printf("\nSize_of_magic_string_function_done\n");
			if(encode_magic_string(magic_string,encInfo) == e_success)
			{
			    printf("\nMagic string Encoding is Successfull\n");
			    printf("\n");

			    encInfo-> extension_size = secret_file_extension_size(encInfo->secret_fname, encInfo);
			    printf("Extension size = %d\n",encInfo->extension_size);

			    if(encode_secret_file_extension_size(encInfo->extension_size, encInfo) == e_success)
			    {
				printf("\nSecret file extension size is encoded successfull\n");
				if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
				{
				    printf("\nSecret file extension encoded successfull\n");
				    if(encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_success)
				    {
					printf("\nSecret file size encoded successfull\n");
					if(encode_secret_file_data(encInfo) == e_success)
					{
					    printf("\nSecret file data is encoded successfull\n");
					    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
						printf("Encode remaining data is copied\n");
					    else
						printf("Failed : To copy the Remaining data\n");
					}
					else	
					    printf("Failed : Secret file data is not encoded\n");
				    }
				    else
				    {
					printf("Failed : secret file size is not encoded\n");
				    }
				}
				else
				    printf("Failed : Secret file extension is not encoded\n");
			    }
			    else
				printf("Filed : Secret file extension size is not encoded\n");
			}
			else
			    printf("Failed : Magic string is not encoded\n"); 
		    }
		    else
			printf("Failed : size of magic string is not encoded\n");
		}
		else
		    printf("Failed : unable to read magic the string\n");
	    }
	    else
		printf("Failed : Unable to copy BMP header\n");
	}
	else
	    printf("Failed : Unable to check capacity\n");
    }
    else
	printf("Failed To open file\n");
}
         //Header       
Status check_capacity(EncodeInfo *encInfo)									// Check capacity
{   
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);						// Secret file size
    printf("Secret file size = %ld\n",encInfo->size_secret_file);
    return e_success;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)						// Copy BMP header
{
    char *header[BMP_HEADER];
    fseek(fptr_src_image,0,SEEK_SET);
    fread(header,sizeof(char),BMP_HEADER,fptr_src_image);
    fwrite(header,sizeof(char),BMP_HEADER,fptr_dest_image);
    int fptr_offset = ftell(fptr_src_image);
    printf("\nCOPY BMP SRC POINTER = %d\n",fptr_offset);
    int dptr_offset = ftell(fptr_dest_image);
    printf("\nCOPY BMP DEST POINTER = %d\n",dptr_offset);
    printf("\n");
    return e_success;
}

uint get_file_size(FILE *fptr)											// File size
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}

Status read_magic_string(char* magic_string)									// Read the magic string from the user
{
    printf("Enter the magic string : ");                    
    scanf("%s", magic_string);
    printf("Magic string : %s\n", magic_string);
    return e_success;
}

int calculate_magic_string_length(const char* magic_string)							// Calculate the length of magic string
{
    return strlen(magic_string);
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)					// Encode the magic string
{
    char buff[8];	

   
    int i;
    for(i = 0; i < encInfo->length; i++)
    {
	fread(buff,1,8,encInfo->fptr_src_image);
	encode_byte_to_lsb(magic_string[i],buff);
	fwrite(buff,1,8,encInfo->fptr_stego_image);
    }
    int fptr_offset = ftell(encInfo->fptr_src_image);
    printf("\nENCODE MAGIC STRING SRC POINTER = %d\n",fptr_offset);
    int dptr_offset = ftell(encInfo->fptr_stego_image);
    printf("\nENCODE MAGIC STRING  DEST  POINTER = %d",dptr_offset);
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)							// Encode magic string to LSB
{
    int temp, num=0;
    for(int i = 0; i < 8; i++)
    {
	image_buffer[i] = image_buffer[i] & (~1);
	temp = data & 1 ;
	image_buffer[i] = image_buffer[i] | temp;
	data = data >> 1;
    }
    for(int i=0;i<8;i++)
    {
	temp= image_buffer[i] & 1;
	num = (temp<<i) | num;
    }
    char ch = (char)num;			                                                              	// Converting ASCII to character
    printf("%c ",ch);    
    return e_success;
}

Status encode_size_of_magic_string(EncodeInfo *encInfo)								// Encode magic string size 32bytes
{
    printf("Encode Magic string size\n");
    char buff[32];
    fread(buff,1,32,encInfo->fptr_src_image);
    encode_size_byte_to_lsb(encInfo->length,buff);
    fwrite(buff,1,32,encInfo->fptr_stego_image);
    int fptr_offset = ftell(encInfo->fptr_src_image);
    printf("\nENCODE MAGIC STRING SIZE SRC POINTER = %d\n",fptr_offset);
    int dptr_offset = ftell(encInfo->fptr_stego_image);
    printf("\nENCODE MAGIC STRING  SIZE DEST  POINTER = %d\n",dptr_offset);
    printf("\n");
    return e_success;
}

Status encode_size_byte_to_lsb(int data, char *image_buffer)							// Encode magic string size to LSB
{
    int temp, num=0;
    for(int i = 0; i < 32; i++)
    {
	image_buffer[i] = image_buffer[i] & (~1);
	temp = data & 1 ;
	image_buffer[i] = image_buffer[i] | temp;
	data = data >> 1;
    }
    return e_success;
}

int secret_file_extension_size(const char *filename, EncodeInfo *encInfo)				// Calculate the Secret file extension size
{
    const char *extension = strrchr(filename,'.');							// Find the Secret file extension 
                                                                                                        // extn_secret_file = extension
    strcpy(encInfo->extn_secret_file,extension);
    if(encInfo->extn_secret_file!= NULL)
	printf("Extension = %s\n",encInfo->extn_secret_file);						// Printing extension
    else
	printf("Match not found\n");
    return strlen(encInfo->extn_secret_file);
}

Status encode_secret_file_extension_size(int extension_size, EncodeInfo *encInfo)			// Encode Secret file extension size
{
    char buff[32];
    fread(buff,1,32,encInfo->fptr_src_image);
    encode_extension_size_byte_to_lsb(extension_size,buff);
    fwrite(buff,1,32,encInfo->fptr_stego_image);
    int fptr_offset = ftell(encInfo->fptr_src_image);
    printf("\nENCODE SIZE SECRET FILE EXTN SRC POINTER = %d\n",fptr_offset);
    int dptr_offset = ftell(encInfo->fptr_stego_image);
    printf("\nENCODE SIZE SECRET FILE EXTN  DEST  POINTER = %d\n",dptr_offset);
    printf("\n");
    return e_success;
}


Status encode_extension_size_byte_to_lsb(int data, char *image_buffer)					// Encode the secret file extension to LSB
{
    int temp, num=0;
    for(int i = 0; i < 32; i++)
    {
	image_buffer[i] = image_buffer[i] & (~1);
	temp = data & 1 ;
	image_buffer[i] = image_buffer[i] | temp;
	data = data >> 1;
    }
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)				// Encode Secret file extension
{
    char buff[8];
    int i;
    for(i = 0; i < encInfo->extension_size; i++)
    {
	fread(buff,1,8,encInfo->fptr_src_image);
	encode_secret_file_extn_byte_to_lsb(encInfo->extn_secret_file[i],buff);
	fwrite(buff,1,8,encInfo->fptr_stego_image);
    }
    int fptr_offset = ftell(encInfo->fptr_src_image);
    printf("\nENCODE  SECRET FILE EXTN SRC POINTER = %d\n",fptr_offset);   
    int dptr_offset = ftell(encInfo->fptr_stego_image);
    printf("\nENCODE  SECRET FILE EXTN DSTN POINTER = %d\n",dptr_offset);
    printf("\n");
    return e_success;
}

Status encode_secret_file_extn_byte_to_lsb(char data, char *image_buffer)				// Encode Secret file extension to LSB
{
    int temp, num=0;
    for(int i = 0; i < 8; i++)
    {
	image_buffer[i] = image_buffer[i] & (~1);
	temp = data & 1 ;
	image_buffer[i] = image_buffer[i] | temp;
	data = data >> 1;
    }
    for(int i=0;i<8;i++)
    {
	temp= image_buffer[i] & 1;								
	num = (temp<<i) | num;
    }
    char ch = (char)num;										// Converting ASCII to character
    printf("%c",ch);											// Printing the secret file data
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)					// Encode Secret file size
{
    char size_buff[32];
    fread(size_buff,1,32,encInfo->fptr_src_image);
    encode_secretfile_size_to_lsb(file_size, size_buff);
    fwrite(size_buff,1,32,encInfo->fptr_stego_image);
    int fptr_offset = ftell(encInfo->fptr_src_image);
    printf("\nENCODE  SECRET FILE SIZE SRC POINTER = %d\n",fptr_offset);   
    int dptr_offset = ftell(encInfo->fptr_stego_image);
    printf("\nENCODE  SECRET FILE SIZE DSTN POINTER = %d\n",dptr_offset);
    printf("\n");
    return e_success;
}

Status encode_secretfile_size_to_lsb(long file_size, char *image_buffer)				// Encode Secret file size to LSB
{
    int temp, num=0;
    for(int i = 0; i < 32; i++)
    {
	image_buffer[i] = image_buffer[i] & (~1);
	temp = file_size & 1 ;
	image_buffer[i] = image_buffer[i] | temp;
	file_size = file_size >> 1;
    }
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)							// Encode secret file data
{
    char buff[encInfo->size_secret_file];
    fseek(encInfo->fptr_secret,0,SEEK_SET);
    for(int i=0;i<encInfo->size_secret_file;i++)
    {
	char ch = fgetc(encInfo->fptr_secret);
	fread(buff, 1,8, encInfo->fptr_src_image);
	encode_byte_to_lsb(ch,buff);
	fwrite(buff,1,8,encInfo->fptr_stego_image);
    }
    int fptr_offset = ftell(encInfo->fptr_src_image);
    printf("\nENCODE  SECRET DATA SRC POINTER = %d\n",fptr_offset);   
    int dptr_offset = ftell(encInfo->fptr_stego_image);
    printf("\nENCODE  SECRET DATA DSTN POINTER = %d\n",dptr_offset);
    printf("\n");
    return e_success;
}

Status encode_data_byte_to_lsb(char data, char *image_buffer, int size)							// Encode the data to LSB
{
    int temp, num=0;
    for(int i = 0; i < size; i++)
    {
	image_buffer[i] = image_buffer[i] & (~1);
	temp = data & 1 ;
	image_buffer[i] = image_buffer[i] | temp;
	data = data >> 1;
    }
    for(int i=0;i<size;i++)
    {
	temp= image_buffer[i] & 1;
	num = (temp<<i) | num;
    }
    char ch = (char)num;				// Converting ASCII to character
    printf("%c",ch);					// Printing the secret file data					
}


Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)			// Copying the remaining data
{
    int ch;
    while ((ch = fgetc(fptr_src)) != EOF)					// Take each character and check whether it is equal to EOF
    {
	fputc(ch, fptr_dest);
    }
    int fptr_offset = ftell(fptr_src);
    printf("\nENCODE  COPY REMAINING SRC POINTER = %d\n",fptr_offset);   
    int dptr_offset = ftell(fptr_dest);
    printf("\nENCODE  COPY REMAINING DSTN POINTER = %d\n",dptr_offset);
    printf("\n");
    return e_success; 
}


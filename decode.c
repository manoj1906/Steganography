#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>

Status open_decode_files(DecodeInfo *dInfo)
{
    // Stego Image file
    dInfo->fptr_stego_image = fopen(dInfo->stego_image_fname, "r");
    // Do Error handling
    if (dInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open stego image file %s\n", dInfo->stego_image_fname);
	return e_failure;
    }
    dInfo->output_fptr = fopen(dInfo->output_fname, "w");
    // Do Error handling
    if (dInfo->output_fptr == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open stego image file %s\n", dInfo->output_fname);
	return e_failure;
    } 
    return e_success;
}

Status do_decoding(DecodeInfo *dInfo,char *magic_string)
{
    if(open_decode_files(dInfo) == e_success)						// Open files
    {
	printf("Success : File open\n");
	if(decode_size_of_magic_string(dInfo) == e_success)				// Magic string size
	{	
	    printf("Success : Size of magic string is Decoded\n");
	    if(decode_magic_string(dInfo,magic_string) == e_success)			// Magic String
	    {
		printf("Success : Magic string is Decoded\n");
		if(decode_secret_file_extn_size(dInfo) == e_success)                     //secret file external size
		{
		    printf("Success : Secret file extension size Decoded\n");            
		    if(decode_secret_file_extn(dInfo) == e_success)                      //secret file extension
		    {
			printf("Success : Secret file extension Decoded\n");
			if(decode_secret_file_size(dInfo) == e_success)                   //secret file size
			{
			    printf("Success : Secret file size Decoded\n");
			    if(decode_secret_file_data(dInfo) == e_success)               //secfet file data
			    {
				printf("Success : Secret file data Decoded\n");

			    }
			    else
				printf("Failed : Secret file data decoding\n");
			}
			else
			    printf("Failed : Secret file size decoding\n");
		    }
		    else
		    printf("Failed : Secret file extension decoding\n");              
		}
		else
		    printf("Failed : Secret file extension size decoding\n");
	    }
	    else
		printf("Failed : Magic string decoding\n");
	}
	else
	    printf("Failed : Size of magic String decoding\n");
    }
    else
	printf("Failed : Unable to open file\n");
}

Status decode_size_of_magic_string(DecodeInfo *dInfo)						// Decoding Magic string size
{
    char image_buff[32];
    int temp,num = 0;
    fseek(dInfo->fptr_stego_image, 54, SEEK_SET);
    fread(image_buff, 1, 32, dInfo->fptr_stego_image);
    for(int i=0;i< 32;i++)
    {
	temp = image_buff[i] & 1;
	num = (temp<<i) | num;
    }
    dInfo->length = num;
    printf("Magic string size = %d\n",dInfo->length);   
    int dptr_offset = ftell(dInfo->fptr_stego_image);
    printf("\nENCODE MAGIC STRING SIZE DEST  POINTER = %d\n",dptr_offset);
    return e_success;
}

Status decode_magic_string(DecodeInfo *dInfo,char *magic_string)					// Decoding the magic string
{
    printf("Magic string = ");
    char image_buff[8], string[10];
    for (int i = 0; i < dInfo->length; i++)
    {
	int num = 0,temp;
	fread(image_buff, 1, 8, dInfo->fptr_stego_image);
	for (int j = 0; j < 8; j++)
	{
	    temp = image_buff[j] & 1;
	    num = (temp<<j) | num;
	}
	char ch = (char)num;
	printf("%c",ch);
	string[i] = ch;
    }

    int dptr_offset = ftell(dInfo->fptr_stego_image);
    printf("\nENCODE MAGIC STRING  DEST  POINTER = %d\n",dptr_offset);
    return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo *dInfo)						// Decode the Secret file extension size
{
    char image_buff[32];
    int temp,num = 0;
    fseek(dInfo->fptr_stego_image, 102, SEEK_SET);
    fread(image_buff, 1, 32, dInfo->fptr_stego_image);
    for(int i=0;i< 32;i++)
    {
	temp = image_buff[i] & 1;
	num = (temp<<i) | num;
    }
    printf("size of extension :%d\n",dInfo->extension_size);
    dInfo->extension_size = num;
    printf("Magic string extension size = %d\n",dInfo->extension_size);   
    int dptr_offset = ftell(dInfo->fptr_stego_image);
    printf("\nENCODE MAGIC STRING EXTENSION SIZE DEST  POINTER = %d\n",dptr_offset);
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *dInfo)					// Decoding the Secert file extension 
{
    printf("Secret file extension = ");
    char image_buff[8], string[10];
    for (int i = 0; i < dInfo->extension_size; i++)
    {
	int num = 0,temp;
	fread(image_buff, 1, 8, dInfo->fptr_stego_image);
	for (int j = 0; j < 8; j++)
	{
	    temp = image_buff[j] & 1;
	    num = (temp<<j) | num;
	}
	char ch = (char)num;
	printf("%c",ch);
	string[i] = ch;
    } 
    int dptr_offset = ftell(dInfo->fptr_stego_image);
    printf("\nENCODE MAGIC STRING SIZE DEST  POINTER = %d\n",dptr_offset);
    return e_success;
}


Status decode_secret_file_size(DecodeInfo *dInfo)					// decoding the Secret file size
{
    char image_buff[32];
    int temp,num = 0;
    fread(image_buff, 1, 32, dInfo->fptr_stego_image);
    for(int i=0;i< 32;i++)
    {
	temp = image_buff[i] & 1;
	num = (temp<<i) | num;
    }
    dInfo->size_secret_file = num;
    printf("Secret file size = %ld\n",dInfo->size_secret_file);   
   int dptr_offset = ftell(dInfo->fptr_stego_image);
    printf("\nENCODE SECRET DATA SIZE DEST POINTER = %d\n",dptr_offset);
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *dInfo)					// Decoding the Secret file data
{
    printf("Secret file data = ");
    char image_buff[8], string[10];
    printf("SECRET FILE SIZE = %lu\n",dInfo->size_secret_file);
    for (int i = 0; i < dInfo->size_secret_file; i++)
    {
	int num = 0,temp;
	fread(image_buff, 1, 8, dInfo->fptr_stego_image);
	for (int j = 0; j < 8; j++)
	{
	    temp = image_buff[j] & 1;
	    num = (temp<<j) | num;
	}
	char ch = (char)num;
	printf(" secret data = %c\n",ch);
	fwrite(&ch, 1, 1, dInfo->output_fptr);
    }
    int dptr_offset = ftell(dInfo->fptr_stego_image);
    printf("\nENCODE MAGIC STRING DATA DEST  POINTER = %d\n",dptr_offset);
    return e_success;
}


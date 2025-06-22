#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <string.h>

#define MAX_MAGIC_STRING_LENGTH 10

unsigned char magic_string[MAX_MAGIC_STRING_LENGTH];

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo dInfo;
    uint img_size;
    //    FILE *fptr;
    int var;
    //char magic_string[MAX_MAGIC_STRING_LENGTH];

    // Fill with sample filenames
    //encInfo.src_image_fname = "beautiful.bmp";
    encInfo.secret_fname = "secret.txt";
    //encInfo.stego_image_fname = "stego_img.bmp";

    if(argc >= 3)
    {
	var = check_operation_type(argv);
	if(var == e_encode)
	{
	    printf("Chosen encoding\n");
	    if(read_and_validate_encode_args(argv,&encInfo) == e_success)
	    {
		printf("Success : Read and Validate function part is done\n");
		if(do_encoding(&encInfo,magic_string) == e_success)
		{
		    printf("Success: open file function\n");
		}
	    }
	    else
	    {
		printf("Error Encoding\n");
	    }
	}
	else if(var == e_decode)
	{
	    printf("Choosen Decoding\n");
	    if(read_and_validate_decode_args(argv,&dInfo) == e_success)
	    {
		printf("Success : Read and Validate function part is done\n");
		if(do_decoding(&dInfo,magic_string) == e_success)
		{
		    printf("Success: open file function\n");
		}
	    }
	    else
	    {
		printf("Error Decoding\n");
	    }
	}
    }
    else
    {
	printf("Error Insufficient number of command line argument\n");
    }

    return 0;
}

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-e") == 0)
    {
	return e_encode;
    }
    else if(strcmp(argv[1], "-d") == 0)
    {
	return e_decode;
    }
    else
    {
	return e_unsupported;
    }
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strcmp((strstr(argv[2],".")),".bmp")==0)
    {
	encInfo->src_image_fname = argv[2] ;
	printf("Image file check Done\n");
    }
    else
    {
	printf("Enter proper file name with .bmp extension\n");
	return e_failure;
    }

    if(strcmp((strstr(argv[3],".")),".txt")==0)
    {
	encInfo->secret_fname,argv[3] = argv[3];
	printf("Scret text File checked\n");
    }
    else
    {
	printf("Enter proper file name with .txt extension\n");
	return e_failure;
    }
    if(argv[4] == NULL)
    {
	encInfo->stego_image_fname = "stego_image.bmp";
	printf("File created\n");
    }
    else
    {
	encInfo->stego_image_fname = argv[4];
    }
    return e_success;
}

Status read_and_validate_decode_args(char *argv[], DecodeInfo *dInfo)
{
    if(strcmp((strstr(argv[2],".")),".bmp")==0)
    {
	dInfo->stego_image_fname = argv[2] ;
	printf("Image file check Done\n");
    }
    else
    {
	printf("Enter proper file name with .bmp extension\n");
	return e_failure;
    }

    if(argv[3] == NULL)
    {
	dInfo->output_fname = "output.txt";
	printf("File created\n");
    }
    else
    {
	dInfo->output_fname = argv[3];
    }
    return e_success;
}



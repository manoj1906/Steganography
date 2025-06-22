#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

#define BMP_HEADER 54
#define MAX_MAGIC_STRING_LENGTH 10
#define LSB_MASK 0xFE


typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
    uint image_capacity;
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;				//

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    //unsigned char magic_string[MAX_MAGIC_STRING_LENGTH];		
    int length;				//
    int extension_size;			//
    //const char *extension;

    char *output_fname;			//
    FILE *output_fptr;			//
    char *extn;
    //    char final_fname[100];
    char secretFilename[100];
    char outputFilename[100];
    char outputFilenameWithoutExtension[100];

} DecodeInfo;


//const char *extension;
/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *dInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *dInfo,char *magic_string);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *dInfo);

/*Decode size of magic string */
Status decode_size_of_magic_string(DecodeInfo *dInfo);

/*Decode lsb to byte*/
Status decode_lsb_to_byte(char *image_buff, int size);

/*Decode magic string*/
Status decode_magic_string(DecodeInfo *dInfo, char *magic_string);

/*Decode secret file extension size*/
Status decode_secret_file_extn_size(DecodeInfo *dInfo);

/*Decode secret file extension*/
Status decode_secret_file_extn(DecodeInfo *dInfo);

/*Decode secret file size*/
Status decode_secret_file_size(DecodeInfo *dInfo);

/*Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *dInfo);

/*reove extesion*/
void removeExtension(char* fileName);

/*read magiv string*/
Status read_magic_string(char *magic_string);

/*calculate magic string*/
int calculate_magic_string_length(const char* magic_string);

/* Store Magic String size */
Status decode_magic_string_size(int length);

/*decode magic string size*/
Status decode_size_of_magic_string(DecodeInfo *dInfo);

/*decode size byte to lsb*/
Status decode_size_byte_to_lsb(int data, char *image_buffer);

/*decode secret file extension*/
Status decode_secret_file_extension_size(int extension_size, DecodeInfo *dInfo);

/*decode extension size byte to lsb*/
Status decode_extension_size_byte_to_lsb(int data, char *image_buffer);

/*decode secret file extension byte to lsb*/
Status decode_secret_file_extn_byte_to_lsb(char data, char *image_buffer);

/*decode secrtfile size to lsb*/
Status decode_secretfile_size_to_lsb(long file_size, char *image_buffer);

/*Status decode_byte_to_lsb*/
Status decode_data_byte_to_lsb(char data, char image_buffer, int size);


#endif

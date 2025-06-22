#ifndef ENCODE_H
#define ENCODE_H

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
//#define MAX_MAGIC_STRING_LENGTH 10
#define LSB_MASK 0xFE


typedef struct _EncodeInfo
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
    long size_secret_file;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;


    //    unsigned char magic_string[MAX_MAGIC_STRING_LENGTH];
    int length;
    int extension_size;
    //const char *extension;

} EncodeInfo;

//const char *extension;
/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo,char *magic_string);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo);

/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

/*read the magic string*/
Status read_magic_string(char* magic_string);

/*calculate magic string*/
int calculate_magic_string_length(const char* magic_string);

/*encode magic string*/
Status encode_magic_string_size(int length);

/*encode size of magic string*/
Status encode_size_of_magic_string(EncodeInfo *encInfo);

/*encode sixe to byte to lsb*/
Status encode_size_byte_to_lsb(int data, char *image_buffer);

/*secret file ectension size */
int secret_file_extension_size(const char *filename, EncodeInfo *encInfo);

/*encode secret file extension size*/
Status encode_secret_file_extension_size(int extension_size, EncodeInfo *encInfo);

/*encode extension sixe byte to lsb*/
Status encode_extension_size_byte_to_lsb(int data, char *image_buffer);

/*encode secret file extension byte to lsb*/
Status encode_secret_file_extn_byte_to_lsb(char data, char *image_buffer);

/*encode secretfile size to lsb*/
Status encode_secretfile_size_to_lsb(long file_size, char *image_buffer);

/*encode dat byte to lsb*/
Status encode_data_byte_to_lsb(char data, char *image_buffer, int size);


#endif

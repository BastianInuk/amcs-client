#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
/*
vbe_info:


section .bss
;vesainfo: resb 512
loopvar: resw 1

mode_info:

*/
typedef struct
{
  unsigned char signature[4]; // db "VBE2";
  unsigned int version; // dw 0x0300;
  unsigned long oem; // dd 0x00;
  unsigned long capabilities; // dd 0x00;
  unsigned long video_modes; // dd 0x00;
  unsigned int video_memory; // dw 0x00;
  unsigned int software_rev; // dw 0x00;
  unsigned long vendor; // dd 0x00;
  unsigned long product_name; // dd 0x00;
  unsigned long product_rev; // dd 0x00;
  unsigned char reserved[222]; // times 222 db 0x00;
  unsigned char oem_data[256]; // db 0x00;

} vbe_info_struct;

typedef struct {
  unsigned int attributes; // resw 1;
  unsigned char window_a; // resb 1;
  unsigned char window_b; // resb 1;
  unsigned int granularity; // resw 1;
  unsigned int window_size; // resw 1;
  unsigned int segment_a; // resw 1;
  unsigned int segment_b; // resw 1;
  unsigned long win_func_ptr; // resd 1;
  unsigned int pitch; // resw 1;
  unsigned int width; // resw 1;
  unsigned int height; // resw 1;
  unsigned char w_char; // resb 1;
  unsigned char y_char; // resb 1;
  unsigned char planes; // resb 1;
  unsigned char bpp; // resb 1;
  unsigned char banks; // resb 1;
  unsigned char memory_model; // resb 1;
  unsigned char bank_size; // resb 1;
  unsigned char image_page; // resb 1;
  unsigned char reserved; // resb 1;
  unsigned char red_mask; // resb 1;
  unsigned char red_position; // resb 1;
  unsigned char green_mask; // resb 1;
  unsigned char green_position; // resb 1;
  unsigned char blue_mask; // resb 1;
  unsigned char blue_position; // resb 1;
  unsigned char reserved_mask; // resb 1;
  unsigned char reserved_position; // resb 1;
  unsigned char direct_color_attribute; // resb 1;
  unsigned char framebuffer; // resb 1;
  unsigned long off_screen_mem_off; // resd 1;
  unsigned int off_screen_mem_size; // resw 1;
  unsigned char reserved1[206];
} mode_info_struct;

typedef struct {
  unsigned int height;
  unsigned int width;
  unsigned int bpp;
} modes_struct;


int main(void) {
 vbe_info_struct vbe_info;
 mode_info_struct mode_info;
 unsigned int vbe_ptr, vbe_modes_ptr, mode_info_ptr; // vbe_ptr
 unsigned int mode, err, video_mode, modes_counter, i;
 unsigned short far *video_mode_ptr;
 modes_struct modes[20];
 unsigned short mode_number[20];
 vbe_info.signature[0] = 'V';
 vbe_info.signature[1] = 'B';
 vbe_info.signature[2] = 'E';
 vbe_info.signature[3] = '2';
 err = 0;
 vbe_ptr = &vbe_info;
 asm {
  mov ax, 0x4F00;
  mov di, vbe_ptr;
  int 0x10;
  mov err, ax;
 }
 video_mode_ptr = vbe_info.video_modes;
 modes_counter = 0;
 mode_info_ptr = &mode_info; // &modes[modes_counter];
 i = 0;
 while(video_mode != 0xFFFF) {
  video_mode = *video_mode_ptr; // 0x17D;
  //mode_info_ptr = &mode_info;
  mode_number[i] = video_mode;
  //printf("%04X\r\n",mode_number[i]);
  video_mode_ptr++;
  i++;
 }
 modes_counter = i;
 //free(&vbe_info);
  //video_mode = *video_mode_ptr;
  //printf("videomode: %4X", video_mode);
 i = 0;
 mode_info_ptr = &mode_info;
 //printf("fisk: %d\r\n", modes_counter);
 while (mode_number[i] != 0xFFFF) {
  //mode_info_ptr = &mode_info;
  mode = mode_number[i];
  asm {
   mov ax, 0x4F01;
   mov cx, mode;
   mov di, mode_info_ptr; // den her virker ikke
   int 0x10;
   mov err, ax;
  }
  //printf("mode, bpp: %04X, %d\r\n", mode, mode_info.bpp);
  //mode_number[modes_counter] = video_mode;
  modes[i].height = mode_info.height;
  modes[i].width = mode_info.width;
  modes[i].bpp = mode_info.bpp;
  //video_mode_ptr++;
  //modes_counter++;
  i++;
 }
 for(i = 0; mode_number[i] != 0xFFFF; i++) {
  printf("Mode: %04X, Resolution: %dx%dx%dbpp\r\n", mode_number[i], modes[i].width, modes[i].height, modes[i].bpp);
 }
 //printf("First mode: 0x%4X\r\n", *video_mode_ptr);
 //printf("modes_counter: %d\r\n", modes_counter);
 //printf("err: 0x%X\r\n", err);
 //printf("resolution: %dx%dx%dbpp\r\n", mode_info.width, mode_info.height, mode_info.bpp);
 //printf("signature: %c%c%c%c\r\n", vbe_info.signature[0], vbe_info.signature[1], vbe_info.signature[2], vbe_info.signature[3]);
 sleep(10);
 return 0;
}
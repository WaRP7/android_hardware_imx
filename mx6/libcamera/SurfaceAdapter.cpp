/*
 * Copyright (C) 2008 The Android Open Source Project
 * Copyright (C) 2012-2015 Freescale Semiconductor, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SurfaceAdapter.h"


typedef struct tRGB{
     unsigned char r;
     unsigned char g;
     unsigned char b;
}RGB;
#if 0
const unsigned char fac_table[256][4] = {
    0x4c, 0x1d, 0xd4, 0xa5,
    0x4e, 0x1f, 0xd5, 0xa5,
    0x4f, 0x20, 0xd5, 0xa6,
    0x51, 0x22, 0xd5, 0xa7,
    0x52, 0x24, 0xd6, 0xa8,
    0x53, 0x26, 0xd6, 0xa8,
    0x55, 0x27, 0xd6, 0xa9,
    0x56, 0x29, 0xd7, 0xaa,
    0x58, 0x2b, 0xd7, 0xaa,
    0x59, 0x2d, 0xd7, 0xab,
    0x5a, 0x2f, 0xd8, 0xac,
    0x5c, 0x30, 0xd8, 0xad,
    0x5d, 0x32, 0xd8, 0xad,
    0x5f, 0x34, 0xd9, 0xae,
    0x60, 0x36, 0xd9, 0xaf,
    0x61, 0x37, 0xd9, 0xaf,
    0x63, 0x39, 0xda, 0xb0,
    0x64, 0x3b, 0xda, 0xb1,
    0x66, 0x3d, 0xda, 0xb2,
    0x67, 0x3f, 0xdb, 0xb2,
    0x68, 0x40, 0xdb, 0xb3,
    0x6a, 0x42, 0xdc, 0xb4,
    0x6b, 0x44, 0xdc, 0xb5,
    0x6d, 0x46, 0xdc, 0xb5,
    0x6e, 0x47, 0xdd, 0xb6,
    0x70, 0x49, 0xdd, 0xb7,
    0x71, 0x4b, 0xdd, 0xb7,
    0x72, 0x4d, 0xde, 0xb8,
    0x74, 0x4f, 0xde, 0xb9,
    0x75, 0x50, 0xde, 0xba,
    0x77, 0x52, 0xdf, 0xba,
    0x78, 0x54, 0xdf, 0xbb,
    0x79, 0x56, 0xdf, 0xbc,
    0x7b, 0x57, 0xe0, 0xbc,
    0x7c, 0x59, 0xe0, 0xbd,
    0x7e, 0x5b, 0xe0, 0xbe,
    0x7f, 0x5d, 0xe1, 0xbf,
    0x80, 0x5f, 0xe1, 0xbf,
    0x82, 0x60, 0xe1, 0xc0,
    0x83, 0x62, 0xe2, 0xc1,
    0x85, 0x64, 0xe2, 0xc1,
    0x86, 0x66, 0xe2, 0xc2,
    0x87, 0x68, 0xe3, 0xc3,
    0x89, 0x69, 0xe3, 0xc4,
    0x8a, 0x6b, 0xe3, 0xc4,
    0x8c, 0x6d, 0xe4, 0xc5,
    0x8d, 0x6f, 0xe4, 0xc6,
    0x8e, 0x70, 0xe5, 0xc6,
    0x90, 0x72, 0xe5, 0xc7,
    0x91, 0x74, 0xe5, 0xc8,
    0x93, 0x76, 0xe6, 0xc9,
    0x94, 0x78, 0xe6, 0xc9,
    0x96, 0x79, 0xe6, 0xca,
    0x97, 0x7b, 0xe7, 0xcb,
    0x98, 0x7d, 0xe7, 0xcb,
    0x9a, 0x7f, 0xe7, 0xcc,
    0x9b, 0x80, 0xe8, 0xcd,
    0x9d, 0x82, 0xe8, 0xce,
    0x9e, 0x84, 0xe8, 0xce,
    0x9f, 0x86, 0xe9, 0xcf,
    0xa1, 0x88, 0xe9, 0xd0,
    0xa2, 0x89, 0xe9, 0xd0,
    0xa4, 0x8b, 0xea, 0xd1,
    0xa5, 0x8d, 0xea, 0xd2,
    0xa6, 0x8f, 0xea, 0xd3,
    0xa8, 0x90, 0xeb, 0xd3,
    0xa9, 0x92, 0xeb, 0xd4,
    0xab, 0x94, 0xeb, 0xd5,
    0xac, 0x96, 0xec, 0xd5,
    0xad, 0x98, 0xec, 0xd6,
    0xaf, 0x99, 0xec, 0xd7,
    0xb0, 0x9b, 0xed, 0xd8,
    0xb2, 0x9d, 0xed, 0xd8,
    0xb3, 0x9f, 0xed, 0xd9,
    0xb4, 0xa0, 0xee, 0xda,
    0xb6, 0xa2, 0xee, 0xdb,
    0xb7, 0xa4, 0xef, 0xdb,
    0xb9, 0xa6, 0xef, 0xdc,
    0xba, 0xa8, 0xef, 0xdd,
    0xbc, 0xa9, 0xf0, 0xdd,
    0xbd, 0xab, 0xf0, 0xde,
    0xbe, 0xad, 0xf0, 0xdf,
    0xc0, 0xaf, 0xf1, 0xe0,
    0xc1, 0xb0, 0xf1, 0xe0,
    0xc3, 0xb2, 0xf1, 0xe1,
    0xc4, 0xb4, 0xf2, 0xe2,
    0xc5, 0xb6, 0xf2, 0xe2,
    0xc7, 0xb8, 0xf2, 0xe3,
    0xc8, 0xb9, 0xf3, 0xe4,
    0xca, 0xbb, 0xf3, 0xe5,
    0xcb, 0xbd, 0xf3, 0xe5,
    0xcc, 0xbf, 0xf4, 0xe6,
    0xce, 0xc0, 0xf4, 0xe7,
    0xcf, 0xc2, 0xf4, 0xe7,
    0xd1, 0xc4, 0xf5, 0xe8,
    0xd2, 0xc6, 0xf5, 0xe9,
    0xd3, 0xc8, 0xf5, 0xea,
    0xd5, 0xc9, 0xf6, 0xea,
    0xd6, 0xcb, 0xf6, 0xeb,
    0xd8, 0xcd, 0xf6, 0xec,
    0xd9, 0xcf, 0xf7, 0xec,
    0xda, 0xd0, 0xf7, 0xed,
    0xdc, 0xd2, 0xf8, 0xee,
    0xdd, 0xd4, 0xf8, 0xef,
    0xdf, 0xd6, 0xf8, 0xef,
    0xe0, 0xd8, 0xf9, 0xf0,
    0xe2, 0xd9, 0xf9, 0xf1,
    0xe3, 0xdb, 0xf9, 0xf1,
    0xe4, 0xdd, 0xfa, 0xf2,
    0xe6, 0xdf, 0xfa, 0xf3,
    0xe7, 0xe0, 0xfa, 0xf4,
    0xe9, 0xe2, 0xfb, 0xf4,
    0xea, 0xe4, 0xfb, 0xf5,
    0xeb, 0xe6, 0xfb, 0xf6,
    0xed, 0xe8, 0xfc, 0xf6,
    0xee, 0xe9, 0xfc, 0xf7,
    0xf0, 0xeb, 0xfc, 0xf8,
    0xf1, 0xed, 0xfd, 0xf9,
    0xf2, 0xef, 0xfd, 0xf9,
    0xf4, 0xf0, 0xfd, 0xfa,
    0xf5, 0xf2, 0xfe, 0xfb,
    0xf7, 0xf4, 0xfe, 0xfb,
    0xf8, 0xf6, 0xfe, 0xfc,
    0xf9, 0xf8, 0xff, 0xfd,
    0xfb, 0xf9, 0xff, 0xfe,
    0xfc, 0xfb, 0xff, 0xfe,
    0xfe, 0xfd, 0x00, 0xff,
    0xff, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x00, 0x00,
    0x02, 0x03, 0x00, 0x01,
    0x04, 0x05, 0x01, 0x02,
    0x05, 0x07, 0x01, 0x02,
    0x07, 0x08, 0x01, 0x03,
    0x08, 0x0a, 0x02, 0x04,
    0x09, 0x0c, 0x02, 0x05,
    0x0b, 0x0e, 0x02, 0x05,
    0x0c, 0x10, 0x03, 0x06,
    0x0e, 0x11, 0x03, 0x07,
    0x0f, 0x13, 0x03, 0x07,
    0x10, 0x15, 0x04, 0x08,
    0x12, 0x17, 0x04, 0x09,
    0x13, 0x18, 0x04, 0x0a,
    0x15, 0x1a, 0x05, 0x0a,
    0x16, 0x1c, 0x05, 0x0b,
    0x17, 0x1e, 0x05, 0x0c,
    0x19, 0x20, 0x06, 0x0c,
    0x1a, 0x21, 0x06, 0x0d,
    0x1c, 0x23, 0x06, 0x0e,
    0x1d, 0x25, 0x07, 0x0f,
    0x1e, 0x27, 0x07, 0x0f,
    0x20, 0x28, 0x07, 0x10,
    0x21, 0x2a, 0x08, 0x11,
    0x23, 0x2c, 0x08, 0x11,
    0x24, 0x2e, 0x08, 0x12,
    0x26, 0x30, 0x09, 0x13,
    0x27, 0x31, 0x09, 0x14,
    0x28, 0x33, 0x0a, 0x14,
    0x2a, 0x35, 0x0a, 0x15,
    0x2b, 0x37, 0x0a, 0x16,
    0x2d, 0x38, 0x0b, 0x16,
    0x2e, 0x3a, 0x0b, 0x17,
    0x2f, 0x3c, 0x0b, 0x18,
    0x31, 0x3e, 0x0c, 0x19,
    0x32, 0x40, 0x0c, 0x19,
    0x34, 0x41, 0x0c, 0x1a,
    0x35, 0x43, 0x0d, 0x1b,
    0x36, 0x45, 0x0d, 0x1b,
    0x38, 0x47, 0x0d, 0x1c,
    0x39, 0x48, 0x0e, 0x1d,
    0x3b, 0x4a, 0x0e, 0x1e,
    0x3c, 0x4c, 0x0e, 0x1e,
    0x3d, 0x4e, 0x0f, 0x1f,
    0x3f, 0x50, 0x0f, 0x20,
    0x40, 0x51, 0x0f, 0x20,
    0x42, 0x53, 0x10, 0x21,
    0x43, 0x55, 0x10, 0x22,
    0x44, 0x57, 0x10, 0x23,
    0x46, 0x58, 0x11, 0x23,
    0x47, 0x5a, 0x11, 0x24,
    0x49, 0x5c, 0x11, 0x25,
    0x4a, 0x5e, 0x12, 0x25,
    0x4c, 0x60, 0x12, 0x26,
    0x4d, 0x61, 0x13, 0x27,
    0x4e, 0x63, 0x13, 0x28,
    0x50, 0x65, 0x13, 0x28,
    0x51, 0x67, 0x14, 0x29,
    0x53, 0x68, 0x14, 0x2a,
    0x54, 0x6a, 0x14, 0x2b,
    0x55, 0x6c, 0x15, 0x2b,
    0x57, 0x6e, 0x15, 0x2c,
    0x58, 0x70, 0x15, 0x2d,
    0x5a, 0x71, 0x16, 0x2d,
    0x5b, 0x73, 0x16, 0x2e,
    0x5c, 0x75, 0x16, 0x2f,
    0x5e, 0x77, 0x17, 0x30,
    0x5f, 0x78, 0x17, 0x30,
    0x61, 0x7a, 0x17, 0x31,
    0x62, 0x7c, 0x18, 0x32,
    0x63, 0x7e, 0x18, 0x32,
    0x65, 0x80, 0x18, 0x33,
    0x66, 0x81, 0x19, 0x34,
    0x68, 0x83, 0x19, 0x35,
    0x69, 0x85, 0x19, 0x35,
    0x6a, 0x87, 0x1a, 0x36,
    0x6c, 0x88, 0x1a, 0x37,
    0x6d, 0x8a, 0x1a, 0x37,
    0x6f, 0x8c, 0x1b, 0x38,
    0x70, 0x8e, 0x1b, 0x39,
    0x72, 0x90, 0x1b, 0x3a,
    0x73, 0x91, 0x1c, 0x3a,
    0x74, 0x93, 0x1c, 0x3b,
    0x76, 0x95, 0x1d, 0x3c,
    0x77, 0x97, 0x1d, 0x3c,
    0x79, 0x98, 0x1d, 0x3d,
    0x7a, 0x9a, 0x1e, 0x3e,
    0x7b, 0x9c, 0x1e, 0x3f,
    0x7d, 0x9e, 0x1e, 0x3f,
    0x7e, 0xa0, 0x1f, 0x40,
    0x80, 0xa1, 0x1f, 0x41,
    0x81, 0xa3, 0x1f, 0x41,
    0x82, 0xa5, 0x20, 0x42,
    0x84, 0xa7, 0x20, 0x43,
    0x85, 0xa9, 0x20, 0x44,
    0x87, 0xaa, 0x21, 0x44,
    0x88, 0xac, 0x21, 0x45,
    0x89, 0xae, 0x21, 0x46,
    0x8b, 0xb0, 0x22, 0x46,
    0x8c, 0xb1, 0x22, 0x47,
    0x8e, 0xb3, 0x22, 0x48,
    0x8f, 0xb5, 0x23, 0x49,
    0x90, 0xb7, 0x23, 0x49,
    0x92, 0xb9, 0x23, 0x4a,
    0x93, 0xba, 0x24, 0x4b,
    0x95, 0xbc, 0x24, 0x4b,
    0x96, 0xbe, 0x24, 0x4c,
    0x98, 0xc0, 0x25, 0x4d,
    0x99, 0xc1, 0x25, 0x4e,
    0x9a, 0xc3, 0x26, 0x4e,
    0x9c, 0xc5, 0x26, 0x4f,
    0x9d, 0xc7, 0x26, 0x50,
    0x9f, 0xc9, 0x27, 0x51,
    0xa0, 0xca, 0x27, 0x51,
    0xa1, 0xcc, 0x27, 0x52,
    0xa3, 0xce, 0x28, 0x53,
    0xa4, 0xd0, 0x28, 0x53,
    0xa6, 0xd1, 0x28, 0x54,
    0xa7, 0xd3, 0x29, 0x55,
    0xa8, 0xd5, 0x29, 0x56,
    0xaa, 0xd7, 0x29, 0x56,
    0xab, 0xd9, 0x2a, 0x57,
    0xad, 0xda, 0x2a, 0x58,
    0xae, 0xdc, 0x2a, 0x58,
    0xaf, 0xde, 0x2b, 0x59,
    0xb1, 0xe0, 0x2b, 0x5a,
};
#endif

static RGB yuvTorgb(unsigned char Y, unsigned char U, unsigned char V){
    RGB rgb;
#if 0
    rgb.r = (int)((Y&0xff) + 1.4075 * ((V&0xff)-128));
    rgb.g = (int)((Y&0xff) - 0.3455 * ((U&0xff)-128) - 0.7169*((V&0xff)-128));
    rgb.b = (int)((Y&0xff) + 1.779 * ((U&0xff)-128));

#elif 0
    rgb.r = (unsigned char)(Y+fac_table[V][0]);
    rgb.g = (unsigned char)(Y-fac_table[U][2]-fac_table[V][3]);
    rgb.b = (unsigned char)(Y+fac_table[U][1]);
#else
        rgb.r = (unsigned char)(Y + (U - 128) + ((104*(U - 128))>>8));  
        rgb.g = (unsigned char)(Y - (89*(V - 128)>>8) - ((183*(U - 128))>>8));  
        rgb.b = (unsigned char)(Y + (V - 128) + ((199*(V - 128))>>8));  
#endif
    return rgb;
}

    //输入YUV422I buffer数据，输出RGB buffer数据；  
static int UYVY422ToRGB888(unsigned char *src_buffer, int w, int h, unsigned char *des_buffer)  
{  
    unsigned char *yuv, *rgb;  
    unsigned char u, v, y1, y2;  

    yuv = src_buffer;  
    rgb = des_buffer;  

    if (yuv == NULL || rgb == NULL)  
    {  
        printf ("error: input data null!\n");     
        return -1;  
    }  

    int size = w * h;  

    for(int i = 0; i < size; i += 2)  
    {  
        y1 = yuv[2*i + 1];  
        y2 = yuv[2*i + 3];  
        u = yuv[2*i];  
        v = yuv[2*i + 2];  

#if 0  
        rgb[3*i]     = (unsigned char)(y1 + 1.402*(u - 128));  
        rgb[3*i + 1] = (unsigned char)(y1 - 0.344*(u - 128) - 0.714*(v - 128));  
        rgb[3*i + 2] = (unsigned char)(y1 + 1.772*(v - 128));  

        rgb[3*i + 3] = (unsigned char)(y2 + 1.375*(u - 128));  
        rgb[3*i + 4] = (unsigned char)(y2 - 0.344*(u - 128) - 0.714*(v - 128));  
        rgb[3*i + 5] = (unsigned char)(y2 + 1.772*(v - 128));  
#endif   

        //为提高性能此处用移位运算；  
        rgb[3*i]     = (unsigned char)(y1 + (u - 128) + ((104*(u - 128))>>8));  
        rgb[3*i + 1] = (unsigned char)(y1 - (89*(v - 128)>>8) - ((183*(u - 128))>>8));  
        rgb[3*i + 2] = (unsigned char)(y1 + (v - 128) + ((199*(v - 128))>>8));  

        rgb[3*i + 3] = (unsigned char)(y2 + (u - 128) + ((104*(u - 128))>>8));  
        rgb[3*i + 4] = (unsigned char)(y2 - (89*(v - 128)>>8) - ((183*(u - 128))>>8));  
        rgb[3*i + 5] = (unsigned char)(y2 + (v - 128) + ((199*(v - 128))>>8));  
    }  

    return 0;  
}  

//
//yuv422p
int convertyuv422torgb565(unsigned char *inbuf,unsigned char *outbuf,int width,int height)
{
  int rows,cols,rowwidth;
  int y,u,v,r,g,b,rdif,invgdif,bdif;
  int size;
  unsigned char *YUVdata,*RGBdata;
  int YPOS,UPOS,VPOS;

  YUVdata = inbuf;
  RGBdata = outbuf;

  rowwidth = width>>1;
  size=width*height*2;
  YPOS=0;
  UPOS=YPOS + size/2;
  VPOS=UPOS + size/4;

  for(rows=0;rows<height;rows++)
  {
    for(cols=0;cols<width;cols++) 
    {
 u = YUVdata[UPOS] - 128;
 v = YUVdata[VPOS] - 128;

 rdif = v + ((v * 103) >> 8);
 invgdif = ((u * 88) >> 8) +((v * 183) >> 8);
 bdif = u +( (u*198) >> 8);

 r = YUVdata[YPOS] + rdif;
 g = YUVdata[YPOS] - invgdif;
 b = YUVdata[YPOS] + bdif;
 r=r>255?255:(r<0?0:r);
 g=g>255?255:(g<0?0:g);
 b=b>255?255:(b<0?0:b);
    
 *(RGBdata++) =( ((g & 0x1C) << 3) | ( b >> 3) );
 *(RGBdata++) =( (r & 0xF8) | ( g >> 5) );

 YPOS++;      
 
 if(cols & 0x01)
 {
    UPOS++;
    VPOS++;      
 } 
    }
    if((rows & 0x01)== 0)
    {
 UPOS -= rowwidth;
 VPOS -= rowwidth;
    }
  }
  return 1;
}
//az: this function is for YUYV422 to RGB888
static void YUY2ToRGB(unsigned char *src, unsigned char *dst, int width, int height){
	int i;
	int j;
    int numOfPixel = width * height;
    unsigned char *rgb = dst;
    int lineWidth = 2*width;

    for(i=0; i<height; i++){
        int startY = i*lineWidth;
        for(j = 0; j < lineWidth; j+=4){
            int Y1 = j + startY;
            int Y2 = Y1+2;
            int U = Y1+1;
            int V = Y1+3;
            int index = (Y1>>1)*3;
            RGB tmp = yuvTorgb(src[Y1], src[U], src[V]);
            rgb[index+0] = tmp.r;
            rgb[index+1] = tmp.g;
            rgb[index+2] = tmp.b;
            index += 3;
            tmp = yuvTorgb(src[Y2], src[U], src[V]);
            rgb[index+0] = tmp.r;
            rgb[index+1] = tmp.g;
            rgb[index+2] = tmp.b;
        }
    }
    return;
}

static void YUY2ToRGB565(unsigned char *src, unsigned char *dst, int width, int height){
	int i;
	int j;
    int numOfPixel = width * height;
    unsigned char *rgb = dst;
    int lineWidth = 2*width;

    for(i=0; i<height; i++){
        int startY = i*lineWidth;
        for(j = 0; j < lineWidth; j+=4){
            int Y1 = j + startY;
            int Y2 = Y1+2;
            int U = Y1+1;
            int V = Y1+3;
            int index = (Y1>>1)*2;
            RGB tmp = yuvTorgb(src[Y1], src[U], src[V]);
            rgb[index+0] = ( ((tmp.g & 0x1C) << 3) | ( tmp.r >> 3) );
            rgb[index+1] = ( (tmp.b & 0xF8) | ( tmp.g >> 5) );
            index += 2;
            tmp = yuvTorgb(src[Y2], src[U], src[V]);
            rgb[index+0] = ( ((tmp.g & 0x1C) << 3) | ( tmp.r >> 3) );
            rgb[index+1] = ( (tmp.b & 0xF8) | ( tmp.g >> 5) );
        }
    }
    return;
}

static void UYVYToRGB565(unsigned char *src, unsigned char *dst, int width, int height){
	int i;
	int j;
    int numOfPixel = width * height;
    unsigned char *rgb = dst;
    int lineWidth = 2*width;

    for(i=0; i<height; i++){
        int startY = i*lineWidth;
        for(j = 0; j < lineWidth; j+=4){
            int U  = j + startY;
            int Y1 = U +1;
            int V  = U +2;
            int Y2 = U +3;
            int index = (U>>1)*2;

            RGB tmp = yuvTorgb(src[Y1], src[U], src[V]);
            rgb[index+0] = ( ((tmp.g & 0x1C) << 3) | ( tmp.r >> 3) );
            rgb[index+1] = ( (tmp.b & 0xF8) | ( tmp.g >> 5) );
            index += 2;
            tmp = yuvTorgb(src[Y2], src[U], src[V]);
            rgb[index+0] = ( ((tmp.g & 0x1C) << 3) | ( tmp.r >> 3) );
            rgb[index+1] = ( (tmp.b & 0xF8) | ( tmp.g >> 5) );
        }
    }
    return;
}

static void dumpToFile(int fd, unsigned char* buf, int width, int height) 
{
    write(fd, buf, width*height);
}

SurfaceAdapter::SurfaceAdapter()
    : mNativeWindow(NULL), mFrameWidth(0), mFrameHeight(0),
      mBufferCount(0), mBufferSize(0), mFormat(0), mQueueableCount(0)
{
    memset(mCameraBuffer, 0, sizeof(mCameraBuffer));
#ifdef NO_GPU
    mTmpBuf = NULL;
#endif
}

SurfaceAdapter::~SurfaceAdapter()
{
    memset(mCameraBuffer, 0, sizeof(mCameraBuffer));
    clearBufferListeners();
}

int SurfaceAdapter::setNativeWindowAttribute(int width,
                                             int height,
                                             int format,
                                             int numBufs)
{
    status_t err = NO_ERROR;

    if (NULL == mNativeWindow) {
        FLOGE("SurfaceAdapter: allocateBuffer invalid parameters");
        return BAD_VALUE;
    }

    // Set gralloc usage bits for window.
    err = mNativeWindow->set_usage(mNativeWindow, CAMERA_GRALLOC_USAGE);
    if (err != 0) {
        FLOGE("native_window_set_usage failed: %s (%d)", strerror(-err), -err);
        if (ENODEV == err) {
            FLOGE("Preview surface abandoned!");
            mNativeWindow = NULL;
        }

        return BAD_VALUE;
    }

    FLOGI("Number of buffers set to NativeWindow %d", numBufs);

    // /Set the number of buffers needed for camera preview
    err = mNativeWindow->set_buffer_count(mNativeWindow, numBufs);
    if (err != 0) {
        FLOGE("native_window_set_buffer_count failed: %s (%d)", strerror(
                  -err), -err);
        if (ENODEV == err) {
            FLOGE("Preview surface abandoned!");
            mNativeWindow = NULL;
        }

        return BAD_VALUE;
    }

    // Set window geometry
    ALOGI("set_buffers_geometry, w %d, h %d, format 0x%x", width, height, format);
    format = HAL_PIXEL_FORMAT_RGB_565;//az
    ALOGI("az set NativeWindow HAL_PIXEL_FORMAT_RGB_565: set_buffers_geometry, w %d, h %d, format 0x%x", width, height, format);
    err = mNativeWindow->set_buffers_geometry(mNativeWindow,
                                              width, height, format);

    if (err != 0) {
        FLOGE("native_window_set_buffers_geometry failed: %s (%d)", strerror(
                  -err), -err);
        if (ENODEV == err) {
            FLOGE("Preview surface abandoned!");
            mNativeWindow = NULL;
        }

        return BAD_VALUE;
    }


    return err;
}

int SurfaceAdapter::allocatePreviewBuffer(int width,
                                          int height,
                                          int format,
                                          int numBufs)
{
    status_t err   = NO_ERROR;
    int undequeued = 0;

    if ((NULL == mNativeWindow) || (numBufs == 0)) {
        FLOGE("allocatePreviewBuffer invalid parameters");
        return BAD_VALUE;
    }

    err = setNativeWindowAttribute(width, height, format, numBufs);
    if (err) {
        FLOGE("setNativeWindowAttribute failed.");
        return err;
    }

    mBufferCount = numBufs;
    mNativeWindow->get_min_undequeued_buffer_count(mNativeWindow, &undequeued);
    mQueueableCount = mBufferCount - undequeued;

    err = allocateBuffer(width, height, format, numBufs, mQueueableCount);
    return err;
}

int SurfaceAdapter::allocatePictureBuffer(int width,
                                          int height,
                                          int format,
                                          int numBufs)
{
    status_t err = NO_ERROR;

    if ((NULL == mNativeWindow) || (numBufs == 0)) {
        FLOGE("allocatePictureBuffer invalid parameters");
        return BAD_VALUE;
    }

    err = setNativeWindowAttribute(width, height, format, numBufs);
    if (err) {
        FLOGE("setNativeWindowAttribute failed.");
        return err;
    }

    mBufferCount    = numBufs;
    mQueueableCount = numBufs;

    err = allocateBuffer(width, height, format, numBufs, mQueueableCount);
    return err;
}

int SurfaceAdapter::allocateBuffer(int width,
                                   int height,
                                   int format,
                                   int numBufs,
                                   int maxQCount)
{
    status_t err;
    int i                       = -1;
    GraphicBufferMapper& mapper = GraphicBufferMapper::get();
    Rect bounds;

    FLOG_RUNTIME("SurfaceAdapter::allocateBuffer:width, height, format, numBufs, maxQCount %d %d %d %d %d", width, height, format, numBufs, maxQCount);

    if ((NULL == mNativeWindow) || (numBufs == 0)) {
        FLOGE("allocateBuffer invalid parameters");
        return BAD_VALUE;
    }

    memset(mCameraBuffer, 0, sizeof(mCameraBuffer));

    // lock the initial queueable buffers
    bounds.left   = 0;
    bounds.top    = 0;
    bounds.right  = width;
    bounds.bottom = height;
    void *pVaddr = NULL;
    int   stride = 0;

    for (i = 0; i < numBufs; i++) {
        buffer_handle_t *buf_h = NULL;
        stride = 0;
        pVaddr = NULL;

        // TODO(XXX): Do we need to keep stride information in camera hal?
        err = mNativeWindow->dequeue_buffer(mNativeWindow, &buf_h, &stride);
        if (err != 0) {
            FLOGE("dequeueBuffer failed: %s (%d)", strerror(-err), -err);
            if (ENODEV == err) {
                FLOGE("Preview surface abandoned!");
                mNativeWindow = NULL;
            }
            goto fail;
        }

        mapper.lock(*buf_h, CAMERA_GRALLOC_USAGE, bounds, &pVaddr);
        mCameraBuffer[i].initialize(buf_h, i);
        mCameraBuffer[i].mWidth  = width;
        mCameraBuffer[i].mHeight = height;
    }

    for (i = 0; i < maxQCount; i++) {
        mNativeWindow->lock_buffer(mNativeWindow, mCameraBuffer[i].mBufHandle);
    }

    // return the rest of the buffers back to ANativeWindow
    for (i = maxQCount; i >= 0 && i < numBufs; i++) {
        err = mNativeWindow->cancel_buffer(mNativeWindow,
                                           mCameraBuffer[i].mBufHandle);
        if (err != 0) {
            FLOGE("cancel_buffer failed: %s (%d)", strerror(-err), -err);
            if (ENODEV == err) {
                FLOGE("Preview surface abandoned!");
                mNativeWindow = NULL;
            }

            goto fail;
        }
        mapper.unlock(*mCameraBuffer[i].mBufHandle);

        // the frame held in SurfaceAdapter.
        mCameraBuffer[i].addReference();
    }

    dispatchBuffers(&mCameraBuffer[0], numBufs, BUFFER_CREATE);

    mFormat      = format;
    mBufferSize  = mCameraBuffer[0].mSize;
    mFrameWidth  = width;
    mFrameHeight = height;

    ALOGI("SurfaceAdapter::allocateBuffer, mBufferSize %d", mBufferSize);

#ifdef NO_GPU
    mTmpBuf = (unsigned char*)malloc(mBufferSize); //az add *3/2
    if(mTmpBuf == NULL) {
        ALOGE("malloc mTmpBuf failed, bytes %d", mBufferSize);
        goto fail;
    }
#endif

    return NO_ERROR;

fail:

    // need to cancel buffers if any were dequeued
    for (int start = 0; start < i && i > 0; start++) {
        int err = mNativeWindow->cancel_buffer(mNativeWindow,
                                               mCameraBuffer[start].mBufHandle);
        if (err != 0) {
            FLOGE("cancelBuffer failed w/ error 0x%08x", err);
            break;
        }
    }

    freeBuffer();
    FLOGE("Error occurred, performing cleanup");

    if (NULL != mErrorListener) {
        mErrorListener->handleError(ERROR_TINY);
    }

    return BAD_VALUE;
}

int SurfaceAdapter::freeBuffer()
{
    status_t ret = NO_ERROR;

    FLOG_RUNTIME("SurfaceAdapter::freeBuffer()");

    GraphicBufferMapper& mapper = GraphicBufferMapper::get();

    // Give the buffers back to display here -  sort of free it
    if (mNativeWindow) {
        for (int i = 0; i < mBufferCount; i++) {
            mapper.unlock(*mCameraBuffer[i].mBufHandle);
            ret = mNativeWindow->cancel_buffer(mNativeWindow,
                                               mCameraBuffer[i].mBufHandle);
            if (ENODEV == ret) {
                FLOGE("Preview surface abandoned!");
                mNativeWindow = NULL;
                return -ret;
            }
            else if (NO_ERROR != ret) {
                FLOGE("cancel_buffer() failed: %s (%d)", strerror(-ret), -ret);
                return -ret;
            }
        }
    }
    else {
        FLOGE("mNativeWindow is NULL");
    }

    memset(mCameraBuffer, 0, sizeof(mCameraBuffer));

    // /Clear the frames with camera adapter map
    dispatchBuffers(NULL, 0, BUFFER_DESTROY);

#ifdef NO_GPU
    if(mTmpBuf) {
        free(mTmpBuf);
        mTmpBuf = NULL;
    }
#endif

    return ret;
}

void SurfaceAdapter::setErrorListener(CameraErrorListener *listener)
{
    mErrorListener = listener;
}

int SurfaceAdapter::setPreviewWindow(preview_stream_ops_t *window)
{
    // /Note that Display Adapter cannot work without a valid window object
    if (!window) {
        FLOGE("NULL window object passed to DisplayAdapter");
        return BAD_VALUE;
    }

    // /Destroy the existing window object, if it exists
    destroy();

    // /Move to new window obj
    mNativeWindow = window;

    return NO_ERROR;
}

void SurfaceAdapter::destroy()
{
    mNativeWindow = NULL;
    mBufferCount  = 0;
    mBufferSize   = 0;
}

int SurfaceAdapter::maxQueueableBuffers()
{
    return mQueueableCount;
}

void SurfaceAdapter::renderBuffer(buffer_handle_t *bufHandle)
{
    status_t ret = NO_ERROR;

    GraphicBufferMapper& mapper = GraphicBufferMapper::get();

    // unlock buffer before sending to display
    mapper.unlock(*bufHandle);

    private_handle_t *handle = (private_handle_t *)(*bufHandle);
#if 0 //az dump YUV buffers to file
#if 0
    static int fd = -1;
    if (fd<0) {
        fd = open("/sdcard/camera.yuv", O_RDWR);
        if (!(fd>0))
            FLOGE("open file for dump ERROR: %d", fd);
    }
    if (fd>0)
        dumpToFile(fd, (unsigned char *)handle->base, mFrameWidth, mFrameHeight);
#else
        static FILE *pf = NULL;
        const char* filepath="/data/data/com.android.camera/files/surface.yuv";
        if (pf == NULL) 
            pf = fopen(filepath, "ab+"); //wb

        if (pf == NULL) {
            FLOGI("open %s failed: %s", filepath, strerror(errno));
        }
        else {
            FLOGI("-----write----- %d: ", mBufferSize);
            FLOGE("%s handle->base:%p",__func__,  handle->base );
            fwrite((unsigned char *)handle->base, mFrameWidth*mFrameHeight*2, 1, pf); //dump virtual add 
            FLOGE("%s handle->phys:%p",__func__,  handle->phys);
            FLOGE("%s handle->phys:%x",__func__,  handle->phys);
            //fwrite((unsigned char *)handle->phys, mFrameWidth*mFrameHeight*2, 1, pf);
            //fclose(pf);
        }
#endif
#endif

//#ifdef NO_GPU
#if 0 //az
    //private_handle_t *handle = (private_handle_t *)(*bufHandle);
    memcpy(mTmpBuf, (void *)handle->base, mBufferSize);
    YUY2ToRGB(mTmpBuf, (unsigned char *)handle->base, mFrameWidth, mFrameHeight);
    FLOGE("%s handle->base:%p",__func__,  handle->base );
#endif

#if 1 //az
    //private_handle_t *handle = (private_handle_t *)(*bufHandle);
#if 0
        memcpy(mTmpBuf, (void *)handle->base, mBufferSize);
        YUY2ToRGB565(mTmpBuf, (unsigned char *)handle->base, mFrameWidth, mFrameHeight);
#else
    //here we process every 4th frame, not each frame
    static int count = 0 ;
    if ((0x3&(count++))==0) {
        memcpy(mTmpBuf, (void *)handle->base, mBufferSize);
        YUY2ToRGB565(mTmpBuf, (unsigned char *)handle->base, mFrameWidth, mFrameHeight);
        memcpy(mTmpBuf, (void *)handle->base, mBufferSize);
    }else {
        memcpy((void *)handle->base,mTmpBuf,  mBufferSize);
    }
#endif
    //UYVYToRGB565(mTmpBuf, (unsigned char *)handle->base, mFrameWidth, mFrameHeight);
    //FLOGE("%s handle->base:%p",__func__,  handle->base );
#endif

#if 0//dump rgb frames to file
        static FILE *pf1 = NULL;
        const char* filepath1="/data/data/com.android.camera/files/camera.rgb";
        if (pf1 == NULL) 
            pf1 = fopen(filepath1, "ab+"); //wb

        if (pf1 == NULL) {
            FLOGI("open %s failed: %s", filepath1, strerror(errno));
        }
        else {
            FLOGI("-----write-----");
            fwrite((unsigned char *)handle->base, mBufferSize/*mFrameWidth*mFrameHeight*3*/, 1, pf1);
            //fclose(pf1);
        }
#endif

    ret = mNativeWindow->enqueue_buffer(mNativeWindow, bufHandle);
    if (ret != 0) {
        FLOGE("Surface::queueBuffer returned error %d", ret);
    }
}

void SurfaceAdapter::cancelBuffer(buffer_handle_t *bufHandle)
{
    status_t ret = NO_ERROR;

    GraphicBufferMapper& mapper = GraphicBufferMapper::get();

    mapper.unlock(*bufHandle);

    ret = mNativeWindow->cancel_buffer(mNativeWindow, bufHandle);
    if (ret != 0) {
        FLOGE("Surface::queueBuffer returned error %d", ret);
    }
}

CameraFrame * SurfaceAdapter::requestBuffer()
{
    status_t err;
    buffer_handle_t *buf;
    int i = 0;
    int stride; // dummy variable to get stride
    GraphicBufferMapper& mapper = GraphicBufferMapper::get();
    Rect  bounds;
    void *pVaddr;

    if (NULL == mNativeWindow) {
        FLOGE("mNativeWindow is null");
        return NULL;
    }

    err = mNativeWindow->dequeue_buffer(mNativeWindow, &buf, &stride);
    if (err != 0) {
        FLOGE("dequeueBuffer failed: %s (%d)", strerror(-err), -err);
        if (ENODEV == err) {
            FLOGE("Preview surface abandoned!");
            mNativeWindow = NULL;
        }

        return NULL;
    }

    err = mNativeWindow->lock_buffer(mNativeWindow, buf);
    if (err != 0) {
        FLOGE("lockbuffer failed: %s (%d)", strerror(-err), -err);
        if (ENODEV == err) {
            FLOGE("Preview surface abandoned!");
            mNativeWindow = NULL;
        }

        return NULL;
    }

    for (i = 0; i < mBufferCount; i++) {
        if (mCameraBuffer[i].mBufHandle == buf)
            break;
    }

    // lock buffer before sending to FrameProvider for filling
    bounds.left   = 0;
    bounds.top    = 0;
    bounds.right  = mFrameWidth;
    bounds.bottom = mFrameHeight;

    mapper.lock(*mCameraBuffer[i].mBufHandle,
                CAMERA_GRALLOC_USAGE,
                bounds,
                &pVaddr);

    return &mCameraBuffer[i];
}


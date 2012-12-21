/*
  Interface to TridentFB proprietary accel interface.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <lib/base/eerror.h>

//#include "cnxtfb.h"
#if 1
#define FBIO_FILL_RECT            0x4627
#define FBIO_COPY_AREA            0x4628
#define FBIO_IMAGE_BLT            0x4629
#define FBIO_STRETCH_COPY         0x4630

#define FBIO_SETOPACITY           0x4631
#define FBIO_FLIPBUFFER           0x4632

#define FBIOGET_CNXTFBHANDLE      0x4620
#define FBIO_WAITFORVSYNC         0x4621
#define FBIO_STARTDISPLAY         0x4622
#define FBIO_STOPDISPLAY          0x4623
#define FBIO_SETBLENDMODE         0x4624
#define FBIO_CHANGEOUTPUTFORMAT   0x4625
#define FBIO_GETFBRESOLUTION      0x4626
#endif 


static int fb_fd;
int tridentFB_accel_init(void)
{
	struct fb_var_screeninfo screeninfo;
	int ret;

	eDebug("[%s] START", __FUNCTION__);
//	printf("[%s] START \n", __FUNCTION__);
	fb_fd = open("/dev/fb/0", O_RDWR);
	if (fb_fd < 0)
	{
		eDebug("<saifei> open fb device fail!\n");
		perror("/dev/fb/0");
		return 1;
	}
	eDebug("[%s] fb_fd 0x%x success", __FUNCTION__, fb_fd);
	return 0;
}

void tridentFB_accel_close(void)
{
	eDebug("[%s] START fb_fd 0x%x", __FUNCTION__, fb_fd);
	if (0 < fb_fd)
		close(fb_fd);
	fb_fd = 0;
	eDebug("[%s] END fb_fd 0x%x", __FUNCTION__, fb_fd);
}

enum
{
	tridentFB_blitAlphaTest=1,
	tridentFB_blitAlphaBlend=2,
	tridentFB_blitScale=4
};

int tridentFB_accel_blit(
		int src_addr, int src_width, int src_height, int src_stride, int bpp,
		int dst_addr, int dst_width, int dst_height, int dst_stride,
		int src_x, int src_y, int width, int height,
		int dst_x, int dst_y, int dwidth, int dheight,
		struct fb_cmap* pal_addr, unsigned int numOfColors, int flags)
{
	//eDebug("[%s] start \n", __FUNCTION__);
	int ret = 0;
	struct fb_image blitData;

	blitData.dx = src_x;  //blit position 
	blitData.dy = src_y;
	blitData.width = dwidth;
	blitData.height = dheight; 
	blitData.depth = bpp; //color depth
	blitData.cmap = *pal_addr;
	blitData.data = src_addr;
	//eDebug("[%s] START fb_fd 0x%x", __FUNCTION__, fb_fd);

	ret = ioctl(fb_fd, FBIO_IMAGE_BLT, &blitData);
	if(ret<0)
	{
		eDebug("[%s] FBIO_IMAGE_BLT error 0x%x \n", __FUNCTION__, fb_fd);
	}
	//eDebug("[%s] FBIO_IMAGE_BLT success 0x%x \n", __FUNCTION__, fb_fd);
	return ret;
}

int tridentFB_accel_fill(
		int dst_addr, int dst_width, int dst_height, int dst_stride,
		int x, int y, int width, int height,
		unsigned long color)
{
	//return 0;
#if 1
	eDebug("<saifei> tridentFB_accel_fill start\n");
	struct fb_var_screeninfo screeninfo;
	struct fb_fillrect fillData;
	int ret = 0;

	eDebug("[%s] src dst_addr 0x%x w %d h %d stride %d x %d y %d w %d h %d color 0x%x", 
		   		  __FUNCTION__, 
		   		  dst_addr, dst_width, dst_height, dst_stride,  
		   		  x, y, width, height, (unsigned int)color);

	
	if (0 >= fb_fd)
		return -1;

	fillData.dx     = x;
    fillData.dy     = y;
    fillData.width  = width;
    fillData.height = height;      
    fillData.rop    =0;
    fillData.color  = color;

	ret = ioctl(fb_fd, FBIO_FILL_RECT, &fillData);

	if(ret<0)
	{
		printf("[%s] FBIO_FILL_RECT error 0x%x \n", __FUNCTION__, fb_fd);
	}
	
#if 0
	ret =ioctl(fb_fd, FBIOGET_VSCREENINFO, &screeninfo);
	if(ret<0)
	{
		printf("[newpp][%s] FBIOGET_VSCREENINFO error 0x%x \n", __FUNCTION__, fb_fd);
	}

	screeninfo.bits_per_pixel=32;

	ret =ioctl(fb_fd, FBIOPUT_VSCREENINFO, &screeninfo);
	if(ret<0)
	{
		printf("[newpp][%s] FBIOPUT_VSCREENINFO error 0x%x \n", __FUNCTION__, fb_fd);
	}
#endif 	
#if 1
	{
		int opacity_level;
		opacity_level = 255;
		ret =ioctl(fb_fd, FBIO_SETOPACITY, &opacity_level);
		if(ret<0)
		{
			printf("[%s] FBIO_SETOPACITY error 0x%x \n", __FUNCTION__, fb_fd);
		}
	}

	ret =ioctl(fb_fd,FBIO_STARTDISPLAY, NULL);
	if(ret<0)
	{
		printf("[%s]  FBIO_STARTDISPLAY error 0x%x \n", __FUNCTION__, fb_fd);
	}
#endif 
	return ret;
#endif 
}


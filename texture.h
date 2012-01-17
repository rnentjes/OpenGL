/* 
 * File:   texture.h
 * Author: rnentjes
 *
 * Created on January 17, 2012, 8:58 PM
 */

#ifndef TEXTURE_H
#define	TEXTURE_H

#ifdef	__cplusplus
extern "C" {
#endif

struct texture {
  int  	 width;
  int  	 height;
  int  	 bytes_per_pixel; /* 3:RGB, 4:RGBA */ 
  unsigned char  pixel_data[128 * 128 * 4 + 1];
};


#ifdef	__cplusplus
}
#endif

#endif	/* TEXTURE_H */


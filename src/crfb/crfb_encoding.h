#ifndef _CRFB_ENCODING_H_
#define _CRFB_ENCODING_H_

#include "crfb_backend.h"
#include "crfb_client.h"
#include "crfb_frame_buffer.h"
#include "crfb_rectangle.h"

typedef int CRFBEncoding;

#define RAW_ENCODING 			        ((CRFBEncoding) 0)
#define COPYRECT_ENCODING 		        ((CRFBEncoding) 1)
// #define RRE_ENCODING 			        ((CRFBEncoding) 2)
// #define CORRE_ENCODING 			        ((CRFBEncoding) 4)
// #define HEXTILE_ENCODING 		        ((CRFBEncoding) 5)
// #define ZLIB_ENCODING 			        ((CRFBEncoding) 6)
// #define TIGHT_ENCODING 			        ((CRFBEncoding) 7)
// #define ZLIBHEX_ENCODING 		        ((CRFBEncoding) 8)
// #define ZRLE_ENCODING 			        ((CRFBEncoding) 16)
// #define JPEG_ENCODING 			        ((CRFBEncoding) 21)
// #define OPEN_H264_ENCODING 		        ((CRFBEncoding) 50)
// #define TIGHT_PNG_ENCODING 		        ((CRFBEncoding) -260)

#define DESKTOP_SIZE_PSEUDO_ENCODING    ((CRFBEncoding) -223)

void crfb_client_recv_raw_encoding(CRFBClient* client, CRFBFramebuffer* buffer, CRFBRectangle* rect);

void crfb_client_recv_copy_rect_encoding(CRFBClient* client, CRFBFramebuffer* buffer, CRFBRectangle* rect);

#endif
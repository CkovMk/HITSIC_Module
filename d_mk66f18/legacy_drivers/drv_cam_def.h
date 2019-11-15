#ifndef _DRV_CAM_DEF_H_
#define _DRV_CAM_DEF_H_

#define  CAM_IMG_R 120
#define  CAM_IMG_C 188

typedef struct _camera_img_t
{
	uint8_t img[CAM_IMG_R][CAM_IMG_C];
	uint64_t tStamp;//time stamp
}camera_img_t;

#endif // ! _DRV_CAM_DEF_H_


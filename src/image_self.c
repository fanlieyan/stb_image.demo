#include <stdio.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"


#ifndef _COMM_DEFINE_
#define _COMM_DEFINE_

typedef int BOOL;
typedef unsigned char cchar;

#ifndef OK
#define OK              0
#endif

#ifndef ERR
#define ERR             -1
#endif

#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif

#define IF_EQ(a,b)      ((a)==(b)?TRUE:FALSE)
#define IF_GT(a,b)      ((a)>(b)?TRUE:FALSE)
#define IF_LT(a,b)      ((a)<(b)?TRUE:FALSE)

#ifndef MAX
#define MAX(a,b)        ((a)>(b)?(a):(b))
#endif

#ifndef MIN
#define MIN(a,b)        ((a)<(b)?(a):(b))
#endif

#define INIT_A(a)       do{memset(a, 0, sizeof(a));}while(0);
#define INIT_S(a)       do{memset(&a, 0, sizeof(a));}while(0);
#define INIT_P(p,t)     do{memset(p, 0, sizeof(t));}while(0);

#define COMM_DEBUG      1
#define RET_V(a)        do{if(a){if(1 == COMM_DEBUG){printf(" [%s:%d] EXP TRUE![%s]\n", \
                            __func__, __LINE__, (#a));}return;}}while(0);
                        
#define RET_R(a,b)      do{if(a){if(1 == COMM_DEBUG){printf(" [%s:%d] EXP TRUE![%s]\n", \
                            __func__, __LINE__, (#a));}return (b);}}while(0);

#define PDBG_I(a)       do{if(1 == COMM_DEBUG){printf(" [%s:%d] %s[%d]\n", \
                            __func__, __LINE__, (#a), (a));}}while(0);

#define PDBG_S(a)       do{if(1 == COMM_DEBUG){printf(" [%s:%d] %s[%s]\n", \
                            __func__, __LINE__, (#a), (a));}}while(0);
                        
#define PDBG_C(a)       do{if(1 == COMM_DEBUG){printf(" [%s:%d] %s[%c]\n", \
                            __func__, __LINE__, (#a), (a));}}while(0);

#define POUT_I(a)       do{printf("%d", (a));}while(0);
#define POUT_S(a)       do{printf("%s", (a));}while(0);
#define POUT_C(a)       do{printf("%c", (a));}while(0);

#define POUT_IL(a)      do{printf("%d\n", (a));}while(0);
#define POUT_SL(a)      do{printf("%s\n", (a));}while(0);
#define POUT_CL(a)      do{printf("%c\n", (a));}while(0);

#define SAFE_FREE(p)    do{if(p){free(p); p = NULL;}}while(0);

#define BUFF_LEN        10000
#define MAX_COUNT       100

#endif

int get_image_size(const char *pInPath, cchar **ppOutImgData, int *pOutW, int *pOutH, int *pOutChn){
	RET_R(!pInPath, ERR);
	//RET_R(!pOutW, ERR);
	//RET_R(!pOutH, ERR);
	//RET_R(!pOutCHn, ERR);

	cchar *pImageData = NULL;
	int iWidth = 0;
	int iHeight = 0;
	int iColorChn = 0;

	pImageData = stbi_load(pInPath, &iWidth, &iHeight, &iColorChn, 0);
	RET_R(!pImageData, ERR);

	PDBG_I(iWidth);
	PDBG_I(iHeight);
	PDBG_I(iColorChn);
	
	if(pOutW){
		*pOutW = iWidth;
	}

	if(pOutH){
		*pOutH = iHeight;
	}

	if(pOutChn){
		*pOutChn = iColorChn;
	}

	if(ppOutImgData){
		*ppOutImgData = pImageData;
	}else{
		stbi_image_free(pImageData);
	}
	return OK;
}

int resize_image(const char *pInPath, const char *pOutPath, const float lResizeRate){
	RET_R(!pInPath, ERR);
	RET_R(!pOutPath, ERR);

	cchar *pImageData = NULL;
	cchar *pOutImageData = NULL;
	int iRet = 0;
	int iWidth = 0;
	int iHeight = 0;
	int iOutWidth = 0;
	int iOutHeight = 0;
	int iColorChn = 0;

	iRet = get_image_size(pInPath, &pImageData, &iWidth, &iHeight, &iColorChn);
	RET_R(iRet != OK, ERR);

	iOutWidth = iWidth*lResizeRate;
	iOutHeight = iHeight*lResizeRate;

	pOutImageData = (cchar *)calloc(1, iOutWidth * iOutHeight * iColorChn);
	RET_R(!pOutImageData, ERR);
	
	stbir_resize(pImageData, iWidth, iHeight, 0, pOutImageData, iOutWidth, iOutHeight, 0, 
				STBIR_TYPE_UINT8, iColorChn, STBIR_ALPHA_CHANNEL_NONE, 0,
				STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP,
                STBIR_FILTER_BOX, STBIR_FILTER_BOX,
                STBIR_COLORSPACE_SRGB, NULL);
	
	stbi_write_png(pOutPath, iOutWidth, iOutHeight, iColorChn, pOutImageData, 0);

	stbi_image_free(pImageData);
	stbi_image_free(pOutImageData);
	return OK;
}

int main(int argc, char **argv){
    PDBG_S("START!");

	RET_R(argc < 3, ERR);

	switch(argv[1][0]){
		case '1':
			get_image_size(argv[2], NULL, NULL, NULL, NULL);
			break;
		case '2':
			resize_image(argv[2], argv[3], argv[4]?atoi(argv[4]):2);
			break;
		default:
			POUT_S("Not support argv[1].");
			break;
	}

    return OK;
}


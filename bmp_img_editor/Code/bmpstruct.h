#include "basicheaders.h"
#ifndef BMP_STRUCT_H
#define BMP_STRUCT_H

// according to official documentation of Microsoft
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;

enum BMPCOLORS {
  RED,
  GREEN,
  BLUE
};

enum BMPRETURNS {
  BMP_OK = 0,
  BMP_ERR_OPENING_FILE = 1,
  BMP_ERR_CREATING_FILE = 2,
  BMP_ERR_READING_FILE = 3,
  BMP_ERR_WRITING_FILE = 4,
  BMP_ERR_FILE_NOT_SUPPORTED = 5,
  BMP_ERR_IMAGE_TOO_LARGE = 6,
  BMP_VALUE_NOT_SET = 7
};

enum BMPERRORS {
  ERROR_OUT_OF_BOUNDS,
  ERROR_MYIMAGE_NOT_DEF
};

struct BITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;  // reserved, must be 0
    WORD bfReserved2;  // reserved, must be 0
    DWORD bfOffBits;
} __attribute__((packed));

struct BITMAPINFOHEADER {
    DWORD biSize;
    DWORD biWidth;
    DWORD biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    DWORD biXPelsPerMeter;
    DWORD biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
    // from V2
    DWORD dwRedMask;
    DWORD dwGreenMask;
    DWORD dwBlueMask;
    // from V3
    DWORD dwAlphaMask;
} __attribute__((packed));

struct BGR {
  BYTE &operator[](BMPCOLORS &color)
  {
    switch (color) {
      case BLUE: return b;
      case GREEN: return g;
      case RED: return r;
    }
    throw ERROR_OUT_OF_BOUNDS;
  }

  DWORD operator()() //return rgb
  {
    return static_cast <DWORD> ((r << 16) | (g << 8) | b);
  }

  BGR()
  : b(0), g(0), r(0)
  {}
  BGR(const BYTE *BGR) : b(BGR[0]), g(BGR[1]), r(BGR[2]) {}
  BGR(BYTE blue, BYTE green, BYTE red) : b(blue), g(green), r(red) {}
  BYTE b, g, r;
} __attribute__((packed));

#endif // BMP_STRUCT_H

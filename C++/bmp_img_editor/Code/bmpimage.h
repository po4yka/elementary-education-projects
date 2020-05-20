#ifndef IMAGEBMP_H
#define IMAGEBMP_H

#include <QObject>

#include "basicheaders.h"

#include "bmpstruct.h"

class BMPimage : public QObject
{
    Q_OBJECT
public:
    std::vector<BGR> pixels;

    BMPimage(QObject *parent = nullptr);
    BMPimage(std::string);
    BMPimage(std::string, DWORD, DWORD);

    ~BMPimage(void);

    BMPimage(const BMPimage&);
    BMPimage(BMPimage&);

    BMPimage& operator=(const BMPimage&);
    BMPimage& operator=(BMPimage&);

    // Basic func
    int OpenImage(void);
    int CreateImage(void);
    int ReadImageHeader(void);
    int ReadImagePixels(void);
    int WriteImageHeader(void);
    int WriteImagePixels(void);
    bool CheckBackVal(int value);

    BGR &index(DWORD, DWORD);
    DWORD size(void);
    std::vector<BGR> get_pixels(void);

    // Getters and Setters
    std::string file_name(void);
    WORD type(void);
    WORD bitcount(void);
    DWORD width(void);
    DWORD height(void);
    void set_file_name(std::string);
    void set_width(DWORD);
    DWORD get_width();
    void set_height(DWORD);
    DWORD get_hight();
    DWORD get_bfSize();
    DWORD get_biSize();
    void set_pixels(std::vector<BGR>);

private:
    struct MyImage {
        std::string file_name;
        std::fstream working_file;
        BITMAPFILEHEADER *file_header;
        BITMAPINFOHEADER *info_header;
        BYTE ref_count;
        DWORD width;
        DWORD height;
        MyImage(WORD ref_count, WORD width, WORD height)
        : ref_count(static_cast <BYTE> (ref_count)), width(width), height(height)
        {}
        MyImage(std::string file_name, WORD ref_count, WORD width, WORD height)
        : file_name(file_name), ref_count(static_cast <BYTE> (ref_count)), width(width), height(height)
        {}
    };

    std::shared_ptr<MyImage> my_image_;
    std::shared_ptr<MyImage> MyImageData(BMPERRORS);
    void MyImageCheck(void);

};

#endif // IMAGEBMP_H

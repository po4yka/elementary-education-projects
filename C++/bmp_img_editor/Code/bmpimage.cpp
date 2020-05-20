#include "basicheaders.h"
#include "bmpimage.h"

#include <QApplication>
#include <QMessageBox>
#include <QDialog>
#include <QInputDialog>

#include <QIcon>

BMPimage::BMPimage(QObject *parent)
{
    Q_UNUSED (parent)  // in order to get rid of warning of unused variable
    my_image_.reset();
}

BMPimage::BMPimage(std::string file_name)
{
  my_image_.reset(new MyImage(file_name, 1, 0, 0));
}

BMPimage::BMPimage(std::string file_name, DWORD width, DWORD height)
{
  my_image_.reset(new MyImage(file_name, 1, static_cast <WORD> (width), static_cast <WORD> (height)));
}

BMPimage::~BMPimage(void)
{
  my_image_->working_file.close();
  delete my_image_->file_header;
  delete my_image_->info_header;
}

BMPimage::BMPimage(const BMPimage &bmpimage) : QObject()
{
  *this = const_cast <BMPimage&>(bmpimage);
}

BMPimage::BMPimage(BMPimage &bmpimage) : QObject()
{
  *this = bmpimage;
}

BMPimage &BMPimage::operator=(const BMPimage &bmpimage)
{
  return *this = const_cast<BMPimage&>(bmpimage);
}

BMPimage &BMPimage::operator=(BMPimage &bmpimage)
{
  if(&bmpimage != this) {
    my_image_.swap(bmpimage.my_image_);
    my_image_->ref_count++;
  }

  return *this;
}

bool BMPimage::CheckBackVal(int value)
{
    bool succeeded = false;
    switch(value) {
      case BMP_OK:
        std::cout << "OK" << std::endl;
        succeeded = true;
        break;
      case BMP_ERR_OPENING_FILE:
        std::cerr << "FAIL\n - Could not open the file" << std::endl;
        QMessageBox::warning(nullptr, "Warning", "Could not open the file!");
        break;
      case BMP_ERR_CREATING_FILE:
        std::cerr << "FAIL\n - Could not create file" << std::endl;
        QMessageBox::warning(nullptr, "Warning", "Could not create file!");
        break;
      case BMP_ERR_READING_FILE:
        std::cerr << "FAIL\n - Could not read data from the file" << std::endl;
        break;
      case BMP_ERR_WRITING_FILE:
        std::cerr << "FAIL\n - Could not write data to the file" << std::endl;
        break;
      case BMP_ERR_FILE_NOT_SUPPORTED:
        std::cerr << "FAIL\n - The file is not in BMP format or is not 24 bits" << std::endl;
        QMessageBox::warning(nullptr, "Warning", "The file is not in BMP format or is not 24 bits!");
        break;
      case BMP_ERR_IMAGE_TOO_LARGE:
        std::cerr << "FAIL\n - The image is too large for this program" << std::endl;
        QMessageBox::warning(nullptr, "Warning", "The image is too large for this program!");
        break;
      case BMP_VALUE_NOT_SET:
        std::cerr << "FAIL\n - Value not set\n" << std::endl;
        QMessageBox::warning(nullptr, "Warning", "Value not set!");
    }
    return succeeded;
}

void BMPimage::MyImageCheck(void)
{
  if (my_image_ == nullptr)
      throw ERROR_MYIMAGE_NOT_DEF;
}

int BMPimage::OpenImage(void)
{
  MyImageCheck();
  std::cout << "[+] Opening BMP file \"" << my_image_->file_name << "\"...";
  if(my_image_->working_file.is_open())
  {
      my_image_->working_file.seekg(0);
      my_image_->working_file.close();
  }
  my_image_->working_file.open(my_image_->file_name.c_str(), std::fstream::in | std::fstream::binary);
  if (!my_image_->working_file.is_open())
    return BMP_ERR_OPENING_FILE;

  return BMP_OK;
}

int BMPimage::CreateImage(void)
{
  MyImageCheck();
  std::cout << "[+] Creating BMP file \"" << my_image_->file_name << "\"...";
  my_image_->working_file.open(my_image_->file_name.c_str(), std::fstream::out | std::fstream::binary);
  if (!my_image_->working_file.is_open())
    return BMP_ERR_CREATING_FILE;

  return BMP_OK;
}

int BMPimage::ReadImageHeader(void)
{
  MyImageCheck();

  std::cout << "[+] Reading image headers from \"" << my_image_->file_name << "\" into memory...";

  if (!my_image_->working_file.is_open())
    return BMP_ERR_READING_FILE;

  BYTE *image_data[2];

  my_image_->file_header = new BITMAPFILEHEADER();
  my_image_->info_header = new BITMAPINFOHEADER();

  image_data[0] = new BYTE[sizeof(BITMAPFILEHEADER)];
  image_data[1] = new BYTE[sizeof(BITMAPINFOHEADER)];

  my_image_->working_file.read(reinterpret_cast<char*>(image_data[0]),
                               sizeof(BITMAPFILEHEADER));
  my_image_->working_file.read(reinterpret_cast<char*>(image_data[1]),
                               sizeof(BITMAPINFOHEADER));

  memcpy(my_image_->file_header, image_data[0], sizeof(BITMAPFILEHEADER));
  memcpy(my_image_->info_header, image_data[1], sizeof(BITMAPINFOHEADER));

  if ((my_image_->file_header->bfType != 0x4D42) ||
      (my_image_->info_header->biBitCount != 24) ||
      (my_image_->info_header->biSize != 40))
  return BMP_ERR_FILE_NOT_SUPPORTED;

  my_image_->height = my_image_->info_header->biHeight;
  my_image_->width  = my_image_->info_header->biWidth;

  delete[] image_data[0];
  delete[] image_data[1];

  return BMP_OK;
}

int BMPimage::ReadImagePixels(void)
{
  MyImageCheck();
  std::cout << "[+] Reading image pixels from \"" << my_image_->file_name << "\" into memory...";

  if (!my_image_->working_file.is_open())
    return BMP_ERR_READING_FILE;

  DWORD height = my_image_->height;
  DWORD width = my_image_->width;

  BYTE pad_offset = static_cast <BYTE> (( ((width * 3 + 3) & static_cast <unsigned int> (~3)) - (width * 3) ) * sizeof(BYTE));
  BGR temp;

  my_image_->working_file.seekg(my_image_->file_header->bfOffBits);

  pixels.resize(width*height);
  for(unsigned row = 0; row < height; ++row) {
      my_image_->working_file.read(reinterpret_cast<char*> (&pixels[row*width]), width*sizeof(BGR)); my_image_->working_file.seekg(pad_offset, std::ios::cur);
  }

  return BMP_OK;
}

int BMPimage::WriteImageHeader(void)
{
  MyImageCheck();
  std::cout << "[+] Writing image headers to \"" << my_image_->file_name << "\"...";

  if (!my_image_->working_file.is_open())
    return BMP_ERR_WRITING_FILE;

  my_image_->file_header = new BITMAPFILEHEADER();
  my_image_->info_header = new BITMAPINFOHEADER();

  DWORD height = my_image_->height;
  DWORD width  = my_image_->width;
  DWORD width_padded = (width * 3 + 3) & (static_cast <unsigned int> (~3));
  DWORD img_size = width_padded * height;

  BYTE fileheader_data[] = {"\x42\x4D"           // signature
                            "\x00\x00\x00\x00"   // file size
                            "\x00\x00\x00\x00"   // reserved 1
                            "\x00\x00\x00\x00"   // reserved 2
                            "\x36\x00\x00\x00"}; // file_offset
  BYTE infoheader_data[] = {"\x28\x00\x00\x00"   // header size
                            "\x00\x00\x00\x00"   // width
                            "\x00\x00\x00\x00"   // height
                            "\x01\x00"           // planes
                            "\x18\x00"           // bits per pixel
                            "\x00\x00\x00\x00"   // compression
                            "\x00\x00\x00\x00"   // imgSize
                            "\x12\x0b\x00\x00"   // X Resolution
                            "\x12\x0b\x00\x00"   // Y Resolution
                            "\x00\x00\x00\x00"   // colours
                            "\x00\x00\x00\x00"}; // important Colours

  memcpy(my_image_->file_header, fileheader_data, sizeof(BITMAPFILEHEADER));
  my_image_->file_header->bfType = 0x4D42;
  my_image_->file_header->bfSize = 0;
  my_image_->file_header->bfReserved1 = 0;
  my_image_->file_header->bfReserved2 = 0;
  my_image_->file_header->bfOffBits = 0x36;
  memcpy(my_image_->info_header, infoheader_data, sizeof(BITMAPINFOHEADER));

  int colorsCount = my_image_->info_header->biBitCount >> 3;
  int bitsOnColor = my_image_->info_header->biBitCount / colorsCount;
  int maskValue = (1 << bitsOnColor) - 1;  // default mask value

  my_image_->info_header->dwRedMask = static_cast <unsigned int> (maskValue << (bitsOnColor * 2));
  my_image_->info_header->dwGreenMask = static_cast <unsigned int> (maskValue << bitsOnColor);
  my_image_->info_header->dwBlueMask = static_cast <unsigned int> (maskValue << maskValue);
  my_image_->info_header->dwAlphaMask = static_cast <unsigned int> (maskValue << (bitsOnColor * 3));

  my_image_->info_header->biWidth = width;
  my_image_->info_header->biHeight = height;
  my_image_->info_header->biSizeImage = img_size;

  my_image_->working_file.seekg(0);
  my_image_->working_file.write(reinterpret_cast <const char*> (my_image_->file_header), sizeof(BITMAPFILEHEADER));
  my_image_->working_file.write(reinterpret_cast <const char*> (my_image_->info_header), sizeof(BITMAPINFOHEADER));

  return BMP_OK;
}

int BMPimage::WriteImagePixels(void)
{
  MyImageCheck();
  std::cout << "[+] Writing image pixels to \"" << my_image_->file_name << "\"...";

  if (!my_image_->working_file.is_open())
    return BMP_ERR_WRITING_FILE;

  DWORD height = my_image_->height;
  DWORD width = my_image_->width;
  BYTE padding_amount = static_cast <BYTE> (((width * 3 + 3) & static_cast <unsigned int> (~3)) - (width * 3));
  BYTE *padding = new BYTE[padding_amount];

  my_image_->working_file.seekg(my_image_->file_header->bfOffBits);
  for (unsigned int i = 0; i < height; i++) {
    for (unsigned int j = 0; j < width; j++)
        my_image_->working_file.write( reinterpret_cast<const char*> ( &pixels.at(static_cast<unsigned long> ( (i * width) + j) ) ), sizeof(BGR) );
    my_image_->working_file.write(reinterpret_cast<const char*>(padding), padding_amount);
  }

  delete[] padding;

  return BMP_OK;
}

BGR &BMPimage::index(DWORD x, DWORD y)
{
  MyImageCheck();
  if ((x >= my_image_->width) ||
      (y >= my_image_->height))
    throw ERROR_OUT_OF_BOUNDS;

  return pixels.at( (y * my_image_->height) + x );
}

WORD BMPimage::type(void)
{
    return my_image_->file_header->bfType;
}

WORD BMPimage::bitcount(void)
{
    return my_image_->info_header->biBitCount;
}

DWORD BMPimage::size(void)
{
  MyImageCheck();
  return (my_image_->width * my_image_->height);
}

std::string BMPimage::file_name(void)
{
  MyImageCheck();
  return my_image_->file_name;
}

void BMPimage::set_file_name(std::string file_name)
{
  MyImageCheck();
  my_image_->file_name = file_name;
}

DWORD BMPimage::width(void)
{
  MyImageCheck();
  return my_image_->width;
}

DWORD BMPimage::height(void)
{
  MyImageCheck();
  return my_image_->height;
}

std::vector<BGR> BMPimage::get_pixels(void)
{
  MyImageCheck();
  return pixels;
}

void BMPimage::set_width(DWORD width)
{
  MyImageCheck();
  my_image_->width = width;
}

DWORD BMPimage::get_width()
{
    MyImageCheck();
    return my_image_->width;
}

DWORD BMPimage::get_hight()
{
    MyImageCheck();
    return my_image_->height;
}

void BMPimage::set_height(DWORD height)
{
  MyImageCheck();
  my_image_->height = height;
}

DWORD BMPimage::get_bfSize()
{
    MyImageCheck();
    return my_image_->file_header->bfSize;
}

void BMPimage::set_pixels(std::vector<BGR> new_pixels)
{
  MyImageCheck();
  pixels = new_pixels;
}

std::shared_ptr<BMPimage::MyImage> BMPimage::MyImageData(BMPERRORS error)
{
  if (my_image_ == nullptr) throw error;

  return my_image_;
}

DWORD BMPimage::get_biSize()
{
    return my_image_->info_header->biSize;
}

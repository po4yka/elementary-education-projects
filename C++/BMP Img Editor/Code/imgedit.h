#ifndef IMGEDIT_H
#define IMGEDIT_H

#include "bmpimage.h"

typedef struct coordPos{
    int x_1 = 0;
    int x_2 = 0;
    int y_1 = 0;
    int y_2 = 0;
} coordPos;

typedef struct rect{
    int x;
    int y;
    int square;
}rect;

class ImgEdit
{
public:
    ImgEdit()
    {
        bmpImg = new BMPimage("../ProgramFiles/input.bmp");
        editing_image_.reset();
        coordinates.x_1 = 0;
        coordinates.x_2 = 0;
        coordinates.y_1 = 0;
        coordinates.y_2 = 0;
        x_dist = 0;
        y_dist = 0;
    }

    ~ImgEdit(void);

    void InitImg(void);
    int saveImg(std::string);

    int check_corr(void);
    int get_hight(void);
    int get_width(void);
    int get_size(void);
    void destr_bmp(void);
    int get_version(void);

    // extra func
    int BinCopy(void);

    // auxiliary func
    QPixmap get_pixmap(void);
    bool open_flag = false;

    int reflection(bool flag);
    int compression(int n, int m);
    int split(int n, int m, int line_thick, QColor line_color);
    int draw_line(int x_1, int y_1, int x_2, int y_2, QColor color_lines);
    int change_color(QColor colToCh, QColor colOnWhCh);
    int make_black_and_white();
    int copy_zone();
    int rotate(int angle);
    int draw_rect(int x_1, int y_1, int x_2, int y_2, int thick, QColor color_lines, QColor color_flood, bool flag);
    int rect_find_refill(QColor colToCh, QColor colOnWhCh);
    bool changeFillColor(int x, int y, QColor oldColor, QColor newColor);

    coordPos coordinates;
    int x_dist;
    int y_dist;
    int x_rect;
    int y_rect;

private:
    BMPimage *bmpImg;
    QImage *image;
    int checkRect(int x, int y, QColor color);
    int checkColor(int x, int y, QColor color);
    int **colorMap;
    bool colorPoint(int x, int y, QColor color);

    std::shared_ptr<ImgEdit> editing_image_;
};

#endif // IMGEDIT_H

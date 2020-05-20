#include "universalqtheader.h"

ImgEdit::~ImgEdit(void)
{
    delete bmpImg;
    delete image;
}

void ImgEdit::InitImg(void)
{
    if (bmpImg->CheckBackVal(bmpImg->OpenImage()) == false)
        return;

    if (bmpImg->CheckBackVal(bmpImg->ReadImageHeader()) == false)
        return;

    if (bmpImg->CheckBackVal(bmpImg->ReadImagePixels()) == false)
        return;

    image = new QImage;
    image->load("../ProgramFiles/input.bmp");
}

int ImgEdit::saveImg(std::string fname)
{
    std::vector<BGR> pixels = bmpImg->pixels;
    DWORD width  = bmpImg->width();
    DWORD height = bmpImg->height();
    BMPimage save_img(fname);

    save_img.set_file_name(fname);
    save_img.set_width(width);
    save_img.set_height(height);
    save_img.set_pixels(pixels);

    if (!save_img.CheckBackVal(save_img.CreateImage()))
        return 1;
    save_img.CheckBackVal(save_img.WriteImageHeader());
    save_img.CheckBackVal(save_img.WriteImagePixels());
    return 0;
}

int ImgEdit::checkColor(int x, int y, QColor color)
{
    int width = static_cast<int> (bmpImg->get_width());
    int hight = static_cast<int> (bmpImg->get_hight());
    unsigned char red = static_cast<unsigned char>(color.red());
    unsigned char green = static_cast<unsigned char>(color.green());
    unsigned char blue = static_cast<unsigned char>(color.blue());

    y = hight - y;

    if (bmpImg->pixels[static_cast<unsigned long> (y * width + x)].r == red &&
            bmpImg->pixels[static_cast<unsigned long> (y * width + x)].g == green &&
            bmpImg->pixels[static_cast<unsigned long> (y * width + x)].b == blue)
    {
        return 1;
    }
    else return 0;
}

int ImgEdit::checkRect(int x, int y, QColor color)
{
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    int x_0 = x;
    int y_0 = y;

    while (checkColor(x_0, y_0, color))
    {
        colorMap[x_0][y_0] = 2;
        a++;
        if (checkColor(x_0, y_0 - 1, color))
        {
            return -1;
        }
        x_0++;
    }
    x_0--;
    a--;
    while(checkColor(x_0, y_0, color))
    {
        colorMap[x_0][y_0] = 2;
        b++;
        if(checkColor(x_0 + 1, y_0, color))
        {
            return -1;
        }
        y_0++;
    }
    y_0--;
    b--;
    while (checkColor(x_0, y_0, color))
    {
        colorMap[x_0][y_0] = 2;
        c++;
        if (checkColor(x_0, y_0 + 1, color))
        {
            return -1;
        }
        x_0--;
    }
    x_0++;
    c--;
    while(checkColor(x_0, y_0, color))
    {
        colorMap[x_0][y_0] = 2;
        d++;
        if(checkColor(x_0 - 1, y_0, color))
        {
            return -1;
        }
        y_0--;
    }
    y_0++;
    d--;
    int deltax = 0;
    int deltay = 0;
    int err = 0;
    if (a == c && b == d)
    {
        while (deltay < b)
        {
            deltax = 0;
            while(deltax < a)
            {
                if (colorMap[x+deltax][y+deltay] < 1)
                    err = 1;
                else
                    colorMap[x+deltax][y+deltay] = 2;
                deltax++;
            }
            deltay++;
        }
        if (!err)
            return a*b;
        else
            return -1;
    }
    return -1;
}
///////////////////////////////////////////////////////////////////
//                                                               //
//                 GETTERS/SETTERS/WORKING FUNC                  //
//                                                               //
///////////////////////////////////////////////////////////////////

int ImgEdit::get_hight(void)
{
    return static_cast<int>( bmpImg->get_hight() );
}

int ImgEdit::get_width(void)
{
    return static_cast<int>( bmpImg->get_width() );
}

int ImgEdit::get_size(void)
{
    return static_cast<int>( bmpImg->get_bfSize() );
}

QPixmap ImgEdit::get_pixmap()
{
    int width = static_cast<int> (bmpImg->get_width());
    image = new QImage(static_cast<int>( bmpImg->get_width() ),
    static_cast<int> ( bmpImg->get_hight() ),
    QImage::Format_RGB666);  // Format_RGB666 - The image is stored using a 24-bit RGB format (6-6-6)
    QColor color = QColor(255, 255, 255);
    image->fill(color);
    QColor pixel;
    int y = static_cast<int>( bmpImg->get_hight() );
    int x = static_cast<int>( bmpImg->get_width() );
    for (int i = y - 1; i >= 0; i--){
        for (int j = 0; j < x; j++){
            pixel.setRed(bmpImg->pixels.at(static_cast<unsigned long> ( (i * width) + j) ).r);
            pixel.setGreen(bmpImg->pixels.at(static_cast<unsigned long> ( (i * width) + j) ).g);
            pixel.setBlue(bmpImg->pixels.at(static_cast<unsigned long> ( (i * width) + j) ).b);
            image->setPixel(j, y - i - 1, pixel.rgb());
        }
    }
    return QPixmap::fromImage(*image);
}
int ImgEdit::get_version()
{
    return static_cast<int>( bmpImg->get_biSize() );
}
int ImgEdit::check_corr(void)
{
    if (open_flag == false)
    {
        return 0;
    }
    return 1;
}
void ImgEdit::destr_bmp(void)
{
    std::vector<BGR>().swap(bmpImg->pixels);
}

///////////////////////////////////////////////////////////////////
//                                                               //
//                            MAIN FUNC                          //
//                                                               //
///////////////////////////////////////////////////////////////////

int ImgEdit::reflection(bool flag)
{   
    int middle;
    coordinates.y_2 = static_cast<int> (bmpImg->get_hight()) - coordinates.y_2;
    coordinates.y_1 = static_cast<int> (bmpImg->get_hight()) - coordinates.y_1;
    int width = static_cast<int> (bmpImg->get_width());

    flag ? middle = (coordinates.x_1 + coordinates.x_2) / 2 : middle = (coordinates.y_1 + coordinates.y_2) / 2;

    if (coordinates.y_2 < coordinates.y_1){
        std::swap(coordinates.y_1, coordinates.y_2);
    }
    if (coordinates.x_1 < coordinates.x_2){
        std::swap(coordinates.x_1, coordinates.x_2);
    }
    if (flag){
        for (int i = coordinates.y_1; i <= coordinates.y_2; i++) {
            for (int j = coordinates.x_2; j <= middle; j++){
                std::swap(bmpImg->pixels.at(static_cast<unsigned long> ( (i * width) + j) ),
                bmpImg->pixels.at(static_cast<unsigned long> ((i * width) + (coordinates.x_1 + coordinates.x_2 - j))));
            }
        }
    }
    else{
        for (int j = coordinates.x_2; j <= coordinates.x_1; j++){
            for (int i = coordinates.y_1; i <= middle; i++){
                std::swap(bmpImg->pixels.at(static_cast<unsigned long> ( (i * width) + j) ),
                bmpImg->pixels.at(static_cast<unsigned long> ( ((coordinates.y_1 + coordinates.y_2 - i) * width) + j) ));
            }
        }
    }

    return 0;
}

int ImgEdit::compression(int n, int m)
{
    int width = static_cast<int> (bmpImg->get_width());
    int hight = static_cast<int> (bmpImg->get_hight());
    int piece_width = width / m;
    int piece_hight = hight / n;

    BGR **rgb;
    rgb = new BGR* [hight];
    for (int i = 0; i < hight; i++){
        rgb[i] = new BGR[width];
    }
    for (int i = 0; i < hight; i++)
    {
        for (int j = 0; j < width; j++)
        {
            rgb[i][j] = bmpImg->pixels.at(static_cast<unsigned long> ( (i * hight) + j));
        }
    }

    for (int i = 0; i < hight - 1; i++){
        if (i % n){
            rgb[i][0].r = '\0';
            rgb[i][0].g = '\0';
            rgb[i][0].b = '\0';
        }
    }

    for (int i = 0; i < hight;){
        if (rgb[i][0].r == '\0' && rgb[i][0].g == '\0' && rgb[i][0].b == '\0'){
                delete rgb[i];
                memmove(rgb + i, rgb + i + 1, ((size_t(hight) - size_t(i) - 1) * sizeof(BGR*)));
                i--;
        }
        i++;
    }

    for (int i = 0; i < piece_hight; i++){
        for (int j = 0; j < width - 1; j++){
            if (j % m){
                rgb[i][j].r = '\0';
                rgb[i][j].g = '\0';
                rgb[i][j].b = '\0';
            }
        }
    }

    for (int i = 0; i < hight; i++){
        for (int j = 0; j < width - 1;){
            if (rgb[i][j].r == '\0' && rgb[i][j].g == '\0' && rgb[i][j].b == '\0'){
                memmove(rgb[i] + j, rgb[i] + j + 1, ((size_t(width) - size_t(j) - 1) * sizeof(BGR)));
                j--;
            }
            j++;
        }
    }

    for (int i = 0; i < piece_hight; i++){
        for (int k = 1; k < m; k++){
            for (int j = 0; j < piece_width; j++){
                rgb[i][j + k * piece_width].r = rgb[i][j].r;
                rgb[i][j + k * piece_width].g = rgb[i][j].g;
                rgb[i][j + k * piece_width].b = rgb[i][j].b;
            }
        }
    }
    for (int i = piece_hight; i < hight; i++){
        rgb[i] = new BGR[width];
    }
    for (int j = 0; j < width; j++){
        for (int k = 1; k < n; k++){
            for (int i = 0; i < piece_hight; i++){
                rgb[i + k * piece_hight][j].r = rgb[i][j].r;
                rgb[i + k * piece_hight][j].g = rgb[i][j].g;
                rgb[i + k * piece_hight][j].b = rgb[i][j].b;
            }
        }
    }

    for (int i = 0; i < hight; i++){
        for (int j = 0; j < width; j++){
            if (i >= piece_hight * n || j >= piece_width * m){
                rgb[i][j].r = 255;
                rgb[i][j].g = 255;
                rgb[i][j].b = 255;
            }
        }
    }

    std::vector<BGR> rgb_ready;
    for (int i = 0; i < hight; i++)
    {
        for (int j = 0; j < width; j++)
        {
            rgb_ready.push_back(rgb[i][j]);
        }
    }

    bmpImg->set_pixels(rgb_ready);

    return 0;
}

int ImgEdit::split(int n, int m, int line_thick, QColor line_color)
{
    int width = static_cast<int> (bmpImg->get_width());
    int hight = static_cast<int> (bmpImg->get_hight()) - 1;

    n = width / n;
    m = hight / m;
    int thick = 0;
    bool flag_thick = false;
    int row = 0, col = 0, j = 0;

    if (line_thick % 2 != 0)
        flag_thick = true;
    thick = line_thick / 2;

    for (col = m; col < hight; col += m)
    {
        if (hight - col > m / 2)
        {
            draw_line(col, 0, col, width, line_color);
            if (flag_thick == true)
            {
                for (j = 0; j < thick; j++)
                {
                    if (col + j < width)
                        draw_line(col + j, 0, col + j, hight, line_color);
                    if (col - j > 0)
                        draw_line(col - j, 0, col - j, hight, line_color);
                }
            } else
            {
                for (int j = 0; j < thick - 1; j++)
                {
                    if (col + j < width)
                        draw_line(col + j, 0, col + j, hight, line_color);
                    if (col - j > 0)
                        draw_line(col - j, 0, col - j, hight, line_color);
                }
                if (col + j + 1 < width)
                    draw_line(col + j + 1, 0, col + j + 1, hight, line_color);
            }
        }
    }

    for (row = n; row < width; row += n)
    {
        if (width - row > n / 2)
        {
            draw_line(0, row, hight, row, line_color);
            if (flag_thick == true)
            {
                for (j = 0; j < thick; j++)
                {
                    if (row + j < hight)
                        draw_line(0, row + j, hight, row + j, line_color);
                    draw_line(0, row - j, hight, row - j, line_color);
                }
            } else
            {
                for (int j = 0; j < thick - 1; j++)
                {
                    if (row + j < hight)
                        draw_line(0, row + j, hight, row + j, line_color);
                    draw_line(0, row - j, hight, row - j, line_color);
                }
                if (row + j + 1 < hight)
                    draw_line(0, row + j + 1, hight, row + j + 1, line_color);
            }
        }
    }

    return 0;
}

// Bresenham Line Drawing Algorithm.
int ImgEdit::draw_line(int x_1, int y_1, int x_2, int y_2, QColor color_lines)
{
    const int deltaX = abs(x_2 - x_1);
    const int deltaY = abs(y_2 - y_1);

    const int signX = x_1 < x_2 ? 1 : -1;
    const int signY = y_1 < y_2 ? 1 : -1;
    int width = static_cast<int> (bmpImg->get_width());
    unsigned char red = static_cast<unsigned char>(color_lines.red());
    unsigned char green = static_cast<unsigned char>(color_lines.green());
    unsigned char blue = static_cast<unsigned char>(color_lines.blue());
    int error = deltaX - deltaY;
    bmpImg->pixels.at(static_cast<unsigned long> ( (x_2 * width) + y_2)).r = red;
    bmpImg->pixels.at(static_cast<unsigned long> ( (x_2 * width) + y_2)).g = green;
    bmpImg->pixels.at(static_cast<unsigned long> ( (x_2 * width) + y_2)).b = blue;
    while(x_1 != x_2 || y_1 != y_2)
    {
        bmpImg->pixels.at(static_cast<unsigned long> ( (x_1 * width) + y_1)).r = red;
        bmpImg->pixels.at(static_cast<unsigned long> ( (x_1 * width) + y_1)).g = green;
        bmpImg->pixels.at(static_cast<unsigned long> ( (x_1 * width) + y_1)).b = blue;
        const int error2 = error * 2;
        if (error2 > -deltaY)
        {
            error -= deltaY;
            x_1 += signX;
        }
        if (error2 < deltaX)
        {
            error += deltaX;
            y_1 += signY;
        }
    }

    return 0;
}

int ImgEdit::change_color(QColor colToCh, QColor colOnWhCh)
{
    unsigned char red_to_change = static_cast<unsigned char>(colToCh.red());
    unsigned char green_to_change = static_cast<unsigned char>(colToCh.green());
    unsigned char blue_to_change = static_cast<unsigned char>(colToCh.blue());

    unsigned char red_on_which = static_cast<unsigned char>(colOnWhCh.red());
    unsigned char green_on_which = static_cast<unsigned char>(colOnWhCh.green());
    unsigned char blue_on_which = static_cast<unsigned char>(colOnWhCh.blue());

    int width = static_cast<int> (bmpImg->get_width());
    int height = static_cast<int> (bmpImg->get_hight());

    for (int i = 0; i < height - 1; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if(bmpImg->pixels.at(static_cast<unsigned long> ( (i * width) + j)).r == red_to_change &&
            bmpImg->pixels.at(static_cast<unsigned long> ( (i * width) + j)).g == green_to_change &&
            bmpImg->pixels.at(static_cast<unsigned long> ( (i * width) + j)).b == blue_to_change)
            {
                bmpImg->pixels.at(static_cast<unsigned long> ( (i * width) + j)).r = red_on_which;
                bmpImg->pixels.at(static_cast<unsigned long> ( (i * width) + j)).g = green_on_which;
                bmpImg->pixels.at(static_cast<unsigned long> ( (i * width) + j)).b = blue_on_which;
            }
        }
    }

    return 0;
}

int ImgEdit::make_black_and_white()
{
    int width = static_cast<int> (bmpImg->get_width());
    int hight = static_cast<int> (bmpImg->get_hight());
    int avg;

    for (int row = 0; row < hight; row++)
    {
        for (int col = 0; col < width; col++)
        {
            avg = (bmpImg->pixels[static_cast<unsigned long>( row * width + col)].r +
            bmpImg->pixels[static_cast<unsigned long>( row * width + col)].g +
            bmpImg->pixels[static_cast<unsigned long>( row * width + col)].b) / 3;
            bmpImg->pixels[static_cast<unsigned long>( row * width + col)].r = static_cast<unsigned char>(avg);
            bmpImg->pixels[static_cast<unsigned long>( row * width + col)].g = static_cast<unsigned char>(avg);
            bmpImg->pixels[static_cast<unsigned long>( row * width + col)].b = static_cast<unsigned char>(avg);
            }
    }
    return 0;
}

int ImgEdit::copy_zone()
{
    coordinates.y_2 = static_cast<int> (bmpImg->get_hight()) - coordinates.y_2;
    coordinates.y_1 = static_cast<int> (bmpImg->get_hight()) - coordinates.y_1;

    // x_1 < x_2 && y_1 < y_2
    if (coordinates.x_1 > coordinates.x_2 && coordinates.y_1 > coordinates.y_2)
    {
        std::swap(coordinates.x_1, coordinates.x_2);
        std::swap(coordinates.y_1, coordinates.y_2);
    } else if (coordinates.x_1 < coordinates.x_2 && coordinates.y_1 > coordinates.y_2)
    {
        std::swap(coordinates.y_1, coordinates.y_2);
    } else if (coordinates.x_1 > coordinates.x_2 && coordinates.y_1 < coordinates.y_2)
    {
        std::swap(coordinates.x_1, coordinates.x_2);
    }

    int width = static_cast<int> (bmpImg->get_width());
    int copy_width = coordinates.x_2 - coordinates.x_1;
    int copy_height = coordinates.y_2 - coordinates.y_1;

    y_dist = static_cast<int> (bmpImg->get_hight()) - y_dist - copy_height;

    if (static_cast<unsigned long>( x_dist + copy_width ) > bmpImg->get_width() || static_cast<unsigned long>( y_dist + copy_height ) > bmpImg->get_hight())
    {
        return 1;
    }

    std::vector<BGR> zone;

    for (int i = coordinates.y_2, m = 0; i > coordinates.y_1; i--, m++)
    {
        for (int j = coordinates.x_1, n = 0; j < coordinates.x_2; j++, n++)
        {
            zone.push_back(bmpImg->pixels.at(static_cast<unsigned long> ( (i * width) + j)));
        }
    }

    for (int i = y_dist + copy_height, m = 0; i > y_dist; i--, m++)
    {
        for (int j = x_dist, n = 0; j < x_dist + copy_width; j++, n++)
        {
            bmpImg->pixels.at(static_cast<unsigned long> ( (i * width) + j)) = zone.at(static_cast<unsigned long> ( (m * copy_width) + n));
        }
    }


    return 0;
}

int ImgEdit::rotate(int angle)
{
    int width = static_cast<int> (bmpImg->get_width());
    int hight = static_cast<int> (bmpImg->get_hight());
    int new_x, new_y;
    int center_x, center_y;
    center_x = (width - 1) / 2;
    center_y = (hight - 1) / 2;
    int prec = 15;
    std::vector <BGR> temp;
    temp.resize(static_cast<unsigned long>(hight * width));

    for (int x = 0; x < width; x++){
        for (int y = 0; y < hight; y++){
            temp[static_cast<unsigned long>(y * width + x)].r = bmpImg->pixels[static_cast<unsigned long>(y * width + x)].r;
            temp[static_cast<unsigned long>(y * width + x)].g = bmpImg->pixels[static_cast<unsigned long>(y * width + x)].g;
            temp[static_cast<unsigned long>(y * width + x)].b = bmpImg->pixels[static_cast<unsigned long>(y * width + x)].b;

            bmpImg->pixels[static_cast<unsigned long>(y * width + x)].r = 255;
            bmpImg->pixels[static_cast<unsigned long>(y * width + x)].g = 255;
            bmpImg->pixels[static_cast<unsigned long>(y * width + x)].b = 255;
        }
    }

    double a_sin = sin(angle * M_PI / 180);
    double a_cos = cos(angle * M_PI / 180);
    for (int x = 0; x < width; x++){
        for (int y = 0; y < hight; y++){
            new_x = static_cast<int>(round((x - center_x) * a_cos - (y - center_y) * a_sin) + center_x);
            new_y = static_cast<int>(round((x - center_x) * a_sin + (y - center_y) * a_cos) + center_y);
            if (new_x > 0 && new_y > 0 && new_x < width && new_y < hight){
                bmpImg->pixels[static_cast<unsigned long>(new_y * width + new_x)].r = temp[static_cast<unsigned long>(y * width + x)].r;
                bmpImg->pixels[static_cast<unsigned long>(new_y * width + new_x)].g = temp[static_cast<unsigned long>(y * width + x)].g;
                bmpImg->pixels[static_cast<unsigned long>(new_y * width + new_x)].b = temp[static_cast<unsigned long>(y * width + x)].b;
            }
        }
    }
    for (int x = 0; x < width; x++){
        for (int y = 0; y < hight; y++){
            if (x + 1 < width && y + 1 < hight && x - 1 > 0 && y - 1 > 0)
            {
                if (bmpImg->pixels[static_cast<unsigned long>(y * width + x)].r == 255 &&
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x)].g == 255 &&
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x)].b == 255 &&
                        bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x)].r != 255 &&
                        bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x)].g != 255 &&
                        bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x)].b != 255 &&
                        bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x)].r != 255 &&
                        bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x)].g != 255 &&
                        bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x)].b != 255 &&
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x + 1)].r != 255 &&
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x + 1)].g != 255 &&
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x + 1)].b != 255 &&
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x - 1)].r != 255 &&
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x - 1)].g != 255 &&
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x - 1)].b != 255)
                {
                    if (abs(bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x - 1)].r - bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x + 1)].r) < prec &&
                        abs(bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x - 1)].g - bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x + 1)].g) < prec &&
                        abs(bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x - 1)].b - bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x + 1)].b) < prec)
                    {
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x)].r = static_cast<unsigned char>((bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x - 1)].r + bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x + 1)].r) / 2);
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x)].g = static_cast<unsigned char>((bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x - 1)].g + bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x + 1)].g) / 2);
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x)].b = static_cast<unsigned char>((bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x - 1)].b + bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x + 1)].b) / 2);
                        continue;

                    } else if (abs(bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x - 1)].r - bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x + 1)].r) < prec &&
                               abs(bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x - 1)].g - bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x + 1)].g) < prec &&
                               abs(bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x - 1)].b - bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x + 1)].b) < prec)
                    {
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x)].r = static_cast<unsigned char>((bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x - 1)].r + bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x + 1)].r) / 2);
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x)].g = static_cast<unsigned char>((bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x - 1)].g + bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x + 1)].g) / 2);
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x)].b = static_cast<unsigned char>((bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x - 1)].b + bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x + 1)].b) / 2);
                        continue;

                    } else if (abs(bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x)].r - bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x)].r) < prec &&
                               abs(bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x)].g - bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x)].g) < prec &&
                               abs(bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x)].b - bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x)].b) < prec)
                    {
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x)].r = static_cast<unsigned char>((bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x)].r + bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x)].r) / 2);
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x)].g = static_cast<unsigned char>((bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x)].g + bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x)].g) / 2);
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x)].b = static_cast<unsigned char>((bmpImg->pixels[static_cast<unsigned long>((y + 1) * width + x)].b + bmpImg->pixels[static_cast<unsigned long>((y - 1) * width + x)].b) / 2);
                        continue;
                    } else
                    {
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x)].r = static_cast<unsigned char>((bmpImg->pixels[static_cast<unsigned long>(y * width + x - 1)].r + bmpImg->pixels[static_cast<unsigned long>(y * width + x + 1)].r) / 2);
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x)].g = static_cast<unsigned char>((bmpImg->pixels[static_cast<unsigned long>(y * width + x - 1)].g + bmpImg->pixels[static_cast<unsigned long>(y * width + x + 1)].g) / 2);
                        bmpImg->pixels[static_cast<unsigned long>(y * width + x)].b = static_cast<unsigned char>((bmpImg->pixels[static_cast<unsigned long>(y * width + x - 1)].b + bmpImg->pixels[static_cast<unsigned long>(y * width + x + 1)].b) / 2);
                    }
                }
            }
        }
    }

    return 0;
}

int ImgEdit::draw_rect(int x_1, int y_1, int x_2, int y_2, int thick, QColor color_lines, QColor color_flood, bool flag)
{
    unsigned char red = static_cast<unsigned char>(color_lines.red());
    unsigned char green = static_cast<unsigned char>(color_lines.green());
    unsigned char blue = static_cast<unsigned char>(color_lines.blue());
    unsigned char red_flood = '\0';
    unsigned char green_flood = '\0';
    unsigned char blue_flood = '\0';
    int width = static_cast<int> (bmpImg->get_width());
    int hight = static_cast<int> (bmpImg->get_hight());
    if (flag){
        red_flood = static_cast<unsigned char>(color_flood.red());
        green_flood = static_cast<unsigned char>(color_flood.green());
        blue_flood = static_cast<unsigned char>(color_flood.blue());
    }
    y_2 = hight - y_2;
    y_1 = hight - y_1;
    if (y_2 < y_1){
        std::swap(y_1, y_2);
    }
    if (x_1 < x_2){
        std::swap(x_1, x_2);
    }
    for (int i = y_1; i <= y_2; i++) {
        for (int j = x_2; j <= x_1; j++){
            if (abs(i - y_1) < thick || abs(j - x_1) < thick || abs(i - y_2) < thick || abs(j - x_2) < thick){
                bmpImg->pixels[static_cast<unsigned long> ( (i * width) + j)].r = red;
                bmpImg->pixels[static_cast<unsigned long> ( (i * width) + j)].g = green;
                bmpImg->pixels[static_cast<unsigned long> ( (i * width) + j)].b = blue;
            }
            else if (flag){
                bmpImg->pixels[static_cast<unsigned long> ( (i * width) + j)].r = red_flood;
                bmpImg->pixels[static_cast<unsigned long> ( (i * width) + j)].g = green_flood;
                bmpImg->pixels[static_cast<unsigned long> ( (i * width) + j)].b = blue_flood;
            }
        }
    }
    return 0;
}

bool ImgEdit::colorPoint(int x, int y, QColor color)
{
    int width = static_cast<int> (bmpImg->get_width());
    int hight = static_cast<int> (bmpImg->get_hight());
    y = hight - y;
    if (x > 0 && y > 0 && x < width && y < hight)
    {
        bmpImg->pixels[static_cast<unsigned long>(y * width + x)].r = static_cast<unsigned char>(color.red());
        bmpImg->pixels[static_cast<unsigned long>(y * width + x)].g = static_cast<unsigned char>(color.green());
        bmpImg->pixels[static_cast<unsigned long>(y * width + x)].b = static_cast<unsigned char>(color.blue());
        return false;
    }
    return true;
}

bool ImgEdit::changeFillColor(int x, int y, QColor oldColor, QColor newColor)
{
    int spanLeft = 0;
    int spanRight = 0;
    int y1max = y;
    int width = static_cast<int> (bmpImg->get_width());
    int hight = static_cast<int> (bmpImg->get_hight());
    while (y1max < hight && checkColor(x, y1max, oldColor))
    {
        colorPoint(x, y1max, newColor);
        y1max++;
    }
    y1max--;
    int y1 = y - 1;
    while (y1 > 0 && checkColor(x, y1, oldColor))
    {
        colorPoint(x, y1, newColor);
        y1--;
    }
    y1++;
    while(y1 <= y1max)
    {

        if (spanLeft == 0 && x > 0 && checkColor(x - 1, y1, oldColor))
        {
            changeFillColor(x - 1, y1, oldColor, newColor);
            spanLeft = 1;
        }
        else if (spanLeft == 1 && x > 0 && !checkColor(x - 1, y1, oldColor))
        {
            spanLeft = 0;
        }
        if (spanRight == 0 && x < width && checkColor(x + 1, y1, oldColor))
        {
            changeFillColor(x + 1, y1, oldColor, newColor);
            spanRight = 1;
        }
        else if (spanRight == 1 && x < width && !checkColor(x + 1, y1, oldColor))
        {
            spanRight = 0;
        }
        y1 += 1;
    }
    return 1;
}

int ImgEdit::rect_find_refill(QColor colToCh, QColor colOnWhCh)
{
    Q_UNUSED(colOnWhCh)
    int square;
    int num = 0;
    int width = static_cast<int> (bmpImg->get_width());
    int hight = static_cast<int> (bmpImg->get_hight());
    colorMap = new int*[width];
    rect *rectArr = new rect[hight];

    for (int i = 0; i < width; i++)
    {
        colorMap[i] = new int[hight];
        for (int j = 0; j < hight; j++)
            colorMap[i][j] = 0;
    }
    for (int x = 0; x < width; x++)
    {
        for(int y = 0; y < hight; y++)
        {
            if (checkColor(x, y, colToCh))
            {
                colorMap[x][y] = 1;
            }
        }
    }

    for (int y = 0; y < hight; y++)
    {
        for(int x = 0; x < width; x++)
        {
            if (colorMap[x][y] == 1)
            {
                square = checkRect(x, y, colToCh);
                if (square > 0)
                {
                    rectArr[num].x = x;
                    rectArr[num].y = y;
                    rectArr[num].square = square;
                    num++;
                }
            }
        }
    }

    int index = 0;
    int max = rectArr[index].square;
    for(int i = 1; i < num; i++)
    {
        if (rectArr[i].square > max)
        {
            max = rectArr[i].square;
            index = i;
        }
    }
    if (num)
    {
        //FIX: It's possible to call this func not from mainwindow
        //changeFillColor(rectArr[index].x, rectArr[index].y, colToCh, colOnWhCh);
        x_rect = rectArr[index].x;
        y_rect = rectArr[index].y;
    }

    return 0;
}
///////////////////////////////////////////////////////////////////
//                                                               //
//                            EXTRA                              //
//                                                               //
///////////////////////////////////////////////////////////////////

int ImgEdit::BinCopy(void)
{
    DWORD width  = bmpImg->width();
    DWORD height = bmpImg->height();
    std::vector<BGR> pixels;

    std::string output_image_file = "../ProgramFiles/output.bmp";

    BMPimage copImg(output_image_file);

    pixels = bmpImg->pixels;
    copImg.set_width(width);
    copImg.set_height(height);
    copImg.set_pixels(pixels);

    copImg.CheckBackVal(copImg.CreateImage());

    copImg.CheckBackVal(copImg.WriteImageHeader());

    copImg.CheckBackVal(copImg.WriteImagePixels());

    return 0;
}

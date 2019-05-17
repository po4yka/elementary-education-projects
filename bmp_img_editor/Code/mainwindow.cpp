#include "universalqtheader.h"
#include "fileinfo.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QImage img;
    img.load(":/back/background.png");
    imgDisplayLabel = new QLabel() ;
    imgDisplayLabel->setPixmap(QPixmap::fromImage(img));
    imgDisplayLabel->adjustSize();
    ui->gridLayout->addWidget(imgDisplayLabel);

    picture = new CustomGraphicsView();

    connect(picture, SIGNAL(selection()), this, SLOT(selection()));
    connect(picture, SIGNAL(click_coords()), this, SLOT(click_coords()));


    _label = new QLabel;
    _label->setAlignment(Qt::AlignHCenter);
    _label->setMinimumSize(_label->sizeHint());
    statusBar()->addWidget(_label);
    _label->setText("Ready!");


    statusBar()->addPermanentWidget(picture->mouseTracker);

    flag = 0;
    flag_fill = 0;
    button_pressed = 0;
    imgEdit = new ImgEdit;

    information = new fileinfo;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_butOpen_clicked()
{
    QString imagePath = "/";
    QString imgName = QFileDialog::getOpenFileName(this,
    tr("Open BMP File"),  QDir::homePath(),
    tr("BMP picture (*.bmp);;All Files (*)"));
    if (imgName == nullptr)
    {
        QMessageBox::warning(this,
        "Warning!",
        "Please choose image for work.");
        return;
    }

    imagePath = QFileInfo(imgName).path();
    imageObject = new QImage();
    imageObject->load(imgName);


    QString resPath = QDir::currentPath();
    resPath.chop(resPath.length() - resPath.lastIndexOf(QChar('/')));
    resPath += "/ProgramFiles";
    if (!QDir(resPath).exists()) {
        QDir().mkdir(resPath);
    }
    resPath += "/input.bmp";
    if (QFile::exists(resPath))
    {
        QFile::remove(resPath);
    }
    QFile::copy(imgName, resPath);


    ui->gridLayout->removeWidget(imgDisplayLabel);
    if(!ui->gridLayout->isEmpty())
    {
        while (QLayoutItem* item = ui->gridLayout->takeAt(0)) {
            delete item->widget();
            delete item;
        }
        imgEdit->destr_bmp();
        delete imgEdit;
        imgEdit = new ImgEdit;
        picture = new CustomGraphicsView;
        connect(picture, SIGNAL(selection()), this, SLOT(selection()));
    }
    if (imgEdit->open_flag == true)
        picture->clear();
    imgEdit->InitImg();
    picture->update(imgEdit->get_pixmap());
    picture->adjustSize();
    ui->gridLayout->addWidget(picture);
    imgEdit->open_flag = true;
    _label->setText("Image was open");

    information->info.name = QFileInfo(imgName).baseName();
    information->info.size = imgEdit->get_size();
    information->info.hight = imgEdit->get_hight();
    information->info.width = imgEdit->get_width();
    information->info.is_readable = QFileInfo(imagePath).isReadable();
    information->info.is_writeable = QFileInfo(imagePath).isWritable();
    information->info.version = imgEdit->get_version();
}

void MainWindow::on_butSave_clicked()
{
    if (!imgEdit->check_corr())
    {
        QMessageBox::critical(this, "Error", "It's impossible to save what isn't");
        return;
    }
    // получаем путь сохранения файла
    QString imagePath = QFileDialog::getSaveFileName(this,
    tr("Save File"), "",
    tr("BMP picture (*.bmp)"));
    if (imagePath == nullptr)
    {
        QMessageBox::warning(this,
        "Warning!",
        "Please choose way to save.");
        return;
    }
    std::string path = imagePath.toStdString().c_str();
    if(imgEdit->saveImg(path))
    {
        QMessageBox::critical(this, "Error", "Wrong saving.");
        return;
    }
    imgEdit->saveImg(path);
    _label->setText("Image was saved");
}

void MainWindow::selection(){
    if (button_pressed)
        {
        imgEdit->coordinates.x_1 = picture->coordinates.x_1;
        imgEdit->coordinates.x_2 = picture->coordinates.x_2;
        imgEdit->coordinates.y_1 = picture->coordinates.y_1;
        imgEdit->coordinates.y_2 = picture->coordinates.y_2;

        if (imgEdit->coordinates.x_1 < 0 || imgEdit->coordinates.x_2 < 0 || imgEdit->coordinates.y_1 < 0 || imgEdit->coordinates.y_2 < 0 ||
                imgEdit->coordinates.x_1 > (imgEdit->get_width() - 1) || imgEdit->coordinates.x_2 > (imgEdit->get_width() - 1) ||
                imgEdit->coordinates.y_1 > (imgEdit->get_hight() - 1) || imgEdit->coordinates.y_2 > (imgEdit->get_hight() - 1)){
            QMessageBox::warning(this, "Error", "You have selected an area outside the picture. Please do it right.");
            button_pressed = 0;
            return;
        }

        if (imgEdit->coordinates.x_1 == imgEdit->coordinates.x_2 || imgEdit->coordinates.y_1 == imgEdit->coordinates.y_2)
        {
            QMessageBox::information(this, "Think about it", "You have selected area which will not change after operation.");
            return;
        }
    }

    if (button_pressed == REFLECTION)
    {
        if (imgEdit->coordinates.y_2 < imgEdit->coordinates.y_1){
            std::swap(imgEdit->coordinates.y_1, imgEdit->coordinates.y_2);
        }
        if (imgEdit->coordinates.x_1 < imgEdit->coordinates.x_2){
            std::swap(imgEdit->coordinates.x_1, imgEdit->coordinates.x_2);
        }
        imgEdit->reflection(flag);
        picture->update(imgEdit->get_pixmap());
        button_pressed = 0;
        _label->setText("Image was reflected");
    }
    else if (button_pressed == COPY_ZONE)
    {
        _label->setText("Choose distance point");
        picture->button_pressed = COPY_POINT;
        button_pressed = COPY_POINT;
    }
    else if (button_pressed == RECT)
    {
        if (imgEdit->coordinates.y_2 < imgEdit->coordinates.y_1){
            std::swap(imgEdit->coordinates.y_1, imgEdit->coordinates.y_2);
        }
        if (imgEdit->coordinates.x_1 < imgEdit->coordinates.x_2){
            std::swap(imgEdit->coordinates.x_1, imgEdit->coordinates.x_2);
        }
        imgEdit->draw_rect(imgEdit->coordinates.x_1, imgEdit->coordinates.y_1, imgEdit->coordinates.x_2, imgEdit->coordinates.y_2,
                           ui->rectLineThick->value(), color_of_rect, color_of_fill_rect, flag);
        picture->update(imgEdit->get_pixmap());
        button_pressed = 0;
        _label->setText("Rectangle was drown");
    }
}

void MainWindow::click_coords()
{
    if (button_pressed == COPY_POINT)
    {
        imgEdit->x_dist = picture->x_dest;
        imgEdit->y_dist = picture->y_dest;

        if (imgEdit->copy_zone())
        {
            QMessageBox::critical(this, "Error", "You chose point of distance out of bounds");
            button_pressed = 0;
            picture->button_pressed = 0;
            return;
        }
        picture->update(imgEdit->get_pixmap());
        button_pressed = 0;
        picture->button_pressed = 0;
        _label->setText("Zone was copied");
    }
}

void MainWindow::on_butRefl_clicked()
{
    flag = -1;
    if (ui->horAx->isChecked())
        flag = 0;
    if (ui->verAx->isChecked())
        flag = 1;
    if (!imgEdit->check_corr())
    {
        QMessageBox::critical(nullptr, "Critical", "It's impossible to make a reflection of the void.");
        return;
    }
    if (flag == -1)
    {
        QMessageBox::critical(nullptr, "Critical", "Please choose a display method");
        return;
    }
    picture->button_pressed = REFLECTION;
    button_pressed = REFLECTION;

    _label->setText("Please choose working zone");
}

void MainWindow::on_butCollage_clicked()
{
    if (!imgEdit->check_corr())
    {
        QMessageBox::critical(nullptr, "Critical", "It's impossible to make collage of the void.");
        return;
    }

    imgEdit->compression(ui->repXcount->value(), ui->repYcount->value());
    picture->update(imgEdit->get_pixmap());
    ui->gridLayout->addWidget(picture);
    _label->setText("Collage was made");
}

void MainWindow::on_butSplit_clicked()
{
    if (!imgEdit->check_corr())
    {
        QMessageBox::critical(nullptr, "Critical", "It's impossible to split the void.");
        return;
    }
    color_split_line = QColorDialog::getColor(Qt::white, nullptr, "Choose color of splitting line");
    if (color_split_line == nullptr)
        return;
    thick_line_split = ui->lineThickCount->value();

    imgEdit->split(ui->numPartXCount->value(),
    ui->numPartYCount->value(),
    ui->lineThickCount->value(), color_split_line);

    picture->update(imgEdit->get_pixmap());
    ui->gridLayout->addWidget(picture);
    _label->setText("Image was splited");
}

void MainWindow::on_butCopy_clicked()
{
    if (!imgEdit->check_corr())
    {
        QMessageBox::critical(nullptr, "Critical", "It's impossible to copy the void.");
        return;
    }
    picture->button_pressed = COPY_ZONE;
    button_pressed = COPY_ZONE;
    QMessageBox::information(this,
    "Ingormation",
    "Please choose zone to copy and after that make double click to point there to past");

    _label->setText("Please choose copying zone");
}

void MainWindow::on_butChange_clicked()
{
    if (!imgEdit->check_corr())
    {
        QMessageBox::critical(nullptr, "Critical", "It's impossible to split the void.");
        return;
    }
    color_to_change = QColorDialog::getColor(Qt::white, nullptr, "Choose color which you want to change");
    color_on_which_change = QColorDialog::getColor(Qt::white, nullptr, "Choose color on which you want to change");
    imgEdit->change_color(color_to_change, color_on_which_change);
    picture->update(imgEdit->get_pixmap());
    ui->gridLayout->addWidget(picture);
    _label->setText("Color of pixels was changed");
}

void MainWindow::on_butBlackAndWhite_clicked()
{
    if (!imgEdit->check_corr())
    {
        QMessageBox::critical(nullptr, "Critical", "It's impossible to make black and white the void.");
        return;
    }
    imgEdit->make_black_and_white();
    picture->update(imgEdit->get_pixmap());
    ui->gridLayout->addWidget(picture);
    _label->setText("Image was made black and white");
}

void MainWindow::on_butRotate_clicked()
{
    if (!imgEdit->check_corr())
    {
        QMessageBox::critical(nullptr, "Critical", "It's impossible to rotate the void.");
        return;
    }
    imgEdit->rotate(ui->degreeCount->value());
    picture->update(imgEdit->get_pixmap());
    _label->setText("Image was rotated");
}

void MainWindow::on_butRect_clicked()
{
    if (!imgEdit->check_corr())
    {
        QMessageBox::critical(nullptr, "Critical", "It's impossible to draw on the void.");
        return;
    }
    if (ui->checkRectFill->isChecked()){
        flag = 1;
    }
    else {
        flag = 0;
    }
    color_of_rect = QColorDialog::getColor(Qt::white, nullptr, "Choose color of rectangle");
    if (color_of_rect == nullptr){
        return;
    }
    color_of_fill_rect = nullptr;
    if (flag){
        color_of_fill_rect = QColorDialog::getColor(Qt::white, nullptr, "Choose color of rectangle fill color");
        if (color_of_fill_rect == nullptr){
            return;
        }
    }
    button_pressed = RECT;
    picture->button_pressed = RECT;

    _label->setText("Please choose working zone");
}

void MainWindow::on_butFill_clicked()
{
    if (!imgEdit->check_corr())
    {
        QMessageBox::critical(nullptr, "Critical", "It's impossible to find anything in the void.");
        return;
    }
    color_to_change = QColorDialog::getColor(Qt::white, nullptr, "Choose color which you want to change");
    color_on_which_change = QColorDialog::getColor(Qt::white, nullptr, "Choose color on which you want to change");
    imgEdit->rect_find_refill(color_to_change, color_to_change);
    imgEdit->changeFillColor(imgEdit->x_rect, imgEdit->y_rect, color_to_change, color_on_which_change);
    picture->update(imgEdit->get_pixmap());
    ui->gridLayout->addWidget(picture);
    _label->setText("Rectangle was filled");
}
///////////////////////////////////////////////////////////////////
//                                                               //
//                           EXTRA                               //
//                                                               //
///////////////////////////////////////////////////////////////////

void MainWindow::on_actionBinCopy_triggered()
{
    imgEdit->BinCopy();
}

///////////////////////////////////////////////////////////////////
//                                                               //
//                    HELP AND REFERENCE                         //
//                                                               //
///////////////////////////////////////////////////////////////////

void MainWindow::on_actionFileInf_triggered()
{
    if (!imgEdit->check_corr())
    {
        QMessageBox::critical(nullptr, "Critical", "It's impossible to say anything the void.");
        return;
    }
    information->setModal(true);
    information->write_information();
    information->exec();
}

void MainWindow::on_actionQt_triggered()
{
    QMessageBox::aboutQt(this, "Информация о Qt");
}

void MainWindow::on_actionAuthor_triggered()
{
    QMessageBox::information(nullptr, "Author", "Program was developed by:\nPochaev Nikita Alekseevich\nGroup: 8381");
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

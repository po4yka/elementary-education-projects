#include "universalqtheader.h"

CustomGraphicsView::CustomGraphicsView(QWidget* parent) : QGraphicsView (parent)
{
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    group = new QGraphicsItemGroup();
    mouse_pressed = false;

    mouseTracker = new QLabel(this);
    this->setMouseTracking(true);
    mouseTracker->setAlignment(Qt::AlignLeft);
    setMouseTracking(true);
}

void CustomGraphicsView::update(QPixmap pixmap)
{
    scene = new QGraphicsScene();
    scene->addPixmap(pixmap);
    this->setScene(scene);
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (button_pressed != COPY_POINT)
    {
        QPoint position = mapToScene(event->pos()).toPoint();
        coordinates.x_1 = position.x();
        coordinates.y_1 = position.y();
        mouse_pressed = true;
    }
}

void CustomGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (button_pressed == COPY_POINT)
    {
        QPoint position = mapToScene(event->pos()).toPoint();
        x_dest = position.x();
        y_dest = position.y();
        mouse_pressed = true;
        emit click_coords();
    }
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event){
    if (button_pressed != COPY_POINT)
    {
        QPoint position = mapToScene(event->pos()).toPoint();
        coordinates.x_2 = position.x();
        coordinates.y_2 = position.y();
        mouse_pressed = false;
        button_pressed = 0;
        this->deleteItemsFromGroup(group);
        emit selection();
    }
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    if (button_pressed)
    {
        if (button_pressed != COPY_POINT)
        {
            QPoint position = mapToScene(event->pos()).toPoint();
            int x = position.x();
            int y = position.y();
            this->deleteItemsFromGroup(group);
            group = new QGraphicsItemGroup();
            QPen penBlue(Qt::blue);
            if (x < 0) { x = 0; }
            if (y < 0) { y = 0; }
            int x1 = coordinates.x_1;
            int y1 = coordinates.y_1;
            if (mouse_pressed && (button_pressed == REFLECTION || button_pressed == COPY_ZONE || button_pressed == RECT))
            {
                group->addToGroup(scene->addLine(x1, y1, x, y1, penBlue));
                group->addToGroup(scene->addLine(x, y1, x, y, penBlue));
                group->addToGroup(scene->addLine(x, y, x1, y, penBlue));
                group->addToGroup(scene->addLine(x1, y, x1, y1, penBlue));
                scene->addItem(group);
            }
        }
    }
    else
    {
        mouseTracker->setText(QString("X = %1, Y = %2").arg(event->x()).arg(event->y()));
    }
}

void CustomGraphicsView::deleteItemsFromGroup(QGraphicsItemGroup *group)
{
    foreach(QGraphicsItem *item, scene->items(group->boundingRect())) {
       if(item->group() == group ) {
          delete item;
       }
    }
}

void CustomGraphicsView::clear()
{
    deleteItemsFromGroup(group);
}

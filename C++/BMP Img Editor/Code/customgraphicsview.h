#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QPointF>
#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsItemGroup>
#include <QTimer>
#include <QLabel>

#include "imgedit.h"

class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CustomGraphicsView(QWidget *parent = nullptr);
    void update(QPixmap pixmap);
    void deleteItemsFromGroup(QGraphicsItemGroup *group);
    void clear();
    coordPos coordinates;
    int x_dest = 0;
    int y_dest = 0;
    QGraphicsScene* scene;
    int button_pressed = 0;
    QLabel *mouseTracker;  // for coords

signals:
    void selection();
    void click_coords();

private slots:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

private:
    QPixmap pixmap;
    QGraphicsItemGroup* group;
    bool mouse_pressed;
};

#endif // CUSTOMGRAPHICSVIEW_H

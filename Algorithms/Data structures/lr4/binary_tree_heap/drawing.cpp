#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "basicheaders.h"
#include "customvector.h"
#include "tree.h"

void MainWindow::setUpDrawingTools(QColor &color) {
    qDebug() << "setUpDrawingTools()" << endl;
    mainGraphicsView->setScene(mainGraphicsScene);

    pen.setColor(color);
    brush.setColor(color);
    font.setFamily("Roboto");
    pen.setWidth(3);
}

void MainWindow::mainGraph() {
    mainGraphicsScene->clear();
    qDebug() << "mainGraph();" << endl;
    if (this->_root == nullptr)
        return;

    QColor color;
    color.setRgb(0, 189, 213);
    setUpDrawingTools(color);

    lrstruct::Vector<lrstruct::Data> nodes_data = _root->setPositionOfNodesForVisualization();
    const int size = _root->DrawingSize / 2;
    QPoint line_pos;
    line_pos.setX(_root->start_pos.x() + size);
    line_pos.setY(_root->start_pos.y() + size);
    lrstruct::Vector<QPoint> nodes_lines = _root->getVectorOfLinesBetwenNodes();
    // drawing lines for nodes
    for(const auto& a : nodes_lines) {
        mainGraphicsScene->addLine(line_pos.x(), line_pos.y(), a.x() + size, a.y() + size, pen);
        line_pos.setX(a.x() + size);
        line_pos.setY(a.y() + size);
    }

    for(int i = 0; i < static_cast<int>(nodes_data.size()); i++) {
        QPoint node_pos(nodes_data[i].posX, nodes_data[i].posY);

        QColor color;
        color.setRgb(0, 229, 247);
        QBrush brush(color);

        if(nodes_data[i].new_node == false) {
            mainGraphicsScene->addEllipse(nodes_data[i].posX, nodes_data[i].posY,_root->DrawingSize, \
                                          _root->DrawingSize, pen, brush);
        } else {
            color.setRgb(255, 0, 0);
            QBrush brush(color);
            mainGraphicsScene->addEllipse(nodes_data[i].posX, nodes_data[i].posY, \
                                          _root->DrawingSize, _root->DrawingSize, pen, brush);
        }

        QGraphicsTextItem *numb = new QGraphicsTextItem();
        numb->setPlainText(QString::number(nodes_data[i].val));
        numb->setDefaultTextColor(Qt::black);
        numb->setPos(node_pos);
        mainGraphicsScene->addItem(numb);
    }
}

/**
 * @brief MainWindow::highlightCurrNode
 * @param currNode
 * @param mode
 * 1 - current working node,
 * 2 - node from pop by stack,
 * 3 - test false
 */
void MainWindow::highlightCurrNode(lrstruct::Node* currNode, int mode) {
    qDebug() << "highlightCurrNode()" << endl;
    QColor colorHighlight;
    switch(mode) {
        case 1:
            colorHighlight.setRgb(254, 86, 145);
            pen.setWidth(4);
            break;
        case 2:
            colorHighlight.setRgb(116, 217, 186);
            pen.setWidth(4);
            break;
        case 3:
            colorHighlight.setRgb(255, 0, 0);
            pen.setWidth(5);
            break;
    }
    pen.setColor(colorHighlight);
    mainGraphicsScene->addEllipse(currNode->_data.posX, currNode->_data.posY, \
                                  currNode->DrawingSize, currNode->DrawingSize, pen);

}

void MainWindow::onRunButtonClicked() {
    qDebug() << "onRunButtonClicked();" << endl;
    isBstTree->setText("-");
    isBinHeap->setText("-");
    mainGraphicsScene->clear();
    std::string readingStr;
    QString tempInp = inputLine->text();
    QTextStream stream(&tempInp);
    foreach (QString i ,QString(stream.readAll()).split(QRegExp("[ \t]"), \
                                                        QString::SkipEmptyParts))
        readingStr.append(i.toUtf8().constData());

    qDebug() << "String was read: " << QString::fromStdString(readingStr) << endl;

    if(readingStr.empty())
        return;

    int check = lrstruct::Node::convertBracketRepresentationToBinTree(readingStr, _root);
    if(check) {
        inputLine->setText("Invalid string tree!");
        return;
    }

    // draw tree
    mainGraph();

    connect(nextStep, SIGNAL(clicked()), _root, SLOT(changeLoopStepSwitcherState()));
    connect(_root, SIGNAL(drawCurrNode(lrstruct::Node*, int)), this, SLOT(highlightCurrNode(lrstruct::Node*, int)));

    maxNode->setText(QString::number(_root->findMaxNode(_root)));
    minNode->setText(QString::number(_root->findMinNode(_root)));

    if(stepImplSwitch->isChecked() == true) {
        _root->setStepByStepFlag();

        if(checkBst->isChecked()) {
            if(lrstruct::Node::isValidBst(_root) == true)
                isBstTree->setText("yes");
            else
                isBstTree->setText("no");
        } else if (checkBinHeap->isChecked()) {
            if(lrstruct::Node::isValidBinHeap(_root) == true)
                isBinHeap->setText("yes");
            else
                isBinHeap->setText("no");
        }
    } else {
        if(lrstruct::Node::isValidBst(_root) == true)
            isBstTree->setText("yes");
        else
            isBstTree->setText("no");
        if(lrstruct::Node::isValidBinHeap(_root) == true)
            isBinHeap->setText("yes");
        else
            isBinHeap->setText("no");
    }
}

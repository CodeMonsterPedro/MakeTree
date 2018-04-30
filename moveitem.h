#ifndef MOVEITEM_H
#define MOVEITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>
#include <QMessageBox>
#include <QLabel>



class MoveItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    QVector<MoveItem*> neighbors;// массив точек присоединенных к этой точке
    bool burn=false;//была ли посещена точка
    int waytohere=0;
    int number;// номер точки
    int score=3000;//очки точки
    QVector<int> time;// веса связей точки точки
    QVector<int> scoreway;// путь по которму нужно пройти от точки начала до данной чтобы получить ее вес
    QLabel numlabel;
    explicit MoveItem(QObject *parent = 0);
    ~MoveItem();
void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);// функция ответа на двойной клик по точке

 signals:
    void SendPos(QPointF cords);// отправка координат точки
    void SetNum(int num,QVector<int> timer);// отправка номера и веса точки
public slots:


private:
    QRectF boundingRect() const;// зона захвата точки
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);// отрисовка точки
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);// реакция программы на передвижение точки
    void mousePressEvent(QGraphicsSceneMouseEvent *event);// реакция программы на захват точки
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);// реакция программы на отпуск точки


};

#endif // MOVEITEM_H

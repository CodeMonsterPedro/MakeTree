#include "moveitem.h"
#include "widget.h"

MoveItem::MoveItem(QObject *parent) :
    QObject(parent), QGraphicsItem()
{
    //this->setCursor(QCursor(QT::));
    //temparray=LineVec;
    //connect(this, SIGNAL(SendPos(QPointF)),SLOT(Widget::TakePos(QPointF)));
}

MoveItem::~MoveItem()
{

}

QRectF MoveItem::boundingRect() const
{
    return QRectF (-30,-30,20,20);// ограничение области за которую можно схватить объект
}

void MoveItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);// черные границы точек
    painter->setBrush(Qt::green);// зеленая заливка точки
    painter->drawEllipse(-30,-30,20,20);// отрисовка самой точки
    Q_UNUSED(option);// освобождение памяти не используемых переменных
    Q_UNUSED(widget);// освобождение памяти не используемых переменных
}

void MoveItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit SendPos(this->pos());// отправка координат точки
     emit SetNum(number,time);// отправка номера и веса точки
Q_UNUSED(event);// освобождение памяти не используемых переменных
}

void MoveItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    /* Устанавливаем позицию графического элемента
     * в графической сцене, транслировав координаты
     * курсора внутри графического элемента
     * в координатную систему графической сцены
     * */
     emit SetNum(number,time);// отправка номера и веса точки
    this->setPos(mapToScene(event->pos().x()+20,event->pos().y()+20));// установка координат точки на месте курсора

}

void MoveItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    /* При нажатии мышью на графический элемент
     * заменяем курсор на руку, которая держит этот элемента
     * */
    this->setCursor(QCursor(Qt::ClosedHandCursor));// изменение внешенго вида курсора
     emit SetNum(number,time);// отправка номера и веса точки
    Q_UNUSED(event);// освобождение памяти не используемых переменных
}

void MoveItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    /* При отпускании мышью элемента
     * заменяем на обычный курсор стрелку
     * */
    this->setCursor(QCursor(Qt::ArrowCursor));// изменение внешенго вида курсора
     emit SetNum(number,time);// отправка номера и веса точки
    Q_UNUSED(event);// освобождение памяти не используемых переменных
}







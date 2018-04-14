#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QPoint>
#include <QVector>


#include <moveitem.h>

namespace Ui {
class Widget;
}

class Widget : public QMainWindow
{
    Q_OBJECT
signals:


public slots:
    void TakeToSetNum(int num,QVector<int> time);// получение номера и веса точки для вывода
    void TakePos(QPointF cords);// получение координат выбраной точки
public:
    explicit Widget(QMainWindow *parent = 0);
    ~Widget();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
    QGraphicsScene *scene;
    QVector<QPointF> LineVec;// масссив для рисования связей
    QVector<MoveItem*> points;// главный массив точек
    QVector<int> visited;// массив посещенных элементов
    int mainA=0,mainB=0;// Номера выбранных точек дл япросчета пути
    bool Check(int next);
    void BaseAlgo();
    void NextStep(int current);
    void ShowTheWay();



};

#endif // WIDGET_H
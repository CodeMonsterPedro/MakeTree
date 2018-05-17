#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QLineF>
#include <QStatusBar>
#include <QGraphicsItemGroup>

/* Функция для получения рандомного числа
 * в диапазоне от минимального до максимального
 * */
static int randomBetween(int low, int high)
{
    return (qrand() % ((high + 1) - low) + low);
}

Widget::Widget(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //connect(this,SLOT(TakePos(QPointF)), SIGNAL(SendPos(QPointF)));
    // Косметическая подготовка приложения
    this->resize(1000,1000);          // Устанавливаем размеры окна приложения
    this->setFixedSize(1000,1000);

    scene = new QGraphicsScene(this);   // Инициализируем графическую сцену
    scene->setItemIndexMethod(QGraphicsScene::NoIndex); // настраиваем индексацию элементов

    ui->graphicsView->resize(980,940);  // Устанавливаем размер graphicsView
    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену в graphicsView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);    // Настраиваем рендер
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    scene->setSceneRect(0,0,800,800); // Устанавливаем размер сцены
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    ui->pushButton_3->setEnabled(false);
    scene->clear(); //очистка сцены
    ui->pushButton_2->setEnabled(true);
    for(int i=1;i<ui->spinBox->value()+1;i++)// цыкл создания точек по количеству в сцетчике
    {
    MoveItem *item = new MoveItem();        // Создаём графический элемент
    item->setPos(randomBetween(30, 770),    // Устанавливаем случайную позицию элемента
                 randomBetween(30, 770));
    connect(item, SIGNAL(SendPos(QPointF)),this,SLOT(TakePos(QPointF)));// подключение функции получения позиции точки
    connect(item, SIGNAL(SetNum(int,QVector<int>)),this,SLOT(TakeToSetNum(int,QVector<int>)));// подключение функции получения номера точки
    points.push_back(item);// добавление точки в массив точек
    item->number=i;// присвоение номера точке
    scene->addItem(item);   // Добавляем элемент на графическую сцену
    }


}

void Widget::TakePos(QPointF cords)// функция рисования связей между точками
{
    if(LineVec.isEmpty())LineVec.push_back(cords);// если массив пуст то выбранна первая точка
    else
    {
        int first=0,second=0;//индексы точек которые будут объеденены
        for(int i=0;i<points.length();i++)// цыкл поиска нужных точек
        {
            if(points[i]->pos()==LineVec[0])first=i;// поиск первой точки
            if(points[i]->pos()==cords)second=i;// поиск второй точки
        }
        int randnumber=qrand() % ((20 + 1) - 1) + 1;
        points[first]->neighbors.append(points[second]);// добавление в соседи вторую точку
        points[first]->time.append(randnumber);// присвоения веса связи
        points[second]->neighbors.append(points[first]);// добавление в соседи первую точку
        points[second]->time.append(randnumber);// присвоения веса связи
        QLineF pointer({LineVec[0].x()-20,LineVec[0].y()-20},{cords.x()-20,cords.y()-20});// ресуется связь между ранее выбраной и новой точкой
        scene->addLine(pointer);// добавление связи на сцену
        LineVec.clear();// очистка массива

    }
}

void Widget::TakeToSetNum(int num,QVector<int> time)// функция отображения номера и веса точки
{
    QString temp="";
    temp.setNum(num);temp .append(";");
    while(!time.isEmpty()){temp.append(QString::number(time.takeFirst()));temp .append("-");}// формирование строки
    statusBar()->showMessage(temp);// вывод информации в панели статуса
}


void Widget::on_pushButton_2_clicked()// функция проверки перед началом главного алгоритма
{
    ui->pushButton_3->setEnabled(true);
    if(ui->spinBox_2->value()==ui->spinBox_3->value())// проверка не указал ли пользователь путь в одну и ту же точку
    {
        QMessageBox msg;
        msg.setText("Нелья положить путь из точки в туже точку");// формирование сообщения для пользователя
        msg.exec();// вывод сообщения
        return;
    }
    if((points[ui->spinBox_2->value()-1]->neighbors.isEmpty()||points[ui->spinBox_3->value()-1]->neighbors.isEmpty())&&!Check(ui->spinBox_2->value()))// проверка не указал ли пользователь путь в одну и ту же точку
    {
        QMessageBox msg;
        msg.setText("Отсутствует связь между точками");// формирование сообщения для пользователя
        msg.exec();// вывод сообщения
        return;
    }
    BaseAlgo();

}

bool Widget::Check(int next)// рекурсивная функция проверки существования связей между точками
{
    for(int i=0;i<points.size();i++)// цыкл для нахождения индекса точки в массиве точек
    {
        if(points[i]->number==next)next=i;
    }

    if(points[next]->neighbors.length()==1&&points[next]->number==ui->spinBox_3->value())return true;// тут что-то не так!!!!
    if(points[next]->neighbors.length()>1)// если соседей больше одного то мы посещяем точку
    {
        points[next]->burn=true;// отметка что точка была посещена
        for(int i=0;i<points[next]->neighbors.size();i++)// цыкл проверки соседей текущей точки на номер окончательной точки
        {
            if(points[next]->neighbors[i]->number==ui->spinBox_3->value())return true;
        }
        for(int i=0;i<points[next]->neighbors.size();i++)// цыкл вызова функций проверки для соседей которые не были посещены
        {
            if(!points[next]->neighbors[i]->burn)Check(points[next]->neighbors[i]->number);
        }

    }
    return false;// при условии что не был найден нужный номер фозвращяется ложное значение
}


void Widget::BaseAlgo()// основная функция алгоритма
{
    QMessageBox msg;
    msg.setText("Прокладываем путь из точки " + QString::number(ui->spinBox_2->value()) + " в точку " + QString::number(ui->spinBox_3->value()));// формирование сообщения для пользователя
    msg.exec();// сообщение пользователю о том что начинается проложение маршрута
    mainA=ui->spinBox_2->value();
    mainB=ui->spinBox_3->value();
    points[mainA-1]->score=0;
    points[mainA-1]->scoreway.append(mainA);
    for(int i=0;i<points.size();i++)// обнуление данных о посещении точек
    {
        points[i]->burn=false;
    }
    NextStep(mainA);// вызов функции расчета цен и пути к точкам
    for(int i=0;i<points.size();i++)// обнуление данных о посещениях
    {
        points[i]->burn=false;
    }
    ShowTheWay();// вывод информации о завершении алгоритма
}

void Widget::NextStep(int current)
{
    for(int i=0;i<points.size();i++)// цыкл нахождения индекса точки в массиве
    {
        if(points[i]->number==current)current=i;
    }
    points[current]->burn=true;// отметка посещения точки
    for(int i=0;i<points[current]->neighbors.size();i++)// цыкл проверки соседей
    {
        if(points[current]->neighbors[i]->score>=(points[current]->time[i]+points[current]->score))
            // если очки на соседе больше чем сумма очков текущей точки и связи с соседом то очкам соседа присваивается сумма связи и очков точки
        {

            points[current]->neighbors[i]->score=(points[current]->time[i]+points[current]->score);
            points[current]->neighbors[i]->scoreway=points[current]->scoreway;
            points[current]->neighbors[i]->scoreway.append(points[current]->neighbors[i]->number);
        }
    }
    for(int i=0;i<points[current]->neighbors.size();i++)// цыкл вызова функций расчета очков для точек
    {
        if(!points[current]->neighbors[i]->burn&&points[current]->neighbors[i]->neighbors.size()>1)NextStep(points[current]->neighbors[i]->number);
    }

}
//Внедерение функции построения дерева
void Widget::ShowTheWay()
{
 QMessageBox msg;
 QString str="Найден путь от точки с номером "+QString::number(mainA) + " до точки с номером "+QString::number(mainB) + ".\nОптимальный путь проложен через последовательность\nточек ";
 for(int i=0;i<points[mainB-1]->scoreway.size();i++)
 {
     str= str + QString::number(points[mainB-1]->scoreway[i])+" ";
 }
 str = str + "\nПуть был проложен ценой в " + QString::number(points[mainB-1]->score);
 msg.setText(str);
 msg.exec();
}


void Widget::on_pushButton_3_clicked()
{
    for(int i=0;i<points.size();i++)if(points[i]->neighbors.size()!=0)Treepoints.append(points[i]);else points[i]->hide();
    items=scene->items();
    for(int i=0;i<items.size();i++)
        if(items[i]->type()==6)scene->removeItem(items[i]);
    //TreeCheck(mainB-1);
    //for(int i=0;i<points.size();i++)if(points[i]->waytohere>1)return;
    for(int i=0;i<Treepoints.size();i++)// цыкл нахождения индекса точки в массиве
    {
        if(Treepoints[i]->number==mainA)mainA=i;
    }
    if(Treepoints[mainA]->neighbors.size()>1)
    {
        Treepoints[mainA]->setPos(10,10);
        Treepoints[mainA]->neighbors[0]->setPos(Treepoints[mainA]->x()+20,Treepoints[mainA]->y());
        for(int i=1;i<Treepoints[mainA]->neighbors.size();i++)Treepoints[mainA]->neighbors[i]->setPos(Treepoints[mainA]->x()+20,Treepoints[mainA]->neighbors[i-1]->y()+20);
        for(int i=0;i<Treepoints[mainA]->neighbors.size();i++)
        {
            QLineF pointer(Treepoints[mainA]->pos(),Treepoints[mainA]->neighbors[i]->pos());// ресуется связь между ранее выбраной и новой точкой
            scene->addLine(pointer);
            if(Treepoints[mainA]->neighbors[i]->neighbors.size()>1)ReCreateGraf(Treepoints[mainA]->neighbors[i]->number);
        }

    }

}

void Widget::TreeCheck(int current)
{
    for(int i=0;i<Treepoints.size();i++)// цыкл нахождения индекса точки в массиве
    {
        if(Treepoints[i]->number==current)current=i;
    }
    points[current]->waytohere++;
    points[current]->burn=true;
    for(int i=0;i<points[current]->neighbors.size();i++)if(points[current]->neighbors[i]->neighbors.size()==1)points[current]->neighbors[i]->burn=true;
    for(int i=0;i<points[current]->neighbors.size();i++)if(!points[current]->neighbors[i]->burn)TreeCheck(points[current]->neighbors[i]->number);
}

void Widget::ReCreateGraf(int current)
{
    for(int i=0;i<Treepoints.size();i++)// цыкл нахождения индекса точки в массиве
    {
        if(Treepoints[i]->number==current)current=i;
    }
    Treepoints[current]->neighbors[0]->setPos(Treepoints[current]->x()+20,Treepoints[current]->y());
    for(int i=1;i<Treepoints[current]->neighbors.size();i++)Treepoints[current]->neighbors[i]->setPos(Treepoints[current]->x()+20,Treepoints[current]->neighbors[i-1]->y()+20);
    for(int i=0;i<Treepoints[current]->neighbors.size();i++)
    {
            QLineF pointer(Treepoints[current]->pos(),Treepoints[current]->neighbors[i]->pos());// ресуется связь между ранее выбраной и новой точкой
            scene->addLine(pointer);
            if(Treepoints[current]->neighbors[i]->neighbors.size()>1)ReCreateGraf(Treepoints[current]->neighbors[i]->number);

    }

}

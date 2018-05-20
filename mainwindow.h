#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//read file
#include <QMessageBox>
#include <QFileSelector>
#include <QFileDialog>

//image
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPixmap>

//container
#include <QList>
#include <QMap>
#include <QLinkedList>

//math
#include "math.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private:
    //img store
    QList<QImage> imgList;
    QMap<QString,QImage*> imgMap;
    const QImage* imgCurrent = nullptr;

    //algorithm
    typedef QImage* (MainWindow::*Algorithm)(const QImage*) ;
    Algorithm algorithms[15] ;
    QList<QCheckBox*> cboxList;
    //sobel
    int thresholding_value = 125;
    uint dilation_times = 1;
    uint erosion_times = 1;

private slots:
	void ReadImage();
	void ReadFile();

    void ShowImageByMenu(QAction* action);

    void on_btn_do_clicked();

private:
    //basic event
    void ShowMessage(QString title, QString msg);

    QImage* CreateOrGetImage(QString path);
    void ShowImage(const QImage* img);

    QImage* GetY(const QImage* img);
    QImage* Histoequailzation(const QImage* img);
    QImage* Sobel(const QImage* img);
    QImage* Dilation(const QImage* img);
    QImage* Erosion(const QImage* img);

    int getColor(const QImage* img,int w,int h);
    void setColor(QImage* img,int w,int h,int color);

};

#endif // MAINWINDOW_H

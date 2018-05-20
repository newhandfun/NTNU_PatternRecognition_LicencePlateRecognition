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

//detail
#include <QListView>
#include <QSpinBox>
#include <QLabel>

//container
#include <QList>
#include <QMap>
#include <QLinkedList>
#include <QListWidget>

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

    //filter
    typedef QImage* (MainWindow::*Filter)(const QImage*) ;
    QList<Filter> filtersList;
    QList<QString> filterNamesList;

    //filter detail
    typedef void (MainWindow::*Detail)(void);
    QList<Detail> detailList;
    //sobel
    int thresholding_value = 125;
    //dilation
    uint dilation_iter = 1;
    int dilation_size = 5;
    //erosion
    uint erosion_iter = 1;
    int erosion_size = 5;

    //filter for execute
    QList<Filter> filterStack;

private slots:
	void ReadImage();
	void ReadFile();

    void ShowImageByMenu(QAction* action);

    void on_btn_do_clicked();
    void on_actionY_Channel_triggered(bool checked);
    void on_actionHistogram_Equalization_toggled(bool arg1);
    void on_actionSobel_triggered(bool checked);

private:
    //basic event
    void ShowMessage(QString title, QString msg);
    void HideDetail();

    QImage* CreateOrGetImage(QString path);
    void ShowImage(const QImage* img);

    QImage* GetY(const QImage* img);
    QImage* Histoequailzation(const QImage* img);
    QImage* Sobel(const QImage* img);
    QImage* Dilation(const QImage* img);
    QImage* Erosion(const QImage* img);

    void GetYDetail();
    void GetHEDetail();
    void GetSobelDetail();
    void GetDilationDetail();
    void GetErosionDetail();

    int getColor(const QImage* img,int w,int h);
    void setColor(QImage* img,int w,int h,int color);

    void AddFilter(Filter method);

private slots:
    void on_threshold_valueChange(int value);
    void on_dilation_iter_valueChange(int value);
    void on_dilation_size_valueChange(int value);
    void on_erosion_iter_valueChange(int value);
    void on_erosion_size_valueChange(int value);

    void on_lwidget_algorithm_stack_itemClicked(QListWidgetItem *item);
    void on_actionDilation_triggered(bool checked);
    void on_actionErosion_triggered(bool checked);
};

#endif // MAINWINDOW_H

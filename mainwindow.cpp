#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //read img
    connect(ui->action_ReadImage,&QAction::triggered, this, &MainWindow::ReadImage);
    connect(ui->action_ReadFile,&QAction::triggered, this, &MainWindow::ReadFile);
    //display old img
    connect(ui->menuRecentImage,&QMenu::triggered,this,&MainWindow::ShowImageByMenu);

    //algorithms
    algorithms[0] = &MainWindow::GetY;
    algorithms[1] = &MainWindow::Histoequailzation;
    algorithms[2] = &MainWindow::Sobel;
    algorithms[3] = &MainWindow::Dilation;
    algorithms[4] = &MainWindow::Erosion;

    //checkbox
    cboxList.push_back(ui->cbox_YTrans);
    cboxList.push_back(ui->cbox_HE);
    cboxList.push_back(ui->cbox_sobel);
    cboxList.push_back(ui->cbox_dilation);
    cboxList.push_back(ui->cbox_erosion);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ReadImage()
{
    QString fileName =  QFileDialog::getOpenFileName(
                    this,"Open Image File",QDir::currentPath()
                );
    if(fileName.isEmpty())
        return;

    QImage* ptr_img = CreateOrGetImage(fileName);

    if(ptr_img==nullptr)
    {
        QMessageBox::information(this,"Image Viewer","Error Displaying image");
        return;
    }

    imgCurrent = ptr_img;

    ShowImage(ptr_img);

}

void MainWindow::ReadFile()
{
    QString dir_name = QFileDialog::getExistingDirectory(this,"Directory Viewer",QDir::currentPath());
    if(dir_name.isEmpty())
        return;
    QDir dir(dir_name);
    if(dir.isEmpty())
        return;
    auto files = dir.entryList();
    foreach (QString file_name, files) {
        QString name = dir.absoluteFilePath(file_name);
        CreateOrGetImage(name);
    }
}

void MainWindow::ShowMessage(QString title,QString msg)
{
    QMessageBox::information(this,title,msg);
}

QImage *MainWindow::CreateOrGetImage(const QString path)
{
    //if image has been saved in map, return it
    if(imgMap.contains(path)){
        return imgMap[path];
    }
    //if not,create a image and store it
    QImage* ptr_image = new QImage(path);
    //if it is not img, return null.
    if(ptr_image->isNull())
        return nullptr;
    imgMap[path] = ptr_image;
    //create a item of menu
    auto ptr_action = new QAction(path,this);
    ui->menuRecentImage->addAction(ptr_action);
    //return img
    return ptr_image;
}

void MainWindow::ShowImage(const QImage* img)
{
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(*img));
    QGraphicsScene* scene = new QGraphicsScene();
    scene->addItem(item);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();
}

QImage* MainWindow::GetY(const QImage* img)
{
    QImage* new_img = new QImage();
    *new_img = img->copy();
    for(int w=0;w<new_img->width();w++)
        for(int h=0;h<new_img->height();h++){
            QColor color_old = new_img->pixel(w,h);
            uint color_new = (uint)((double)color_old.red()*0.299
                              + (double)color_old.green() * 0.587
                              + (double)color_old.blue() * 0.114);
            new_img->setPixel(w,h,qRgb(color_new,color_new,color_new));
        }
    ShowImage(new_img);
    return new_img;
}

QImage *MainWindow::Histoequailzation(const QImage *img)
{
    QImage* new_img = new QImage();
    *new_img = img->copy();

    uint histogram[256] = {0};
    for(int w=0;w<img->width();w++){
        for(int h=0;h<img->height();h++){
            QColor color = img->pixel(w,h);
             histogram[color.red()]++;
        }
    }

    uint cdf[256] = {0};
    cdf[0] = histogram[0];
    for(int i=1;i<256;i++){
        cdf[i] = histogram[i];
        cdf[i] += cdf[i-1];
    }

    uint M_x_N = img->width()*img->height();
    uint cdf_min = cdf[0];
    uint MN_mi_cdf = M_x_N - cdf_min;

    for(int w=0;w<img->width();w++){
        for(int h=0;h<img->height();h++){
            QColor col = img->pixel(w,h);
            uint color = (cdf[col.red()] - cdf_min)*255/MN_mi_cdf;
            new_img->setPixel(w,h,qRgb(color,color,color));
        }
    }
    ShowImage(new_img);
    return new_img;
}

QImage *MainWindow::Sobel(const QImage *img)
{
    QImage* new_img = new QImage();
    *new_img = img->copy();

    int filter_x[3][3] = {{1,0,-1},{2,0,-2},{1,0,-1}};
    int filter_y[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};

    int width = img->width() - 2;
    int height = img->height() - 2;

    for(int w=1;w<width;w++){
        for(int h=1;h<height;h++){
            int pixel_x = (filter_x[0][0] * getColor(img,w-1,h-1)) + (filter_x[0][1] * getColor(img,w,h-1)) + (filter_x[0][2] * getColor(img,w+1,h-1)) +
                          (filter_x[1][0] * getColor(img,w-1,h))   + (filter_x[1][1] * getColor(img,w,h))   + (filter_x[1][2] * getColor(img,w+1,h)) +
                          (filter_x[2][0] * getColor(img,w-1,h+1)) + (filter_x[2][1] * getColor(img,w,h+1)) + (filter_x[2][2] * getColor(img,w+1,h+1))
            ;
            int pixel_y = (filter_y[0][0] * getColor(img,w-1,h-1)) + (filter_y[0][1] * getColor(img,w,h-1)) + (filter_y[0][2] * getColor(img,w+1,h-1)) +
                          (filter_y[1][0] * getColor(img,w-1,h))   + (filter_y[1][1] * getColor(img,w,h))   + (filter_y[1][2] * getColor(img,w+1,h)) +
                          (filter_y[2][0] * getColor(img,w-1,h+1)) + (filter_y[2][1] * getColor(img,w,h+1)) + (filter_y[2][2] * getColor(img,w+1,h+1))
            ;

            double pixel_x_dou = (double)pixel_x;
            double pixel_y_dou = (double)pixel_y;
            int color_new = (int)sqrt(pixel_x_dou*pixel_x_dou + pixel_y_dou*pixel_y_dou);

            if(color_new>thresholding_value)
                color_new = 255;
            else
                color_new = 0;

            setColor(new_img,w,h,color_new);
        }
    }

    ShowImage(new_img);
    return new_img;
}

QImage *MainWindow::Dilation(const QImage *img)
{
    QImage* new_img = new QImage();
    *new_img = img->copy();

    int width = img->width();
    int height = img->height();

    for(uint times=0;times<dilation_times;times++){
        for(int w=0;w<width;w++){
            for(int h=0;h<height;h++){
                int col_old = getColor(new_img,w,h);
                if(col_old!=0)continue;
                for(int x_off=-1;x_off<2;x_off++){
                    for(int y_off=-1;y_off<2;y_off++){
                        int x = w+x_off;
                        int y = h+y_off;
                        if(x<0||y<0)continue;
                        int col_xy = getColor(img,x,y);
                        if(col_xy!=0){
                            setColor(new_img,w,h,255);
                            break;
                        }
                    }
                }
            }
        }
    }

    ShowImage(new_img);
    return new_img;
}

QImage *MainWindow::Erosion(const QImage *img)
{
    QImage* new_img = new QImage();
    *new_img = img->copy();

    int width = img->width();
    int height = img->height();

    for(uint times=0;times<erosion_times;times++){
        for(int w=0;w<width;w++){
            for(int h=0;h<height;h++){
                int col_old = getColor(new_img,w,h);
                if(col_old==0)continue;
                for(int x_off=-1;x_off<2;x_off++){
                    for(int y_off=-1;y_off<2;y_off++){
                        int x = w+x_off;
                        int y = h+y_off;
                        if(x<0||y<0)continue;
                        int col_xy = getColor(img,x,y);
                        if(col_xy==0){
                            setColor(new_img,w,h,0);
                            break;
                        }
                    }
                }
            }
        }
    }

    ShowImage(new_img);
    return new_img;
}

int MainWindow::getColor(const QImage *img, int w, int h)
{
    QColor col = img->pixel(w,h);
    return col.red();
}

void MainWindow::setColor(QImage* img,int w,int h,int color){
    img->setPixel(w,h,qRgb(color,color,color));
}

void MainWindow::ShowImageByMenu(QAction* action)
{
    ShowImage(imgMap[action->text()]);
    imgCurrent = imgMap[action->text()];
}

void MainWindow::on_btn_do_clicked()
{
    if(imgCurrent==nullptr)
        return;
    QImage* img_last = new QImage();
    *img_last = imgCurrent->copy();
    QImage* img_new = nullptr;
    for(int step=0;step<cboxList.count();step++){
        if(!cboxList[step]->isChecked())continue;
        if(img_new!=nullptr){
            delete img_last;
            img_last = img_new;
        }
        img_new = (this->*algorithms[step])(img_last);
    }
    if(img_new==nullptr)
        return;
}

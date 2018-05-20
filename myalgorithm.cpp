#include "myalgorithm.h"



const QImage *MyAlgorithm::Execute(const QImage *input)
{
    if(ptr_img!=nullptr)
        delete ptr_img;
    ptr_img = new QImage();
    ptr_img = input;
    Body(ptr_img);
    return ptr_img;
}

QString MyAlgorithm::Name()
{
    return name;
}

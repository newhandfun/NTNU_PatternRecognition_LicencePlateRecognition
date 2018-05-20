#ifndef MYALGORITHM_H
#define MYALGORITHM_H
#include <QImage>

class MyAlgorithm
{
public:
    MyAlgorithm();
    ~MyAlgorithm()
    {
        if(ptr_img==nullptr)
            return;
        delete ptr_img;
    }

    const QImage* Execute(const QImage* input);
    virtual void Body(const QImage* input);

    QString Name();
protected:
    const QImage* ptr_img = nullptr;
    QString name;
};

#endif // MYALGORITHM_H

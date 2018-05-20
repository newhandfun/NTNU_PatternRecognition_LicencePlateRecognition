#ifndef FILTERYCHANNEL_H
#define FILTERYCHANNEL_H

#include "myalgorithm.h"

class FilterYChannel : public MyAlgorithm
{
public:
    void Body(const QImage* input) override;
};

#endif // FILTERYCHANNEL_H

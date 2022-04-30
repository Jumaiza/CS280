#ifndef NEWSPAPERSUBSCRIBER
#define NEWSPAPERSUBSCRIBER

#include "Address.h"

class NewsPaperSubscriber
{
    double rate;
    // attributes
    Address st_Address;

public:
    NewsPaperSubscriber(Address addy)
    {
        st_Address = addy;
    }

    NewsPaperSubscriber() {}

    void setRate(double r)
    {
        rate = r;
    }

    double getRate()
    {
        return rate;
    }

    void setAddress(Address newAddy)
    {
        st_Address = newAddy;
    }

    Address getAddress()
    {
        return st_Address;
    }

    virtual void print() = 0;

    bool equals(NewsPaperSubscriber *subscriberTwo)
    {
        if (st_Address == subscriberTwo->getAddress())
        {
            return true;
        }
        return false;
    }
};
#endif
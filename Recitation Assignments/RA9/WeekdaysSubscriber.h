#include <iostream>
using namespace std;

#include "NewsPaperSubscriber.h"

class WeekdaysSubscriber : public NewsPaperSubscriber
{
public:
    WeekdaysSubscriber(Address addy)
    {
        // set rate from inherited functtion
        setRate(7.5);
        // set address from inherited function
        setAddress(addy);
    }

    void print()
    {
        // prints address
        cout << getAddress().AddrContents() << endl;
        // prints rate and service type
        cout << "Rate: " << getRate() << endl;
        cout << "Service Type: Week Days Subscriber" << endl;
    }
};

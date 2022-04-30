#include <iostream>
using namespace std;

#include "NewsPaperSubscriber.h"

class WeekendSubscriber : public NewsPaperSubscriber
{
public:
    WeekendSubscriber(Address addy)
    {
        // set rate from inherited functtion
        setRate(4.5);
        // set address from inherited function
        setAddress(addy);
    }

    void print()
    {
        // prints address
        cout << getAddress().AddrContents() << endl;
        // prints rate and service type
        cout << "Rate: " << getRate() << endl;
        cout << "Service Type: Weekend Subscriber" << endl;
    }
};

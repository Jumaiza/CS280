#include <iostream>
using namespace std;

#include "NewsPaperSubscriber.h"

class WeeklySubscriber : public NewsPaperSubscriber
{
public:
    WeeklySubscriber(Address addy)
    {
        // set rate from inherited functtion
        setRate(10.5);
        // set address from inherited function
        setAddress(addy);
    }

    void print()
    {
        // prints address
        cout << getAddress().AddrContents() << endl;
        // prints rate and service type
        cout << "Rate: " << getRate() << endl;
        cout << "Service Type: Weekly Subscriber" << endl;
    }
};
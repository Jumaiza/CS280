// Address.h
//Definition of Address class
#ifndef ADDRESS_H
#define ADDRESS_H

#include <string> 
#include <sstream> 

using std::string;
using std::stringstream;

class Address
{
    private:
	 string streetAddress, city, state;
     long zipCode;

	public:
	Address()
	{
		streetAddress = "";
        city = "";
        state = "";
        zipCode = 0;
	}
    //-----------------------------------------------------------------
    //  Constructor: Sets up this address with the specified data.
    //-----------------------------------------------------------------
    Address(string street, string town, string st, long zip)
    {
        streetAddress = street;
        city = town;
        state = st;
        zipCode = zip;
    }
    
    bool operator==(Address thisAddress)
    {
    	if(zipCode == thisAddress.getzipCode())
    		return true;
    	
    	return false;
    }
    
    string getstreetAddress()
    {
    	return streetAddress;
    }
    
    string getCity()
    {
    	return city;
    }
    
    string getState()
    {
    	return state;
    }
    
    long getzipCode()
    {
    	return zipCode;
    }

    //-----------------------------------------------------------------
    //  Returns a description of this Address object.
    //-----------------------------------------------------------------
    string AddrContents() const
    {
        string result = "";
		
		stringstream ss;
    	ss << zipCode;
    
        result = streetAddress + "\n";
        result += city + ", " + state + " " + ss.str();

        return result;
    }


}; // end class Address


#endif


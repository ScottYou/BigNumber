#define ver_Array    /// Array version BigNumber
#define ASize 200   /// It must be enough for calculate ...

#include <iostream>
#include <list>
#include <stdint.h>
#include <string>

#include <cstdio>

using namespace std;

#ifndef _BIGNUMBER_H
#define _BIGNUMBER_H

//Big number class definition. This is just an example.
class BigNumber{
private:
    bool sgn;               // 1 for positive, 0 for negative
    uint16_t num_of_bits;   // the bit number in hex representation
#ifdef ver_Array
    uint8_t data[ASize];    // the hex number bit beginning from LSB
#else
    list<uint8_t> data;     // the hex number bit beginning from LSB
#endif // ver_Array

public:
    //constructors
    BigNumber();        // default: zero, sgn doesn't matter for 0
    BigNumber(int);     // directly convert from an int
    BigNumber(bool, uint16_t, uint8_t*);
    BigNumber(string);  // directly convert from a string in hex/dec
    BigNumber(const BigNumber&);    // copy a BigNumber

    //overloaded arithmetic operators as member functions
    int8_t NumCmp( const BigNumber, const BigNumber);
    bool operator>(const BigNumber);
    bool operator<(const BigNumber);
    bool operator>=(const BigNumber);
    bool operator<=(const BigNumber);
    bool operator==(const BigNumber);
    bool operator!=(const BigNumber);

    BigNumber operator+(const BigNumber);
    BigNumber operator-(const BigNumber);
    BigNumber operator*(const BigNumber);
    BigNumber operator/(const BigNumber); //integer division: 3/2==1
    BigNumber operator%(const BigNumber);
    BigNumber PowerMod( BigNumber, BigNumber);
    BigNumber Mod(const BigNumber);
    BigNumber InvMod(const BigNumber);
    BigNumber AddMod(const BigNumber, const BigNumber);
    BigNumber SubMod(const BigNumber, const BigNumber);
    BigNumber MulMod(const BigNumber, const BigNumber);
    BigNumber DivMod(const BigNumber, const BigNumber);

    //interface functions
    void Print( uint16_t showbit, uint16_t space );
    void Print();
    uint16_t GetNumBit(){ return this->num_of_bits;}
    void GetData( uint8_t *out){
        for( int i=0; i<ASize; i++) out[i]=this->data[i];
    }
};

#endif // _BIGNUMBER_H

#define ver_Array

#include "BigNumber.h"
using namespace std;

void BigNumber::Print( uint16_t showbit, uint16_t space ){  // in hex
    //if( showbit%space!=0 ){ cout << "Error input!"; return; }
    cout << hex;
    if( !this->sgn ) cout << '-'; uint16_t ii;
    for( ii=0; ii<showbit-(this->num_of_bits); ii++){
        cout << '0'; if( (ii+1)%space==0 ) cout << ' ';
    }
#ifdef ver_Array
    for( uint16_t jj=(this->num_of_bits); jj>0; jj--, ii++){
        cout << (int)data[jj-1]; if( (ii+1)%space==0 ) cout << ' ';
    }
#else
    list<uint8_t>::iterator it;
    for( it=this->data.end(); (it--)!=this->data.begin(); ii++){
        cout << (int)(*it); if( (ii+1)%space==0 ) cout << ' ';
    }
#endif // ver_Array
    cout << endl;
}

void BigNumber::Print(){
    cout << hex;
    if( this->num_of_bits==0 ){ cout << "0" << endl; return; }
    if( !this->sgn ) cout << '-';
#ifdef ver_Array
    for( uint16_t jj=(this->num_of_bits); jj>0; jj--){
        cout << (int)data[jj-1];
    }
#else
    list<uint8_t>::iterator it;
    for( it=this->data.end(); (it--)!=this->data.begin(); )
        cout << (int)(*it);
#endif // ver_Array
    cout << endl;
}

BigNumber::BigNumber(){
    sgn = 1;    // 0: sgn = 1
    num_of_bits = 0;

#ifdef ver_Array
    for( uint16_t jj=0; jj<ASize; jj++) data[jj] = 0;
#else
    data.clear();
#endif // ver_Array
}

BigNumber::BigNumber( int input){
    if( input>=0 )  sgn = 1;
    else{   sgn = 0; input = -input;   }
    num_of_bits = 0;

#ifdef ver_Array
    while(input>0){
        data[num_of_bits++] = (uint8_t)input%0x10;
        input >>= 4;
    }
    for( uint16_t ii = num_of_bits; ii<ASize; ii++) data[ii] = 0;
#else
    while(input>0){
        data.push_back( (uint8_t)input%0x10 );
        input >>= 4;
        num_of_bits++;
    }
#endif // ver_Array
}

BigNumber::BigNumber( bool sign, uint16_t nbit, uint8_t* dat){  // for hex input
    sgn = sign;
    num_of_bits = nbit;
#ifdef ver_Array
    uint16_t i;
    for( i=ASize; i>nbit; i--) data[i-1] = 0;
    for( ; (dat[nbit-i]==0 || dat[nbit-i]>0x10) && i>0; i--){   data[i-1] = 0;  num_of_bits--; }
    for( ; i>0; i--)    data[i-1] = dat[nbit-i];
#else
    data.clear();
    for( uint16_t i=0; i<nbit; i++)
        data.push_front( dat[i] );
#endif // ver_Array
}

BigNumber::BigNumber( string instr){ //hex
    uint16_t strlen = instr.length();
    uint16_t i=0, j;

    if(instr[i]=='+'){ sgn=1; i++;}
    else if(instr[i]=='-'){ sgn=0; i++;}
    else sgn=1;
    for(; instr[i]=='0'; i++);
    num_of_bits = strlen-i;
#ifdef ver_Array
    for( j=num_of_bits-1; i<strlen; i++, j--){
        if( '0'<=instr[i] && instr[i]<='9' )
            data[j] = instr[i]-'0';
        else if( 'a'<=instr[i] && instr[i]<='f' )
            data[j] = instr[i]-'a'+10;
        else if( 'A'<=instr[i] && instr[i]<='F' )
            data[j] = instr[i]-'A'+10;
        else cout << "error!" << endl;
    }
    for( j=num_of_bits; j<ASize; j++) data[j] = 0;
#else
    data.clear();
    for(; i<strlen; i++){
        if( '0'<=instr[i] && instr[i]<='9' )
            data.push_front(instr[i]-'0');
        else if( 'a'<=instr[i] && instr[i]<='f' )
            data.push_front(instr[i]-'a'+10);
        else if( 'A'<=instr[i] && instr[i]<='F' )
            data.push_front(instr[i]-'A'+10);
        else cout << "error!" << endl;
    }
#endif // ver_Array
}

#ifdef ver_Array
void ShowData( uint8_t *input, uint16_t bit){
    for( uint16_t ii=bit; ii>0; ii--)
        cout << hex << (int)input[ii-1];
    cout << endl;
}
#else
list<uint8_t> GetData( list<uint8_t>input){
    list<uint8_t> out;
    list<uint8_t>::iterator ite;
    for( ite=input.begin(); ite!=input.end(); ite++) out.push_back(*ite);
    return out;
}
void ShowData( list<uint8_t>input){
    list<uint8_t>::iterator it; cout << hex;
    for( it=input.end(); (it--)!=input.begin(); )
        cout << (int)(*it);
    cout << endl;
}
#endif // ver_Array

BigNumber::BigNumber(const BigNumber& input){
    sgn = input.sgn;
    num_of_bits = input.num_of_bits;
#ifdef ver_Array
    uint16_t i=0;
    for( ; i<num_of_bits; i++) data[i] = input.data[i];
    for( ; i<ASize; i++) data[i] = 0;
#else
    data = GetData(input.data);
#endif // ver_Array
}

#ifdef ver_Array
int8_t DataCmp( const uint8_t *fro, const uint8_t *bac){  // fro big->1, bac big->-1, equal->0
    for( uint16_t ii=ASize; ii>0; ii--){
        if( fro[ii-1]>bac[ii-1] ) return 1;
        if( fro[ii-1]<bac[ii-1] ) return -1;
    }
    return 0;
}
#else
int8_t DataCmp( list<uint8_t> fro, list<uint8_t> bac){  // fro big->1, bac big->-1, equal->0
    if( fro.size() > bac.size() ) return 1;
    if( fro.size() < bac.size() ) return -1;
    list<uint8_t>::iterator froit, bacit;
    froit=fro.end(); bacit=bac.end();
    for(; (froit--)!=fro.begin()&&(bacit--)!=bac.begin(); ){
        if((*froit)>(*bacit)) return 1;
        if((*froit)<(*bacit)) return -1;
    }
    return 0;
}
#endif // ver_Array

int8_t BigNumber::NumCmp( const BigNumber fro, const BigNumber bac){ // fro big->1, bac big->-1, equal->0
    if( bac.num_of_bits == 0 ){
        if( fro.num_of_bits == 0 ) return 0;
        else    return ((fro.sgn)? 1: -1);
    }
    if( fro.num_of_bits == 0 ) return ((bac.sgn)? -1: 1);
    if( fro.sgn && !bac.sgn) return 1;
    if( !fro.sgn && bac.sgn) return -1;
    int8_t frp = (fro.sgn) ? 1 : -1; // if front.sgn positive
    //if( fro.num_of_bits > bac.num_of_bits ) return frp; if( fro.num_of_bits < bac.num_of_bits ) return -frp;
    return frp*DataCmp(fro.data, bac.data);
}

#ifdef ver_Array
void DataAdd( uint8_t *out, const uint8_t *add1, const uint8_t *add2, uint16_t &bnum){
    //uint8_t *out = new uint8_t[ASize];
    uint8_t carry = 0;
    for( uint16_t ii=0; ii<ASize; ii++){
        out[ii] = (add1[ii]+add2[ii]+carry)%0x10;
        if( out[ii]>0 ) bnum = ii+1;
        carry = (add1[ii]+add2[ii]+carry)>>4;
    }
    //return out;
}
#else
list<uint8_t> DataAdd( list<uint8_t> add1, list<uint8_t> add2, uint16_t &bnum){
    list<uint8_t> out;
    list<uint8_t>::iterator init1, init2;
    uint8_t temp; bool carry=false;
    init1 = add1.begin();
    init2 = add2.begin();
    bnum = 0;
    for(;init1!=add1.end()&&init2!=add2.end(); init1++, init2++){
        temp = *init1 + *init2 + (uint8_t)carry;
        if(temp>>4){carry = true; temp-=0x10;}
        else        carry = false;
        out.push_back(temp);
        bnum++;
    }
    if( init1!=add1.end() ){
        for(;init1!=add1.end(); init1++){
            temp = *init1 + (uint8_t)carry;
            if(temp>>4){carry = true; temp-=0x10;}
            else        carry = false;
            out.push_back(temp);
            bnum++;
        }
    }else if( init2!=add2.end() ){
        for(;init2!=add2.end(); init2++){
            temp = *init2 + (uint8_t)carry;
            if(temp>>4){carry = true; temp-=0x10;}
            else        carry = false;
            out.push_back(temp);
            bnum++;
        }
    }
    if(carry){
        out.push_back(1);
        bnum++;
    }

    init1 = out.end();while( (*(--init1))==0 && !out.empty() ){ out.pop_back(); bnum--;}
    //if( bnum!=out.size() ) cout << "[ERROR in bnum]\n";
    return out;
}
#endif // ver_Array

#ifdef ver_Array
void DataSub( uint8_t *out, const uint8_t *add, const uint8_t *sub, uint16_t &bnum, bool &ifre){
    //uint8_t *out = new uint8_t[ASize];
    uint16_t ii = 0;
    //uint8_t tmp[ASize];
    uint8_t borrow = 0;
    switch( DataCmp( add, sub) ){
        case 1:
            for( ; ii<ASize; ii++){
                out[ii] = (0x10+add[ii]-sub[ii]-borrow)%0x10;
                if( out[ii]>0 ) bnum = ii+1;
                borrow = 1-((0x10+add[ii]-sub[ii]-borrow)>>4);
            } ifre = 0;
            break;
        case -1:
            for( ; ii<ASize; ii++){
                out[ii] = (0x10+sub[ii]-add[ii]-borrow)%0x10;
                if( out[ii]>0 ) bnum = ii+1;
                borrow = 1-((0x10+sub[ii]-add[ii]-borrow)>>4);
            } ifre = 1;
            break;
        case 0:
            ifre = 1;
            for( ; ii<ASize; ii++) out[ii] = 0;
    }
    //return out;
}
void DataSub( uint8_t *out, const uint8_t *add, const uint8_t *sub, uint16_t &bnum){
    bool ifre;
    DataSub( out, add, sub, bnum, ifre);
}
#else
list<uint8_t> DataSub( list<uint8_t> add, list<uint8_t> sub, uint16_t &bnum, bool &ifre){
    list<uint8_t> comp, unit, out;
    list<uint8_t>::iterator subit;
    uint16_t bcomp=0, tempb, nouse;
    comp.clear();
    for( subit=sub.begin(); subit!=sub.end(); subit++, bcomp++)
        comp.push_back(0xf-*subit);
    comp= DataAdd( add, comp, nouse);
    unit.clear(); unit.push_back(1);
    out = DataAdd( comp, unit, bnum);
    //cout << "out:"; ShowData(out);
    //cout << "bnum:" << bnum;

    for( subit=out.begin(), tempb=bcomp; subit!=out.end()&&tempb>0; subit++, tempb--);
    //cout << "tempb:" << tempb;

    if(subit!=out.end()){
        //cout << "out:"; ShowData(out);
        while( *subit==0 )  *(subit++) = 0xf;
        (*subit)--;
        //cout << "out:"; ShowData(out);
        ifre = false;
    }else{
        for( subit=out.begin(); subit!=out.end(); subit++)
            (*subit) = 0xf-(*subit);
        for(; tempb>0; tempb--)
            out.push_back(0xf);
        out = DataAdd( out, unit, bnum);
        ifre = true;
    }

    subit = out.end();while( (*(--subit))==0 && !out.empty() ){ out.pop_back(); bnum--;}
    //if( bnum!=out.size() ) cout << "[ERROR in bnum]\n";
    return out;
}
#endif // ver_Array

#ifdef ver_Array
void DataMul( uint8_t *out, const uint8_t *mul1, const uint8_t *mul2, uint16_t &bnum){
    uint16_t tmp[ASize];
    //uint8_t *out = new uint8_t[ASize];
    uint16_t carry = 0;
    for( uint16_t ss=0; ss<ASize; ss++){
        tmp[ss] = 0;
        for( uint16_t ii=0; ii<=ss; ii++)
            tmp[ss] += mul1[ii]*mul2[ss-ii];
        out[ss] = (uint8_t) (tmp[ss]+carry)%0x10;
        if( out[ss]>0 ) bnum = ss+1;
        carry = (tmp[ss]+carry)>>4;
    }
    //return out;
}
#else
list<uint8_t> DataMul( list<uint8_t> mul1, list<uint8_t> mul2, uint16_t &bnum){
    list<uint8_t> out, tlist1, tlist2;
    list<uint8_t>::iterator init1, init2;
    uint8_t tint;
    uint16_t b, i, pin2=0;

    out.clear();
    for( init2 = mul2.begin(); init2!=mul2.end(); init2++, pin2++){
        tlist1.clear(); tlist2.clear(); tlist2.push_back(0);
        for( init1 = mul1.begin(); init1!=mul1.end(); init1++){
            tint = (*init1)*(*init2);
            tlist1.push_back(tint%0x10);
            tlist2.push_back(tint>>4);
        }
        tlist1 = DataAdd( tlist1, tlist2, b);
        for( i=0; i<pin2; i++) tlist1.push_front(0);
        out = DataAdd( out, tlist1, bnum);
    }
    return out;
}
#endif // ver_Array

#ifdef ver_Array
void DataDiv( uint8_t *quo, uint8_t *rest, const uint8_t *ori, const uint8_t *div, uint16_t &bnum, uint16_t &rnum){ // ori & div are nonzero
    for( uint16_t ii=0; ii<ASize; ii++) quo[ii] = 0;
    for( uint16_t ii=0; ii<ASize; ii++) rest[ii] = ori[ii];
    uint16_t bori = ASize, bdiv = ASize, bquo, brest, nouse;
    uint8_t qtest, uprest;

    while( ori[bori-1]==0 && bori>0 ) bori--;   brest = bori;
    while( div[bdiv-1]==0 && bdiv>0 ) bdiv--;
    //cout << bori << " " << bdiv << "?" << ((bori<bdiv)?"T":"F") << "!!!\n";
    bnum = 0; rnum = bori;
    if( bori<bdiv ) return;

    uint8_t tmp[ASize]; for( uint16_t ii=0; ii<ASize; ii++) tmp[ii] = 0;
    uint8_t tmp2[ASize];
    uint8_t mul[ASize];

    int8_t chk;
    bquo = bori-bdiv+1;
    //cout << bori << " " << bdiv << "---\n";
    for( ; bquo>0; bquo--){
/*        if( bquo+bdiv<2 || bquo+bdiv-2>=ASize ){
            cout << bori << "_" << bquo << "_" << bdiv << "_" << ASize << "OOPS";
            getchar();} */
        uprest = rest[bquo+bdiv-2];
        if(bquo+bdiv<=ASize)    uprest += (rest[bquo+bdiv-1])<<4 ;
        qtest = uprest / div[bdiv-1];
        //cout << hex << (int)qtest << " " << (int)uprest << " " << (int)div[bdiv-1] << endl;

        for( uint16_t ii = qtest; ii>0; ii--){
            tmp[bquo-1] = ii;
            DataMul( mul, div, tmp, nouse);
            chk = DataCmp( rest, mul);
            if( chk==1){
                quo[bquo-1] = ii;   if( bnum==0 ) bnum = bquo;
                //cout <<"in_ " << brest <<" "; ShowData( rest, brest);
                //cout <<" mul" << nouse <<" "; ShowData( mul, nouse);
                DataSub( tmp2, rest, mul, brest);
                for( uint16_t jj=0; jj<ASize; jj++) rest[jj] = tmp2[jj];
                //cout <<"out " << brest <<" "; ShowData( rest, brest);
                break;
            }
            if( chk==0){
                quo[bquo-1] = ii;   if( bnum==0 ) bnum = bquo;
                for( uint16_t jj=0; jj<ASize; jj++) rest[jj] = 0;
                rnum = 0;
                return;
            }
        }
        tmp[bquo-1] = 0;
    }
    rnum = brest;
}
#else
list<uint8_t> DataDiv( list<uint8_t> ori, list<uint8_t> div, uint16_t &bnum, bool ifquo){
    //if(DataCmp(ori, div)<0){ return BigNumber()}
    list<uint8_t> out;
    list<uint8_t> *tlist = new list<uint8_t>[5];
    uint16_t b, i, divb;
    uint8_t j, temp;
    bool ifre;
    out.clear();
    bnum = 0;   // for quotient
    tlist[0] = GetData(div);
    //ShowData(ori);ShowData(tlist[0]);
    for( divb=0; DataCmp(ori, tlist[0])>=0; divb++){
        tlist[0].push_front(0);
        //ShowData(tlist[0]);
    }
    //cout << "divb = " << divb << endl;
    for( i=0; i<divb; i++){
        //cout << "For i = " << i << " / " << divb << endl;
        tlist[0].pop_front();
        for(j=0; DataCmp(ori, tlist[j])>=0; j++){
            tlist[j+1] = DataAdd(tlist[j], tlist[j], b);
            //ShowData(tlist[j+1]);
        }
        //ShowData(tlist[j-1]);cout << endl;
        for( temp=0; j>0; j--){
            if(DataCmp(ori, tlist[j-1])>=0){
                //if( DataCmp(ori, tlist[j-1])==0 ) cout << "!!!";
                //ShowData(ori);ShowData(tlist[j-1]); cout << "----\n";
                ori = DataSub(ori, tlist[j-1], b, ifre);
                //if(ifre){ cout << "ERROR!!\n"; cin >> b;}
                temp += 1<<(j-1);
                //ShowData(ori);
            }
        }
        //cout << "  ori: "; ShowData(ori);
        out.push_front(temp);
        bnum++;
    }
    if( !ifquo) bnum = ori.size(); // for remainder
    return ((ifquo)?out:ori);
}
#endif // ver_Array

bool BigNumber::operator>(const BigNumber input){ return (NumCmp(*this, input)>0); }
bool BigNumber::operator<(const BigNumber input){ return (NumCmp(*this, input)<0); }
bool BigNumber::operator>=(const BigNumber input){return (NumCmp(*this, input)>=0);}
bool BigNumber::operator<=(const BigNumber input){return (NumCmp(*this, input)<=0);}
bool BigNumber::operator==(const BigNumber input){return (NumCmp(*this, input)==0);}
bool BigNumber::operator!=(const BigNumber input){return (NumCmp(*this, input)!=0);}

BigNumber BigNumber::operator+(const BigNumber input){
    BigNumber out;
    uint8_t tmp[ASize];
    if( input.num_of_bits==0 ){
        out = BigNumber(*this);
    }else if( this->num_of_bits==0 ){
        out = BigNumber(input);
    }else if( this->sgn==input.sgn ){
        DataAdd( tmp, this->data, input.data, out.num_of_bits);
        out.sgn = this->sgn;
        for( uint16_t ii=0; ii<ASize; ii++)
            out.data[ii] = tmp[ii];
    }else{
        bool ifre;
        DataSub( tmp, this->data, input.data, out.num_of_bits, ifre);
        out.sgn = (this->sgn)^ifre;
        for( uint16_t ii=0; ii<ASize; ii++)
            out.data[ii] = tmp[ii];

    }
    return BigNumber(out);
}

BigNumber BigNumber::operator-(const BigNumber input){
    BigNumber out;
    uint8_t tmp[ASize];
    if( input.num_of_bits==0 ){
        out = BigNumber(*this);
    }else if( this->num_of_bits==0 ){
        out = BigNumber(input);
        out.sgn ^= 1;
    }if( this->sgn!=input.sgn ){
        DataAdd( tmp, this->data, input.data, out.num_of_bits);
        out.sgn = this->sgn;
        for( uint16_t ii=0; ii<ASize; ii++)
            out.data[ii] = tmp[ii];
    }else{
        bool ifre;
        DataSub( tmp, this->data, input.data, out.num_of_bits, ifre);
        out.sgn = (this->sgn)^ifre;
        for( uint16_t ii=0; ii<ASize; ii++)
            out.data[ii] = tmp[ii];
    }
    return BigNumber(out);
}

BigNumber BigNumber::operator*(const BigNumber input){
    if( this->num_of_bits==0 || input.num_of_bits==0) return BigNumber();
    BigNumber out;
    uint8_t tmp[ASize];
    out.sgn = !(this->sgn) ^ (input.sgn);
    DataMul( tmp, this->data, input.data, out.num_of_bits);
    for( uint16_t ii=0; ii<ASize; ii++)
        out.data[ii] = tmp[ii];
    return BigNumber(out);
}

BigNumber BigNumber::operator/(const BigNumber input){
    if( this->num_of_bits==0) return BigNumber(0);
    if( input.num_of_bits==0){
        cout << "Error: divisor can't be zero!" << endl;
        //return NULL;
    }
    BigNumber out; uint8_t tmp[ASize], tmp2[ASize]; uint16_t nouse;
    out.sgn = !(this->sgn) ^ (input.sgn);
    DataDiv( tmp, tmp2, this->data, input.data, out.num_of_bits, nouse);
    for( uint16_t ii=0; ii<ASize; ii++)
        out.data[ii] = tmp[ii];
    return BigNumber(out);
}

BigNumber BigNumber::operator%(const BigNumber input){
    if( this->num_of_bits==0) return BigNumber(input);
    if( input.num_of_bits==0){
        cout << "Error: divisor can't be zero!" << endl;
        //return NULL;
    }
    BigNumber out; uint8_t tmp[ASize], tmp2[ASize]; uint16_t nouse;
    out.sgn = !(this->sgn) ^ (input.sgn);
    DataDiv( tmp2, tmp, this->data, input.data, nouse, out.num_of_bits);
    for( uint16_t ii=0; ii<ASize; ii++)
        out.data[ii] = tmp[ii];
    return BigNumber(out);
}

BigNumber BigNumber::PowerMod( BigNumber power, BigNumber mod){
    if( power.sgn == 0 || mod.sgn == 0){
        cout << "Error: PowerMod Parameter should be positive" << endl;
        //return NULL;
    }
    BigNumber out = 1;
    BigNumber NowPow = (*this)%mod;

#ifdef ver_Array
    //cout << "a    :"; this->Print();
    //cout << "power:"; power.Print();
    //cout << "mod  :"; mod.Print();

    for( uint16_t ii=0; ii<power.num_of_bits; ii++){
        //cout << "[1]"; NowPow.Print(); out.Print();
        if((power.data[ii])&0x1)  out = (out*NowPow)%mod;
        NowPow = (NowPow*NowPow)%mod;   //2
        //cout << "[2]"; NowPow.Print(); out.Print();
        if((power.data[ii])&0x2)  out = (out*NowPow)%mod;
        NowPow = (NowPow*NowPow)%mod;   //4
        //cout << "[3]"; NowPow.Print(); out.Print();
        if((power.data[ii])&0x4)  out = (out*NowPow)%mod;
        NowPow = (NowPow*NowPow)%mod;   //8
        //cout << "[4]"; NowPow.Print(); out.Print();
        if((power.data[ii])&0x8)  out = (out*NowPow)%mod;
        NowPow = (NowPow*NowPow)%mod;   //16
    }
#else
    list<uint8_t>::iterator it;
    for( it=power.data.begin(); it!=power.data.end(); it++){
        //cout << "P0"; NowPow.Print(); out.Print();
        if((*it)&0x1)  out = (out*NowPow)%mod;
        NowPow = (NowPow*NowPow)%mod;   //2
        //cout << "P1"; NowPow.Print(); out.Print();
        if((*it)&0x2)  out = (out*NowPow)%mod;
        NowPow = (NowPow*NowPow)%mod;   //4
        //cout << "P2"; NowPow.Print(); out.Print();
        if((*it)&0x4)  out = (out*NowPow)%mod;
        NowPow = (NowPow*NowPow)%mod;   //8
        //cout << "P3"; NowPow.Print(); out.Print();
        if((*it)&0x8)  out = (out*NowPow)%mod;
        NowPow = (NowPow*NowPow)%mod;   //16
    }
#endif // ver_Array
    return BigNumber(out);
}

void Extend_Euclid( BigNumber a, BigNumber b, BigNumber &x, BigNumber &y){
    if( a<b ){  Extend_Euclid( b, a, x, y); return;}
    if( b == 0 ){
        x = 1; y = 0;
        return; //return a;
    }
    BigNumber x1, y1;
    Extend_Euclid(b, a%b, x1, y1); //BigNumber gcd = Extend_Euclid(b, a%b, x1, y1);
    x = y1;
    y = x1 - (a/b)*y1;
    //return gcd;
}

BigNumber BigNumber::Mod( BigNumber mod){
    BigNumber tmp = (*this);
    if(tmp>=0)  return tmp%mod;
    tmp = (BigNumber(0)-tmp)%mod;
    //return ((tmp==0)?BigNumber(0):mod-tmp);
    return ((tmp==0)?BigNumber(0):mod-tmp);
}

BigNumber BigNumber::InvMod(const BigNumber mod){
    BigNumber inv, quo;
    Extend_Euclid( mod, (*this)%mod, quo, inv);
    return inv.Mod(mod);
}

BigNumber BigNumber::AddMod(const BigNumber add, const BigNumber mod){
    return ((*this)+add).Mod(mod);
}

BigNumber BigNumber::SubMod(const BigNumber sub, const BigNumber mod){
    return ((*this)-sub).Mod(mod);
}

BigNumber BigNumber::MulMod(const BigNumber mul, const BigNumber mod){
    return ((*this)*mul).Mod(mod);
}

BigNumber BigNumber::DivMod(const BigNumber div, const BigNumber mod){
    return (*this).MulMod( BigNumber(div).InvMod(mod), mod);
}

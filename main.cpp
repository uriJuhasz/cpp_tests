#include <iostream>
#include <iomanip>
#include <memory>
#include <chrono>
#include <cmath>
#include <cassert>
#include <random>
using namespace std;

class C{ protected:virtual ~C(){}};
class D:public C{ public:virtual ~D(){}};

class EP{
public:
    EP() : next(nullptr){}
    virtual ~EP(){}
    EP(const EP&) = delete;
    EP& operator=(const EP&) = delete;
    EP* next;
    virtual EP* f(EP*p){
//        next=p;
//        p=nullptr;
//        EP* r = next->next;
//        next=nullptr;
        return p;
    }
};

class EP1 : public EP{
public:
    EP1() : EP(){}
    virtual EP1* f(EP*){
        throw new exception();
        return this;
    }
};

class EUPM{
public:
    EUPM() : next(nullptr){}
    virtual ~EUPM(){}
    unique_ptr<EUPM> next;
    virtual unique_ptr<EUPM> f(unique_ptr<EUPM> p){
//        next=move(p);
        return move(p); //move(next->next);
    }

};

class EUPM1 : public EUPM{
public:
    EUPM1() : EUPM(){}
    virtual unique_ptr<EUPM> f(unique_ptr<EUPM> p){
        throw new exception();
        return p;
    }
};


class EUPR{
public:
    EUPR() : next(nullptr){}
    virtual ~EUPR(){}
    unique_ptr<EUPR> next;
    virtual unique_ptr<EUPR> f(unique_ptr<EUPR>& p){
//        next=move(p);
//        return move(next->next);
        return move(p);
    }

};

class EUPR1 : public EUPR{
public:
    EUPR1() : EUPR(){}
    virtual unique_ptr<EUPR> f(unique_ptr<EUPR>& p){
        throw new exception();
        return move(p);
    }
};

void measure(int f(),string s)
{
    auto startT= chrono::system_clock::now();
    f();
    auto endT = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = endT-startT;

    std::cout << s << " time: " << setw(10) << lround(1000000 * elapsed_seconds.count()) << "ns" << endl;
}


constexpr long long num = 2000000000L;
constexpr long long numP = num/100L;
/*
template<typename P>void f(){
    P up1(new P());
    long j=0;
    for (long long i=0;i<num;i++)
    {
        up1->next = move(up1);
        j = i+j;
    }
}*/

bool ud(){
    random_device r;
    int x = ((r() % 100) + 100) %100;
    return (x*x-2*x+1!=x*x-2);
}

EP* ffP(EP* p){
    p->next=p;
    EP* r = p->next->next;
    return r;
}
int fP(){
    EP* p1 = ud() ? new EP : new EP1;
    EP* p2;
    long j=0;
    for (long long i=0;i<num;i++){
//        if (i%numP==0)
//            cout << "i:" << i << endl;
        p2=p1->f(p1);
        p1=p2->f(p2);
        j+=i*j;
    }
    return j;
}

/*unique_ptr<EUP> ffUP(unique_ptr<EUP>& p){
    EUP* q = p.get();
    assert(q);
    q->next = move(p);
    return move(q->next->next);
}*/
int fUPR(){
    unique_ptr<EUPR> p1 = ud() ? make_unique<EUPR>() : make_unique<EUPR1>();
    unique_ptr<EUPR> p2;
    long j=0;
    for (long long i=0;i<num;i++){
        p2=p1->f(p1);
//        p1=nullptr;
        p1=p2->f(p2);
//        p2=nullptr;
        j+=i*j;
    }
    return j;
}


/*
unique_ptr<EUP> ffUPM(unique_ptr<EUP> p){
    EUP* q = p.get();
    assert(q);
    q->next = move(p);
    return move(q->next->next);
}*/
int fUPM(){
    unique_ptr<EUPM> p1 = ud() ? make_unique<EUPM>() : make_unique<EUPM1>();
    unique_ptr<EUPM> p2;
    long j = 0;
    for (long i=0;i<num;i++){
        p2=p1->f(move(p1));
        p1=p2->f(move(p2));
        j+=i*j;
    }
    return j;
}

/*
shared_ptr<EUP> ffSP(shared_ptr<E> p){
    shared_ptr<E> r = p;
//    r->next=nullptr;
    return r;
}
void fSP(){
    shared_ptr<E> p1 = make_shared<E>();
    shared_ptr<E> p2;
    for (long i=0;i<num;i++){
//        cout << "SPi:" << i << endl;
        p2=ffSP(p1);
        p1=ffSP(p2);
    }
}
*/
int main()
{
    cout << "Hello world!" << endl;
    cout << "C* : " << sizeof(C*) << endl;
    unique_ptr<D> up(new D());
    shared_ptr<D> sp(new D());
    cout << "unique_ptr<C> : " << sizeof(up) << endl;
    cout << "shared_ptr<C> : " << sizeof(sp) << endl;

    measure(fP  ,"          *");
    measure(fUPM,"unique_ptrM");
    measure(fUPR,"unique_ptr&");
//    measure(fSP,"shared_ptr");
    return 0;
}

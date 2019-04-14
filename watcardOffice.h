#ifndef __WATCARDOFFICE_H__
#define __WATCARDOFFICE_H__

#include "watcard.h"
#include <queue>

_Monitor Printer;
_Monitor Bank;

_Task WATCardOffice {

    struct Args {
        unsigned int student_id;
        unsigned int amount;
        WATCard *w_card;

        Args(unsigned int s_id, unsigned int amount, WATCard *w_card) : student_id(s_id), amount(amount), w_card(w_card){}
    };

    struct Job {                           // marshalled arguments and return future
        Args args;                         // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;          // return future
        Job( Args args ) : args( args ) {}
    };

    _Task Courier {     // communicates with bank
        unsigned int id;
        Bank &bank;
        WATCardOffice &office;
        Printer &prt;


        void main();
       public:
        Courier(unsigned int id, Bank &bank, WATCardOffice &office, Printer &prt);
    };                 

    Printer &prt;
    Bank &bank;
    unsigned int numCouriers;
    Courier **couriers;
    std::queue<Job *> jobs;

    void main();

  public:
    _Event Lost {};                        // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

#endif
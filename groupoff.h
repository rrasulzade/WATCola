#ifndef __GROUPOFF_H__
#define __GROUPOFF_H__

#include "watcard.h"
#include <vector>

_Monitor Printer; 

_Task Groupoff {
	Printer &prt;
	unsigned int numStudents;
	unsigned int sodaCost;
	unsigned int groupoffDelay;
	std::vector<WATCard::FWATCard> g_cards;

    void main();
  public:
    Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    WATCard::FWATCard giftCard();
};


#endif
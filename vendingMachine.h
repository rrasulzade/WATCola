#ifndef __VendingMachine_H__
#define __VendingMachine_H__

#include "watcard.h"

_Monitor Printer;
_Task NameServer;

_Task VendingMachine {
	Printer &prt;
	NameServer &nameServer;
	unsigned int id;
	unsigned int sodaCost;
	unsigned int maxStockPerFlavour;
	unsigned int *sodaInventory;
	bool isRestocked;
	bool ExceptionStock;
	bool ExceptionFunds;
	uBaseTask *student;
    void main();
  public:
    enum Flavours {BLACK_CHERRY, CREAM_SODA, ROOT_BEER, JAZZ_LIME, NUM_FLAVOURS}; // flavours of soda
    _Event Funds {};                       // insufficient funds
    _Event Stock {};                       // out of stock for particular flavour
    VendingMachine(Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour);
	~VendingMachine();
    void buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
  private:
	Flavours flavourRequested;
};


#endif
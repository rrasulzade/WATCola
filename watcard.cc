#include "watcard.h"


// WATCard constructor 
// When a WATCard is created it has a $0 balance
WATCard::WATCard() : balance(0) {}


// The courier calls deposit after a funds transfer. 
void WATCard::deposit( unsigned int amount ){
	balance += amount;
}


// A vending machine calls withdraw when a soda is purchased.
void WATCard::withdraw( unsigned int amount ){
	balance -= amount;
}


// returns current WATCard balance
unsigned int WATCard::getBalance(){
	return balance;
}

/* END */
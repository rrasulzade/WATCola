#include "student.h"
#include "printer.h"
#include "groupoff.h"
#include "nameServer.h"
#include "watcardOffice.h"

#include "MPRNG.h"

#define DEFAULT_BALANCE 5


// Student constructor
Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff,
             									unsigned int id, unsigned int maxPurchases ) :
						prt(prt), nameServer(nameServer), cardOffice(cardOffice), groupoff(groupoff),
												id(id), maxPurchases(maxPurchases){}



// periodically buys some of the favourite soda from
// a vending machine
void Student::main(){

	// selecting a random number of bottles to purchase
	unsigned int bottleNum = mprnGen(1, maxPurchases);

	// selecting a random favourite flavour
	VendingMachine::Flavours flavour = (VendingMachine::Flavours) mprnGen(3);

	// print start message
	prt.print(Printer::Student, id, 'S', flavour, bottleNum);

    // creates a WATCard via the WATCardOffice
	WATCard::FWATCard w_card = cardOffice.create(id, 5);

	// creates a gift card via the Groupoff
	WATCard::FWATCard g_card = groupoff.giftCard();


	// obtains the location of a vending machine from the name server
	VendingMachine *v_machine = nameServer.getMachine(id);

	// print selected vending machine message
	prt.print(Printer::Student, (int) id, 'V', v_machine->getId());

    bool isUsed = false;
	for(unsigned int i = 0; i < bottleNum; i++){
		yield(mprnGen(1,10));

    	for(;;){
    		try{
    			_Enable{
                    _Select( g_card ){
                        // gift card is used
                        isUsed = true;

                        // pay with GiftCard and reset
                        v_machine->buy(flavour, *g_card());

                        // print GiftCard balance message
                        prt.print(Printer::Student, (int) id, 'G', g_card()->getBalance());
                        delete g_card();
                        g_card.reset();
                    }
                    or _Select( w_card ){

                        // pay with WATCard
                        v_machine->buy(flavour, *w_card());

                        // print WATCard balance message
                        prt.print(Printer::Student, (int) id, 'B', w_card()->getBalance());
                    }

                   break;
                }
    		}
    		catch(WATCardOffice::Lost){
    			// print WATCard lost message
				prt.print(Printer::Student, (int) id, 'L');

				// the student must create a new WATCard
				w_card = cardOffice.create(id, DEFAULT_BALANCE);
    		}
    		catch(VendingMachine::Funds){

    			// student transfers the current vending-machine
    			// soda-cost plus $5 to their WATCard
    			unsigned int amount = v_machine->cost() + 5;
    			cardOffice.transfer(id, amount, w_card());

    		}
    		catch(VendingMachine::Stock){
    			// the student must obtain a new vending machine
    			// from the name server
				v_machine = nameServer.getMachine(id);

				// print selected vending machine message
				prt.print(Printer::Student, (int) id, 'V', v_machine->getId());
    		}

    	}

	}

    // prevent memory leak, wait giftcard to be delivered
    if(!isUsed){
        _Select( g_card ) {
            delete g_card();
            g_card.reset();
        }
    }

    // watcard office might throw Lost exception
    try{
        delete w_card();
    }
    catch(WATCardOffice::Lost){}


	//print Finish message
	prt.print(Printer::Student, (int) id, 'F');
}


/* END */

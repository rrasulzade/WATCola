#include "vendingMachine.h"
#include "nameServer.h"
#include "printer.h"

VendingMachine::VendingMachine(Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    														unsigned int maxStockPerFlavour) :
								prt(prt), nameServer(nameServer), id(id), sodaCost(sodaCost), maxStockPerFlavour(maxStockPerFlavour)
{
    isRestocked = false;
    ExceptionStock = false;
    ExceptionFunds = false;
    student = NULL;
    sodaInventory = new unsigned int[NUM_FLAVOURS];

    for (int i = 0; i < NUM_FLAVOURS; i++) {
        sodaInventory[i] = 0;
    }
}

VendingMachine::~VendingMachine() {
    delete [] sodaInventory;
}

void VendingMachine::buy(Flavours flavour, WATCard &card) {
    student = &uThisTask();                        // remember the student for administrator
    if (sodaInventory[flavour] == 0) ExceptionStock = true;  // stock exception occurred
    else if (card.getBalance() < sodaCost) ExceptionFunds = true;  // funds exception occurred
    else {                            // no exception, provide the bottle and debit the watcard
        card.withdraw(sodaCost);
        sodaInventory[flavour]--;
        flavourRequested = flavour;         // remember the flavour for administrator
    }
}

unsigned int *VendingMachine::inventory() {
	return sodaInventory;
}

void VendingMachine::restocked(){
    isRestocked = true;
}

_Nomutex unsigned int VendingMachine::cost() {
	return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() {
	return id;
}

void VendingMachine::main() {
    // print start message
    prt.print(Printer::Vending, (int) id, 'S', (int) sodaCost);

    nameServer.VMregister(this);   // register with nameServer

    for (;;) {

        _Accept(~VendingMachine) { break; }
        or
        _When(isRestocked) _Accept(buy) {      // when machine is not restocked, buy can be called
            if (ExceptionStock) {          // stock exception happened
                _Resume Stock() _At *student;  // throw stock exception at student
                ExceptionStock = false;
            }
            else
            if (ExceptionFunds) {          // funds exception happened
                _Resume Funds() _At *student;   // throw funds exception at student
                ExceptionFunds = false;
            }
            // print student bought soda message
            else prt.print(Printer::Vending, (int) id, 'B', (int) flavourRequested, sodaInventory[flavourRequested]);
        }
        or
        _Accept(inventory) {
            isRestocked = false;   // restocking started
            // print start reloading by truck message
            prt.print(Printer::Vending, (int) id, 'r');
        }
        or
        _Accept(restocked) {
            // print complete reloading by truck message
            prt.print(Printer::Vending, (int) id, 'R');
        }
    }

    // print finish message
    prt.print(Printer::Vending, (int) id, 'F');
}

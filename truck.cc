#include "truck.h"
#include "MPRNG.h"
#include "nameServer.h"
#include "bottlingPlant.h"
#include "vendingMachine.h"
#include "printer.h"
#include <iostream>

// Truck Constructor
Truck::Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant,
            unsigned int numVendingMachines, unsigned int maxStockPerFlavour) :
            prt(prt), nameServer(nameServer), plant(plant), numVendingMachines(numVendingMachines),
            maxStockPerFlavour(maxStockPerFlavour) {}

// Truck Destructor
Truck::~Truck() {}

void Truck::main() {

    // print starting message
    prt.print(Printer::Truck, 'S');

    // The truck begins by obtaining the location of
    // each vending machine from the name server.
    VendingMachine **machineList = nameServer.getMachineList();
    unsigned int cargo[VendingMachine::NUM_FLAVOURS];
    unsigned int lastMachine = 0;

    for (;;) {
                // truck yields a random number of times [1, 10]
                yield(mprnGen(1, 10));

                try {
                    _Enable {
                        // obtain a new shipment of soda
                        plant.getShipment(cargo);
                    }
                }
                // If the bottling plant is closing down, the truck terminates.
                catch(BottlingPlant::Shutdown) {
                    break;
                }

                unsigned int total = 0;

                for (unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++) {
                    total += cargo[i];
                }

                // print picked up shipment message
                prt.print(Printer::Truck, 'P', (int) total);

                // To ensure fairness, the vending machines are restocked in cyclic order
                for (unsigned int i = lastMachine; i < lastMachine + numVendingMachines; i++) {
                    VendingMachine *machine = machineList[i % numVendingMachines];
                    unsigned int *inventory = machine->inventory();
                    unsigned int notReplenished = 0;

                    // print begin delivery to vending machine message
                    prt.print(Printer::Truck, 'd', (int) machine->getId(), (int) total);

                    // The truck can only restock up to MaxStockPerFlavour
                    // for each flavour in each vending machine
                    for (unsigned int j = 0; j < VendingMachine::NUM_FLAVOURS; j++) {
                        unsigned int demand = maxStockPerFlavour - inventory[j];
                        unsigned int supply = cargo[j] < demand ? cargo[j] : demand;
                        inventory[j] += supply;
                        cargo[j] -= supply;
                        total -= supply;
                        notReplenished += demand - supply;
                    }

                    // print unsuccessfully filled vending machine message
                    prt.print(Printer::Truck, 'U', (int) machine->getId(), (int) notReplenished);

                    machine->restocked();

                    // print end delivery to vending machine message
                    prt.print(Printer::Truck, 'D', (int) machine->getId(), (int) total);

                    // update last restocked machine
                    if (total == 0) {
                        lastMachine = (i + 1) % numVendingMachines;
                        break;
                    }
                }
    }

    // print Finished message
    prt.print(Printer::Truck, 'F');
}

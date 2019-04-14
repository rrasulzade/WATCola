#include "bottlingPlant.h"
#include "vendingMachine.h"
#include "printer.h"
#include "truck.h"
#include "MPRNG.h"


BottlingPlant::BottlingPlant(Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                            unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                            unsigned int timeBetweenShipments) :
                            prt(prt), nameServer(nameServer), numVendingMachines(numVendingMachines),
                            maxShippedPerFlavour(maxShippedPerFlavour), maxStockPerFlavour(maxStockPerFlavour),
                            timeBetweenShipments(timeBetweenShipments)
{
    shipment = new unsigned int[VendingMachine::Flavours::NUM_FLAVOURS];
    closingDown = false;
}

BottlingPlant::~BottlingPlant() {
    delete [] shipment;
}

void BottlingPlant::getShipment(unsigned int cargo[]) {
    if (!closingDown) {        // if bottlingPlant is not closing down, load the cargo
        for (unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++) {
            cargo[i] = shipment[i];
        }
    }else{
        truck_bench.wait();
    }
}

void BottlingPlant::main() {
    prt.print(Printer::BottlingPlant, 'S');

    // create a truck
    Truck truck(prt, nameServer, *this, numVendingMachines, maxStockPerFlavour);

    for (;;) {
        // make production run
        unsigned int production = 0;
        for (unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++) {
            unsigned int produce = mprnGen(0, maxShippedPerFlavour);
            shipment[i] = produce;
            production += produce;
        }

        // print production run message
        prt.print(Printer::BottlingPlant, 'G', (int) production);

        _Accept(~BottlingPlant) {
            closingDown = true;   // bottlingPlant closed down
            _Accept(getShipment) {   // wait for truck to get last shipment
                _Resume Shutdown() _At truck;    // inform the truck
                truck_bench.signalBlock();
            }
            break;
        }
        or
        _Accept(getShipment) {
            // print shipment picked up message
            prt.print(Printer::BottlingPlant, 'P');
            yield(timeBetweenShipments);   // yield between shipments
        }
    }

    // print finish message
    prt.print(Printer::BottlingPlant, 'F');
}

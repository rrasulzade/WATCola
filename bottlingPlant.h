#ifndef __BOTTLING_PLANT__
#define __BOTTLING_PLANT__

_Monitor Printer;
_Task NameServer;

_Task BottlingPlant {
    Printer &prt;
    NameServer &nameServer;
    unsigned int numVendingMachines;
    unsigned int maxShippedPerFlavour;
    unsigned int maxStockPerFlavour;
    unsigned int timeBetweenShipments;
    unsigned int *shipment;
    bool closingDown;
    uCondition truck_bench;
    void main();
public:
    _Event Shutdown {}; // shutdown plant
    BottlingPlant(Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
    unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
    unsigned int timeBetweenShipments);
    ~BottlingPlant();
    void getShipment(unsigned int cargo[]);
};

#endif
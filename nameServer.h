#ifndef __NameServer_H__
#define __NameServer_H__

#include "vendingMachine.h"

_Monitor Printer;

_Task NameServer {
	Printer &prt;
	unsigned int numVendingMachines;
	unsigned int numStudents;
	VendingMachine **vendingMachines;
	unsigned int *vmstudentmap;
	unsigned int vid;
	unsigned int sid;
    void main();
  public:
    NameServer(Printer &prt, unsigned int numVendingMachines, unsigned int numStudents);
	~NameServer();
    void VMregister(VendingMachine *vendingmachine);
    VendingMachine *getMachine(unsigned int id);
    VendingMachine **getMachineList();
};


#endif

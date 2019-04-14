#include "nameServer.h"
#include "printer.h"

NameServer::NameServer(Printer &prt, unsigned int numVendingMachines, unsigned int numStudents) :
								prt(prt), numVendingMachines(numVendingMachines), numStudents(numStudents)
{
	vendingMachines = new VendingMachine*[numVendingMachines];
	vmstudentmap = new unsigned int[numStudents];

	for (unsigned int i = 0; i < numVendingMachines; i++) {
		vendingMachines[i] = NULL;
	}
}

NameServer::~NameServer() {
	delete [] vendingMachines;
	delete [] vmstudentmap;
}

// Vending machines call VMregister to register themselves
// so students can subsequently locate them. 
void NameServer::VMregister(VendingMachine *vendingmachine) {
	vid = vendingmachine->getId();
	vendingMachines[vid] = vendingmachine;
}


// A student calls getMachine to find a vending machine,
VendingMachine *NameServer::getMachine(unsigned int id) {
	sid = id;                               // remember student id for administrator
	VendingMachine *machine = vendingMachines[vmstudentmap[sid]]; // get machine
	return machine;
}

VendingMachine **NameServer::getMachineList() {
	return vendingMachines;
}

void NameServer::main(){
	// print start message
	prt.print(Printer::NameServer, 'S');

	// assign vending machines to students
	for (unsigned int i = 0; i < numStudents; i++) {
		vmstudentmap[i] = i % numVendingMachines;
	}

	for(;;) {

		_Accept(~NameServer) { break; }
		or
		_Accept(VMregister) {
			// print machine registering message
			prt.print(Printer::NameServer, 'R', (int) vid);
		}
		or
		_Accept(getMachine) {
			// cycle through vendingmachines
			vmstudentmap[sid] = (vmstudentmap[sid] + 1) % numVendingMachines;
			// print new vending machine request message
			prt.print(Printer::NameServer, 'N', (int) sid, (int) vmstudentmap[sid]);
		}
		or
		_Accept(getMachineList) {}
	}

	// print finish message
	prt.print(Printer::NameServer, 'F');
}

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <string>

#include "config.h"
#include "MPRNG.h"
#include "printer.h"
#include "parent.h"
#include "bank.h"
#include "watcardOffice.h"
#include "watcard.h"
#include "student.h"
#include "groupoff.h"
#include "vendingMachine.h"
#include "nameServer.h"
#include "bottlingPlant.h"

#define INT_MAX 2147483647

MPRNG mprnGen;

using namespace std;


// Prints appropriate usage error messages
void usage( char *argv[] ) {
    cout << "Usage: " << argv[0] << " [ config-file [ Seed ] ]" << endl;
    exit( EXIT_FAILURE );
} // usage



// converts string to unsigned int
// and check for invalid paramaters
// eg: 12a, a23, aa, etc
bool converter(string str, unsigned int &val){
  try{
      size_t pos;
      val = stoul( str, &pos, 10 );
      return pos == str.size();       // check for invalid parameter
    }
    catch(invalid_argument& e){
        return false;
    }
}


void uMain::main(){

	unsigned int seed = getpid();
	string config_file = "soda.config";

  // Parse command line input parameters
  T1: {
      switch ( argc ){
        case 3:
          if(!converter(string(argv[2]), seed)) break;        // check for invalid parameter
        case 2:
          config_file = string(argv[1]);						// check for invalid parameter
        default:
          // check boundaries
          if (argc > 3    ||
              0 >= seed   || seed > INT_MAX )
          {
            break;
          }

          break T1;
        }

      // if error occurs, print usage message:
      usage( argv );
  }

  // initialize printer
  mprnGen.seed(seed);

  // read and parse the simulation configurations
  ConfigParms cparms;
  processConfigFile( config_file.c_str() , cparms );


  // initializations
  Printer prt(cparms.numStudents, cparms.numVendingMachines, cparms.numCouriers );

  Bank bank(cparms.numStudents);

  Parent parent(prt, bank, cparms.numStudents, cparms.parentalDelay);

  Groupoff groupoff(prt, cparms.numStudents, cparms.sodaCost, cparms.groupoffDelay );

  NameServer name_server(prt, cparms.numVendingMachines, cparms.numStudents );

  WATCardOffice w_office(prt, bank, cparms.numCouriers );

  VendingMachine* v_machine[cparms.numVendingMachines];
  for (unsigned int i = 0; i < cparms.numVendingMachines; i++){
  	v_machine[i] = new VendingMachine(prt, name_server, i, cparms.sodaCost, cparms.maxStockPerFlavour );
  }

  BottlingPlant *plant = new BottlingPlant(prt, name_server, cparms.numVendingMachines, cparms.maxShippedPerFlavour, cparms.maxStockPerFlavour, cparms.timeBetweenShipments);

 	Student* students[cparms.numStudents];
 	for (unsigned int i = 0; i < cparms.numStudents; i++){
 		students[i] = new Student(prt, name_server, w_office, groupoff, i, cparms.maxPurchases );
 	}


 	//wait students to finish
 	for (unsigned int i = 0; i < cparms.numStudents; i++){
 		delete students[i];
 	}

  // wait plant to shutdown, to avoid a deadlock
  delete plant;

 	//wait vending machines to finish
 	for (unsigned int i = 0; i < cparms.numVendingMachines; i++){
 		delete v_machine[i];
 	}
}

/* END */

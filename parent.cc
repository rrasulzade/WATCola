#include "parent.h"
#include "MPRNG.h"
#include "bank.h"
#include "printer.h"


// Parent constructor
Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay ) :
										prt(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay){}


void Parent::main(){
	// print start message
	prt.print(Printer::Parent, 'S');

	for(;;){
		_Accept( ~Parent ){
			break;
		}
		_Else{
			//the parent yields for parentalDelay times
			// before each gift is transferred
			yield(parentalDelay);

			unsigned int money = mprnGen(1,3);
			unsigned int student_id = mprnGen(numStudents-1);

			// deposit a gift to the student
			bank.deposit(student_id, money);

			// print deposit gift message
			prt.print(Printer::Parent, 'D', student_id, money);

		}
	}

	//print Finish message
	prt.print(Printer::Parent, 'F');
}


/* END */
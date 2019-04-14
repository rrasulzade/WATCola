#include "bank.h"


// Bank constructor
Bank::Bank( unsigned int numStudents ) : numStudents(numStudents){

	// each student’s account initially starts with a balance of $0.
	accounts = new unsigned int[numStudents];
	for (unsigned int i = 0; i < numStudents; i++){
		accounts[i] = 0;
	}
}


// Bank destructor
Bank::~Bank(){
	delete[] accounts;
}	


// endows gifts to a specific student 
void Bank::deposit( unsigned int id, unsigned int amount ){
	accounts[id] += amount;
}

// transfers money for a specific student.
void Bank::withdraw( unsigned int id, unsigned int amount ){
	accounts[id] -= amount;
}

/* END */
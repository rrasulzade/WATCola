#include "watcardOffice.h"
#include "MPRNG.h"
#include "printer.h"
#include "bank.h"

// WATCardOffice constructor: initialize couriers
WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) : 
  											prt(prt), bank(bank), numCouriers(numCouriers) {
    couriers = new Courier*[numCouriers];
    for (unsigned int i = 0; i < numCouriers; ++i){
    	couriers[i] = new Courier(i, bank, *this, prt );
    }
 }

// WATCardOffice destructor: delete couriers
 WATCardOffice::~WATCardOffice(){
	// wait couriers to finish their job
	for (unsigned int i = 0; i < numCouriers; ++i){
    	delete couriers[i];
    }
    delete[] couriers;
}	


// A student performs an asynchronous call to create to 
// create a “real” WATCard with an initial balance.
WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ){
	Job *j = new Job( Args( sid, amount, new WATCard() ) );
	jobs.push(j);
	return j->result;
}


// student performs an asynchronous call to transfer when its
// WATCard indicates there is insufficient funds to buy a soda
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ){
	Job *j = new Job( Args(sid, amount, card) );
	jobs.push(j);
	return j->result;
}


WATCardOffice::Job *WATCardOffice::requestWork(){
	Job *j = NULL;
	if(!jobs.empty()){
		j = jobs.front();
		jobs.pop();
	}
	return j;
}



void WATCardOffice::main(){
	// print start message
	prt.print(Printer::WATCardOffice, 'S');

	for(;;){
		_Accept( ~WATCardOffice ){
			// notify couriers to finish
			for (unsigned int i = 0; i < numCouriers; i++){
					jobs.push(NULL);
			}

			// wait for couriers
			while(!jobs.empty()){
				_Accept( requestWork );
			}

			break;
		} or _Accept( transfer ){

			// print transfer call complete
			prt.print(Printer::WATCardOffice, 'T', jobs.front()->args.student_id, jobs.front()->args.amount);

		} or _Accept( create ){

			// print create call complete
			prt.print(Printer::WATCardOffice, 'C', jobs.front()->args.student_id, jobs.front()->args.amount);

		} or _When( !jobs.empty() ) _Accept( requestWork ){

			// request work call complete
			prt.print(Printer::WATCardOffice, 'W');

		}	
	}	

	// print finish message
	prt.print(Printer::WATCardOffice, 'F');

}	


/********************************************** COURIER **********************************************/ 


 // Courier task constructor
WATCardOffice::Courier::Courier(unsigned int id, Bank &bank, WATCardOffice &office, Printer &prt) :
												bank(bank), office(office), prt(prt){}



void WATCardOffice::Courier::main(){

	// print starting message
	prt.print(Printer::Courier, (int) id, 'S');

	for(;;){

		// Each courier task calls requestWork, 
		// blocks until a Job request is ready
		Job *j = office.requestWork();
		if(j == NULL) break;

		// print start funds transfer
        prt.print(Printer::Courier, (int) id, 't',  j->args.student_id, j->args.amount);

		// sends a request through a courier to the bank
        bank.withdraw(j->args.student_id, j->args.amount);

        // courier updates the student’s WATCard
        j->args.w_card->deposit(j->args.amount);

        // 1 in 6 chance a courier loses a student’s WATCard
		if(mprnGen(5) == 0){
			// the exception WATCardOffice::Lost is inserted into the future
			j->result.exception(new Lost);

			// the current WATCard is deleted
			if(j->args.w_card != NULL){
				// delete j->args.w_card;
				j->args.w_card = NULL;
			}
		}
		else{
			// insert new WATCard into future
			j->result.delivery(j->args.w_card);
		}

		// print complete funds transfer
		prt.print(Printer::Courier, (int) id, 'T',  j->args.student_id, j->args.amount);

		delete j;
	}

	// print finishing message
	prt.print(Printer::Courier, (int) id, 'F');
}

/* END */
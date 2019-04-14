#include "groupoff.h"

#include "printer.h"
#include "MPRNG.h"


Groupoff::Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ) :
							prt(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {}


// creates empty future WATCard and returns to student
WATCard::FWATCard Groupoff::giftCard(){
	WATCard::FWATCard tmp_card;
	g_cards.push_back(tmp_card);
	return g_cards[g_cards.size()-1];
}


void Groupoff::main(){
	// print start message
	prt.print(Printer::Groupoff, 'S');

	// task begins by accepting a call from each 
	// student to obtain a future gift-card.
	for (unsigned int i = 0; i < numStudents; ++i){
		_Accept( giftCard );
	}

	for(;;){
		_Accept( ~Groupoff ){
			break;
		} 
		_Else{
			// groupoff yields for groupoffDelay times
			yield(groupoffDelay);

			if(g_cards.size() > 0){
				// Then groupoff periodically puts a real WATCard with 
				// value SodaCost into a random future gift-card.
				WATCard *w_card = new WATCard;
				w_card->deposit(sodaCost);

				// print deposit gift
				prt.print(Printer::Groupoff, 'D', sodaCost);
			
				unsigned int sid = mprnGen(g_cards.size()-1);

				g_cards[sid].delivery(w_card);

				// A future gift-card is assigned only once, need to remove it
				g_cards.erase(g_cards.begin() + sid );
			}
		}
	}

	// print Finish message
	prt.print(Printer::Groupoff, 'F');	
}

/* END */
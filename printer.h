#ifndef __Printer_H__
#define __Printer_H__


_Monitor Printer {
	unsigned int numStudents;
	unsigned int numVendingMachines;
	unsigned int numCouriers;
	unsigned int total;

  public:
    enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );

  private:

  	struct info {
    	bool overwrite;
    	unsigned int numBlocked;
    	Kind kind;
    	char state;
    	unsigned int lid;
    	int value1, value2;

        info();
    };

    struct info *buffer;
    void flush(bool isFinished);
    void helper_print(Kind kind, char state, unsigned int lid, int value1, int value2);
    
};


#endif
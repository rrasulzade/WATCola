// Concurrent-Safe Pseudo Random-Number Generator : generate repeatable sequence of values that
//   appear random by scrambling the bits of an integer value.

#ifndef __MPRNG_H__
#define __MPRNG_H__

_Monitor MPRNG {
  public:
    MPRNG( unsigned int seed = 1009 ) { srand( seed ); }  // set seed
    void seed( unsigned int seed ) { srand( seed ); }   // set seed
    unsigned int operator()() { return rand(); }      // [0,UINT_MAX]
    unsigned int operator()( unsigned int u ) { return operator()() % (u + 1); } // [0,u]
    unsigned int operator()( unsigned int l, unsigned int u ) { return operator()( u - l ) + l; } // [l,u]
}; // MPRNG


// global random number generator
extern MPRNG mprnGen;

#endif // __MPRNG_H__

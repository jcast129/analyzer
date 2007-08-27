
#ifndef ROOT_THaDetMap
#define ROOT_THaDetMap

//////////////////////////////////////////////////////////////////////////
//
// THaDetMap
//
// Standard detector map for a Hall A detector.
// The detector map defines the hardware channels that correspond to a
// single detector. Typically, "channels" are Fastbus addresses 
// characterized by
//
//   Crate, Slot, array of channels
//
//////////////////////////////////////////////////////////////////////////

#include "Rtypes.h"
#include <vector>

class THaDetMap {

public:
  struct Module {
    UShort_t crate;
    UShort_t slot;
    UShort_t lo;
    UShort_t hi;
    UInt_t   first;  // logical number of first channel
    UInt_t   model;  // model number of module (for ADC/TDC identification).
                     // Upper two bytes of model -> ADC/TDC-ness of module
    Int_t   refindex;  // for pipeline TDCs: index to a reference channel
    Double_t resolution; // Resolution (s/chan) for TDCs
  };

  // Flags for GetMinMaxChan()
  enum ECountMode {
    kLogicalChan         = 0,
    kRefIndex            = 1
  };
  // Flags for Fill()
  enum EFillFlags {
    kDoNotClear          = BIT(0),    // Don't clear the map first
    kFillLogicalChannel  = BIT(10),   // Parse the logical channel number
    kFillModel           = BIT(11),   // Parse the model number
    kFillRefIndex        = BIT(12)    // Parse the reference index
  };

  THaDetMap();
  THaDetMap( const THaDetMap& );
  THaDetMap& operator=( const THaDetMap& );
  virtual ~THaDetMap();
  
  virtual Int_t     AddModule( UShort_t crate, UShort_t slot, 
			       UShort_t chan_lo, UShort_t chan_hi,
			       UInt_t first=0, UInt_t model=0,
			       Int_t refindex=-1 );
          void      Clear()  { fNmodules = 0; }
  virtual Int_t     Fill( const std::vector<int>& values, UInt_t flags = 0 );
          void      GetMinMaxChan( Int_t& min, Int_t& max,
				   ECountMode mode = kLogicalChan ) const;
          Module*   GetModule( UShort_t i ) const { return (Module*)fMap+i; }
          Int_t     GetNchan( UShort_t i ) const;
          Int_t     GetTotNumChan() const;
          Int_t     GetSize() const { return static_cast<Int_t>(fNmodules); }

          UInt_t    GetModel( UShort_t i ) const;
          Bool_t    IsADC( UShort_t i ) const;
          Bool_t    IsTDC( UShort_t i ) const;
  static  UInt_t    GetModel( Module* d );
  static  Bool_t    IsADC( Module* d );
  static  Bool_t    IsTDC( Module* d );

  virtual void      Print( Option_t* opt="" ) const;
  virtual void      Reset();

  static const int kDetMapSize = (1<<16)-1;  //Sanity limit on map size

protected:
  UShort_t     fNmodules;  // Number of modules (=crate,slot) in the map

  Module*      fMap;       // Array of modules, each module is a 7-tuple
                           // (create,slot,chan_lo,chan_hi,first_logical,
                           // model,refindex)

  Int_t        fMaplength; // current size of the fMap array
  
  static const UInt_t kADCBit = BIT(31);
  static const UInt_t kTDCBit = BIT(30);
  static const UInt_t kModelMask = 0x0000ffff;
  
  ClassDef(THaDetMap,0)   //The standard detector map
};

inline Bool_t THaDetMap::IsADC(Module* d) {
  return d->model & kADCBit;
}

inline Bool_t THaDetMap::IsTDC(Module* d) {
  return d->model & kTDCBit;
}

inline UInt_t THaDetMap::GetModel(Module* d) {
  return d->model & kModelMask;
}

inline Bool_t THaDetMap::IsADC( UShort_t i ) const {
  return IsADC(GetModule(i));
}

inline Bool_t THaDetMap::IsTDC( UShort_t i ) const {
  return IsTDC(GetModule(i));
}

inline UInt_t THaDetMap::GetModel( UShort_t i) const {
  return GetModel(GetModule(i));
}

inline Int_t THaDetMap::GetNchan( UShort_t i ) const {
  // Return the number of channels of the i-th module
  return (fMap+i)->hi - (fMap+i)->lo + 1;
}

#endif

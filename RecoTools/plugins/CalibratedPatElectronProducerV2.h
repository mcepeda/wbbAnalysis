#ifndef CalibratedPatElectronProducerV2_h
#define CalibratedPatElectronProducerV2_h

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

class CalibratedPatElectronProducerV2: public edm::EDProducer
 {
  public:

    //static void fillDescriptions( edm::ConfigurationDescriptions & ) ;

    explicit CalibratedPatElectronProducerV2( const edm::ParameterSet & ) ;
    virtual ~CalibratedPatElectronProducerV2();
    virtual void produce( edm::Event &, const edm::EventSetup & ) ;

  private:

    edm::InputTag inputPatElectrons ;
    std::string dataset ;
    bool isAOD ;
    bool isMC ;
    bool updateEnergyError ;
    bool debug ;
    uint energyMeasurementType;
    
 } ;

#endif

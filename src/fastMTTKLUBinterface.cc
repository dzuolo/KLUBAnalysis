#include "fastMTTKLUBinterface.h"

#define DEBUG false

// Constructor
fastMTTKLUBinterface::fastMTTKLUBinterface (TLorentzVector tau1, TLorentzVector tau2, TLorentzVector met, TMatrixD met_cov, int pairType, int DM1, int DM2)
{
  if (DEBUG)
  {
    std::cout << "insid tau1: " << tau1.Pt() << " " << tau1.Eta() << " " << tau1.Phi() << " " << tau1.E() << " " << tau1.Px() << " " << tau1.Py() << std::endl;
    std::cout << "insid tau2: " << tau2.Pt() << " " << tau2.Eta() << " " << tau2.Phi() << " " << tau2.E() << " " << tau2.Px() << " " << tau2.Py() << std::endl;
    std::cout << "inside met: " << met.Pt()  << " " << met.Eta()  << " " << met.Phi()  << " " << met.Px() << " " << met.Py()  << std::endl;
  }

  // MET
  covMET_.ResizeTo(2,2);
  covMET_(0,0) = met_cov(0,0);
  covMET_(0,1) = met_cov(0,1);
  covMET_(1,0) = met_cov(1,0);
  covMET_(1,1) = met_cov(1,1);
  METx_   = met.Px();
  METy_   = met.Py();

  // Leptons
  double mass1, mass2;
  int decay1, decay2;
  classic_svFit::MeasuredTauLepton::kDecayType l1Type, l2Type;

  if (pairType == 0) // MuTau
  {
    l1Type = classic_svFit::MeasuredTauLepton::kTauToMuDecay;
    mass1  = 105.658e-3;
    decay1 = -1;
    l2Type = classic_svFit::MeasuredTauLepton::kTauToHadDecay;
    mass2  = tau2.M();
    decay2 = DM2;
  }

  else if (pairType == 1) // EleTau
  {
    l1Type = classic_svFit::MeasuredTauLepton::kTauToElecDecay;
    mass1  = 0.51100e-3;
    decay1 = -1;
    l2Type = classic_svFit::MeasuredTauLepton::kTauToHadDecay;
    mass2  = tau2.M();
    decay2 = DM2;
  }

  else // TauTau
  {
    l1Type = classic_svFit::MeasuredTauLepton::kTauToHadDecay;
    mass1  = tau1.M();
    decay1 = DM1;
    l2Type = classic_svFit::MeasuredTauLepton::kTauToHadDecay;
    mass2  = tau2.M();
    decay2 = DM2; 
  }

  // Fill the measuredTauLeptons
  measuredTauLeptons_.push_back(classic_svFit::MeasuredTauLepton(l1Type, tau1.Pt(), tau1.Eta(), tau1.Phi(), mass1, decay1));
  measuredTauLeptons_.push_back(classic_svFit::MeasuredTauLepton(l2Type, tau2.Pt(), tau2.Eta(), tau2.Phi(), mass2, decay2));
  if (DEBUG)
  {
    std::cout << "measured1    : " << measuredTauLeptons_.at(0).pt() << " " << measuredTauLeptons_.at(0).eta() << " " << measuredTauLeptons_.at(0).phi() << " " << measuredTauLeptons_.at(0).energy() << " " << measuredTauLeptons_.at(0).px() << " " << measuredTauLeptons_.at(0).py() << std::endl;
    std::cout << "measured2    : " << measuredTauLeptons_.at(1).pt() << " " << measuredTauLeptons_.at(1).eta() << " " << measuredTauLeptons_.at(1).phi() << " " << measuredTauLeptons_.at(1).energy() << " " << measuredTauLeptons_.at(1).px() << " " << measuredTauLeptons_.at(1).py() << std::endl;
  }
}


// Destructor
fastMTTKLUBinterface::~fastMTTKLUBinterface() {}


// FitAndGetResult
LorentzVector fastMTTKLUBinterface::FitAndGetResult()
{

  // Declare algo
  FastMTT algo;

  algo.run(measuredTauLeptons_, METx_, METy_, covMET_);
  LorentzVector ttp4 = algo.getBestP4();

  return ttp4;
}

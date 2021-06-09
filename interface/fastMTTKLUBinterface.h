#ifndef fastMTTKLUBinterface_h
#define fastMTTKLUBinterface_h

// -------------------------------------------------------------------------------------------------------------- //
//                                                                                                                //
//   class fastMTTKLUBinterface                                                                                   //
//                                                                                                                //
//   Class to compute FastMTT:                                                                                    //
//    - https://github.com/SVfit/ClassicSVfit/blob/fastMTT_19_02_2019/                                            //
//                                                                                                                //
//   Author: Davide Zuolo (Milano-Bicocca)                                                                        //
//   Date  : June 2021                                                                                            //
//                                                                                                                //
// -------------------------------------------------------------------------------------------------------------- //

// Standard libraries
#include <vector>
#include <string>
#include <cmath>

// ROOT libraries
#include <TLorentzVector.h>
#include "TMatrixD.h"

// ClassicSVfit libraries
#include "../../TauAnalysis/ClassicSVfit/interface/FastMTT.h"
#include "../../TauAnalysis/ClassicSVfit/interface/ClassicSVfit.h"
#include "../../TauAnalysis/ClassicSVfit/interface/MeasuredTauLepton.h"

// fastMTTKLUBinterface class
class fastMTTKLUBinterface {

  public:
    fastMTTKLUBinterface (TLorentzVector tau1, TLorentzVector tau2, TLorentzVector met, TMatrixD met_cov, int pairType, int DM1, int DM2);
    ~fastMTTKLUBinterface ();

    LorentzVector FitAndGetResult();

  private:
    std::vector<classic_svFit::MeasuredTauLepton> measuredTauLeptons_;
    double METx_;
    double METy_;
    TMatrixD covMET_;

};

#endif // fastMTTKLUBinterface_h

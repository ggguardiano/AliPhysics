#ifndef DEFORMATION__H
#define DEFORMATION__H
#include "AliAnalysisTaskSE.h"
#include "TComplex.h"
#include "AliEventCuts.h"
#include "AliVEvent.h"
#include "AliGFW.h"
#include "AliPID.h"
#include "AliMCEvent.h"
#include "AliGFWCuts.h"
#include "TString.h"
#include "AliProfileBS.h"
#include "AliCkContainer.h"
#include "TRandom.h"
#include "AliAODTracklets.h"
#include "AliAODVZERO.h"
#include "AliESDtrack.h"
#include "AliESDtrackCuts.h"
#include "AliESDEvent.h"
#include "AliVMultiplicity.h"
#include "AliPtContainer.h"


class TList;
class TH1D;
class TH2D;
class TH3D;
class TProfile;
class TProfile2D;
class TComplex;
class AliAODEvent;
class AliVTrack;
class AliVVertex;
class AliInputEventHandler;
class AliAODTrack;
class TClonesArray;
class AliAODVertex;
class AliAnalysisUtils;
class TProfile;
class AliGFWWeights;
class AliVParticle;
class AliGFWCuts;
class AliGFWFlowContainer;
class AliPIDResponse;
class AliPIDCombined;

namespace EFF_FLAG {
    enum {
      noeff = 1,
      consteff = 2,
      gausseff = 4,
      flateff = 8,
      powereff = 16,
      inputeff = 32
    };
}
enum {kCh = 0, kPi = 1, kKa = 2, kPr = 4};
class AliAnalysisTaskDeform : public AliAnalysisTaskSE {
 public:
  AliAnalysisTaskDeform();
  AliAnalysisTaskDeform(const char *name, Bool_t IsMC=kTRUE, TString StageSwitch="", TString ContainerSubfix="", Int_t Nkeys = 1);
  virtual ~AliAnalysisTaskDeform();
  virtual void UserCreateOutputObjects();
  virtual void NotifyRun();
  virtual void UserExec(Option_t *option);
  virtual void Terminate(Option_t *);
  Bool_t CheckTrigger(Double_t);
  Bool_t AcceptAOD(AliAODEvent*, Double_t lvtxXYZ[3]);
  void SetTriggerType(UInt_t newval) {fTriggerType = newval; };
  void SetEventCutFlag(Int_t newval) { fEventCutFlag = newval; };
  void FillWeights(AliAODEvent*, const Double_t &vz, const Double_t &l_Cent, Double_t *vtxp);
  void FillWeightsMC(AliAODEvent*, const Double_t &vz, const Double_t &l_Cent, Double_t *vtxp);
  void FillSpectraMC(AliAODEvent *fAOD, const Double_t &vz, const Double_t &l_Cent, Double_t *vtxp);
  //void ProduceEfficiencies(AliESDEvent *fAOD, const Double_t &vz, const Double_t &l_Cent, Double_t *vtxp);
  void VnMpt(AliAODEvent *fAOD, const Double_t &vz, const Double_t &l_Cent, Double_t *vtxp);
  Int_t GetStageSwitch(TString instr);
  AliGFW::CorrConfig GetConf(TString head, TString desc, Bool_t ptdif) { return fGFW->GetCorrelatorConfig(desc,head,ptdif);};
  void CreateCorrConfigs();
  void LoadWeightAndMPT();
  void GetSingleWeightFromList(AliGFWWeights **inWeights, TString pf="");
  void FillWPCounter(Double_t[5], Double_t, Double_t);
  void FillWPCounter(vector<vector<double>> &inarr, double w, double p);
  void FillWPCounter(vector<vector<double>> &inarr, vector<double> w, double p);
  Bool_t LoadMyWeights(const Int_t &lRunNo = 0);
  Int_t GetBayesPIDIndex(AliVTrack*);
  Int_t GetPIDIndex(const Int_t &pdgcode);
  void SetDisablePID(Bool_t newval) { fDisablePID = newval; };
  void SetPtBins(Int_t nBins, Double_t *ptbins);
  void SetEtaBins(Int_t nBins, Double_t *etabins);
  void SetMultiBins(Int_t nBins, Double_t *multibins);
  void SetV0MBins(Int_t nBins, Double_t *multibins);
  void SetNchV0M(Double_t centMin, Double_t centMax) { fV0MCentMin = centMin; fV0MCentMax = centMax; fUseNchInV0M = true; };
  void SetV2dPtMultiBins(Int_t nBins, Double_t *multibins);
  void SetEta(Double_t newval) { fEta = newval; fEtaLow=-9999; };
  void SetEta(Double_t etaLow, Double_t etaHigh) { fEtaLow = etaLow; fEta = etaHigh; };
  void SetEtaNch(Double_t newval) { fEtaNch = newval; };
  void SetEtaV2Sep(Double_t newval) { fEtaV2Sep = newval; };
  void SetChargedPt(Double_t chPtMin, Double_t chPtMax) { fUseChargedPtCut = true; fchPtMin = chPtMin; fchPtMax = chPtMax; }
  void SetUseNch(Bool_t newval) { fUseNch = newval; };
  void SetUseWeightsOne(Bool_t newval) { fUseWeightsOne = newval; };
  void ExtendV0MAcceptance(Bool_t newval) { fExtendV0MAcceptance = newval; };
  void SetSystFlag(Int_t newval) { if(!fGFWSelection) fGFWSelection = new AliGFWCuts(); fGFWSelection->SetupCuts(newval); }; //Flag for systematics
  void SetConsistencyFlag(UInt_t newval) { fConsistencyFlag = newval; };
  void SetCentralityEstimator(TString newval) { if(fCentEst) delete fCentEst; fCentEst = new TString(newval); };
  void SetContSubfix(TString newval) {if(fContSubfix) delete fContSubfix; fContSubfix = new TString(newval); };
  void OverrideMCFlag(Bool_t newval) { fIsMC = newval; };
  Int_t GetNtotTracks(AliAODEvent*, const Double_t &ptmin, const Double_t &ptmax, Double_t *vtxp);
  Int_t GetNtotMCTracks(const Double_t &ptmin, const Double_t &ptmax);
  void SetUseRecoNchForMC(Bool_t newval) { fUseRecoNchForMC = newval; };
  void SetNBootstrapProfiles(Int_t newval) {if(newval<0) {printf("Number of subprofiles cannot be < 0!\n"); return; }; fNBootstrapProfiles = newval; };
  void SetWeightSubfix(TString newval) { fWeightSubfix=newval; }; //base (runno) + subfix (systflag), delimited by ;. First argument always base, unless is blank. In that case, w{RunNo} is used for base.
  void SetPseudoEfficiency(Double_t newval) {fPseudoEfficiency = newval; };
  void SetNchCorrelationCut(Double_t l_slope=1, Double_t l_offset=0, Bool_t l_enable=kTRUE) { fCorrPar[0] = l_slope; fCorrPar[1] = l_offset; fUseCorrCuts = l_enable; };
  Bool_t CheckNchCorrelation(const Int_t &lNchGen, const Int_t &lNchRec) { return (fCorrPar[0]*lNchGen + fCorrPar[1] < lNchRec); };
  void SetBypassTriggerAndEventCuts(Bool_t newval) { fBypassTriggerAndEvetCuts = newval; };
  void SetV0PUCut(TString newval) { if(fV0CutPU) delete fV0CutPU; fV0CutPU = new TF1("fV0CutPU", newval.Data(), 0, 100000); };
  void SetEventWeight(unsigned int weight) { fEventWeight = weight; };
  void SetUse15oPass2PU(bool use) { fUSe15opass2PU = use; };
  void SetRequirePositiveCharge(bool newval) {fRequirePositive = newval;};
  void SetUse2DEfficiencies(bool newval) {fUse2DEff = newval;};
  void SetEfficiencyIndex(UInt_t newval) {fEfficiencyIndex = newval;}
  void SetOnTheFly(bool newval) {fOnTheFly = newval;}
  void SetFillMptPowers(bool newval) { fFillMptPowers = newval; }
  void SetUseCentralityForOTF(bool newval) { fUseCentralityOTF = newval; }
  void SetIPBins(Int_t nBins, Double_t *multibins);
  void SetUsePIDNUA(bool newval) { fUsePIDNUA = newval; }
 protected:
  AliEventCuts fEventCuts;
 private:
  AliAnalysisTaskDeform(const AliAnalysisTaskDeform&);
  AliAnalysisTaskDeform& operator=(const AliAnalysisTaskDeform&);
  Int_t fStageSwitch;
  Int_t fSystFlag;
  Int_t fEventCutFlag; //0 for standard AliEventCuts; 1 for LHC15o pass2; 2 for LHC18qr pass3
  TString *fContSubfix;
  TString *fCentEst;
  Bool_t fExtendV0MAcceptance;
  Bool_t fIsMC;
  Bool_t fBypassTriggerAndEvetCuts;
  Bool_t fUSe15opass2PU;
  Bool_t fOnTheFly;
  Bool_t fUseCentralityOTF;
  AliMCEvent *fMCEvent; //! MC event
  Bool_t fUseRecoNchForMC; //Flag to use Nch from reconstructed, when running MC closure
  TRandom *fRndm; 
  Int_t fNBootstrapProfiles; //Number of profiles for bootstrapping
  TAxis *fPtAxis;
  TAxis *fEtaAxis;
  TAxis *fMultiAxis;      //Multiplicity axis (either for V0M or Nch)
  TAxis *fIPAxis;      //Impact parameter axis for on-the-fly
  TAxis *fV0MMultiAxis;   //Defaults V0M bins
  Double_t *fPtBins; //!
  Int_t fNPtBins; //!
  Int_t fNEtaBins; //!
  Double_t *fEtaBins;
  Double_t *fMultiBins; //!
  Int_t fNMultiBins; //!
  Double_t fV0MCentMin;
  Double_t fV0MCentMax;
  Bool_t fUseNchInV0M;
  Bool_t fUseNch;
  Bool_t fUseWeightsOne;
  Double_t fEta;
  Double_t fEtaLow;
  Double_t fEtaNch;
  Double_t fEtaV2Sep; 
  Double_t fchPtMin;
  Double_t fchPtMax;
  Bool_t fUseChargedPtCut;
  AliPIDResponse *fPIDResponse; //!
  AliPIDCombined *fBayesPID; //!
  TList *fQAList; //
  TH1D* fEventCount; //!
  TH1D *fMultiDist;
  TH1D *fIPDist;
  TH1D* fChPtDist; //!
  TH2D **fMultiVsV0MCorr; //!
  TH2D *fNchTrueVsReco; //!
  TH2D *fESDvsFB128;
  TList *fptVarList;
  AliCkContainer **fCkCont;
  AliPtContainer  **fPtCont;
  TList *fCovList;
  TList *fV2dPtList;
  static const int Ncovpfs = 14;
  AliProfileBS **fCovariance; //!
  AliProfileBS **fCovariancePowerMpt; //!
  AliProfileBS **fMpt; //
  UInt_t fTriggerType;
  TList *fWeightList; //!
  AliGFWWeights **fWeights;//! This should be stored in TList
  TList *fSpectraList; //!
  TH3D **fSpectraGen; //!
  TH3D **fSpectraRec; //!
  TH2D **fDetectorResponse; //!
  TString fWeightSubfix;
  Int_t fRunNo; //!
  AliGFWCuts *fGFWSelection;
  AliGFWCuts *fGFWNtotSelection;
  AliGFWFlowContainer *fFC;
  AliGFW *fGFW; //! not stored
  vector<AliGFW::CorrConfig> corrconfigs; //! do not store
  TList *fEfficiencyList;
  vector<vector<TH2D*>> fEfficiency; //TH2Ds for efficiency calculation
  TH1D **fEfficiencies; //TH1Ds for picking up efficiencies
  Double_t fPseudoEfficiency; //Pseudo efficiency to reject tracks. Default value set to 2, only used when the value is <1
  TH3D *fPtvsCentvsPower; //!
  TH1D *fPtDist; //!
  TH3D *fDCAxyVsPt_noChi2;
  TH2D *fWithinDCAvsPt_withChi2;
  TH3D *fDCAxyVsPt_withChi2;
  TH2D *fWithinDCAvsPt_noChi2;
  TH1D *fV0MMulti;
  TH2D *fITSvsTPCMulti;
  TH1D *fV2dPtMulti;
  Double_t fCorrPar[2]; //Yes need to store
  Bool_t fUseCorrCuts; //Yes need to store
  TF1 *fSPDCutPU; //Store these
  TF1 *fV0CutPU; //Store these
  TF1 *fCenCutLowPU; //Store these
  TF1 *fCenCutHighPU; //Store these
  TF1 *fMultCutPU; //Store these
  Double_t fImpactParameterMC;
  int EventNo;
  unsigned int fEventWeight; 
  vector<vector<vector<double>>>  wpPt;
  std::map<double,double> centralitymap;  
  AliESDtrackCuts *fStdTPCITS2011; //Needed for counting tracks for custom event cuts
  Bool_t FillFCs(const AliGFW::CorrConfig &corconf, const Double_t &cent, const Double_t &rndmn, const Bool_t deubg=kFALSE);
  Bool_t Fillv2dPtFCs(const AliGFW::CorrConfig &corconf, const Double_t &dpt, const Double_t &rndmn, const Int_t index);
  Bool_t FillCovariance(AliProfileBS* target, const AliGFW::CorrConfig &corconf, const Double_t &cent, const Double_t &d_mpt, const Double_t &dw_mpt, const Double_t &l_rndm);
  Bool_t AcceptAODTrack(AliAODTrack *lTr, Double_t*, const Double_t &ptMin=0.5, const Double_t &ptMax=2, Double_t *vtxp=0);
  Bool_t AcceptAODTrack(AliAODTrack *lTr, Double_t*, const Double_t &ptMin, const Double_t &ptMax, Double_t *vtxp, Int_t &nTot);
  Bool_t AcceptESDTrack(AliESDtrack *lTr, UInt_t&, Double_t*, const Double_t &ptMin=0.5, const Double_t &ptMax=2, Double_t *vtxp=0);
  Bool_t AcceptESDTrack(AliESDtrack *lTr, UInt_t&, Double_t*, const Double_t &ptMin, const Double_t &ptMax, Double_t *vtxp, Int_t &nTot);
  Bool_t AcceptCustomEvent(AliAODEvent*);
  Bool_t AcceptCustomEvent(AliESDEvent*);
  AliMCEvent *getMCEvent();
  double getAMPTCentrality();
  Double_t getEfficiency(double &lpt, int iCent);
  vector<Double_t> getPowerEfficiency(double &lpt, int iCent);
  Bool_t fDisablePID;
  UInt_t fConsistencyFlag;
  UInt_t fEfficiencyIndex;
  Bool_t fRequireReloadOnRunChange;
  Bool_t fRequirePositive;
  Bool_t fUse2DEff;
  Bool_t fUsePIDNUA;
  Bool_t fFillMptPowers;
  Double_t *GetBinsFromAxis(TAxis *inax);
  ClassDef(AliAnalysisTaskDeform,1);
};

#endif
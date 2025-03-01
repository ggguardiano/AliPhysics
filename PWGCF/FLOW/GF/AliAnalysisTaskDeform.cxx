#include "AliAnalysisTaskDeform.h"
#include "AliEventCuts.h"
#include "AliAnalysisManager.h"
#include "AliAnalysisFilter.h"
#include "AliAnalysisUtils.h"
#include "AliVEvent.h"
#include "AliAODEvent.h"
#include "AliESDEvent.h"
#include "AliVTrack.h"
#include "AliVParticle.h"
#include "AliAODTrack.h"
#include "AliESDtrack.h"
#include "AliVVertex.h"
#include "AliAODVertex.h"
#include "AliCentrality.h"
#include "AliExternalTrackParam.h"
#include "AliMultSelection.h"
#include "AliInputEventHandler.h"
#include "TList.h"
#include "TProfile.h"
#include "AliEventCuts.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "AliStack.h"
#include "TClonesArray.h"
#include "TProfile.h"
#include "AliGFWWeights.h"
#include "AliGFWFlowContainer.h"
#include "AliGFW.h"
#include "TClonesArray.h"
#include "AliGFWCuts.h"
#include "AliAODMCParticle.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TNamed.h"
#include "AliPID.h"
#include "AliPIDResponse.h"
#include "AliPIDCombined.h"
#include "AliAODMCHeader.h"
#include "AliGenEventHeader.h"
#include "AliCollisionGeometry.h"
#include "AliGenHijingEventHeader.h"

ClassImp(AliAnalysisTaskDeform);

AliAnalysisTaskDeform::AliAnalysisTaskDeform():
  AliAnalysisTaskSE(),
  fStageSwitch(0),
  fSystFlag(0),
  fEventCutFlag(0),
  fContSubfix(0),
  fCentEst(0),
  fExtendV0MAcceptance(kTRUE),
  fIsMC(kFALSE),
  fBypassTriggerAndEvetCuts(kFALSE),
  fUSe15opass2PU(kFALSE),
  fOnTheFly(false),
  fUseCentralityOTF(true),
  fMCEvent(0),
  fUseRecoNchForMC(kFALSE),
  fRndm(0),
  fNBootstrapProfiles(10),
  fPtAxis(0),
  fEtaAxis(0),
  fMultiAxis(0),
  fIPAxis(0),
  fV0MMultiAxis(0),
  fPtBins(0),
  fNPtBins(0),
  fNEtaBins(0),
  fEtaBins(0),
  fMultiBins(0),
  fNMultiBins(0),
  fV0MCentMin(0),
  fV0MCentMax(90),
  fUseNchInV0M(kFALSE),
  fUseNch(kFALSE),
  fUseWeightsOne(kFALSE),
  fEta(0.4),
  fEtaLow(-9999),
  fEtaNch(0.8),
  fEtaV2Sep(0.4),
  fchPtMin(0.2),
  fchPtMax(3.0),
  fUseChargedPtCut(false),
  fPIDResponse(0),
  fBayesPID(0),
  fQAList(0),
  fEventCount(0),
  fMultiDist(0),
  fIPDist(0),
  fChPtDist(0),
  fMultiVsV0MCorr(0),
  fNchTrueVsReco(0),
  fESDvsFB128(0),
  fptVarList(0),
  fCkCont(0),
  fPtCont(0),
  fCovList(0),
  fV2dPtList(0),
  fCovariance(0),
  fCovariancePowerMpt(0),
  fMpt(0),
  fTriggerType(AliVEvent::kMB+AliVEvent::kINT7),
  fWeightList(0),
  fWeights(0),
  fSpectraList(0),
  fSpectraGen(0),
  fSpectraRec(0),
  fDetectorResponse(0),
  fRunNo(0),
  fGFWSelection(0),
  fGFWNtotSelection(0),
  fFC(0),
  fGFW(0),
  fEfficiencyList(0),
  fEfficiency(0),
  fEfficiencies(0),
  fPseudoEfficiency(2.),
  fPtvsCentvsPower(0),
  fPtDist(0),
  fDCAxyVsPt_noChi2(0),
  fWithinDCAvsPt_withChi2(0),
  fDCAxyVsPt_withChi2(0),
  fWithinDCAvsPt_noChi2(0),
  fV0MMulti(0),
  fITSvsTPCMulti(0),
  fV2dPtMulti(0),
  fSPDCutPU(0),
  fV0CutPU(0),
  fCenCutLowPU(0),
  fCenCutHighPU(0),
  fMultCutPU(0),
  fImpactParameterMC(-1.0),
  fStdTPCITS2011(0),
  fDisablePID(kTRUE),
  fConsistencyFlag(3),
  fEfficiencyIndex(0),
  fRequireReloadOnRunChange(kFALSE),
  fRequirePositive(kFALSE),
  fUse2DEff(kFALSE),
  fUsePIDNUA(kFALSE),
  fFillMptPowers(kFALSE),
  EventNo(0),
  fEventWeight(PtSpace::kOne),
  wpPt(0)
{
};
AliAnalysisTaskDeform::AliAnalysisTaskDeform(const char *name, Bool_t IsMC, TString stageSwitch, TString ContSubfix, int Nkeys):
  AliAnalysisTaskSE(name),
  fStageSwitch(0),
  fSystFlag(0),
  fEventCutFlag(0),
  fContSubfix(0),
  fCentEst(0),
  fExtendV0MAcceptance(kTRUE),
  fIsMC(IsMC),
  fBypassTriggerAndEvetCuts(kFALSE),
  fUSe15opass2PU(kFALSE),
  fOnTheFly(false),
  fUseCentralityOTF(true),
  fMCEvent(0),
  fUseRecoNchForMC(kFALSE),
  fNBootstrapProfiles(10),
  fRndm(0),
  fPtAxis(0),
  fEtaAxis(0),
  fMultiAxis(0),
  fIPAxis(0),
  fV0MMultiAxis(0),
  fPtBins(0),
  fNPtBins(0),
  fNEtaBins(0),
  fEtaBins(0),
  fMultiBins(0),
  fNMultiBins(0),
  fV0MCentMin(0),
  fV0MCentMax(90),
  fUseNchInV0M(kFALSE),
  fUseNch(kFALSE),
  fUseWeightsOne(kFALSE),
  fEta(0.4),
  fEtaLow(-9999),
  fEtaNch(0.8),
  fEtaV2Sep(0.4),
  fchPtMin(0.2),
  fchPtMax(3.0),
  fUseChargedPtCut(false),
  fPIDResponse(0),
  fBayesPID(0),
  fQAList(0),
  fEventCount(0),
  fMultiDist(0),
  fIPDist(0),
  fChPtDist(0),
  fMultiVsV0MCorr(0),
  fNchTrueVsReco(0),
  fESDvsFB128(0),
  fptVarList(0),
  fCkCont(0),
  fPtCont(0),
  fCovList(0),
  fV2dPtList(0),
  fCovariance(0),
  fCovariancePowerMpt(0),
  fMpt(0),
  fTriggerType(AliVEvent::kMB+AliVEvent::kINT7),
  fWeightList(0),
  fWeights(0),
  fSpectraList(0),
  fSpectraGen(0),
  fSpectraRec(0),
  fDetectorResponse(0),
  fRunNo(0),
  fGFWSelection(0),
  fGFWNtotSelection(0),
  fFC(0),
  fGFW(0),
  fEfficiencyList(0),
  fEfficiency(0),
  fEfficiencies(0),
  fPseudoEfficiency(2.),
  fPtvsCentvsPower(0),
  fPtDist(0),
  fDCAxyVsPt_noChi2(0),
  fWithinDCAvsPt_withChi2(0),
  fDCAxyVsPt_withChi2(0),
  fWithinDCAvsPt_noChi2(0),
  fV0MMulti(0),
  fITSvsTPCMulti(0),
  fV2dPtMulti(0),
  fSPDCutPU(0),
  fV0CutPU(0),
  fCenCutLowPU(0),
  fCenCutHighPU(0),
  fMultCutPU(0),
  fImpactParameterMC(-1.0),
  fStdTPCITS2011(0),
  fDisablePID(kTRUE),
  fConsistencyFlag(3),
  fEfficiencyIndex(0),
  fRequireReloadOnRunChange(kFALSE),
  fRequirePositive(kFALSE),
  fUse2DEff(kFALSE),
  fUsePIDNUA(kFALSE),
  fFillMptPowers(kFALSE),
  EventNo(0),
  fEventWeight(PtSpace::kOne),
  wpPt(0)
{
  fStageSwitch = GetStageSwitch(stageSwitch);
  SetContSubfix(ContSubfix);
  fCentEst = new TString("V0M");
  if(!fStageSwitch) AliFatal("Stage switch is 0, not sure what should be done!\n");
  if(fStageSwitch==1)
    DefineOutput(1,TList::Class());
  if(fStageSwitch==2)
    DefineOutput(1,TList::Class());
  if(fStageSwitch==3) {
    if(!fIsMC) { //Efficiency and NUA only important for data
      DefineInput(1,TList::Class()); //NUA
      for(int key(0);key<Nkeys;++key)
        DefineInput(2+key,TList::Class());  //NUE
    };
    DefineOutput(1,TList::Class());
    DefineOutput(2,AliGFWFlowContainer::Class());
    DefineOutput(3,TList::Class());
    DefineOutput(4,TList::Class());
  };
  SetNchCorrelationCut(1,0,kFALSE);
};
AliAnalysisTaskDeform::~AliAnalysisTaskDeform() {
  SetNchCorrelationCut(1,0,kFALSE);
};
void AliAnalysisTaskDeform::UserCreateOutputObjects(){
  printf("Stage switch is %i\n\n\n",fStageSwitch);
  if(!fGFWSelection) SetSystFlag(0);
  fGFWSelection->PrintSetup();
  fSystFlag = fGFWSelection->GetSystFlagIndex();
  if(fGFWSelection->GetSystFlagIndex() == 20) SetCentralityEstimator("CL0");
  else if(fGFWSelection->GetSystFlagIndex() == 21) SetCentralityEstimator("CL1");
  OpenFile(1);
  const Int_t temp_NV0MBinsDefault=fExtendV0MAcceptance?11:10;
  Double_t temp_V0MBinsDefault[12] = {0,5,10,20,30,40,50,60,70,80,90,101}; //Last bin to include V0M beyond anchor point
  if(!fV0MMultiAxis) SetV0MBins(temp_NV0MBinsDefault,temp_V0MBinsDefault);
  Double_t *l_V0MBinsDefault=GetBinsFromAxis(fV0MMultiAxis);
  Int_t l_NV0MBinsDefault=fV0MMultiAxis->GetNbins();
  if(l_V0MBinsDefault[l_NV0MBinsDefault]>90) fExtendV0MAcceptance = kTRUE; //If V0M is beyond 90, then we need to extend the V0M acceptance!
  if(fOnTheFly && !fUseCentralityOTF) { 
    if(!fIPAxis) SetMultiBins(l_NV0MBinsDefault,l_V0MBinsDefault);
    fMultiBins = GetBinsFromAxis(fIPAxis);
    fNMultiBins = fIPAxis->GetNbins();
  } 
  else {
    if(!fMultiAxis) SetMultiBins(l_NV0MBinsDefault,l_V0MBinsDefault);
    fMultiBins = GetBinsFromAxis(fMultiAxis);
    fNMultiBins = fMultiAxis->GetNbins();
  }
  if(!fV2dPtMulti) {
    Double_t temp_bn[] = {0,1e6};
    SetV2dPtMultiBins(1,temp_bn);
  };
  const Int_t l_NPtBinsDefault = 25;
  Double_t l_PtBinsDefault[l_NPtBinsDefault+1] = {0.20, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95,
                     1.00, 1.10, 1.20, 1.30, 1.40, 1.50, 1.60, 1.70, 1.80, 1.90,
                     2.00, 2.20, 2.40, 2.60, 2.80, 3.00};
  if(!fPtAxis) SetPtBins(l_NPtBinsDefault,l_PtBinsDefault);
  fPtBins = GetBinsFromAxis(fPtAxis);
  fNPtBins = fPtAxis->GetNbins();
  TString spNames[] = {"ch","pi","ka","pr"};
  if(fStageSwitch==1) {
    fRequireReloadOnRunChange = kFALSE;
    fWeightList = new TList();
    fWeightList->SetOwner(kTRUE);
    TString wNames[] = {"ch","pi","ka","pr"};
    fWeights = new AliGFWWeights*[4];
    for(Int_t i=0; i<4;i++) {
      fWeights[i] = new AliGFWWeights();
      fWeights[i]->SetPtBins(fNPtBins,fPtBins);
      // fWeights[i]->SetPtBins(NbinsPtForV2,binsPtForV2);
      fWeights[i]->SetName(Form("weight_%s",wNames[i].Data()));
      fWeights[i]->Init(!fIsMC,fIsMC);
      fWeightList->Add(fWeights[i]);
    }
    int nEventCutLabel = 6; 
    fEventCount = new TH1D("fEventCount","Event counter",nEventCutLabel,0,nEventCutLabel);
    TString eventCutLabel[6]={"Input","Centrality","Trigger","AliEventCuts","Vertex","Tracks"};
    for(int i=0;i<nEventCutLabel;++i) fEventCount->GetXaxis()->SetBinLabel(i+1,eventCutLabel[i].Data());
    fWeightList->Add(fEventCount);
    PostData(1,fWeightList);
  };
  if(fStageSwitch==2) {
    fSpectraList = new TList();
    fSpectraList->SetOwner(kTRUE);
    int Neta_Default = 1;
    double l_eta_Default[] = {-0.8,0.8};
    if(!fEtaAxis) { printf("Setting default eta bins\n"); SetEtaBins(Neta_Default,l_eta_Default);}
    fEtaBins=GetBinsFromAxis(fEtaAxis);
    fNEtaBins=fEtaAxis->GetNbins();
    TString spNames[]={"ch","pi","ka","pr"};
    Int_t l_NNchBins = 3000;
    Double_t *l_NchBins = new Double_t[l_NNchBins+1];
    for(int i(0);i<=l_NNchBins;++i) l_NchBins[i] = i+0.5;
    fSpectraGen = new TH3D*[4];
    fSpectraRec = new TH3D*[4];
    fDetectorResponse = new TH2D*[4];
    for(Int_t i=0;i<4;++i) {
      fSpectraGen[i] = new TH3D(Form("spectraGen_%s",spNames[i].Data()),Form("spectraGen_%s",spNames[i].Data()),fNPtBins,fPtBins,fNEtaBins,fEtaBins,fNMultiBins,fMultiBins);
      fSpectraRec[i] = new TH3D(Form("spectraRec_%s",spNames[i].Data()),Form("spectraRec_%s",spNames[i].Data()),fNPtBins,fPtBins,fNEtaBins,fEtaBins,fNMultiBins,fMultiBins);
      fSpectraList->Add(fSpectraGen[i]);
      fSpectraList->Add(fSpectraRec[i]);
      fDetectorResponse[i] = new TH2D(Form("fDetectorResponse_%s",spNames[i].Data()),Form("Detector Response %s",spNames[i].Data()),l_NNchBins,l_NchBins,l_NNchBins,l_NchBins);
      fSpectraList->Add(fDetectorResponse[i]);
    }
    int nEventCutLabel = 6; 
    fEventCount = new TH1D("fEventCount","Event counter",nEventCutLabel,0,nEventCutLabel);
    TString eventCutLabel[6]={"Input","Centrality","Trigger","AliEventCuts","Vertex","Tracks"};
    for(int i=0;i<nEventCutLabel;++i) fEventCount->GetXaxis()->SetBinLabel(i+1,eventCutLabel[i].Data());
    fSpectraList->Add(fEventCount);
    PostData(1,fSpectraList);
  }
  if(fStageSwitch==3) {
    fRndm = new TRandom(0);
    fRequireReloadOnRunChange = kFALSE;
    fWeightList = (TList*)GetInputData(1);
    fWeights = new AliGFWWeights*[4];
    const char* species[] = {"_ch","_pi","_ka","_pr"};
    if(!fIsMC) { //Efficiencies and NUA are only for the data or if specified for pseudoefficiencies
      if(fUsePIDNUA) {
        if(!fWeightList) AliFatal("NUA list not set or does not exist!\n");
        TString lBase(""); //base
        TString lSubfix(""); //subfix
        for(int i(0);i<4;++i) {
          lBase = Form("weight%s",species[i]); 
          lSubfix = fGFWSelection->NeedsExtraWeight()?fGFWSelection->GetSystPF():"";
          lBase+=lSubfix;
          fWeights[i] = (AliGFWWeights*)fWeightList->FindObject(lBase.Data());
          if(!fWeights[i]) AliFatal(Form("Weights %s not not found in the list provided!\n",lBase.Data()));
          fWeights[i]->CreateNUA();
        }
      }
      fEfficiencyList = (TList*)GetInputData(2+fEfficiencyIndex); //Efficiencies start from input slot 2
      if(fUse2DEff) {
        fEfficiency.resize(l_NV0MBinsDefault,vector<TH2D*>(4));
        for(int iSp=0;iSp<4;++iSp) {
          for(Int_t i=0;i<l_NV0MBinsDefault;i++) {
            fEfficiency[i][iSp] = (TH2D*)fEfficiencyList->FindObject(Form("EffRescaled%s_Cent%i%s",species[iSp],i,fGFWSelection->GetSystPF()));
            if(!fEfficiency[i][iSp]) {
              if(!i) AliFatal("Could not fetch efficiency!\n");
              printf("Could not find efficiency for V0M bin no. %i! Cloning the previous efficiency instead...\n",i);
              fEfficiency[i][iSp] = (TH2D*)fEfficiency[i-1][iSp]->Clone(Form("EffRescaled%s_Cent%i%s",species[iSp],i,fGFWSelection->GetSystPF()));
            };
          }
        }
      }
      else {
        fEfficiencies = new TH1D*[l_NV0MBinsDefault];
        for(Int_t i=0;i<l_NV0MBinsDefault;i++) {

            fEfficiencies[i] = (TH1D*)fEfficiencyList->FindObject(Form("EffRescaled_Cent%i%s",i,fGFWSelection->GetSystPF()));
            if(fEfficiencies[i] && fPseudoEfficiency<1) fEfficiencies[i]->Scale(fPseudoEfficiency);
            if(!fEfficiencies[i]) {
              if(!i) AliFatal("Could not fetch efficiency!\n");
              printf("Could not find efficiency for V0M bin no. %i! Cloning the previous efficiency instead...\n",i);
              fEfficiencies[i] = (TH1D*)fEfficiencies[i-1]->Clone(Form("EffRescaled_Cent%i%s",i,fGFWSelection->GetSystPF()));
            };
        }
      }
    };
    if(fOnTheFly)
    {
      vector<double> b = {0.0,3.72,5.23,7.31,8.88,10.20,11.38,12.47,13.50,14.51,100.0};
      vector<double> cent = {0.0,5.0,10.0,20.0,30.0,40.0,50.0,60.0,70.0,80.0,100.0};
      for(size_t i(0); i<b.size(); ++i) centralitymap[b[i]]=cent[i];
      fIPDist = new TH1D("IPDistribution","Impact parameter distribution; b; N(events)",fIPAxis->GetNbins(),GetBinsFromAxis(fIPAxis));
      fptVarList->Add(fIPDist);
    }
    // if(!LoadMyWeights(0)) return; //Loading run-avg NUA weights
    fptVarList = new TList();
    fptVarList->SetOwner(kTRUE);
    fPtCont = new AliPtContainer*[4];
    fCkCont = new AliCkContainer*[4];
    int endPID = (fDisablePID)?1:4;
    for(Int_t i=0;i<endPID;i++) {
      fCkCont[i] = new AliCkContainer(Form("ckcont_%s",spNames[i].Data()),Form("ckcont_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
      fptVarList->Add(fCkCont[i]);
      fPtCont[i] = new AliPtContainer(Form("ptcont_%s",spNames[i].Data()),Form("ptcont_%s",spNames[i].Data()),fNMultiBins,fMultiBins,8,false);
      fptVarList->Add(fPtCont[i]);
      fPtCont[i]->SetEventWeight(fEventWeight);
      if(fNBootstrapProfiles) {
        fCkCont[i]->InitializeSubsamples(fNBootstrapProfiles);
        fPtCont[i]->InitializeSubsamples(fNBootstrapProfiles);
      }
    }
    if(fFillMptPowers) {
      fMpt = new AliProfileBS*[8*4];
      for(int i(0);i<endPID;++i) {
        fMpt[8*i] = new AliProfileBS(Form("mpt_%s",spNames[i].Data()),Form("mpt_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
        fptVarList->Add(fMpt[8*i]);
        fMpt[1+8*i] = new AliProfileBS(Form("mptsq_%s",spNames[i].Data()),Form("mptsq_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
        fptVarList->Add(fMpt[1+8*i]);
        fMpt[2+8*i] = new AliProfileBS(Form("mptcube_%s",spNames[i].Data()),Form("mptcube_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
        fptVarList->Add(fMpt[2+8*i]);
        fMpt[3+8*i] = new AliProfileBS(Form("mptquart_%s",spNames[i].Data()),Form("mptquart_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
        fptVarList->Add(fMpt[3+8*i]);
        fMpt[4+8*i] = new AliProfileBS(Form("mptpent_%s",spNames[i].Data()),Form("mptpent_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
        fptVarList->Add(fMpt[4+8*i]);
        fMpt[5+8*i] = new AliProfileBS(Form("mpthexa_%s",spNames[i].Data()),Form("mpthexa_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
        fptVarList->Add(fMpt[5+8*i]);
        fMpt[6+8*i] = new AliProfileBS(Form("mpthept_%s",spNames[i].Data()),Form("mpthept_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
        fptVarList->Add(fMpt[6+8*i]);
        fMpt[7+8*i] = new AliProfileBS(Form("mptocto_%s",spNames[i].Data()),Form("mptocto_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
        fptVarList->Add(fMpt[7+8*i]);
      }
      if(fNBootstrapProfiles) for(int i(0);i<8*endPID;++i) fMpt[i]->InitializeSubsamples(fNBootstrapProfiles);
    }
    fMultiDist = new TH1D("MultiDistribution","Multiplicity distribution; #it{N}_{ch}; N(events)",fNMultiBins,fMultiBins);
    fV0MMulti = new TH1D("V0M_Multi","V0M_Multi",l_NV0MBinsDefault,l_V0MBinsDefault);
    fptVarList->Add(fMultiDist);
    fptVarList->Add(fV0MMulti);
    fMultiVsV0MCorr = new TH2D*[2];
    fMultiVsV0MCorr[0] = new TH2D("MultVsV0M_BeforeConsistency","MultVsV0M_BeforeConsistency",103,0,103,fNMultiBins,fMultiBins[0],fMultiBins[fNMultiBins]);
    fMultiVsV0MCorr[1] = new TH2D("MultVsV0M_AfterConsistency","MultVsV0M_AfterConsistency",103,0,103,fNMultiBins,fMultiBins[0],fMultiBins[fNMultiBins]);
    fESDvsFB128 = new TH2D("ESDvsFB128","; N(FB128); N(ESD)",500,-0.5,4999.5,1500,-0.5,14999.5);
    fptVarList->Add(fMultiVsV0MCorr[0]);
    fptVarList->Add(fMultiVsV0MCorr[1]);
    fptVarList->Add(fESDvsFB128);
    //ITS vs TPC tracklets cut for PU
    fITSvsTPCMulti = new TH2D("TPCvsITSclusters",";TPC clusters; ITS clusters",1000,0,10000,5000,0,50000);
    fptVarList->Add(fITSvsTPCMulti);
    if(fIsMC) {
      fNchTrueVsReco = new TH2D("NchTrueVsReco",";Nch (MC-true); Nch (MC-reco)",fNMultiBins,fMultiBins,fNMultiBins,fMultiBins);
      fptVarList->Add(fNchTrueVsReco);
    }
    PostData(1,fptVarList);
    //Setting up the FlowContainer
    TObjArray *oba = new TObjArray();
    oba->Add(new TNamed("ChGap22","ChGap22")); //for gap (|eta|>0.4) case
    oba->Add(new TNamed("ChGap24","ChGap24")); //for gap (|eta|>0.4) case
    oba->Add(new TNamed("ChFull22","ChFull22")); //no-gap case
    oba->Add(new TNamed("ChFull24","ChFull24")); //no-gap case

    oba->Add(new TNamed("ChGap32","ChGap32")); //for gap (|eta|>0.4) case
    oba->Add(new TNamed("ChGap34","ChGap34")); //for gap (|eta|>0.4) case
    oba->Add(new TNamed("ChFull32","ChFull32")); //no-gap case
    oba->Add(new TNamed("ChFull34","ChFull34")); //no-gap case

    oba->Add(new TNamed("ChGap42","ChGap42")); //gap case

    oba->Add(new TNamed("LM22","LM22")); //for gap (|eta|>0.4) case
    oba->Add(new TNamed("MR22","MR22")); //for gap (|eta|>0.4) case
    oba->Add(new TNamed("LR22","LR22")); //for gap (|eta|>0.4) case

    oba->Add(new TNamed("LLMR24","LLMR24")); //for gap (|eta|>0.4) case
    oba->Add(new TNamed("LMMR24","LMMR24")); //for gap (|eta|>0.4) case
    oba->Add(new TNamed("LMRR24","LMRR24")); //for gap (|eta|>0.4) case

    oba->Add(new TNamed("ChSC234","ChSC234")); //for SC{2,3}
    oba->Add(new TNamed("ChSC244","ChSC244")); //for SC{2,3}

    oba->Add(new TNamed("ChFull28","ChFull28"));
    oba->Add(new TNamed("PiGap22","PiGap22"));
    oba->Add(new TNamed("PiFull22","PiFull22"));
    oba->Add(new TNamed("PiFull24","PiFull24"));
    oba->Add(new TNamed("PiGap32","PiGap32"));
    oba->Add(new TNamed("PiFull32","PiFull32"));
    oba->Add(new TNamed("PiFull34","PiFull34"));
    oba->Add(new TNamed("KaGap22","KaGap22"));
    oba->Add(new TNamed("KaFull22","KaFull22"));
    oba->Add(new TNamed("KaFull24","KaFull24"));
    oba->Add(new TNamed("KaGap32","KaGap32"));
    oba->Add(new TNamed("KaFull32","KaFull32"));
    oba->Add(new TNamed("KaFull34","KaFull34"));
    oba->Add(new TNamed("PrGap22","PrGap22"));
    oba->Add(new TNamed("PrFull22","PrFull22"));
    oba->Add(new TNamed("PrFull24","PrFull24"));
    oba->Add(new TNamed("PrGap32","PrGap32"));
    oba->Add(new TNamed("PrFull32","PrFull32"));
    oba->Add(new TNamed("PrFull34","PrFull34"));

    oba->Add(new TNamed("PiDiff22","PiDiff22"));
    oba->Add(new TNamed("KaDiff22","KaDiff22"));
    oba->Add(new TNamed("PrDiff22","PrDiff22"));
    oba->Add(new TNamed("PiDiff24","PiDiff24"));
    oba->Add(new TNamed("KaDiff24","KaDiff24"));
    oba->Add(new TNamed("PrDiff24","PrDiff24"));

    fFC = new AliGFWFlowContainer();
    TString fcname("FlowContainer");
    if(!fContSubfix->IsNull()) fcname.Append(fContSubfix->Data());
    fFC->SetName(fcname.Data());
    fFC->Initialize(oba,fNMultiBins,fMultiBins,fNBootstrapProfiles);
    delete oba;
    PostData(2,fFC);
    Int_t pows[] = {3,0,2,2,3,3,3}; //5th harm. sum = 3, b/c {-2 -3}
    //Int_t powsFull[] = {5,0,4,4,3,3,3};
    Int_t powsFull[] = {9,0,8,4,7,3,6,0,5};
    fGFW = new AliGFW();
    fGFW->AddRegion("refN",7,pows,-0.8,-fEtaV2Sep,1,1);
    fGFW->AddRegion("refP",7,pows,fEtaV2Sep,0.8,1,1);
    if(fEtaV2Sep>=0) fGFW->AddRegion("subMid",7,pows,-fEtaV2Sep,fEtaV2Sep,1,1);
    fGFW->AddRegion("mid",9,powsFull,-0.8,0.8,1,2);

    fGFW->AddRegion("PiRefN",7,pows,-0.8,-fEtaV2Sep,1,4);
    fGFW->AddRegion("PiRefP",7,pows,fEtaV2Sep,0.8,1,4);
    fGFW->AddRegion("KaRefN",7,pows,-0.8,-fEtaV2Sep,1,8);
    fGFW->AddRegion("KaRefP",7,pows,fEtaV2Sep,0.8,1,8);
    fGFW->AddRegion("PrRefN",7,pows,-0.8,-fEtaV2Sep,1,16);
    fGFW->AddRegion("PrRefP",7,pows,fEtaV2Sep,0.8,1,16);

    fGFW->AddRegion("PiMid",9,powsFull,-0.8,0.8,1,32);
    fGFW->AddRegion("KaMid",9,powsFull,-0.8,0.8,1,64);
    fGFW->AddRegion("PrMid",9,powsFull,-0.8,0.8,1,128);  
    CreateCorrConfigs();
    //Covariance
    fCovList = new TList();
    fCovList->SetOwner(kTRUE);
    fCovariance = new AliProfileBS*[Ncovpfs*4];
    for(Int_t i=0;i<endPID;i++) {
      fCovariance[Ncovpfs*i] = new AliProfileBS(Form("covmpt_%s",spNames[i].Data()),Form("covmpt_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
      fCovList->Add(fCovariance[Ncovpfs*i]);
      fCovariance[1+Ncovpfs*i] = new AliProfileBS(Form("covnopt_%s",spNames[i].Data()),Form("covnopt_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
      fCovList->Add(fCovariance[1+Ncovpfs*i]); 
      fCovariance[2+Ncovpfs*i] = new AliProfileBS(Form("covmpt_v3_%s",spNames[i].Data()),Form("covmpt_v3_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
      fCovList->Add(fCovariance[2+Ncovpfs*i]);
      fCovariance[3+Ncovpfs*i] = new AliProfileBS(Form("covnopt_v3_%s",spNames[i].Data()),Form("covnopt_v3_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
      fCovList->Add(fCovariance[3+Ncovpfs*i]);
      fCovariance[4+Ncovpfs*i] = new AliProfileBS(Form("covmpt_v23_%s",spNames[i].Data()),Form("covmpt_v23_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
      fCovList->Add(fCovariance[4+Ncovpfs*i]);
      fCovariance[5+Ncovpfs*i] = new AliProfileBS(Form("covnopt_v23_%s",spNames[i].Data()),Form("covnopt_v23_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
      fCovList->Add(fCovariance[5+Ncovpfs*i]);
      fCovariance[6+Ncovpfs*i] = new AliProfileBS(Form("covmpt_v24_%s",spNames[i].Data()),Form("covmpt_v24_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
      fCovList->Add(fCovariance[6+Ncovpfs*i]);
      fCovariance[7+Ncovpfs*i] = new AliProfileBS(Form("covnopt_v24_%s",spNames[i].Data()),Form("covnopt_v24_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
      fCovList->Add(fCovariance[7+Ncovpfs*i]);
      fCovariance[8+Ncovpfs*i] = new AliProfileBS(Form("covmpt_v34_%s",spNames[i].Data()),Form("covmpt_v34_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
      fCovList->Add(fCovariance[8+Ncovpfs*i]);
      fCovariance[9+Ncovpfs*i] = new AliProfileBS(Form("covnopt_v34_%s",spNames[i].Data()),Form("covnopt_v34_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
      fCovList->Add(fCovariance[9+Ncovpfs*i]);
      fCovariance[10+Ncovpfs*i] = new AliProfileBS(Form("covpt2_v2_%s",spNames[i].Data()),Form("covpt2_v2_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
      fCovList->Add(fCovariance[10+Ncovpfs*i]);
      fCovariance[11+Ncovpfs*i] = new AliProfileBS(Form("covpt2_v3_%s",spNames[i].Data()),Form("covpt2_v3_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
      fCovList->Add(fCovariance[11+Ncovpfs*i]);
      fCovariance[12+Ncovpfs*i] = new AliProfileBS(Form("covpt3_v2_%s",spNames[i].Data()),Form("covpt3_v2_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
      fCovList->Add(fCovariance[12+Ncovpfs*i]);
      fCovariance[13+Ncovpfs*i] = new AliProfileBS(Form("covpt4_v2_%s",spNames[i].Data()),Form("covpt4_v2_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
      fCovList->Add(fCovariance[13+Ncovpfs*i]);
      if(fFillMptPowers) {
        fCovariancePowerMpt = new AliProfileBS*[3*4];
        for(Int_t i=0;i<endPID;i++) {
          fCovariancePowerMpt[3*i] = new AliProfileBS(Form("covmpt2_v2_%s",spNames[i].Data()),Form("covmpt2_v2_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
          fCovList->Add(fCovariancePowerMpt[3*i]);
          fCovariancePowerMpt[1+3*i] = new AliProfileBS(Form("covmpt3_v2_%s",spNames[i].Data()),Form("covmpt3_v2_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
          fCovList->Add(fCovariancePowerMpt[1+3*i]);
          fCovariancePowerMpt[2+3*i] = new AliProfileBS(Form("covmpt4_v2_%s",spNames[i].Data()),Form("covmpt4_v2_%s",spNames[i].Data()),fNMultiBins,fMultiBins);
          fCovList->Add(fCovariancePowerMpt[2+3*i]);
        };
      };
    }
    if(fNBootstrapProfiles) for(Int_t i=0;i<Ncovpfs*endPID;i++) fCovariance[i]->InitializeSubsamples(fNBootstrapProfiles);
    if(fFillMptPowers && fNBootstrapProfiles) for(Int_t i=0;i<3*endPID;i++) fCovariancePowerMpt[i]->InitializeSubsamples(fNBootstrapProfiles);
    PostData(3,fCovList);
    fQAList = new TList();
    fQAList->SetOwner(kTRUE);
    fEventCuts.AddQAplotsToList(fQAList,kTRUE);
    int nEventCutLabel = 6; 
    fEventCount = new TH1D("fEventCount","Event counter",nEventCutLabel,0,nEventCutLabel);
    TString eventCutLabel[6]={"Input","Centrality","Trigger","AliEventCuts","Vertex","Tracks"};
    for(int i=0;i<nEventCutLabel;++i) fEventCount->GetXaxis()->SetBinLabel(i+1,eventCutLabel[i].Data());
    fQAList->Add(fEventCount);
    if(fUseChargedPtCut) {
      fChPtDist = new TH1D("fChPtDist","#it{p}_{T,ch} distribution",100,fchPtMin,fchPtMax);
      fQAList->Add(fChPtDist);
      fPtDist = new TH1D("fPtDist","#it{p}_{T} distribution",100,fPtBins[0],fPtBins[fNPtBins]);
      fQAList->Add(fPtDist);
    }
    printf("User output objects created!\n");
    PostData(4,fQAList);
  }
  fEventCuts.OverrideAutomaticTriggerSelection(fTriggerType,true);
  if(fExtendV0MAcceptance) {
    fEventCuts.OverrideCentralityFramework(1);
    fEventCuts.SetCentralityEstimators("V0M","CL0");
    fEventCuts.SetCentralityRange(0.f,101.f);
  }
  //Creating cuts for 15o_pass2 and 18qr_pass3. 18qr_pass3 not implemented yet.
  //Would like to do that in a more elegant way, but not at this point, unfortunatelly
  if(fEventCutFlag) { //Only initialize them if necessary
    fSPDCutPU = new TF1("fSPDCutPU", "450. + 3.9*x", 0, 50000);
    if(!fV0CutPU) fV0CutPU = new TF1("fV0CutPU", "[0]+[1]*x - 6.*[2]*([3] + [4]*sqrt(x) + [5]*x + [6]*x*sqrt(x) + [7]*x*x)", 0, 100000); //Only if not initialized externally. Set to 0 for ESD MC, as that seems to be problematic?
    fCenCutLowPU = new TF1("fCenCutLowPU", "[0]+[1]*x - 5.5*([2]+[3]*x+[4]*x*x+[5]*x*x*x)", 0, 100);
    fCenCutHighPU = new TF1("fCenCutHighPU", "[0]+[1]*x + 5.5*([2]+[3]*x+[4]*x*x+[5]*x*x*x)", 0, 100);
    fMultCutPU = new TF1("fMultCutPU", "[0]+[1]*x+[2]*exp([3]-[4]*x) - 6.*([5]+[6]*exp([7]-[8]*x))", 0, 100);
    if(fEventCutFlag==1 || fEventCutFlag==101) {
       Double_t parV0[8] = {33.4237, 0.953516, 0.0712137, 227.923, 8.9239, -0.00319679, 0.000306314, -7.6627e-07};
       fV0CutPU->SetParameters(parV0);
       Double_t parV0CL0[6] = {0.0193587, 0.975914, 0.675714, 0.0292263, -0.000549509, 5.86421e-06};
       fCenCutLowPU->SetParameters(parV0CL0);
       fCenCutHighPU->SetParameters(parV0CL0);
       Double_t parFB32[9] = {-812.822, 6.41796, 5421.83, -0.382601, 0.0299686, -26.6249, 321.388, -0.82615, 0.0167828};
       fMultCutPU->SetParameters(parFB32);
    }
  };
  fGFWNtotSelection = new AliGFWCuts();
  fGFWNtotSelection->SetupCuts(0);
  fGFWNtotSelection->SetEta(fEtaNch);
  AliAnalysisManager *man=AliAnalysisManager::GetAnalysisManager();
  AliInputEventHandler* inputHandler = (AliInputEventHandler*) (man->GetInputEventHandler());
  fPIDResponse = inputHandler->GetPIDResponse();
  fBayesPID = new AliPIDCombined();
  fBayesPID->SetDefaultTPCPriors();
  fBayesPID->SetSelectedSpecies(AliPID::kSPECIES);
  fBayesPID->SetDetectorMask(AliPIDResponse::kDetTPC+AliPIDResponse::kDetTOF);
  LoadWeightAndMPT();
};
void AliAnalysisTaskDeform::UserExec(Option_t*) {
  EventNo++;
  if(fOnTheFly){
    fMCEvent = getMCEvent();
    Double_t l_Cent = (fUseCentralityOTF)?getAMPTCentrality():fImpactParameterMC;
    Int_t nTracks = fMCEvent->GetNumberOfPrimaries();
    if(nTracks < 1) { return; }
    Double_t wp[5] = {0,0,0,0,0}; //Initial values, [species][w*p]
    wpPt.clear();
    wpPt.resize(1,vector<vector<double>>(10,vector<double>(10)));
    fGFW->Clear();
    Double_t ptMin = fPtBins[0];
    Double_t ptMax = fPtBins[fNPtBins];
    for(Int_t i=0;i<nTracks;i++) {
      AliMCParticle* lPart = dynamic_cast<AliMCParticle*>(fMCEvent->GetTrack(i));
      if(!lPart) { continue; };
      Double_t l_pt=lPart->Pt();
      Double_t l_phi=lPart->Phi();
      Double_t l_eta=lPart->Eta();
      if (TMath::Abs(l_eta) > 0.8) continue;
      if (l_pt<ptMin || l_pt>ptMax) continue;
      if(TMath::Abs(l_eta)<fEta)  { //for mean pt, only consider -0.4-0.4 region
        FillWPCounter(wp,1,l_pt); 
        FillWPCounter(wpPt[0],1,l_pt);
      }  //Actually, no need for if() statememnt now since GFW knows about eta's, so I can fill it all the time
      fGFW->Fill(l_eta,1,l_phi,1,3); //filling both gap (bit mask 1) and full (bit mas 2). Since this is MC, weight is 1.
    };
    if(wp[0]==0) return; //if no single charged particles, then surely no PID either, no sense to continue
    Double_t l_Random = fRndm->Rndm();
    fCkCont[0]->FillObs(wp,l_Cent,l_Random);
    fPtCont[0]->FillRecursive(wpPt[0],l_Cent,l_Random);
    fPtCont[0]->FillCk(wpPt[0],l_Cent,l_Random);
    fPtCont[0]->FillSkew(wpPt[0],l_Cent,l_Random);
    fPtCont[0]->FillKurtosis(wpPt[0],l_Cent,l_Random);
    fV0MMulti->Fill(l_Cent);
    fMultiDist->Fill(l_Cent);
    fIPDist->Fill(fImpactParameterMC);
    Double_t mptev = wp[1]/wp[0];
    if(fFillMptPowers) {
        fMpt[0]->FillProfile(l_Cent,mptev,fUseWeightsOne?1:wp[0],l_Random);
        fMpt[1]->FillProfile(l_Cent,mptev*mptev,fUseWeightsOne?1:(wp[0]*wp[0]),l_Random);
        fMpt[2]->FillProfile(l_Cent,mptev*mptev*mptev,fUseWeightsOne?1:(wp[0]*wp[0]*wp[0]),l_Random);
        fMpt[3]->FillProfile(l_Cent,mptev*mptev*mptev*mptev,fUseWeightsOne?1:(wp[0]*wp[0]*wp[0]*wp[0]),l_Random);
        fMpt[4]->FillProfile(l_Cent,mptev*mptev*mptev*mptev*mptev,fUseWeightsOne?1:(wp[0]*wp[0]*wp[0]*wp[0]*wp[0]),l_Random);
        fMpt[5]->FillProfile(l_Cent,mptev*mptev*mptev*mptev*mptev*mptev,fUseWeightsOne?1:(wp[0]*wp[0]*wp[0]*wp[0]*wp[0]*wp[0]),l_Random);
        fMpt[6]->FillProfile(l_Cent,mptev*mptev*mptev*mptev*mptev*mptev*mptev,fUseWeightsOne?1:(wp[0]*wp[0]*wp[0]*wp[0]*wp[0]*wp[0]*wp[0]),l_Random);
        fMpt[7]->FillProfile(l_Cent,mptev*mptev*mptev*mptev*mptev*mptev*mptev*mptev,fUseWeightsOne?1:(wp[0]*wp[0]*wp[0]*wp[0]*wp[0]*wp[0]*wp[0]*wp[0]),l_Random);
    }
    PostData(1,fptVarList);
    //Filling FCs
    for(Int_t l_ind=0; l_ind<corrconfigs.size(); l_ind++) {
      Bool_t filled = FillFCs(corrconfigs.at(l_ind),l_Cent,l_Random);
    };


    FillCovariance(fCovariance[0],corrconfigs.at(0),l_Cent,mptev,wp[0],l_Random);
    FillCovariance(fCovariance[1],corrconfigs.at(0),l_Cent,1,wp[0],l_Random);
    FillCovariance(fCovariance[2],corrconfigs.at(16),l_Cent,mptev,wp[0],l_Random);
    FillCovariance(fCovariance[3],corrconfigs.at(16),l_Cent,1,wp[0],l_Random);
    FillCovariance(fCovariance[4],corrconfigs.at(33),l_Cent,mptev,wp[0],l_Random);
    FillCovariance(fCovariance[5],corrconfigs.at(33),l_Cent,1,wp[0],l_Random);
    FillCovariance(fCovariance[6],corrconfigs.at(8),l_Cent,mptev,wp[0],l_Random);
    FillCovariance(fCovariance[7],corrconfigs.at(8),l_Cent,1,wp[0],l_Random);
    FillCovariance(fCovariance[8],corrconfigs.at(24),l_Cent,mptev,wp[0],l_Random);
    FillCovariance(fCovariance[9],corrconfigs.at(24),l_Cent,1,wp[0],l_Random);
    //Covariance of vn with multi-particle pt-correlation
    vector<double> pt2corr = fPtCont[0]->getEventCorrelation(wpPt[0],2);
    if(pt2corr[1]!=0) {
      double pt2ev = pt2corr[0]/pt2corr[1];
      FillCovariance(fCovariance[10],corrconfigs.at(0),l_Cent,pt2ev,pt2corr[1],l_Random);
      FillCovariance(fCovariance[11],corrconfigs.at(16),l_Cent,pt2ev,pt2corr[1],l_Random);
    }
    vector<double> pt3corr = fPtCont[0]->getEventCorrelation(wpPt[0],3);
    if(pt3corr[1]!=0) {
      double pt3ev = pt3corr[0]/pt3corr[1];
      FillCovariance(fCovariance[12],corrconfigs.at(0),l_Cent,pt3ev,pt3corr[1],l_Random);
    }
    vector<double> pt4corr = fPtCont[0]->getEventCorrelation(wpPt[0],4);
    if(pt4corr[1]!=0) {
      double pt4ev = pt4corr[0]/pt4corr[1];
      FillCovariance(fCovariance[13],corrconfigs.at(0),l_Cent,pt4ev,pt4corr[1],l_Random);
    }
    //Covariance of vn with powers of mpt
    if(fFillMptPowers) {
      FillCovariance(fCovariancePowerMpt[0],corrconfigs.at(0),l_Cent,mptev*mptev,wp[0]*wp[0],l_Random);
      FillCovariance(fCovariancePowerMpt[1],corrconfigs.at(0),l_Cent,mptev*mptev*mptev,wp[0]*wp[0]*wp[0],l_Random);
      FillCovariance(fCovariancePowerMpt[2],corrconfigs.at(0),l_Cent,mptev*mptev*mptev*mptev,wp[0]*wp[0]*wp[0]*wp[0],l_Random);
    }

    PostData(3,fCovList);
    return;
  }
  AliAODEvent *fAOD = dynamic_cast<AliAODEvent*>(InputEvent());
  if(!fAOD) return;
  if(fIsMC) {
    fMCEvent = dynamic_cast<AliMCEvent *>(MCEvent());
    if (!fMCEvent) return;
  }
  fEventCount->Fill("Input",1);
  AliMultSelection *l_MultSel = (AliMultSelection*)fInputEvent->FindListObject("MultSelection");
  if(!l_MultSel) { AliFatal("MultSelection not found\n"); return; }
  Double_t l_Cent  = l_MultSel->GetMultiplicityPercentile(fCentEst->Data());
  if(l_Cent<0) return;
  if(fUseNchInV0M && (l_Cent>fV0MCentMax||l_Cent<fV0MCentMin)) return; 
  fEventCount->Fill("Centrality",1);
  if(!fBypassTriggerAndEvetCuts)
    if(!CheckTrigger(l_Cent)) return;
  fEventCount->Fill("Trigger",1);
  Double_t vtxXYZ[] = {0.,0.,0.};
  if(!AcceptAOD(fAOD, vtxXYZ)) return;
  Double_t vz = fAOD->GetPrimaryVertex()->GetZ();
  if(!fGFWSelection->AcceptVertex(fAOD)) return;
  fEventCount->Fill("Vertex",1);
  if(fStageSwitch==1)
    fIsMC?FillWeightsMC(fAOD, vz,l_Cent,vtxXYZ):FillWeights(fAOD, vz,l_Cent,vtxXYZ);
  if(fStageSwitch==2)
    FillSpectraMC(fAOD,vz,l_Cent,vtxXYZ);
  if(fStageSwitch==3)
    VnMpt(fAOD,vz,l_Cent,vtxXYZ);
};
void AliAnalysisTaskDeform::NotifyRun() {
  if(!fEventCutFlag || fEventCutFlag>100) { //Only relevant if we're using the standard AliEventCuts
    //Reinitialize AliEventCuts (done automatically on check):
    Bool_t dummy = fEventCuts.AcceptEvent(InputEvent());
    if(fUSe15opass2PU) fEventCuts.SetRejectTPCPileupWithITSTPCnCluCorr(kTRUE);

    //Then override PU cut if required:
    if(fGFWSelection->GetSystFlagIndex()==22)
      fEventCuts.fESDvsTPConlyLinearCut[0] = 1500.;
  };
}
void AliAnalysisTaskDeform::Terminate(Option_t*) {
};
Bool_t AliAnalysisTaskDeform::CheckTrigger(Double_t lCent) {
  UInt_t fSelMask = ((AliInputEventHandler*)(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler()))->IsEventSelected();
  //Apparently, MB trigger can also mark special triggers, leaving depleted regions in multi. To avoid this, pass true, if MB has been triggered.
  //This would fail if spec. triggers would also flag MB trigger, which seems to NOT be the case.
  if(!(fTriggerType&fSelMask)) { return kFALSE; }; //printf("Returning from the generic check\n");
  if(fSelMask&(fTriggerType&(AliVEvent::kINT7+AliVEvent::kMB))) {return kTRUE; }; //printf("Passed by MB trigger!\n");
  if((fSelMask&fTriggerType&AliVEvent::kCentral) && lCent>10) {return kFALSE; }; //printf("Returnning from kCent case\n");
  if((fSelMask&fTriggerType&AliVEvent::kSemiCentral) && (lCent<30 || lCent>50)) {return kFALSE; }; //printf("Returning from kSC case\n");
  return kTRUE;
};
AliMCEvent *AliAnalysisTaskDeform::getMCEvent() {
  AliMCEvent* ev = dynamic_cast<AliMCEvent*>(MCEvent());
  if(!ev) { AliFatal("MC event not found!"); return 0; }
  AliGenEventHeader *header = dynamic_cast<AliGenEventHeader*>(ev->GenEventHeader());
  if(!header) { AliFatal("MC event not generated!"); return 0; }
  AliCollisionGeometry* headerH;
  TString genName;
  TList *ltgen = (TList*)ev->GetCocktailList();
  if (ltgen) {
  for(auto&& listObject: *ltgen){
    genName = Form("%s",listObject->GetName());
    if (genName.Contains("Hijing")) {
      headerH = dynamic_cast<AliCollisionGeometry*>(listObject);
      break;
      }
    }
  }
  else headerH = dynamic_cast<AliCollisionGeometry*>(ev->GenEventHeader());
  if(headerH){
      fImpactParameterMC = headerH->ImpactParameter();
  }
  return ev;
}
double AliAnalysisTaskDeform::getAMPTCentrality()
{
  vector<double> b = {0.0,3.72,5.23,7.31,8.88,10.20,11.38,12.47,13.50,14.51,100.0};
  vector<double>::iterator it = upper_bound(b.begin(),b.end(),fImpactParameterMC);
  double l_cent = (fImpactParameterMC<0)?-1.0:(centralitymap[b[it-b.begin()]]+centralitymap[b[it-b.begin()-1]])/2.0;
  return l_cent;
}
Bool_t AliAnalysisTaskDeform::AcceptAOD(AliAODEvent *inEv, Double_t *lvtxXYZ) {
  if(!fBypassTriggerAndEvetCuts) {
    if(!fEventCutFlag) { if(!fEventCuts.AcceptEvent(inEv)) return 0; } //Don't perform AcceptEvent if not relevant
    else if(!AcceptCustomEvent(inEv)) return 0;
    if(fEventCutFlag>100) Bool_t dummy = fEventCuts.AcceptEvent(inEv); //if flag > 100, then also store QA output from AcceptEvent
  };
  fEventCount->Fill("AliEventCuts",1);
  const AliAODVertex* vtx = dynamic_cast<const AliAODVertex*>(inEv->GetPrimaryVertex());
  if(!vtx || vtx->GetNContributors() < 1)
    return kFALSE;
  const AliAODVertex* vtxSPD = dynamic_cast<const AliAODVertex*>(inEv->GetPrimaryVertexSPD());
  Double_t dMaxResol = 0.25; // suggested from DPG
  Double_t cov[6] = {0};
  vtxSPD->GetCovarianceMatrix(cov);
  Double_t zRes = TMath::Sqrt(cov[5]);
  if ( vtxSPD->IsFromVertexerZ() && (zRes > dMaxResol)) return kFALSE;
  const Double_t aodVtxZ = vtx->GetZ();
  if(TMath::Abs(aodVtxZ) > 10)
    return kFALSE;
  vtx->GetXYZ(lvtxXYZ);
  return kTRUE;
};
Bool_t AliAnalysisTaskDeform::AcceptAODTrack(AliAODTrack *mtr, Double_t *ltrackXYZ, const Double_t &ptMin, const Double_t &ptMax, Double_t *vtxp) {
  if(mtr->Pt()<ptMin) return kFALSE;
  if(mtr->Pt()>ptMax) return kFALSE;
  if(ltrackXYZ && vtxp) {
    mtr->GetXYZ(ltrackXYZ);
    ltrackXYZ[0] = ltrackXYZ[0]-vtxp[0];
    ltrackXYZ[1] = ltrackXYZ[1]-vtxp[1];
    ltrackXYZ[2] = ltrackXYZ[2]-vtxp[2];
  } else return kFALSE; //DCA cut is a must for now
  return fGFWSelection->AcceptTrack(mtr,fSystFlag==1?0:ltrackXYZ,0,kFALSE); //All complementary DCA track cuts for FB768 are disabled
};
Bool_t AliAnalysisTaskDeform::AcceptESDTrack(AliESDtrack *mtr, UInt_t& primFlag, Double_t *ltrackXYZ, const Double_t &ptMin, const Double_t &ptMax, Double_t *vtxp) {
  if(mtr->Pt()<ptMin) return kFALSE;
  if(mtr->Pt()>ptMax) return kFALSE;
  if(ltrackXYZ) {
    Float_t fD, fZ;
    mtr->GetImpactParameters(fD,fZ);
    ltrackXYZ[0] = fD;
    ltrackXYZ[1] = fZ;
  } else return kFALSE; //DCA cut is a must for now
  return fGFWSelection->AcceptTrack(mtr,fSystFlag==1?0:ltrackXYZ,0,primFlag);
};
Bool_t AliAnalysisTaskDeform::AcceptAODTrack(AliAODTrack *mtr, Double_t *ltrackXYZ, const Double_t &ptMin, const Double_t &ptMax, Double_t *vtxp, Int_t &nTot) {
  if(mtr->Pt()<ptMin) return kFALSE;
  if(mtr->Pt()>ptMax) return kFALSE;
  if(ltrackXYZ && vtxp) {
    mtr->GetXYZ(ltrackXYZ);
    ltrackXYZ[0] = ltrackXYZ[0]-vtxp[0];
    ltrackXYZ[1] = ltrackXYZ[1]-vtxp[1];
    ltrackXYZ[2] = ltrackXYZ[2]-vtxp[2];
  } else return kFALSE; //DCA cut is a must for now
  if(fGFWNtotSelection->AcceptTrack(mtr,ltrackXYZ,0,kFALSE)) nTot++;
  return fGFWSelection->AcceptTrack(mtr,fSystFlag==1?0:ltrackXYZ,0,kFALSE); //All complementary DCA track cuts for FB768 are disabled
};
Bool_t AliAnalysisTaskDeform::AcceptESDTrack(AliESDtrack *mtr, UInt_t& primFlag, Double_t *ltrackXYZ, const Double_t &ptMin, const Double_t &ptMax, Double_t *vtxp, Int_t &nTot) {
  if(mtr->Pt()<ptMin) return kFALSE;
  if(mtr->Pt()>ptMax) return kFALSE;
  if(ltrackXYZ) {
    Float_t fD, fZ;
    mtr->GetImpactParameters(fD,fZ);
    ltrackXYZ[0] = fD;
    ltrackXYZ[1] = fZ;
  } else return kFALSE; //DCA cut is a must for now
  UInt_t dummy;
  if(fGFWNtotSelection->AcceptTrack(mtr,ltrackXYZ,0,dummy)) nTot++;
  return fGFWSelection->AcceptTrack(mtr,fSystFlag==1?0:ltrackXYZ,0,primFlag); //All complementary DCA track cuts for FB768 are disabled
};
Int_t AliAnalysisTaskDeform::GetStageSwitch(TString instr) {
  if(instr.Contains("weights")) return 1;
  if(instr.Contains("Efficiency")) return 2;
  if(instr.Contains("VnMpt")) return 3;
  return 0;
}
void AliAnalysisTaskDeform::FillWeightsMC(AliAODEvent *fAOD, const Double_t &vz, const Double_t &l_Cent, Double_t *vtxp) {
  //MC generated
  AliAODTrack *lTrack;
  // AliVParticle *lPart;
  Double_t trackXYZ[3];
  Double_t dummyDouble[] = {0.,0.};
  Double_t ptMin = fPtBins[0];
  Double_t ptMax = fPtBins[fNPtBins];
  TClonesArray *tca = (TClonesArray*)fInputEvent->FindListObject("mcparticles");
  Int_t nPrim = tca->GetEntries();
  AliVParticle *lPart;
  Int_t partNotFetched=0;
  for (Int_t ipart = 0; ipart < nPrim; ipart++) {
    lPart = (AliAODMCParticle*)tca->At(ipart);
    if (!lPart) { continue; };
    /* get particlePDG */
    Int_t pdgcode = TMath::Abs(lPart->PdgCode());
    if (!lPart->IsPhysicalPrimary()) continue;
    if (lPart->Charge()==0.) continue;
    if (TMath::Abs(lPart->Eta()) > fEta) continue;
    Double_t pt = lPart->Pt();
    if (pt<ptMin || pt>ptMax) continue;
    fWeights[0]->Fill(lPart->Phi(),lPart->Eta(),vz,lPart->Pt(),l_Cent,2);
    if(fDisablePID) continue;
    if(pdgcode==211) fWeights[1]->Fill(lPart->Phi(),lPart->Eta(),vz,lPart->Pt(),l_Cent,2);
    if(pdgcode==321) fWeights[2]->Fill(lPart->Phi(),lPart->Eta(),vz,lPart->Pt(),l_Cent,2);
    if(pdgcode==2212) fWeights[3]->Fill(lPart->Phi(),lPart->Eta(),vz,lPart->Pt(),l_Cent,2);
  };
  fEventCount->Fill("Tracks",1);
  //MC reconstructed
  for(Int_t lTr=0;lTr<fAOD->GetNumberOfTracks();lTr++) {
    lTrack = (AliAODTrack*)fAOD->GetTrack(lTr);
    if(!AcceptAODTrack(lTrack,trackXYZ,ptMin,ptMax,vtxp)) continue;
    lPart = (AliAODMCParticle*)tca->At(TMath::Abs(lTrack->GetLabel()));
    if(!lPart) continue;
    if(!lPart->IsPhysicalPrimary()) continue;
    if(TMath::Abs(lTrack->Eta())>fEta) continue;
    if(!fGFWSelection->AcceptTrack(lTrack,dummyDouble)) continue;
    fWeights[0]->Fill(lPart->Phi(),lPart->Eta(),vz,lPart->Pt(),l_Cent,1);
    if(fDisablePID) continue;
    Int_t PIDIndex = GetBayesPIDIndex(lTrack)+1;
    if(PIDIndex) fWeights[PIDIndex]->Fill(lPart->Phi(),lPart->Eta(),vz,lPart->Pt(),l_Cent,1);
  };
  PostData(1,fWeightList);
}
void AliAnalysisTaskDeform::FillWeights(AliAODEvent *fAOD, const Double_t &vz, const Double_t &l_Cent, Double_t *vtxp) {
  AliAODTrack *lTrack;
  Double_t trackXYZ[3];
  Double_t ptMin = fPtBins[0];
  Double_t ptMax = fPtBins[fNPtBins];
  for(Int_t lTr=0;lTr<fAOD->GetNumberOfTracks();lTr++) {
    lTrack = (AliAODTrack*)fAOD->GetTrack(lTr);
    if(!lTrack) continue;
    Double_t trackXYZ[] = {0.,0.,0.};
    if(!AcceptAODTrack(lTrack,trackXYZ,ptMin,ptMax,vtxp)) continue;
    Double_t leta = lTrack->Eta();
    Double_t lpt = lTrack->Pt();
    ((AliGFWWeights*)fWeightList->At(0))->Fill(lTrack->Phi(),lTrack->Eta(),vz,lTrack->Pt(),l_Cent,0);
    if(fDisablePID) continue;
    Int_t PIDIndex = GetBayesPIDIndex(lTrack)+1;
    if(PIDIndex) ((AliGFWWeights*)fWeightList->At(PIDIndex))->Fill(lTrack->Phi(),lTrack->Eta(),vz,lTrack->Pt(),l_Cent,0);
  };
  fEventCount->Fill("Tracks",1);
  PostData(1,fWeightList);
}
void AliAnalysisTaskDeform::FillSpectraMC(AliAODEvent *fAOD, const Double_t &vz, const Double_t &l_Cent, Double_t *vtxp) {
 //MC generated
  AliAODTrack *lTrack;
  // AliVParticle *lPart;
  Double_t trackXYZ[3];
  Double_t dummyDouble[] = {0.,0.};
  Double_t ptMin = fPtBins[0];
  Double_t ptMax = fPtBins[fNPtBins];
  TClonesArray *tca = (TClonesArray*)fInputEvent->FindListObject("mcparticles");
  Int_t nPrim = tca->GetEntries();
  AliVParticle *lPart;
  Int_t partNotFetched=0;
  Int_t nTotNchMC=0; Int_t nTotNchReco=0;
  Int_t nTotNpiMC=0; Int_t nTotNpiReco=0;
  Int_t nTotNkaMC=0; Int_t nTotNkaReco=0;
  Int_t nTotNprMC=0; Int_t nTotNprReco=0;
  for (Int_t ipart = 0; ipart < nPrim; ipart++) {
    lPart = (AliAODMCParticle*)tca->At(ipart);
    if (!lPart) { continue; };
    /* get particlePDG */
    Int_t pdgcode = TMath::Abs(lPart->PdgCode());
    if (!lPart->IsPhysicalPrimary()) continue;
    if (lPart->Charge()==0.) continue;
    if(fRequirePositive && lPart->Charge()<0) continue;
    if (TMath::Abs(lPart->Eta()) > fEtaBins[fNEtaBins]) continue;
    Double_t pt = lPart->Pt();
    if (pt<ptMin || pt>ptMax) continue;
    nTotNchMC++;
    fSpectraGen[0]->Fill(pt,lPart->Eta(),l_Cent);
    if(fDisablePID) continue;
    if(pdgcode==211) { fSpectraGen[1]->Fill(pt,lPart->Eta(),l_Cent); nTotNpiMC++;}
    if(pdgcode==321) { fSpectraGen[2]->Fill(pt,lPart->Eta(),l_Cent); nTotNkaMC++;}
    if(pdgcode==2212) { fSpectraGen[3]->Fill(pt,lPart->Eta(),l_Cent); nTotNprMC++;}
  };
  //MC reconstructed
  for(Int_t lTr=0;lTr<fAOD->GetNumberOfTracks();lTr++) {
    lTrack = (AliAODTrack*)fAOD->GetTrack(lTr);
    if(!AcceptAODTrack(lTrack,trackXYZ,ptMin,ptMax,vtxp)) continue;
    lPart = (AliAODMCParticle*)tca->At(TMath::Abs(lTrack->GetLabel()));
    if(!lPart) continue;
    if(!lPart->IsPhysicalPrimary()) continue;
    if(fRequirePositive && lPart->Charge()<0) continue;
    if(TMath::Abs(lTrack->Eta())>fEtaBins[fNEtaBins]) continue;
    if(!fGFWSelection->AcceptTrack(lTrack,dummyDouble)) continue;
    nTotNchReco++;
    fSpectraRec[0]->Fill(lPart->Pt(),lPart->Eta(),l_Cent);
    if(fDisablePID) continue;
    Int_t PIDIndex = GetBayesPIDIndex(lTrack)+1;
    if(PIDIndex) fSpectraRec[PIDIndex]->Fill(lPart->Pt(),lPart->Eta(),l_Cent);
    switch(PIDIndex) {
      case 1: nTotNpiReco++; break;
      case 2: nTotNkaReco++; break;
      case 3: nTotNprReco++; break;
    }
  };
  fDetectorResponse[0]->Fill(nTotNchMC,nTotNchReco);
  fDetectorResponse[1]->Fill(nTotNpiMC,nTotNpiReco);
  fDetectorResponse[2]->Fill(nTotNkaMC,nTotNkaReco);
  fDetectorResponse[3]->Fill(nTotNprMC,nTotNprReco);
  PostData(1,fSpectraList);
}
Int_t AliAnalysisTaskDeform::GetNtotTracks(AliAODEvent* lAOD, const Double_t &ptmin, const Double_t &ptmax, Double_t *vtxp) {
  Double_t ltrackXYZ[3];
  AliAODTrack *lTrack;
  Int_t nTotNoTracks=0;
  for(Int_t lTr=0;lTr<lAOD->GetNumberOfTracks();lTr++) {
    lTrack = (AliAODTrack*)lAOD->GetTrack(lTr);
    if(!lTrack) continue;
    if(!AcceptAODTrack(lTrack,ltrackXYZ,ptmin,ptmax,vtxp,nTotNoTracks)) continue;
  };
  return nTotNoTracks;
}
void AliAnalysisTaskDeform::FillWPCounter(Double_t inArr[5], Double_t w, Double_t p) {
  inArr[0] += w;       // = w1p0
  inArr[1] += w*p;     // = w1p1
  inArr[2] += w*w*p*p; // = w2p2
  inArr[3] += w*w*p;   // = w2p1
  inArr[4] += w*w;     // = w2p0
}
void AliAnalysisTaskDeform::FillWPCounter(vector<vector<double>> &inarr, double w, double p)
{
  for(int i=0;i<=6;++i)
  {
    for(int j=0;j<=6;++j)
    {
      inarr[i][j] += pow(w,i)*pow(p,j);
    }
  }
  return;
}
void AliAnalysisTaskDeform::VnMpt(AliAODEvent *fAOD, const Double_t &vz, const Double_t &l_Cent, Double_t *vtxp) {
  AliAODTrack *lTrack;
  Double_t wp[4][5] = {{0,0,0,0,0},
                    {0,0,0,0,0},
                    {0,0,0,0,0},
                    {0,0,0,0,0}}; //Initial values, [species][w*p]
  wpPt.clear(); wpPt.resize(4,vector<vector<double>>(10,vector<double>(10)));
  fGFW->Clear();
  Int_t iCent = fV0MMulti->FindBin(l_Cent);
  if(!iCent || iCent>fV0MMulti->GetNbinsX()) return;
  iCent--;
  Int_t lPosCount=0, lNegCount=0, lMidCount=0;
  Double_t ptMin = fPtBins[0];
  Double_t ptMax = fPtBins[fNPtBins];
  Int_t nTotNoTracks=0;
  Int_t nTotTracksFB128=0;
  if(fIsMC) {
    Int_t nTotNoTracksMC=0;
    Int_t nTotNoTracksReco=0;
    if(fUseRecoNchForMC) nTotNoTracksReco = GetNtotTracks(fAOD,ptMin,ptMax,vtxp);
    TClonesArray *tca = (TClonesArray*)fInputEvent->FindListObject("mcparticles");
    Int_t nPrim = tca->GetEntries();
    if(nPrim<1) return;
    AliAODMCParticle *lPart;
    for(Int_t ipart = 0; ipart < nPrim; ipart++) {
      lPart = (AliAODMCParticle*)tca->At(ipart);
      if (!lPart->IsPhysicalPrimary()) continue;
      if (lPart->Charge()==0.) continue;
      if(fRequirePositive && lPart->Charge()<0) continue;
      Int_t pdgcode = TMath::Abs(lPart->PdgCode());
      int PIDIndex = 0;
      if(pdgcode==211) PIDIndex = 1;
      if(pdgcode==321) PIDIndex = 2;
      if(pdgcode==2212) PIDIndex = 3;
      //Hardcoded cuts to inhereted from AcceptAODTrack
      Double_t leta = lPart->Eta();
      if(TMath::Abs(leta) > 0.8) continue;
      Double_t pt = lPart->Pt();
      if(!fUseChargedPtCut && (pt<ptMin || pt>ptMax)) continue;
      if(leta<-fEtaV2Sep) lNegCount++;
      if(leta>fEtaV2Sep) lPosCount++;
      if(TMath::Abs(leta)<fEtaNch) nTotNoTracksMC++; //Nch calculated in EtaNch region
      if(TMath::Abs(leta)<fEta)  { //for mean pt, only consider -0.4-0.4 region
        FillWPCounter(wp[0],1,pt); 
        FillWPCounter(wpPt[0],1,pt);
          if(!fDisablePID) {
            if(PIDIndex==1) { FillWPCounter(wp[PIDIndex],1,pt); FillWPCounter(wpPt[PIDIndex],1,pt); }
            if(PIDIndex==2) { FillWPCounter(wp[PIDIndex],1,pt); FillWPCounter(wpPt[PIDIndex],1,pt); }
            if(PIDIndex==3) { FillWPCounter(wp[PIDIndex],1,pt); FillWPCounter(wpPt[PIDIndex],1,pt); }
        }
      }
      if(fUseChargedPtCut) { //Using different pt cuts for charged and PID particles
        if(PIDIndex && pt>ptMin && pt<ptMax) { fPtDist->Fill(pt); fGFW->Fill(leta,1,lPart->Phi(),1,(1<<(PIDIndex+1))+(1<<(PIDIndex+4))); } 
        if(pt>fchPtMin && pt<fchPtMax) { fChPtDist->Fill(pt); fGFW->Fill(leta,1,lPart->Phi(),1,3); }
        continue;
      }
      if(PIDIndex) fGFW->Fill(leta,1,lPart->Phi(),1,(1<<(PIDIndex+1))+(1<<(PIDIndex+4))); //filling both gap and full for PID
      fGFW->Fill(leta,1,lPart->Phi(),1,3); //filling both gap (bit mask 1) and full (bit maks 2). Since this is MC, weight is 1.
    };
    nTotNoTracks = fUseRecoNchForMC?nTotNoTracksReco:nTotNoTracksMC;
    if(fUseRecoNchForMC) fNchTrueVsReco->Fill(nTotNoTracksMC,nTotNoTracksReco);
  } else {
    if(!fUsePIDNUA && !LoadMyWeights(fAOD->GetRunNumber())) return; //Only load wieghts for data
    Bool_t usingPseudoEff = (fPseudoEfficiency<1);
    for(Int_t lTr=0;lTr<fAOD->GetNumberOfTracks();lTr++) {
      if(usingPseudoEff) if(fRndm->Uniform()>fPseudoEfficiency) continue;
      lTrack = (AliAODTrack*)fAOD->GetTrack(lTr);
      if(!lTrack) continue;
      if(fRequirePositive && lTrack->Charge()<0) continue;
      Int_t PIDIndex = 0;
      if(!fDisablePID) PIDIndex = GetBayesPIDIndex(lTrack)+1;
    
      Double_t leta = lTrack->Eta();
      Double_t trackXYZ[] = {0.,0.,0.};
      //Counting FB128 for QA:
      if(lTrack->TestFilterBit(128)) nTotTracksFB128++;
      if(!AcceptAODTrack(lTrack,trackXYZ,ptMin,ptMax,vtxp)) continue;
      nTotNoTracks++;
      if(leta<-fEtaV2Sep) lNegCount++;
      if(leta>fEtaV2Sep) lPosCount++;
      if(fEtaV2Sep>0 && TMath::Abs(leta)<fEtaV2Sep) lMidCount++;
      Double_t lpt = lTrack->Pt();
      Double_t weff = (fUse2DEff)?fEfficiency[iCent][0]->GetBinContent(fEfficiency[iCent][0]->FindBin(lpt,leta)):fEfficiencies[iCent]->GetBinContent(fEfficiencies[iCent]->FindBin(lpt));
      if(weff==0.0) continue;
      weff = 1./weff; 
      Double_t weff_PID;
      Bool_t noPID = fDisablePID;
      if(PIDIndex) {
        weff_PID = (fUse2DEff)?fEfficiency[iCent][PIDIndex]->GetBinContent(fEfficiency[iCent][PIDIndex]->FindBin(lpt,leta)):weff;
      }
      if(TMath::Abs(lTrack->Eta())<fEta)  { 
        FillWPCounter(wp[0],weff,lpt); 
        FillWPCounter(wpPt[0],weff,lpt);
        if(PIDIndex && weff_PID != 0.0) {
          FillWPCounter(wp[PIDIndex],1./weff_PID,lpt); 
          FillWPCounter(wpPt[PIDIndex],1./weff_PID,lpt);
        }
      }  
      Double_t wacc = fWeights[0]->GetNUA(lTrack->Phi(),lTrack->Eta(),vz);
      if(PIDIndex && weff_PID != 0.0) { 
        Double_t wacc_PID = 0.0;
        (fUsePIDNUA)?wacc_PID = fWeights[PIDIndex]->GetNUA(lTrack->Phi(),lTrack->Eta(),vz):wacc;
        fGFW->Fill(lTrack->Eta(),1,lTrack->Phi(),wacc_PID*1./weff_PID,(1<<(PIDIndex+1))+(1<<(PIDIndex+4))); //filling both gap and full for PID
      }
      fGFW->Fill(lTrack->Eta(),1,lTrack->Phi(),wacc*weff,3); //filling both gap (bit mask 1) and full (bit mask 2)
    };
  };
  if(wp[0][0]==0) return; //if no single charged particles, then surely no PID either, no sense to continue
  fEventCount->Fill("Tracks",1);
  fMultiVsV0MCorr[0]->Fill(l_Cent,nTotNoTracks);
  //here in principle one could use the GFW output to check if the values are calculated, but this is more efficient
  if(fConsistencyFlag&1) if(!lPosCount || !lNegCount) return; // only events where v2{2, gap} could be calculated
  if(fConsistencyFlag&2) if(nTotNoTracks<4) return; //only events where v2{4} can be calculated (assuming same region as nch)
  if(fConsistencyFlag&4) if(lPosCount<2 || lNegCount<2) return; //Only events where v2{4, gap} can be calculated
  if(fConsistencyFlag&8) if(lMidCount<2) return; //If less than 2 particles in mid, reject. Relevant, if calculating v24{3-sub}
  fMultiVsV0MCorr[1]->Fill(l_Cent,nTotNoTracks);
  //Filling pT variance
  Double_t l_Multi = fUseNch?(1.0*nTotNoTracks):l_Cent;
  //A check in case l_Multi is completely off the charts (in MC, sometimes it ends up being... -Xe-310???)
  if(fUseNch && l_Multi<1) return;
  //Fetching number of ESD tracks -> for QA. Only after all the events are/were rejected
  AliAODHeader *head = (AliAODHeader*)fAOD->GetHeader();
  Int_t nESD = head->GetNumberOfESDTracks();
  fESDvsFB128->Fill(nTotTracksFB128,nESD);
  Double_t l_Random = fRndm->Rndm();
  int endPID = (fDisablePID)?1:4;
  for(int i(0);i<endPID;++i){
    fCkCont[i]->FillObs(wp[i],l_Multi,l_Random);
    fPtCont[i]->FillRecursive(wpPt[i],l_Multi,l_Random);
    fPtCont[i]->FillCk(wpPt[i],l_Multi,l_Random);
    fPtCont[i]->FillSkew(wpPt[i],l_Multi,l_Random);
    fPtCont[i]->FillKurtosis(wpPt[i],l_Multi,l_Random);
  }
  if(fFillMptPowers) {
    for(int i(0); i<endPID;++i) {
      if(wp[i][0]==0.0) continue;
      Double_t mptev = wp[i][1]/wp[i][0];
      fMpt[8*i]->FillProfile(l_Multi,mptev,fUseWeightsOne?1:wp[i][0],l_Random);
      fMpt[1+8*i]->FillProfile(l_Multi,mptev*mptev,fUseWeightsOne?1:(wp[i][0]*wp[i][0]),l_Random);
      fMpt[2+8*i]->FillProfile(l_Multi,mptev*mptev*mptev,fUseWeightsOne?1:(wp[i][0]*wp[i][0]*wp[i][0]),l_Random);
      fMpt[3+8*i]->FillProfile(l_Multi,mptev*mptev*mptev*mptev,fUseWeightsOne?1:(wp[i][0]*wp[i][0]*wp[i][0]*wp[i][0]),l_Random);
      fMpt[4+8*i]->FillProfile(l_Multi,mptev*mptev*mptev*mptev*mptev,fUseWeightsOne?1:(wp[i][0]*wp[i][0]*wp[i][0]*wp[i][0]*wp[i][0]),l_Random);
      fMpt[5+8*i]->FillProfile(l_Multi,mptev*mptev*mptev*mptev*mptev*mptev,fUseWeightsOne?1:(wp[i][0]*wp[i][0]*wp[i][0]*wp[i][0]*wp[i][0]*wp[i][0]),l_Random);
      fMpt[6+8*i]->FillProfile(l_Multi,mptev*mptev*mptev*mptev*mptev*mptev*mptev,fUseWeightsOne?1:(wp[i][0]*wp[i][0]*wp[i][0]*wp[i][0]*wp[i][0]*wp[i][0]*wp[i][0]),l_Random);
      fMpt[7+8*i]->FillProfile(l_Multi,mptev*mptev*mptev*mptev*mptev*mptev*mptev*mptev,fUseWeightsOne?1:(wp[i][0]*wp[i][0]*wp[i][0]*wp[i][0]*wp[i][0]*wp[i][0]*wp[i][0]*wp[i][0]),l_Random);
    }
  }
  fV0MMulti->Fill(l_Cent);
  fMultiDist->Fill(l_Multi);
  PostData(1,fptVarList);
  //Filling FCs
  for(Int_t l_ind=0; l_ind<corrconfigs.size(); l_ind++) {
    Bool_t filled = FillFCs(corrconfigs.at(l_ind),l_Multi,l_Random);
  };
  PostData(2,fFC);
  for(int i(0);i<endPID;++i) {
    if(wp[i][0]==0.0) continue;
    Double_t mptev = wp[i][1]/wp[i][0];
    FillCovariance(fCovariance[Ncovpfs*i],corrconfigs.at(i),l_Multi,mptev,wp[i][0],l_Random);
    FillCovariance(fCovariance[1+Ncovpfs*i],corrconfigs.at(i),l_Multi,1,wp[i][0],l_Random);
    FillCovariance(fCovariance[2+Ncovpfs*i],corrconfigs.at(16+i),l_Multi,mptev,wp[i][0],l_Random);
    FillCovariance(fCovariance[3+Ncovpfs*i],corrconfigs.at(16+i),l_Multi,1,wp[i][0],l_Random);
    FillCovariance(fCovariance[4+Ncovpfs*i],corrconfigs.at(33+i),l_Multi,mptev,wp[i][0],l_Random);
    FillCovariance(fCovariance[5+Ncovpfs*i],corrconfigs.at(33+i),l_Multi,1,wp[i][0],l_Random);
    FillCovariance(fCovariance[6+Ncovpfs*i],corrconfigs.at(8+i),l_Multi,mptev,wp[i][0],l_Random);
    FillCovariance(fCovariance[7+Ncovpfs*i],corrconfigs.at(8+i),l_Multi,1,wp[i][0],l_Random);
    FillCovariance(fCovariance[8+Ncovpfs*i],corrconfigs.at(24+i),l_Multi,mptev,wp[i][0],l_Random);
    FillCovariance(fCovariance[9+Ncovpfs*i],corrconfigs.at(24+i),l_Multi,1,wp[i][0],l_Random);
    //Covariance of vn with multi-particle pt-correlation
    vector<double> pt2corr = fPtCont[i]->getEventCorrelation(wpPt[i],2);
    if(pt2corr[1]!=0) {
      double pt2ev = pt2corr[0]/pt2corr[1];
      FillCovariance(fCovariance[10+Ncovpfs*i],corrconfigs.at(i),l_Multi,pt2ev,pt2corr[1],l_Random);
      FillCovariance(fCovariance[11+Ncovpfs*i],corrconfigs.at(16+i),l_Multi,pt2ev,pt2corr[1],l_Random);
    }
    vector<double> pt3corr = fPtCont[i]->getEventCorrelation(wpPt[i],3);
    if(pt3corr[1]!=0) {
      double pt3ev = pt3corr[0]/pt3corr[1];
      FillCovariance(fCovariance[12+Ncovpfs*i],corrconfigs.at(i),l_Multi,pt3ev,pt3corr[1],l_Random);
    }
    vector<double> pt4corr = fPtCont[i]->getEventCorrelation(wpPt[i],4);
    if(pt4corr[1]!=0) {
      double pt4ev = pt4corr[0]/pt4corr[1];
      FillCovariance(fCovariance[13+Ncovpfs*i],corrconfigs.at(i),l_Multi,pt4ev,pt4corr[1],l_Random);
    }
    //Covariance of vn with powers of mpt
    if(fFillMptPowers) {
      FillCovariance(fCovariancePowerMpt[3*i],corrconfigs.at(i),l_Multi,mptev*mptev,wp[i][0]*wp[i][0],l_Random);
      FillCovariance(fCovariancePowerMpt[1+3*i],corrconfigs.at(i),l_Multi,mptev*mptev*mptev,wp[i][0]*wp[i][0]*wp[i][0],l_Random);
      FillCovariance(fCovariancePowerMpt[2+3*i],corrconfigs.at(i),l_Multi,mptev*mptev*mptev*mptev,wp[i][0]*wp[i][0]*wp[i][0]*wp[i][0],l_Random);
    }
  }
  PostData(3,fCovList);
}
Bool_t AliAnalysisTaskDeform::FillFCs(const AliGFW::CorrConfig &corconf, const Double_t &cent, const Double_t &rndmn, const Bool_t debug) {
  Double_t dnx, val;
  dnx = fGFW->Calculate(corconf,0,kTRUE).Re();
  if(debug) printf("FillFCs: dnx = %f\n",dnx);
  if(dnx==0) return kFALSE;
  if(!corconf.pTDif) {
    val = fGFW->Calculate(corconf,0,kFALSE).Re()/dnx;
    if(debug) printf("FillFCs: val = %f\n",val);
    if(TMath::Abs(val)<1)
      fFC->FillProfile(corconf.Head.Data(),cent,val,fUseWeightsOne?1:dnx,rndmn);
    return kTRUE;
  };
  return kTRUE;
};
Bool_t AliAnalysisTaskDeform::Fillv2dPtFCs(const AliGFW::CorrConfig &corconf, const Double_t &dpt, const Double_t &rndmn, const Int_t index) {
  if(!index || index>fV2dPtList->GetEntries()) return kFALSE;
  Double_t dnx, val;
  dnx = fGFW->Calculate(corconf,0,kTRUE).Re();
  if(dnx==0) return kFALSE;
  if(!corconf.pTDif) {
    val = fGFW->Calculate(corconf,0,kFALSE).Re()/dnx;
    if(TMath::Abs(val)<1)
      ((AliGFWFlowContainer*)fV2dPtList->At(index))->FillProfile(corconf.Head.Data(),dpt,val,fUseWeightsOne?1:dnx,rndmn);
    return kTRUE;
  };
  return kTRUE;
};
Bool_t AliAnalysisTaskDeform::FillCovariance(AliProfileBS *target, const AliGFW::CorrConfig &corconf, const Double_t &cent, const Double_t &d_mpt, const Double_t &dw_mpt, const Double_t &l_rndm) {
  Double_t dnx, val;
  dnx = fGFW->Calculate(corconf,0,kTRUE).Re();
  if(dnx==0) return kFALSE;
  if(!corconf.pTDif) {
    val = fGFW->Calculate(corconf,0,kFALSE).Re()/dnx;
    if(TMath::Abs(val)<1)
      target->FillProfile(cent,val*d_mpt,fUseWeightsOne?1:dnx*dw_mpt,l_rndm);
    return kTRUE;
  };
  return kTRUE;
};
void AliAnalysisTaskDeform::CreateCorrConfigs() {
  corrconfigs.push_back(GetConf("ChGap22","refP {2} refN {-2}", kFALSE));     //ChGap22 0-3
  corrconfigs.push_back(GetConf("PiGap22","PiRefP {2} PiRefN {-2}", kFALSE));
  corrconfigs.push_back(GetConf("KaGap22","KaRefP {2} KaRefN {-2}", kFALSE));
  corrconfigs.push_back(GetConf("PrGap22","PrRefP {2} PrRefN {-2}", kFALSE));

  corrconfigs.push_back(GetConf("ChFull22","mid {2 -2}", kFALSE));  //ChFull22 4-7
  corrconfigs.push_back(GetConf("PiFull22","PiMid {2 -2}", kFALSE));
  corrconfigs.push_back(GetConf("KaFull22","KaMid {2 -2}", kFALSE));
  corrconfigs.push_back(GetConf("PrFull22","PrMid {2 -2}", kFALSE));

  corrconfigs.push_back(GetConf("ChGap24","refP {2 2} refN {-2 -2}", kFALSE));  //ChGap24 8-11
  corrconfigs.push_back(GetConf("PiGap24","PiRefP {2 2} PiRefN {-2 -2}", kFALSE));
  corrconfigs.push_back(GetConf("KaGap24","KaRefP {2 2} KaRefN {-2 -2}", kFALSE));
  corrconfigs.push_back(GetConf("PrGap24","PrRefP {2 2} PrRefN {-2 -2}", kFALSE));

  corrconfigs.push_back(GetConf("ChFull24","mid {2 2 -2 -2}", kFALSE));   //ChFull24 12-15
  corrconfigs.push_back(GetConf("PiFull24","PiMid {2 2 -2 -2}", kFALSE));
  corrconfigs.push_back(GetConf("KaFull24","KaMid {2 2 -2 -2}", kFALSE));
  corrconfigs.push_back(GetConf("PrFull24","PrMid {2 2 -2 -2}", kFALSE));
//v3:
  corrconfigs.push_back(GetConf("ChGap32","refP {3} refN {-3}", kFALSE));   //ChGap32 16-19
  corrconfigs.push_back(GetConf("PiGap32","PiRefP {3} PiRefN {-3}", kFALSE));
  corrconfigs.push_back(GetConf("KaGap32","KaRefP {3} KaRefN {-3}", kFALSE));
  corrconfigs.push_back(GetConf("PrGap32","PrRefP {3} PrRefN {-3}", kFALSE));

  corrconfigs.push_back(GetConf("ChFull32","mid {3 -3}", kFALSE));   //ChFull32 20-23
  corrconfigs.push_back(GetConf("PiFull32","PiMid {3 -3}", kFALSE));
  corrconfigs.push_back(GetConf("KaFull32","KaMid {3 -3}", kFALSE));
  corrconfigs.push_back(GetConf("PrFull32","PrMid {3 -3}", kFALSE));
  
  corrconfigs.push_back(GetConf("ChGap34","refP {3 3} refN {-3 -3}", kFALSE));   //ChGap34 24-27
  corrconfigs.push_back(GetConf("PiGap34","PiRefP {3 3} PiRefN {-3 -3}", kFALSE));
  corrconfigs.push_back(GetConf("KaGap34","KaRefP {3 3} KaRefN {-3 -3}", kFALSE));
  corrconfigs.push_back(GetConf("PrGap34","PrRefP {3 3} PrRefN {-3 -3}", kFALSE));

  corrconfigs.push_back(GetConf("ChFull34","mid {3 3 -3 -3}", kFALSE));    //ChFull34 28-31
  corrconfigs.push_back(GetConf("PiFull34","PiMid {3 3 -3 -3}", kFALSE));
  corrconfigs.push_back(GetConf("KaFull34","KaMid {3 3 -3 -3}", kFALSE));
  corrconfigs.push_back(GetConf("PrFull34","PrMid {3 3 -3 -3}", kFALSE));

  corrconfigs.push_back(GetConf("ChGap42","refP {4} refN {-4}", kFALSE));   //ChGap42 32

  corrconfigs.push_back(GetConf("ChSC234","refP {2 3} refN {-2 -3}", kFALSE));    //  ChSC234 33-36
  corrconfigs.push_back(GetConf("PiSC234","PiRefP {2 3} PiRefN {-2 -3}", kFALSE));    
  corrconfigs.push_back(GetConf("KaSC234","KaRefP {2 3} KaRefN {-2 -3}", kFALSE));    
  corrconfigs.push_back(GetConf("PrSC234","PrRefP {2 3} PrRefN {-2 -3}", kFALSE));    

  corrconfigs.push_back(GetConf("ChSC244","refP {2 4} refN {-2 -4}", kFALSE));    //  ChSC244 37
  corrconfigs.push_back(GetConf("ChFull28","mid {2 2 2 2 -2 -2 -2 -2}",kFALSE));  //  ChFull28 38
  
//v24 3-sub
  if(fEtaV2Sep<0) return; //if eta < 0, then pos & neg are w/o SE and thus doesn't make sense to calculate v24
  corrconfigs.push_back(GetConf("LM22","refP {2} subMid {-2}", kFALSE));
  corrconfigs.push_back(GetConf("MR22","subMid {2} refN {-2}", kFALSE));
  corrconfigs.push_back(GetConf("LR22","refP {2} refN {-2}", kFALSE));
  corrconfigs.push_back(GetConf("LLMR24","refP {2 2} subMid {-2} refN {-2}", kFALSE));
  corrconfigs.push_back(GetConf("LMMR24","refP {2} subMid {-2 -2} refN {2}", kFALSE));
  corrconfigs.push_back(GetConf("LMRR24","refP {2} subMid {2} refN {-2 -2}", kFALSE));

  corrconfigs.push_back(GetConf("PiDiff22","PiMid {2} mid {-2}",kFALSE));
  corrconfigs.push_back(GetConf("KaDiff22","KaMid {2} mid {-2}",kFALSE));
  corrconfigs.push_back(GetConf("PrDiff22","PrMid {2} mid {-2}",kFALSE));  
  corrconfigs.push_back(GetConf("PiDiff24","PiMid {2} mid {2 -2 -2}",kFALSE));
  corrconfigs.push_back(GetConf("KaDiff24","KaMid {2} mid {2 -2 -2}",kFALSE));
  corrconfigs.push_back(GetConf("PrDiff24","PrMid {2} mid {2 -2 -2}",kFALSE));
  return;
};
void AliAnalysisTaskDeform::GetSingleWeightFromList(AliGFWWeights **inWeights, TString pf) {
  (*inWeights) = (AliGFWWeights*)fWeightList->FindObject(Form("weight_%s",pf.Data()));
  if(!(*inWeights)) AliFatal(Form("Could not find weight %s in weight list\n", pf.Data()));
  if(!(*inWeights)->CalculateIntegratedEff()) AliFatal("Could not calculate integrated efficiency!\n");
  (*inWeights)->CreateNUA();
};
void AliAnalysisTaskDeform::LoadWeightAndMPT() {//AliAODEvent *inEv) {
  if(!fRequireReloadOnRunChange) return;
  if(!fWeightList) AliFatal("Weight list not set!\n");

  // Int_t l_RunNo = inEv->GetRunNumber();
  TString spNames[] = {"ch","pi","ka","pr"};
  fWeights = new AliGFWWeights*[4];
  for(Int_t i=0;i<4;i++) GetSingleWeightFromList(&fWeights[i],spNames[i]);
}
Int_t AliAnalysisTaskDeform::GetBayesPIDIndex(AliVTrack *l_track) {
  Double_t l_Probs[AliPID::kSPECIES];
  Double_t l_MaxProb[] = {0.95,0.85,0.85};
  Bool_t l_TOFUsed = fBayesPID->ComputeProbabilities(l_track, fPIDResponse, l_Probs) & AliPIDResponse::kDetTOF;
  Int_t pidInd = 0;
  for(Int_t i=0;i<AliPID::kSPECIES; i++) pidInd=(l_Probs[i]>l_Probs[pidInd])?i:pidInd;
  Int_t retInd = pidInd-AliPID::kPion; //Not interested in e+mu, so realign to 0
  if(retInd<0 || retInd>2) return -1; //Shouldn't be larger than 2, but just to be safe
  if(l_Probs[pidInd] < l_MaxProb[retInd]) return -1;
  //check nsigma cuts
  if(TMath::Abs(fPIDResponse->NumberOfSigmasTPC(l_track,(AliPID::EParticleType)pidInd))>3) return -1;
  if(l_TOFUsed) if(TMath::Abs(fPIDResponse->NumberOfSigmasTOF(l_track,(AliPID::EParticleType)pidInd))>3) return -1;
  return retInd;
}
Bool_t AliAnalysisTaskDeform::LoadMyWeights(const Int_t &lRunNo) {
  if(!fWeightList) AliFatal("NUA list not set or does not exist!\n");
  if(lRunNo && lRunNo == fRunNo) return kTRUE;
  TString lBase(""); //base
  TString lSubfix(""); //subfix
  if(fWeightSubfix.IsNull()) { //If none specified, then follow the usual procedure
    lBase = Form("w%i",lRunNo);
    lSubfix = fGFWSelection->NeedsExtraWeight()?fGFWSelection->GetSystPF():"";
  } else {
    Int_t delind = fWeightSubfix.Index(";");
    if(delind<0) {//Only base
      lBase = fWeightSubfix;
      lSubfix = fGFWSelection->NeedsExtraWeight()?fGFWSelection->GetSystPF():"";
    } else if(!delind) {//Standard base, override subfix
      lBase = Form("w%i",lRunNo);
      lSubfix = fWeightSubfix(1,fWeightSubfix.Length());
    } else {
      lBase = fWeightSubfix(0,delind);
      lSubfix = fWeightSubfix(delind+1,fWeightSubfix.Length());
    }
  }
  lBase+=lSubfix;
  fWeights[0] = (AliGFWWeights*)fWeightList->FindObject(lBase.Data());
  if(!fWeights[0]) AliFatal(Form("Weights %s not not found in the list provided!\n",lBase.Data()));
  fWeights[0]->CreateNUA();
  return kTRUE;
}
void AliAnalysisTaskDeform::SetPtBins(Int_t nPtBins, Double_t *PtBins) {
  if(fPtAxis) delete fPtAxis;
  fPtAxis = new TAxis(nPtBins, PtBins);
}
void AliAnalysisTaskDeform::SetEtaBins(Int_t nbins, Double_t *etabins) {
  if(fEtaAxis) delete fEtaAxis;
  fEtaAxis = new TAxis(nbins,etabins);
}
void AliAnalysisTaskDeform::SetMultiBins(Int_t nMultiBins, Double_t *multibins) {
  if(fMultiAxis) delete fMultiAxis;
  fMultiAxis = new TAxis(nMultiBins, multibins);
}
void AliAnalysisTaskDeform::SetIPBins(Int_t nIPBins, Double_t *IPbins) {
  if(fIPAxis) delete fIPAxis;
  fIPAxis = new TAxis(nIPBins, IPbins);
}
void AliAnalysisTaskDeform::SetV0MBins(Int_t nMultiBins, Double_t *multibins) {
  if(fV0MMultiAxis) delete fV0MMultiAxis;
  fV0MMultiAxis = new TAxis(nMultiBins, multibins);
}
void AliAnalysisTaskDeform::SetV2dPtMultiBins(Int_t nMultiBins, Double_t *multibins) {
  if(fV2dPtMulti) delete fV2dPtMulti;
  fV2dPtMulti = new TH1D("v2_vs_mpt_mbins","v2_vs_mpt_mbins",nMultiBins, multibins);
}
Double_t *AliAnalysisTaskDeform::GetBinsFromAxis(TAxis *inax) {
  Int_t lBins = inax->GetNbins();
  Double_t *retBins = new Double_t[lBins+1];
  for(Int_t i=0;i<lBins;i++)
    retBins[i] = inax->GetBinLowEdge(i+1);
  retBins[lBins] = inax->GetBinUpEdge(lBins);
  return retBins;
}
Int_t AliAnalysisTaskDeform::GetPIDIndex(const Int_t &pdgcode) {
  if(TMath::Abs(pdgcode)==211) return 1;
  if(TMath::Abs(pdgcode)==321) return 2;
  if(TMath::Abs(pdgcode)==2212) return 3;
  if(TMath::Abs(pdgcode)==3222 || TMath::Abs(pdgcode)==3112) return 4;
  if(TMath::Abs(pdgcode)==3312) return 5;
  if(TMath::Abs(pdgcode)==3334) return 6;
  return 0;
}
Bool_t AliAnalysisTaskDeform::AcceptCustomEvent(AliAODEvent* fAOD) { //From Alex
  Float_t v0Centr    = -100.;
  Float_t cl1Centr   = -100.;
  Float_t cl0Centr   = -100.;
  AliMultSelection* MultSelection = 0x0;
  MultSelection = (AliMultSelection*)fAOD->FindListObject("MultSelection");
  if(!MultSelection) {
    AliWarning("AliMultSelection object not found!");
    return kFALSE;
  } else {
    v0Centr = MultSelection->GetMultiplicityPercentile("V0M");
    cl1Centr = MultSelection->GetMultiplicityPercentile("CL1");
    cl0Centr = MultSelection->GetMultiplicityPercentile("CL0");
  }
  if(v0Centr>=80.||v0Centr<0) return kFALSE; //This would have to be adjusted for vs. V0M
  Int_t nITSClsLy0 = fAOD->GetNumberOfITSClusters(0);
  Int_t nITSClsLy1 = fAOD->GetNumberOfITSClusters(1);
  Int_t nITSCls = nITSClsLy0 + nITSClsLy1;
  AliAODTracklets *aodTrkl = (AliAODTracklets*)fAOD->GetTracklets();
  Int_t nITSTrkls = aodTrkl->GetNumberOfTracklets(); //ESD: esd->GetMultiplicity()->GetNumberOfTracklets()
  const Int_t nTracks = fAOD->GetNumberOfTracks(); //ESD: est->GetNumberOfTracks()
  Int_t multTrk = 0;
  for (Int_t it = 0; it < nTracks; it++) {
    AliAODTrack* aodTrk = (AliAODTrack*)fAOD->GetTrack(it);
    if(!aodTrk){
        delete aodTrk;
        continue;
    }
    if(aodTrk->TestFilterBit(32)) multTrk++; //GetStandardITSTPCTrackCuts2011()
  }
  AliAODVZERO* aodV0 = fAOD->GetVZEROData();
  Float_t multV0a = aodV0->GetMTotV0A();
  Float_t multV0c = aodV0->GetMTotV0C();
  Float_t multV0Tot = multV0a + multV0c;
  UShort_t multV0aOn = aodV0->GetTriggerChargeA();
  UShort_t multV0cOn = aodV0->GetTriggerChargeC();
  UShort_t multV0On = multV0aOn + multV0cOn;
  //pile-up cuts
  if(cl0Centr<fCenCutLowPU->Eval(v0Centr)) return kFALSE;
  if (cl0Centr > fCenCutHighPU->Eval(v0Centr)) return kFALSE;
  if(Float_t(nITSCls)>fSPDCutPU->Eval(nITSTrkls)) return kFALSE;
  if(multV0On<fV0CutPU->Eval(multV0Tot)) return kFALSE;
  if(Float_t(multTrk)<fMultCutPU->Eval(v0Centr)) return kFALSE;
  if(((AliAODHeader*)fAOD->GetHeader())->GetRefMultiplicityComb08()<0) return kFALSE;
  if(fAOD->IsIncompleteDAQ()) return kFALSE;
  return kTRUE;
}
Bool_t AliAnalysisTaskDeform::AcceptCustomEvent(AliESDEvent* fESD) { //From Alex
  Float_t v0Centr    = -100.;
  Float_t cl1Centr   = -100.;
  Float_t cl0Centr   = -100.;
  AliMultSelection* MultSelection = 0x0;
  MultSelection = (AliMultSelection*)fESD->FindListObject("MultSelection");
  if(!MultSelection) {
    AliWarning("AliMultSelection object not found!");
    return kFALSE;
  } else {
    v0Centr = MultSelection->GetMultiplicityPercentile("V0M");
    cl1Centr = MultSelection->GetMultiplicityPercentile("CL1");
    cl0Centr = MultSelection->GetMultiplicityPercentile("CL0");
  }
  if(v0Centr>=80.||v0Centr<0) return kFALSE; //This would have to be adjusted for vs. V0M
  Int_t nITSClsLy0 = fESD->GetNumberOfITSClusters(0);
  Int_t nITSClsLy1 = fESD->GetNumberOfITSClusters(1);
  Int_t nITSCls = nITSClsLy0 + nITSClsLy1;
  Int_t nITSTrkls = fESD->GetMultiplicity()->GetNumberOfTracklets();
  const Int_t nTracks = fESD->GetNumberOfTracks();
  Int_t multTrk = 0;
  AliESDtrack *esdTrack;
  for (Int_t it = 0; it < nTracks; it++) {
    esdTrack = (AliESDtrack*)fESD->GetTrack(it);
    if(!esdTrack) continue;
    if(fStdTPCITS2011->AcceptTrack(esdTrack)) multTrk++;
  }
  AliESDVZERO* esdV0 = fESD->GetVZEROData();
  Float_t multV0a = esdV0->GetMTotV0A();
  Float_t multV0c = esdV0->GetMTotV0C();
  Float_t multV0Tot = multV0a + multV0c;
  UShort_t multV0aOn = esdV0->GetTriggerChargeA();
  UShort_t multV0cOn = esdV0->GetTriggerChargeC();
  UShort_t multV0On = multV0aOn + multV0cOn;
  //pile-up cuts
  if(cl0Centr<fCenCutLowPU->Eval(v0Centr)) return kFALSE;
  if (cl0Centr > fCenCutHighPU->Eval(v0Centr)) return kFALSE;
  if(Float_t(nITSCls)>fSPDCutPU->Eval(nITSTrkls)) return kFALSE;
  if(multV0On<fV0CutPU->Eval(multV0Tot)) return kFALSE; //Problematic for MC for whatever reason? On AODs work perfectly fine
  if(Float_t(multTrk)<fMultCutPU->Eval(v0Centr)) return kFALSE;
  AliESDtrackCuts::MultEstTrackType estType = fESD->GetPrimaryVertexTracks()->GetStatus() ? AliESDtrackCuts::kTrackletsITSTPC : AliESDtrackCuts::kTracklets;
  if(AliESDtrackCuts::GetReferenceMultiplicity(fESD,estType,0.8) < 0) return kFALSE;
  if(fESD->IsIncompleteDAQ()) return kFALSE;
  return kTRUE;
}
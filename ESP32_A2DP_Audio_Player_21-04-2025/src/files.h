
// 16 BIT PCM
// MONO
// EXPORT SINGNED 16 BIT PCM
// EXPORT 24000 CUSTOM


#ifndef CARD_H
#define CARD_H


const char* atozB4[] = {
  "SYSLAB_A.wav","SYSLAB_B.wav","SYSLAB_C.wav","SYSLAB_D.wav","SYSLAB_E.wav","SYSLAB_F.wav","SYSLAB_G.wav","SYSLAB_H.wav","SYSLAB_I.wav","SYSLAB_J.wav","SYSLAB_K.wav","SYSLAB_L.wav",
  "SYSLAB_M.wav","SYSLAB_N.wav","SYSLAB_O.wav","SYSLAB_P.wav","SYSLAB_Q.wav","SYSLAB_R.wav","SYSLAB_S.wav","SYSLAB_T.wav","SYSLAB_U.wav","SYSLAB_V.wav","SYSLAB_W.wav","SYSLAB_X.wav","SYSLAB_Y.wav","SYSLAB_Z.wav"
};
const int arraysizeatozB4 = sizeof(atozB4)/sizeof(atozB4[0]);
/*----------------------------------------------------------------------------------------------------------------*/
const char* CAT[] = {
  "SYSLAB_CAT.wav","SYSLAB_C.wav","SYSLAB_A.wav","SYSLAB_T.wav"
};
const int arraysizeCAT = sizeof(CAT)/sizeof(CAT[0]);
/*----------------------------------------------------------------------------------------------------------------*/
const char* BAT[] ={
  "SYSLAB_BAT.wav","SYSLAB_B.wav","SYSLAB_A.wav","SYSLAB_T.wav"
};
const int arraysizeBAT = sizeof(BAT)/sizeof(BAT[0]);
/*----------------------------------------------------------------------------------------------------------------*/
const char* NOSE[] ={
  "NOSE.wav","SYSLAB_N.wav","SYSLAB_O.wav","SYSLAB_S.wav","SYSLAB_E.wav"
};
const int arraysizeNOSE = sizeof(NOSE)/sizeof(NOSE[0]);
/*----------------------------------------------------------------------------------------------------------------*/
const char* DOG[] = {
  "DOG.wav","SYSLAB_D.wav","SYSLAB_O.wav","SYSLAB_G.wav"
};
const int arraysizeDOG = sizeof(DOG)/sizeof(DOG[0]);
/*----------------------------------------------------------------------------------------------------------------*/
const char* GOAT[] = {
  "GOAT.wav","SYSLAB_G.wav","SYSLAB_O.wav","SYSLAB_A.wav","SYSLAB_T.wav"
};
const int arraysizeGOAT = sizeof(GOAT)/sizeof(GOAT[0]);

//**************Lesson 1*************//
const char* AT[] = {
  "AT.wav","Letter_A.wav","Letter_T.wav"
};
const int arraysizeAT = sizeof(AT)/sizeof(AT[0]);

const char* AN[] = {
  "AN.wav","Letter_A.wav","Letter_N.wav"
};
const int arraysizeAN = sizeof(AN)/sizeof(AN[0]);

const char* SAP[] = {
  "SAP.wav","Letter_S.wav","Letter_A.wav","Letter_P.wav"
};
const int arraysizeSAP = sizeof(SAP)/sizeof(SAP[0]);

const char* TAP[] = {
  "TAP.wav","Letter_T.wav","Letter_A.wav","Letter_P.wav"
};
const int arraysizeTAP = sizeof(TAP)/sizeof(TAP[0]);

const char* PAT[] = {
  "PAT.wav","Letter_P.wav","Letter_A.wav","Letter_T.wav"
};
const int arraysizePAT = sizeof(PAT)/sizeof(PAT[0]);

const char* PAN[] = {
  "PAN.wav","Letter_P.wav","Letter_A.wav","Letter_N.wav"
};
const int arraysizePAN = sizeof(PAN)/sizeof(PAN[0]);

const char* NAP[] = {
  "NAP.wav","Letter_N.wav","Letter_A.wav","Letter_P.wav"
};
const int arraysizeNAP = sizeof(NAP)/sizeof(NAP[0]);

const char* SAT[] = {
  "SAT.wav","Letter_S.wav","Letter_A.wav","Letter_T.wav"
};
const int arraysizeSAT = sizeof(SAT)/sizeof(SAT[0]);

const char* TAN[] = {
  "TAN.wav","Letter_T.wav","Letter_A.wav","Letter_N.wav"
};
const int arraysizeTAN = sizeof(TAN)/sizeof(TAN[0]);

const char* NAT[] = {
  "NAT.wav","Letter_N.wav","Letter_A.wav","Letter_T.wav"
};
const int arraysizeNAT = sizeof(NAT)/sizeof(NAT[0]);

const char* SNP[] = {
  "SNP.wav","Letter_S.wav","Letter_N.wav","Letter_P.wav"
};
const int arraysizeSNP = sizeof(SNP)/sizeof(SNP[0]);

const char* NPA[] = {
  "Twinkle Twinkle Little Star.wav"
};
const int arraysizeNPA = sizeof(NPA)/sizeof(NPA[0]);

const char* APJ[] = {
  "APJ_Abdul_Kalam.wav"
};
const int arraysizeAPJ = sizeof(APJ)/sizeof(APJ[0]);

const char* File_is_not_available[] = {
  "File_is_not_available.wav"
};
const int arrayFile_is_not_available = sizeof(File_is_not_available) / sizeof(File_is_not_available[0]);

const char* Battery_LOW[] = {"Battery_Is_Low.wav"};
const char *POWER_OFF[] = { "POWER OFF.wav" };

#endif
/* 
** Purpose: 42 Interface
**
** Notes:
**   1. Use 42's ReadFromSocket() and WriteToSocket() because they contain
**      autogenerated interface code that uses AcStruct. Don't use 42's iokit's
**      InitSocketClient() becuase it exits the program on errors.
**
** License:
**   Written by David McComas, licensed under the copyleft GNU
**   General Public License (GPL). 
**
** References:
**   1. OpenSatKit Object-based Application Developer's Guide.
**   2. cFS Application Developer's Guide.
**
*/

/*
** Include Files:
*/

#include "cfe.h"
#include "ac42.h"

static void InitParameters(struct AcType *Ac);

static AC42_Class Ac42;

/******************************************************************************
** Function: AC42_Constructor
**
** Initialize a 42 AC data object.
**
** Notes:
**   1. This must be called prior to any other function.
**   2. AC structure initialization mimics 42's AcApp.c dynamic memory 
**      configuration so AcApp's functions can be used unmodified.
**
*/
void AC42_Constructor(void)
{
   int32 CfeStatus;
   
   CFE_PSP_MemSet((void*)&Ac42, 0, sizeof(AC42_Class));

   CfeStatus = OS_MutSemCreate(&Ac42.MutexId, AC42_MUTEX_NAME, 0);
   
   /* Dynamics */
   Ac42.Obj.Nb = AC42_NB;
   Ac42.Obj.B  = &Ac42.Var.B[0];
 
   Ac42.Obj.Ng = AC42_NG;
   Ac42.Obj.G  = &Ac42.Var.G[0];

   /* Sensors */
   Ac42.Obj.Ngyro = AC42_NGYRO;
   Ac42.Obj.Gyro  = &Ac42.Var.Gyro[0];

   Ac42.Obj.Nmag  = AC42_NMAG;
   Ac42.Obj.MAG   = &Ac42.Var.MAG[0];

   Ac42.Obj.Ncss  = AC42_NCSS;
   Ac42.Obj.CSS   = &Ac42.Var.CSS[0];

   Ac42.Obj.Nfss  = AC42_NFSS;
   Ac42.Obj.FSS   = &Ac42.Var.FSS[0];

   Ac42.Obj.Nst   = AC42_NST;
   Ac42.Obj.ST    = &Ac42.Var.ST[0];

   Ac42.Obj.Ngps  = AC42_NGPS;
   Ac42.Obj.GPS   = &Ac42.Var.GPS[0];

   /* Actuators */

   Ac42.Obj.Nwhl = AC42_NWHL;
   Ac42.Obj.Whl  = &Ac42.Var.Whl[0];

   Ac42.Obj.Nmtb = AC42_NMTB;
   Ac42.Obj.MTB  = &Ac42.Var.MTB[0];

   Ac42.Obj.Nthr = AC42_NTHR;
   Ac42.Obj.Thr  = &Ac42.Var.Thr[0];

   InitParameters(&(Ac42.Obj));

} /* End AC42_Constructor() */


/******************************************************************************
** Function: AC42_GetPtr
**
*/
AC42_Class* AC42_GetPtr(void)
{
   
   OS_MutSemTake(Ac42.MutexId); 
   
   return  &Ac42;
 
}

/******************************************************************************
** Function: AC42_ReleasePtr
**
*/
void AC42_ReleasePtr(AC42_Class* Ac)
{

   OS_MutSemGive(Ac42.MutexId);

}


/******************************************************************************
** Function:  InitParameters
**
** This initial version was created by cutting and pasting the default
** parameters in AcParmDump00.txt created by running "./42 OSK" in standalone 
** with "./AcApp 0". AcParmDump00.txt was copied to the osk_42_lib fsw to 
** preserve the initial value file.
**
** The goal is to use a tool to automate the parameter-to-FSW app 
** definition.
*/
static void InitParameters(struct AcType *Ac) 
{

   Ac->svb[0] = 1.000000000000e+00;
   Ac->svb[1] = 0.000000000000e+00;
   Ac->svb[2] = 0.000000000000e+00;
   Ac->bvb[0] = -2.078000000000e-05;
   Ac->bvb[1] = -1.726000000000e-05;
   Ac->bvb[2] = -2.770000000000e-06;
   Ac->Hvb[0] =  1.338004006967e-03;
   Ac->Hvb[1] = -2.356030865574e+00;
   Ac->Hvb[2] = -1.859392602549e-03; 
   Ac->G[0].Cmd.Ang[0] = 1.570796326795e+00;
   Ac->G[0].Cmd.Ang[1] = 0.000000000000e+00;
   Ac->G[0].Cmd.Ang[2] = 0.000000000000e+00;
   Ac->Whl[0].Tcmd = 4.317034137377e-04;
   Ac->Whl[1].Tcmd = 3.908564277547e-04;
   Ac->Whl[2].Tcmd = 1.846120782472e-03;
   Ac->Whl[3].Tcmd = 1.886967768455e-03;
   Ac->MTB[0].Mcmd = 6.494112381320e+00;
   Ac->MTB[1].Mcmd = 4.234444938026e-02;
   Ac->MTB[2].Mcmd = -4.898141533579e+01;
   Ac->Cmd.Ang[0] = 6.916920265054e-310;
   Ac->Cmd.Ang[1] = 6.916920374303e-310;
   Ac->Cmd.Ang[2] = 6.916920368368e-310;
   Ac->ID = 0;
   Ac->EchoEnabled = 1;
   Ac->Nb = 2;
   Ac->Ng = 1;
   Ac->Nwhl = 4;
   Ac->Nmtb = 3;
   Ac->Nthr = 0;
   Ac->Ncmg = 0;
   Ac->Ngyro = 3;
   Ac->Nmag = 3;
   Ac->Ncss = 8;
   Ac->Nfss = 1;
   Ac->Nst = 1;
   Ac->Ngps = 1;
   Ac->Nacc = 0;
   Ac->DT = 2.000000000000e-01;
   Ac->mass = 1.020000000000e+03;
   Ac->cm[0] = 2.041176470588e+00; 
   Ac->cm[1] = -1.798039215686e-01;
   Ac->cm[2] = -1.470588235294e-02;
   Ac->MOI[0][0] = 2.759831372549e+03;
   Ac->MOI[0][1] = 3.775882352941e+02;
   Ac->MOI[0][2] = 3.088235294118e+01;
   Ac->MOI[1][0] = 3.775882352941e+02;
   Ac->MOI[1][1] = 2.117500000000e+03;
   Ac->MOI[1][2] = -1.348529411765e+02;
   Ac->MOI[2][0] = 3.088235294118e+01;
   Ac->MOI[2][1] = -1.348529411765e+02;
   Ac->MOI[2][2] = 3.835272549020e+03;
   Ac->B[0].mass = 1.000000000000e+03;
   Ac->B[0].cm[0] = 2.000000000000e+00;
   Ac->B[0].cm[1] = 0.000000000000e+00;
   Ac->B[0].cm[2] = 0.000000000000e+00;
   Ac->B[0].MOI[0][0] = 1.000000000000e+03;
   Ac->B[0].MOI[0][1] = -0.000000000000e+00;
   Ac->B[0].MOI[0][2] = -0.000000000000e+00;
   Ac->B[0].MOI[1][0] = -0.000000000000e+00;
   Ac->B[0].MOI[1][1] = 2.000000000000e+03;
   Ac->B[0].MOI[1][2] = -0.000000000000e+00;
   Ac->B[0].MOI[2][0] = -0.000000000000e+00;
   Ac->B[0].MOI[2][1] = -0.000000000000e+00;
   Ac->B[0].MOI[2][2] = 2.000000000000e+03;
   Ac->B[1].mass = 2.000000000000e+01;
   Ac->B[1].cm[0] = 0.000000000000e+00;
   Ac->B[1].cm[1] = -8.000000000000e+00;
   Ac->B[1].cm[2] = 0.000000000000e+00;
   Ac->B[1].MOI[0][0] = 1.000000000000e+02;
   Ac->B[1].MOI[0][1] = -0.000000000000e+00;
   Ac->B[1].MOI[0][2] = -0.000000000000e+00;
   Ac->B[1].MOI[1][0] = -0.000000000000e+00;
   Ac->B[1].MOI[1][1] = 2.000000000000e+01;
   Ac->B[1].MOI[1][2] = -0.000000000000e+00;
   Ac->B[1].MOI[2][0] = -0.000000000000e+00;
   Ac->B[1].MOI[2][1] = -0.000000000000e+00;
   Ac->B[1].MOI[2][2] = 1.000000000000e+02;
   Ac->G[0].IsUnderActiveControl = 1;
   Ac->G[0].IsSpherical = 0;
   Ac->G[0].RotDOF = 1;
   Ac->G[0].TrnDOF = 0;
   Ac->G[0].RotSeq = 231;
   Ac->G[0].TrnSeq = 123;
   Ac->G[0].CGiBi[0][0] = 1.000000000000e+00;
   Ac->G[0].CGiBi[0][1] = 0.000000000000e+00;
   Ac->G[0].CGiBi[0][2] = -0.000000000000e+00;
   Ac->G[0].CGiBi[1][0] = -0.000000000000e+00;
   Ac->G[0].CGiBi[1][1] = 1.000000000000e+00;
   Ac->G[0].CGiBi[1][2] = 0.000000000000e+00;
   Ac->G[0].CGiBi[2][0] = 0.000000000000e+00;
   Ac->G[0].CGiBi[2][1] = 0.000000000000e+00;
   Ac->G[0].CGiBi[2][2] = 1.000000000000e+00;
   Ac->G[0].CBoGo[0][0] = 1.000000000000e+00;
   Ac->G[0].CBoGo[0][1] = 0.000000000000e+00;
   Ac->G[0].CBoGo[0][2] = -0.000000000000e+00;
   Ac->G[0].CBoGo[1][0] = -0.000000000000e+00;
   Ac->G[0].CBoGo[1][1] = 1.000000000000e+00;
   Ac->G[0].CBoGo[1][2] = 0.000000000000e+00;
   Ac->G[0].CBoGo[2][0] = 0.000000000000e+00;
   Ac->G[0].CBoGo[2][1] = 0.000000000000e+00;
   Ac->G[0].CBoGo[2][2] = 1.000000000000e+00;
   Ac->G[0].AngGain[0] = 4.000000000000e+00;
   Ac->G[0].AngGain[1] = 0.000000000000e+00;
   Ac->G[0].AngGain[2] =  0.000000000000e+00;
   Ac->G[0].AngRateGain[0] = 4.000000000000e+01;
   Ac->G[0].AngRateGain[1] = 0.000000000000e+00;
   Ac->G[0].AngRateGain[2] = 0.000000000000e+00;
   Ac->G[0].PosGain[0] = 0.000000000000e+00;
   Ac->G[0].PosGain[1] = 0.000000000000e+00;
   Ac->G[0].PosGain[2] = 0.000000000000e+00;
   Ac->G[0].PosRateGain[0] = 0.000000000000e+00;
   Ac->G[0].PosRateGain[1] = 0.000000000000e+00;
   Ac->G[0].PosRateGain[2] = 0.000000000000e+00;
   Ac->G[0].MaxAngRate[0] = 1.745329251994e-02;
   Ac->G[0].MaxAngRate[1] = 0.000000000000e+00;
   Ac->G[0].MaxAngRate[2] = 0.000000000000e+00;
   Ac->G[0].MaxPosRate[0] = 0.000000000000e+00;
   Ac->G[0].MaxPosRate[1] = 0.000000000000e+00;
   Ac->G[0].MaxPosRate[2] = 0.000000000000e+00;
   Ac->G[0].MaxTrq[0] = 1.000000000000e+01;
   Ac->G[0].MaxTrq[1] = 0.000000000000e+00;
   Ac->G[0].MaxTrq[2] = 0.000000000000e+00;
   Ac->G[0].MaxFrc[0] = 0.000000000000e+00;
   Ac->G[0].MaxFrc[1] = 0.000000000000e+00;
   Ac->G[0].MaxFrc[2] = 0.000000000000e+00;
   Ac->Gyro[0].Axis[0] = 1.000000000000e+00;
   Ac->Gyro[0].Axis[1] = 0.000000000000e+00;
   Ac->Gyro[0].Axis[2] = 0.000000000000e+00;
   Ac->Gyro[1].Axis[0] = 0.000000000000e+00;
   Ac->Gyro[1].Axis[1] = 1.000000000000e+00;
   Ac->Gyro[1].Axis[2] = 0.000000000000e+00;
   Ac->Gyro[2].Axis[0] = 0.000000000000e+00;
   Ac->Gyro[2].Axis[1] = 0.000000000000e+00;
   Ac->Gyro[2].Axis[2] = 1.000000000000e+00;
   Ac->MAG[0].Axis[0] = 1.000000000000e+00;
   Ac->MAG[0].Axis[1] = 0.000000000000e+00;
   Ac->MAG[0].Axis[2] = 0.000000000000e+00;
   Ac->MAG[1].Axis[0] = 0.000000000000e+00;
   Ac->MAG[1].Axis[1] = 1.000000000000e+00;
   Ac->MAG[1].Axis[2] = 0.000000000000e+00;
   Ac->MAG[2].Axis[0] = 0.000000000000e+00;
   Ac->MAG[2].Axis[1] = 0.000000000000e+00;
   Ac->MAG[2].Axis[2] = 1.000000000000e+00;
   Ac->CSS[0].Body = 0;
   Ac->CSS[0].Axis[0] = 5.773502691896e-01;
   Ac->CSS[0].Axis[1] = 5.773502691896e-01;
   Ac->CSS[0].Axis[2] = 5.773502691896e-01;
   Ac->CSS[0].Scale = 1.000000000000e+00;
   Ac->CSS[1].Body = 0;
   Ac->CSS[1].Axis[0] = 5.773502691896e-01;
   Ac->CSS[1].Axis[1] = 5.773502691896e-01;
   Ac->CSS[1].Axis[2] = -5.773502691896e-01;
   Ac->CSS[1].Scale = 1.000000000000e+00;
   Ac->CSS[2].Body = 0;
   Ac->CSS[2].Axis[0] = 5.773502691896e-01;
   Ac->CSS[2].Axis[1] = -5.773502691896e-01;
   Ac->CSS[2].Axis[2] = 5.773502691896e-01;
   Ac->CSS[2].Scale = 1.000000000000e+00;
   Ac->CSS[3].Body = 0;
   Ac->CSS[3].Axis[0] = 5.773502691896e-01;
   Ac->CSS[3].Axis[1] = -5.773502691896e-01;
   Ac->CSS[3].Axis[2] = -5.773502691896e-01;
   Ac->CSS[3].Scale = 1.000000000000e+00;
   Ac->CSS[4].Body = 0;
   Ac->CSS[4].Axis[0] = -5.773502691896e-01;
   Ac->CSS[4].Axis[1] = 5.773502691896e-01;
   Ac->CSS[4].Axis[2] = 5.773502691896e-01;
   Ac->CSS[4].Scale = 1.000000000000e+00;
   Ac->CSS[5].Body = 0;
   Ac->CSS[5].Axis[0] = -5.773502691896e-01;
   Ac->CSS[5].Axis[1] = 5.773502691896e-01;
   Ac->CSS[5].Axis[2] = -5.773502691896e-01;
   Ac->CSS[5].Scale = 1.000000000000e+00;
   Ac->CSS[6].Body = 0;
   Ac->CSS[6].Axis[0] = -5.773502691896e-01;
   Ac->CSS[6].Axis[1] = -5.773502691896e-01;
   Ac->CSS[6].Axis[2] =  5.773502691896e-01;
   Ac->CSS[6].Scale = 1.000000000000e+00;
   Ac->CSS[7].Body = 0;
   Ac->CSS[7].Axis[0] = -5.773502691896e-01;
   Ac->CSS[7].Axis[1] = -5.773502691896e-01;
   Ac->CSS[7].Axis[2] = -5.773502691896e-01;
   Ac->CSS[7].Scale = 1.000000000000e+00;
   Ac->FSS[0].qb[0] = 0.000000000000e+00;
   Ac->FSS[0].qb[1] = 7.071067811865e-01;
   Ac->FSS[0].qb[2] = 0.000000000000e+00;
   Ac->FSS[0].qb[3] = 7.071067811865e-01;
   Ac->FSS[0].CB[0][0] = 6.123233995737e-17;
   Ac->FSS[0].CB[0][1] = 0.000000000000e+00;
   Ac->FSS[0].CB[0][2] = -1.000000000000e+00;
   Ac->FSS[0].CB[1][0] = 0.000000000000e+00;
   Ac->FSS[0].CB[1][1] = 1.000000000000e+00;
   Ac->FSS[0].CB[1][2] = 0.000000000000e+00;
   Ac->FSS[0].CB[2][0] = 1.000000000000e+00;
   Ac->FSS[0].CB[2][1] = -0.000000000000e+00;
   Ac->FSS[0].CB[2][2] = 6.123233995737e-17;
   Ac->ST[0].qb[0] = 0.000000000000e+00;
   Ac->ST[0].qb[1] = 1.000000000000e+00;
   Ac->ST[0].qb[2] = 0.000000000000e+00;
   Ac->ST[0].qb[3] = 6.123233995737e-17;
   Ac->ST[0].CB[0][0] = -1.000000000000e+00;
   Ac->ST[0].CB[0][1] =  0.000000000000e+00;
   Ac->ST[0].CB[0][2] =  -1.224646799147e-16;
   Ac->ST[0].CB[1][0] =  0.000000000000e+00;
   Ac->ST[0].CB[1][1] =  1.000000000000e+00;
   Ac->ST[0].CB[1][2] =  0.000000000000e+00;
   Ac->ST[0].CB[2][0] =  1.224646799147e-16;
   Ac->ST[0].CB[2][1] =  -0.000000000000e+00;
   Ac->ST[0].CB[2][2] =  -1.000000000000e+00;
   Ac->Whl[0].Axis[0] = 5.773502691896e-01;
   Ac->Whl[0].Axis[1] =  5.773502691896e-01;
   Ac->Whl[0].Axis[2] =  5.773502691896e-01;
   Ac->Whl[0].DistVec[0] = 4.330127018922e-01;
   Ac->Whl[0].DistVec[1] = 4.330127018922e-01;
   Ac->Whl[0].DistVec[2] = 4.330127018922e-01;
   Ac->Whl[0].J = 1.200000000000e-02;
   Ac->Whl[0].Tmax = 1.400000000000e-01;
   Ac->Whl[0].Hmax = 4.000000000000e+01;
   Ac->Whl[1].Axis[0] = -5.773502691896e-01;
   Ac->Whl[1].Axis[1] =  5.773502691896e-01;
   Ac->Whl[1].Axis[2] =  5.773502691896e-01;
   Ac->Whl[1].DistVec[0] = -4.330127018922e-01;
   Ac->Whl[1].DistVec[1] =  4.330127018922e-01;
   Ac->Whl[1].DistVec[2] =  4.330127018922e-01;
   Ac->Whl[1].J = 1.200000000000e-02;
   Ac->Whl[1].Tmax = 1.400000000000e-01;
   Ac->Whl[1].Hmax = 4.000000000000e+01;
   Ac->Whl[2].Axis[0] = -5.773502691896e-01;
   Ac->Whl[2].Axis[1] =  -5.773502691896e-01;
   Ac->Whl[2].Axis[2] =  5.773502691896e-01;
   Ac->Whl[2].DistVec[0] = -4.330127018922e-01;
   Ac->Whl[2].DistVec[1] =  -4.330127018922e-01;
   Ac->Whl[2].DistVec[2] =  4.330127018922e-01;
   Ac->Whl[2].J = 1.200000000000e-02;
   Ac->Whl[2].Tmax = 1.400000000000e-01;
   Ac->Whl[2].Hmax = 4.000000000000e+01;
   Ac->Whl[3].Axis[0] = 5.773502691896e-01;
   Ac->Whl[3].Axis[1] = -5.773502691896e-01;
   Ac->Whl[3].Axis[2] = 5.773502691896e-01;
   Ac->Whl[3].DistVec[0] = 4.330127018922e-01;
   Ac->Whl[3].DistVec[1] = -4.330127018922e-01;
   Ac->Whl[3].DistVec[2] =  4.330127018922e-01;
   Ac->Whl[3].J = 1.200000000000e-02;
   Ac->Whl[3].Tmax = 1.400000000000e-01;
   Ac->Whl[3].Hmax = 4.000000000000e+01;
   Ac->MTB[0].Axis[0] = 1.000000000000e+00;
   Ac->MTB[0].Axis[1] =  0.000000000000e+00;
   Ac->MTB[0].Axis[2] =  0.000000000000e+00;
   Ac->MTB[0].DistVec[0] = 1.000000000000e+00;
   Ac->MTB[0].DistVec[1] =  0.000000000000e+00;
   Ac->MTB[0].DistVec[2] =  0.000000000000e+00;
   Ac->MTB[0].Mmax = 1.800000000000e+02;
   Ac->MTB[1].Axis[0] = 0.000000000000e+00;
   Ac->MTB[1].Axis[1] =  1.000000000000e+00;
   Ac->MTB[1].Axis[2] =  0.000000000000e+00;
   Ac->MTB[1].DistVec[0] = 0.000000000000e+00;
   Ac->MTB[1].DistVec[1] = 1.000000000000e+00;
   Ac->MTB[1].DistVec[2] = 0.000000000000e+00;
   Ac->MTB[1].Mmax = 1.800000000000e+02;
   Ac->MTB[2].Axis[0] = 0.000000000000e+00;
   Ac->MTB[2].Axis[1] =  0.000000000000e+00;
   Ac->MTB[2].Axis[2] =  1.000000000000e+00;
   Ac->MTB[2].DistVec[0] = 0.000000000000e+00;
   Ac->MTB[2].DistVec[1] =  0.000000000000e+00;
   Ac->MTB[2].DistVec[2] =  1.000000000000e+00;
   Ac->MTB[2].Mmax = 1.800000000000e+02;
   Ac->PrototypeCtrl.wc = 3.141592653590e-01;
   Ac->PrototypeCtrl.amax = 1.000000000000e-02;
   Ac->PrototypeCtrl.vmax = 8.726646259972e-03;
   Ac->PrototypeCtrl.Kprec = 0.000000000000e+00;
   Ac->PrototypeCtrl.Knute = 0.000000000000e+00;
   Ac->AdHocCtrl.Kr[0] = 0.000000000000e+00;
   Ac->AdHocCtrl.Kr[1] =  0.000000000000e+00;
   Ac->AdHocCtrl.Kr[2] =  0.000000000000e+00;
   Ac->AdHocCtrl.Kp[0] = 0.000000000000e+00;
   Ac->AdHocCtrl.Kp[1] =  0.000000000000e+00;
   Ac->AdHocCtrl.Kp[2] =  0.000000000000e+00;
   Ac->SpinnerCtrl.Ispin = 0.000000000000e+00;
   Ac->SpinnerCtrl.Itrans = 0.000000000000e+00;
   Ac->SpinnerCtrl.SpinRate = 0.000000000000e+00;
   Ac->SpinnerCtrl.Knute = 0.000000000000e+00;
   Ac->SpinnerCtrl.Kprec = 0.000000000000e+00;
   Ac->ThreeAxisCtrl.Kr[0] = 0.000000000000e+00;
   Ac->ThreeAxisCtrl.Kr[1] =  0.000000000000e+00;
   Ac->ThreeAxisCtrl.Kr[2] =  0.000000000000e+00;
   Ac->ThreeAxisCtrl.Kp[0] = 0.000000000000e+00;
   Ac->ThreeAxisCtrl.Kp[1] =  0.000000000000e+00;
   Ac->ThreeAxisCtrl.Kp[2] =  0.000000000000e+00;
   Ac->ThreeAxisCtrl.Kunl = 0.000000000000e+00;
   Ac->IssCtrl.Kr[0] = 0.000000000000e+00;
   Ac->IssCtrl.Kr[1] =  0.000000000000e+00;
   Ac->IssCtrl.Kr[2] =  0.000000000000e+00;
   Ac->IssCtrl.Kp[0] = 0.000000000000e+00;
   Ac->IssCtrl.Kp[1] =  0.000000000000e+00;
   Ac->IssCtrl.Kp[2] =  0.000000000000e+00;
   Ac->IssCtrl.Tmax = 0.000000000000e+00;
   Ac->CmgCtrl.Kr[0] = 0.000000000000e+00;
   Ac->CmgCtrl.Kr[1] =  0.000000000000e+00;
   Ac->CmgCtrl.Kr[2] =  0.000000000000e+00;
   Ac->CmgCtrl.Kp[0] = 0.000000000000e+00;
   Ac->CmgCtrl.Kp[1] =  0.000000000000e+00;
   Ac->CmgCtrl.Kp[2] =  0.000000000000e+00;
   Ac->ThrCtrl.Kw[0] = 0.000000000000e+00;
   Ac->ThrCtrl.Kw[1] =  0.000000000000e+00;
   Ac->ThrCtrl.Kw[2] =  0.000000000000e+00;
   Ac->ThrCtrl.Kth[0] = 0.000000000000e+00;
   Ac->ThrCtrl.Kth[1] =  0.000000000000e+00;
   Ac->ThrCtrl.Kth[2] =  0.000000000000e+00;
   Ac->ThrCtrl.Kv = 0.000000000000e+00;
   Ac->ThrCtrl.Kp = 0.000000000000e+00;    
   Ac->CfsCtrl.Kr[0] = 3.863763921569e+02;
   Ac->CfsCtrl.Kr[1] = 2.964500000000e+02;
   Ac->CfsCtrl.Kr[2] = 5.369381568628e+02;
   Ac->CfsCtrl.Kp[0] = 2.759831372549e+01;
   Ac->CfsCtrl.Kp[1] = 2.117500000000e+01;
   Ac->CfsCtrl.Kp[2] = 3.835272549020e+01;
   Ac->CfsCtrl.Kunl = 1.000000000000e+06;
   Ac->ThrSteerCtrl.Kr[0] = 0.000000000000e+00;
   Ac->ThrSteerCtrl.Kr[1] = 0.000000000000e+00;
   Ac->ThrSteerCtrl.Kr[2] = 0.000000000000e+00;
   Ac->ThrSteerCtrl.Kp[0] = 0.000000000000e+00;
   Ac->ThrSteerCtrl.Kp[1] = 0.000000000000e+00;
   Ac->ThrSteerCtrl.Kp[2] = 0.000000000000e+00;

}


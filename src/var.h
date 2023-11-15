#include <Arduino.h>

const int pin_CS = A7;

const String fileName_var = "var.txt";
const String fileName_hist = "hisory.txt";
const String fileName_name = "name.txt";

const int varLength = 5;
const int varCountMax=20;

const int varLength_Str = 20;

const int ind_var0 = 0;
float var0 = 0.012;

const int ind_var1 = 1;
float var1 = 2.3;

const int ind_var2 = 2;
float var2 = 1;

const int ind_var3 = 3;
int var3 = 16;

const int ind_Profil0=0;
const int ind_Profil1=1;
const int ind_Profil2=2;
const int ind_Profil3=3;
const int ind_Profil4=4;

String ProfiTab[]={"Profil 0","Profil 1","Profil 2","Profil 3","Profil 4"};

int testPhase = 0;
int testCount = 0;
int tmp0 = 0;
int tmp1 = 0;
int tmp2 = 0;
int tmp3 = 0;

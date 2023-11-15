#include <Arduino.h>

#include <var.h>
#include <MySD.h>

MySD mySD(pin_CS);

String TestStr();

void setup()
{
  Serial.begin(9600);
  Serial.println("Serial port ok..");

  pinMode(pin_CS, OUTPUT);

  mySD.Init();
}

void loop()
{
  switch (testPhase)
  {
  case 0:
  {
    /*
    testPhase=100; test zapisu zmiennych
    testPhase=200; test odczytu zmiennych
    testPhase=300; test zapisu historii
    testPhase=400; test odczytu historii
    testPhase=500; test zapisu nazw profili
    testPhase=600; test odczytu nazw profili
    */
    mySD.Dir();
    testPhase = 500;
    break;
  }
  case 100:
  {
    Serial.println("Test zapisu zmiennych");

    mySD.WriteVariable_float(fileName_var, var0, ind_var0);
    mySD.WriteVariable_float(fileName_var, var1, ind_var1);
    mySD.WriteVariable_float(fileName_var, var2, ind_var2);
    mySD.WriteVariable_int(fileName_var, var3, ind_var3);
    testPhase = 200;
    break;
  }
  case 200:
  {
    Serial.println("Test odczytu zmiennych");
    Serial.println("");
    int _tmpInt = mySD.ReadVariable_int(fileName_var, ind_var3);
    Serial.print(String(_tmpInt) + ":");

    float _tmpFloat = mySD.ReadVariable_float(fileName_var, ind_var0);
    Serial.print(String(_tmpFloat) + ":");

    _tmpFloat = mySD.ReadVariable_float(fileName_var, ind_var2);
    Serial.print(String(_tmpFloat) + ":");

    _tmpFloat = mySD.ReadVariable_float(fileName_var, ind_var1);
    Serial.print(String(_tmpFloat) + ":");

    testPhase = 1000;

    break;
  }
  case 300:
  {
    String _dataStr = TestStr();
    mySD.Write(fileName_hist, _dataStr);
    testCount++;
    if (testCount < 100)
    {
      break;
    }
    testPhase = 400;
    break;
  }
  case 400:
  {
    mySD.Read(fileName_hist);
    testPhase = 1000;
    break;
  }
  case 500:
  {
    for (int x = 0; x < 5; x++)
    {
      mySD.WriteVariable_String(fileName_name, ProfiTab[x], x);
    }
    testPhase = 600;
    break;
  }
  case 600:
  {
    String _dataStr = "";
    _dataStr = mySD.ReadVariable_String(fileName_name, 4);
    Serial.println(_dataStr);

    _dataStr = mySD.ReadVariable_String(fileName_name, 3);
    Serial.println(_dataStr);

    _dataStr = mySD.ReadVariable_String(fileName_name, 2);
    Serial.println(_dataStr);

    _dataStr = mySD.ReadVariable_String(fileName_name, 1);
    Serial.println(_dataStr);

    _dataStr = mySD.ReadVariable_String(fileName_name, 0);
    Serial.println(_dataStr);

    testPhase = 1000;
    break;
  }

  case 1000:
  {
    Serial.println("");
    Serial.println("");
    Serial.println("Koniec testu");

    mySD.Dir();

    testPhase = 9000;
    break;
  }
  default:
    break;
  }

}
String TestStr()
{
  tmp0 = analogRead(A0);
  tmp1 = analogRead(A1);
  tmp2 = analogRead(A2);
  tmp3 = analogRead(A3);

  String _dataStr = String(tmp0) + ":";
  _dataStr += String(tmp1) + ":";
  _dataStr += String(tmp2) + ":";
  _dataStr += String(tmp3);

  return _dataStr;
}

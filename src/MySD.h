#include <Arduino.h>
#include <SD.h>

class MySD
{
private:
    int pin_CS = 0;
    bool isCardReady = false;
    bool isPowerOn = true;

    void FileInit(String, int);
    void WriteVariable(String, String, int);
    String ReadVariable(String, int);

    Sd2Card card;
    SdVolume volume;
    SdFile root;
    File myFile;

public:
    MySD(int);
    void Init();
    void Dir();
    void Write(String, String);
    void Read(String);

    void WriteVariable_float(String, float, int);
    void WriteVariable_int(String, int, int);
    float ReadVariable_float(String, int);
    int ReadVariable_int(String, int);
    void WriteVariable_String(String, String, int);
    String ReadVariable_String(String, int);
};

MySD::MySD(int _pin)
{
    pin_CS = _pin;
}

void MySD::Init()
{
    SD.begin(pin_CS);
    if (card.init(SPI_HALF_SPEED, pin_CS))
    {
        isCardReady = true;
        Serial.println("SD card ok..");
    }
    else
    {
        isCardReady = false;
        Serial.println("SD card not ready..");
        return;
    }
    if (!volume.init(card))
    {
        Serial.println("card not compatible");
        isCardReady = false;
        return;
    }
    root.openRoot(volume);
}

void MySD::Dir()
{
    if (!isCardReady || !isPowerOn)
        return;

    uint32_t volumeSize;

    Serial.println("");
    Serial.print("Volume type:  FAT");
    Serial.println(volume.fatType(), DEC);

    volumeSize = volume.blocksPerCluster();
    volumeSize *= volume.clusterCount();
    volumeSize /= 2;

    String _dataStr = "Volume size:  " + String(volumeSize);
    _dataStr += " kb";
    Serial.println(_dataStr);
    Serial.println("");

    root.ls(LS_R | LS_DATE | LS_SIZE);
}

void MySD::Write(String _fileName, String _dataToWrite)
{
    if (!isCardReady || !isPowerOn)
        return;

    myFile = SD.open(_fileName, FILE_WRITE);
    if (!myFile)
        return;

    myFile.println(_dataToWrite);
    myFile.close();
}

void MySD::Read(String _fileName)
{
    if (!isCardReady || !isPowerOn)
        return;
    myFile = SD.open(_fileName, FILE_READ);
    if (!myFile)
        return;

    Serial.println("");
    Serial.println("");
    Serial.println("Myfile read:  ");
    Serial.println("");
    while (myFile.available())
    {
        Serial.write(myFile.read());
    }
    myFile.close();
}

void MySD::WriteVariable_float(String _fileName, float _value, int _index)
{
    if (!isCardReady || !isPowerOn)
        return;
    String _valueStr = String(_value);
    WriteVariable(_fileName, _valueStr, _index);
}

float MySD::ReadVariable_float(String _fileName, int _index)
{
    if (!isCardReady || !isPowerOn)
        return 0;
    String _dartaStr = ReadVariable(_fileName, _index);
    return _dartaStr.toFloat();
}

void MySD::WriteVariable_int(String _fileName, int _value, int _index)
{
    if (!isCardReady || !isPowerOn)
        return;
    String _valueStr = String(_value);
    WriteVariable(_fileName, _valueStr, _index);
}

int MySD::ReadVariable_int(String _fileName, int _index)
{
    if (!isCardReady || !isPowerOn)
        return 0;
    String _dartaStr = ReadVariable(_fileName, _index);
    return _dartaStr.toInt();
}

void MySD::WriteVariable_String(String _fileName, String _valueStr, int _index)
{
    if (!isCardReady || !isPowerOn)
        return;
    _valueStr += "                    ";
    _valueStr = _valueStr.substring(0, 20);
    _index *= varLength_Str;

    if (!SD.exists(_fileName))
        FileInit(_fileName, 100);

    myFile = SD.open(_fileName, O_RDWR);
    if (!myFile)
        return;

    myFile.seek(_index);
    myFile.print(_valueStr);
    myFile.close();
}

String MySD::ReadVariable_String(String _fileName, int _index)
{
    char charTmp;
    String _value = "";

    if (!isCardReady || !isPowerOn)
        return "";

    myFile = SD.open(_fileName, O_RDWR);
    if (!myFile)
        return "";

    _index *= varLength_Str;
    myFile.seek(_index);
    for (int x = 0; x < varLength_Str; x++)
    {
        charTmp = myFile.read();
        _value += charTmp;
    }
    return _value;
}

// metody prywatne

String MySD::ReadVariable(String _fileName, int _index)
{
    char charTmp;
    String _value = "";
    _index *= (varLength + 1);

    myFile = SD.open(_fileName, O_RDWR);
    if (!myFile)
        return "";

    myFile.seek(_index);
    for (int x = 0; x < varLength; x++)
    {
        charTmp = myFile.read();
        _value += charTmp;
    }
    return _value;
}

void MySD::WriteVariable(String _fileName, String _valueStr, int _index)
{
    int _lengthTmp = _valueStr.length();
    String _valueStrTmp = "00000";

    if (_lengthTmp == varLength)
    {
        _valueStrTmp = _valueStr;
    }

    if (_lengthTmp < varLength)
    {
        _valueStrTmp = _valueStrTmp.substring(0, varLength - _lengthTmp);
        _valueStrTmp += _valueStr;
    }

    if (_lengthTmp > varLength)
        _valueStrTmp = _valueStr.substring(0, varLength);

    _index *= (varLength + 1);
    _valueStrTmp += ";";

    if (!SD.exists(_fileName))
        FileInit(_fileName, (varLength + 1) * varCountMax);

    myFile = SD.open(_fileName, O_RDWR);
    if (!myFile)
        return;

    myFile.seek(_index);
    myFile.print(_valueStrTmp);
    myFile.close();



    
}

void MySD::FileInit(String _fileName, int _length)
{
    myFile = SD.open(_fileName, FILE_WRITE);
    if (!myFile)
        return;
    for (int x = 0; x < _length; x++)
        myFile.print(" ");

    myFile.close();
}


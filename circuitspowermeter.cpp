#include <ESP8266WiFi.h>
#include <SPI.h>
#define ADS_SPISPEED 1250000

uint8_t _ads0_cs_pin;
uint8_t _ads0_rdy_pin;
uint8_t _ads1_cs_pin;
uint8_t _ads1_rdy_pin;


void initADS(byte cspin, byte drdypin)
{
  pinMode(cspin, OUTPUT);
  pinMode(drdypin, INPUT);

  SPI.begin();

  SPI.beginTransaction(SPISettings(ADS_SPISPEED, MSBFIRST, SPI_MODE1));
  delayMicroseconds(10);
  digitalWrite(cspin, LOW); // select ADS
  delayMicroseconds(50);

  //Reset to Power-Up Values (FEh)
  SPI.transfer(0xFE);
  delayMicroseconds(100);

  byte status_reg = 0 ;  // address (datasheet p. 30)
  byte status_data = 0x01; //status: Most Significant Bit First, Auto-Calibration Disabled, Analog Input Buffer Disabled
  //0x03; //to activate buffer
  SPI.transfer(0x50 | status_reg);
  SPI.transfer(0x00);   // 2nd command byte, write one register only
  SPI.transfer(status_data);   // write the databyte to the register
  delayMicroseconds(10);

  //PGA SETTING
  //1 ±5V        000 (1)
  //2 ±2.5V      001 (2)
  //4 ±1.25V     010 (3)
  //8 ±0.625V    011 (4)
  //16 ±312.5mV  100 (5)
  //32 ±156.25mV 101 (6)
  //64 ±78.125mV 110 (7) OR 111 (8)
  byte adcon_reg = 2; //A/D Control Register (Address 02h)
  byte adcon_data = 0x20; // 0 01 00 000 => Clock Out Frequency = fCLKIN, Sensor Detect OFF, gain 1
  //0x25 for setting gain to 32, 0x27 to 64
  SPI.transfer(0x50 | adcon_reg);
  SPI.transfer(0x00);   // 2nd command byte, write one register only
  SPI.transfer(adcon_data);   // write the databyte to the register
  delayMicroseconds(10);

  //Set sampling rate
  byte drate_reg = 3; // Choosing Data Rate register = third register.
  byte drate_data = 0b11110000; // 11110000 = 30,000SPS 11000000 = 3,750SPS
  SPI.transfer(0x50 | drate_reg);
  SPI.transfer(0x00);   // 2nd command byte, write one register only
  SPI.transfer(drate_data);   // write the databyte to the register
  delayMicroseconds(10);

  //done with settings, can close SPI transaction now
  digitalWrite(cspin, HIGH); //unselect ADS
  SPI.endTransaction();
  delayMicroseconds(50);

}

void setADSch(byte cspin, byte drdypin, byte channel)
{
  SPI.beginTransaction(SPISettings(ADS_SPISPEED, MSBFIRST, SPI_MODE1));
  digitalWrite(cspin, LOW);
  delayMicroseconds(50);
  while (!digitalRead(drdypin)) {} ; // Wait for DRDY pin to become high..
  while (digitalRead(drdypin)) {} ; // Wait for DRDY pin to become low..

  // Stop Read Data Continuesly 0000  1111 (0Fh)
  SPI.transfer(0x0F);
  delayMicroseconds(50);

  byte data = (channel << 4) | 0b1000; //AIN-channel and AINCOM
  SPI.transfer(0x50 | 1); // write (0x50) MUX register (0x01)
  SPI.transfer(0x00);   // number of registers to be read/written − 1, write one register only
  SPI.transfer(data);   // write the databyte to the register
  delayMicroseconds(10);

  //SYNC command 1111 1100
  SPI.transfer(0xFC);
  delayMicroseconds(10);

  //WAKEUP 0000 0000
  SPI.transfer(0x00);
  delayMicroseconds(10);

  for (int i = 5; i--; i > 0) // Wait 5 drdy periods for analog input to settle (settling time)
  {
    while (!digitalRead(drdypin)) {} ; // Wait for DRDY pin to become high..
    while (digitalRead(drdypin)) {} ; // Wait for DRDY pin to become low..
  }

  // Read Data Continuesly 0000  0011 (03h)
  SPI.transfer(0x03);
  delayMicroseconds(50);

  //done, can close SPI transaction now
  digitalWrite(cspin, HIGH); //unselect ADS
  SPI.endTransaction();
}

long readADS(byte cspin, byte drdypin)
{
  long adc_val;

  SPI.beginTransaction(SPISettings(ADS_SPISPEED, MSBFIRST, SPI_MODE1));
  digitalWrite(cspin, LOW);

  while (!digitalRead(drdypin)) {}; // Wait for DRDY pin to become high..
  while (digitalRead(drdypin)) {} ; // Wait for DRDY pin to become low..

  adc_val = SPI.transfer(0);
  adc_val <<= 8; //shift to left
  adc_val |= SPI.transfer(0);
  adc_val <<= 8;
  adc_val |= SPI.transfer(0);

  //The ADS1255/6 output 24 bits of data in Binary Two’s
  //Complement format. The LSB has a weight of
  //2VREF/(PGA(223 − 1)). A positive full-scale input produces
  //an output code of 7FFFFFh and the negative full-scale
  //input produces an output code of 800000h.
  if (adc_val > 0x7fffff) { //if MSB == 1
    adc_val = 16777216ul - adc_val; //do 2's complement, discard sign
  }

  //done with settings, can close SPI transaction now
  digitalWrite(cspin, HIGH); //unselect ADS
  delayMicroseconds(50);
  SPI.endTransaction();

  return adc_val;
}

long readADSac(byte cspin, byte drdypin)
{
  uint64_t ac_val = 0;
  uint64_t acm_val = 0;
  long adc_val;
  int i = 0;

  SPI.beginTransaction(SPISettings(ADS_SPISPEED, MSBFIRST, SPI_MODE1));
  digitalWrite(cspin, LOW);

  for (int y = 0; y < 2; y++) // sample 2 sine waves (at 50hz)
  {
    ac_val = 0;
    cli(); // Disable interrupts
    for (i = 0; i < 600; i++)
    {
      while (!digitalRead(drdypin)) {}; // Wait for DRDY pin to become high..
      while (digitalRead(drdypin)) {} ; // Wait for DRDY pin to become low..

      adc_val = 0;
      adc_val = SPI.transfer(0);
      adc_val <<= 8; //shift to left
      adc_val |= SPI.transfer(0);
      adc_val <<= 8;
      adc_val |= SPI.transfer(0);
      ac_val += abs(adc_val - 0x3ffef0); // Center measurement
    }
    sei(); // Enable interrupts
    acm_val += ac_val / 600;
  }

  //done with settings, can close SPI transaction now
  digitalWrite(cspin, HIGH); //unselect ADS
  delayMicroseconds(50);
  SPI.endTransaction();

  return max(long(acm_val / 2) - 252, long(0)); // Filter noise
}
/*
void debugvoltread()
{
   setADSch(ADS0_CS_PIN, ADS0_RDY_PIN, 7); // Volt
   while (!digitalRead(ADS0_RDY_PIN)) {}; // Wait for DRDY pin to become high..
   while (digitalRead(ADS0_RDY_PIN)) {} ; // Wait for DRDY pin to become low..

    uint32_t adc_val_volt = 0;
    digitalWrite(ADS0_CS_PIN, LOW);
    adc_val_volt = 0;
    adc_val_volt = SPI.transfer(0);
    adc_val_volt <<= 8; //shift to left
    adc_val_volt |= SPI.transfer(0);
    adc_val_volt <<= 8;
    adc_val_volt |= SPI.transfer(0);
    digitalWrite(ADS0_CS_PIN, HIGH);

}
*/
byte readADSpower(byte adchannel, int32 * mA, int32 * mV, int32 * mW, int32 * mVA)
{
#define NROFSAMPLES 200
  int32_t ac_mA[NROFSAMPLES]; // 200 samples is 20 ms
  int32_t ac_mA_offset = 0;
  int32_t ac_mV[NROFSAMPLES];
  int32_t ac_mV_offset = 0;
  double ac_mA_gain = 1;
  double ac_mV_gain = 1;

  int32_t adc_val_amp = 0;
  int32_t adc_val_volt = 0;

  int i = 0;
  byte adc_cs_amp_pin;
  byte adc_drdy_amp_pin;
  byte adc_cs_volt_pin;
  byte adc_drdy_volt_pin;
  // Set adc channels for measuring volt and amps
  if (adchannel < 7)
  {
    ac_mA_offset = 5; // Offset ad 0
    ac_mA_gain = 0.01168; // Gain ad 0 (calculated: 0.01168)
    adc_cs_amp_pin = _ads0_cs_pin;
    adc_drdy_amp_pin = _ads0_rdy_pin;
    setADSch(adc_cs_amp_pin, adc_drdy_amp_pin, adchannel); // Amps


    ac_mV_offset = 0; // Offset ad 1
    ac_mV_gain = 0.092; // Gain ad 1
    adc_cs_volt_pin = _ads1_cs_pin;
    adc_drdy_volt_pin = _ads1_rdy_pin;
    setADSch(adc_cs_volt_pin, adc_drdy_volt_pin, 7); // Volt
  }
  else if (adchannel < 14)
  {
    ac_mA_offset = 5; // Offset ad 1
    ac_mA_gain = 0.0145; // Gain ad 1
    adc_cs_amp_pin = _ads1_cs_pin;
    adc_drdy_amp_pin = _ads1_rdy_pin;
    setADSch(adc_cs_amp_pin, adc_drdy_amp_pin, adchannel); // Amps

    ac_mV_offset = 0; // Offset ad 0 mV
    ac_mV_gain = 0.092; // Gain ad 0
    adc_cs_volt_pin = _ads0_cs_pin;
    adc_drdy_volt_pin = _ads0_rdy_pin;
    setADSch(adc_cs_volt_pin, adc_drdy_volt_pin, 7); // Volt
  }
  else return -1;

  //DEBUG ("Reading AD channel %d\n",adchannel);
  SPI.beginTransaction(SPISettings(ADS_SPISPEED, MSBFIRST, SPI_MODE1));

  cli(); // Disable interrupts
  for (i = 0; i < NROFSAMPLES; i++)
  {
    while (!digitalRead(adc_drdy_amp_pin)) {}; // Wait for DRDY pin to become high..
    while (digitalRead(adc_drdy_amp_pin)) {} ; // Wait for DRDY pin to become low..

    digitalWrite(adc_cs_amp_pin, LOW);
    adc_val_amp = 0;
    adc_val_amp = SPI.transfer(0);
    adc_val_amp <<= 8; //shift to left
    adc_val_amp |= SPI.transfer(0);
    adc_val_amp <<= 8;
    adc_val_amp |= SPI.transfer(0);
    digitalWrite(adc_cs_amp_pin, HIGH);

    ac_mA[i] = adc_val_amp;

    while (!digitalRead(adc_drdy_volt_pin)) {}; // Wait for DRDY pin to become high..
    while (digitalRead(adc_drdy_volt_pin)) {} ; // Wait for DRDY pin to become low..

    digitalWrite(adc_cs_volt_pin, LOW);
    adc_val_volt = 0;
    adc_val_volt = SPI.transfer(0);
    adc_val_volt <<= 8; //shift to left
    adc_val_volt |= SPI.transfer(0);
    adc_val_volt <<= 8;
    adc_val_volt |= SPI.transfer(0);
    digitalWrite(adc_cs_volt_pin, HIGH);

    ac_mV[i] = adc_val_volt;
  }
  sei(); // Enable interrupts

  //done with settings, can close SPI transaction now
  delayMicroseconds(50);
  SPI.endTransaction();

  int64_t sum_mA = 0;
  int64_t sum_mV = 0;
  int64_t sum_mW = 0;
  for (i = 0; i < NROFSAMPLES; i++)
  {
    double ac_mA_calc = 0;
    ac_mA_calc = (ac_mA[i] - 0x400000); // Center measurement and calculate mains current in mA
    ac_mA_calc += ac_mA_offset;
    ac_mA_calc *= ac_mA_gain;
    sum_mA += abs(ac_mA_calc);

    double ac_mV_calc = 0;
    ac_mV_calc = (ac_mV[i] - 0x400000);
    ac_mV_calc += ac_mV_offset;
    ac_mV_calc *= ac_mV_gain;
    sum_mV += abs(ac_mV_calc);

    ac_mV_calc = 230000; // Voltage reading is not working :-(
    double mW_calc = ac_mA_calc * ac_mV_calc;
    sum_mW += mW_calc;
    //ac_val_watt += ((adc_val_amp - 0x3ffef0) * (adc_val_volt - 0x3ffef0))/10000; // Calculate milliwatts
    //DEBUG ("%.3f,%.3f\n", ac_mA[i], ac_volt[i]);
    //DEBUG ("%d\n", ac_mA[i]);
    yield();
  }

  *mA = sum_mA / NROFSAMPLES;
  *mV = sum_mV / NROFSAMPLES;
  *mW = (sum_mW / NROFSAMPLES) / 1000;
  *mVA = (*mA * *mV) / 1000;
}


/*

//Maximum value of ADS
#define ADC_COUNTS 32768
#define PHASECAL 1.7
#define VCAL 0.6
#define ICAL 0.003

double filteredI;
double lastFilteredV, filteredV; //Filtered_ is the raw analog value minus the DC offset
int sampleV;                    //sample_ holds the raw analog read value
int sampleI;

double offsetV;                          //Low-pass filter output
double offsetI;                          //Low-pass filter output

double realPower,
       apparentPower,
       powerFactor,
       Vrms,
       Irms;
double phaseShiftedV; //Holds the calibrated phase shifted voltage.
int startV; //Instantaneous voltage at start of sample window.
double sqV, sumV, sqI, sumI, instP, sumP; //sq = squared, sum = Sum, inst = instantaneous
boolean lastVCross, checkVCross; //Used to measure number of times threshold is crossed.

double squareRoot(double fg)
{
  double n = fg / 2.0;
  double lstX = 0.0;
  while (n != lstX)
  {
    lstX = n;
    n = (n + fg / n) / 2.0;
  }
  return n;
}


void calcVI(unsigned int crossings, unsigned int timeout)
{

  unsigned int crossCount = 0;                             //Used to measure number of times threshold is crossed.
  unsigned int numberOfSamples = 0;                        //This is now incremented

  //-------------------------------------------------------------------------------------------------------------------------
  // 1) Waits for the waveform to be close to 'zero' (mid-scale adc) part in sin curve.
  //-------------------------------------------------------------------------------------------------------------------------
  boolean st = false;                                //an indicator to exit the while loop

  unsigned long start = millis();    //millis()-start makes sure it doesnt get stuck in the loop if there is an error.

  while (st == false)                                //the while loop...
  {
    //     startV = ads0.readADC_SingleEnded(0);                    //using the voltage waveform
    if ((abs(startV) < (ADC_COUNTS * 0.55)) && (abs(startV) > (ADC_COUNTS * 0.45))) st = true; //check its within range
    if ((millis() - start) > timeout) st = true;
  }

  //-------------------------------------------------------------------------------------------------------------------------
  // 2) Main measurement loop
  //-------------------------------------------------------------------------------------------------------------------------
  start = millis();

  while ((crossCount < crossings) && ((millis() - start) < timeout))
  {
    numberOfSamples++;                       //Count number of times looped.
    lastFilteredV = filteredV;               //Used for delay/phase compensation

    //-----------------------------------------------------------------------------
    // A) Read in raw voltage and current samples
    //-----------------------------------------------------------------------------
    //    sampleV = ads0.readADC_SingleEnded(0);                 //Read in raw voltage signal
    //   sampleI = ads0.readADC_SingleEnded(1);                 //Read in raw current signal

    //-----------------------------------------------------------------------------
    // B) Apply digital low pass filters to extract the 2.5 V or 1.65 V dc offset,
    //     then subtract this - signal is now centred on 0 counts.
    //-----------------------------------------------------------------------------
    offsetV = offsetV + ((sampleV - offsetV) / 1024);
    filteredV = sampleV - offsetV;
    offsetI = offsetI + ((sampleI - offsetI) / 1024);
    filteredI = sampleI - offsetI;

    //-----------------------------------------------------------------------------
    // C) Root-mean-square method voltage
    //-----------------------------------------------------------------------------
    sqV = filteredV * filteredV;                //1) square voltage values
    sumV += sqV;                                //2) sum

    //-----------------------------------------------------------------------------
    // D) Root-mean-square method current
    //-----------------------------------------------------------------------------
    sqI = filteredI * filteredI;                //1) square current values
    sumI += sqI;                                //2) sum

    //-----------------------------------------------------------------------------
    // E) Phase calibration
    //-----------------------------------------------------------------------------
    phaseShiftedV = lastFilteredV + PHASECAL * (filteredV - lastFilteredV);

    //-----------------------------------------------------------------------------
    // F) Instantaneous power calc
    //-----------------------------------------------------------------------------
    instP = phaseShiftedV * filteredI;          //Instantaneous Power
    sumP += instP;                              //Sum

    //-----------------------------------------------------------------------------
    // G) Find the number of times the voltage has crossed the initial voltage
    //    - every 2 crosses we will have sampled 1 wavelength
    //    - so this method allows us to sample an integer number of half wavelengths which increases accuracy
    //-----------------------------------------------------------------------------
    lastVCross = checkVCross;
    if (sampleV > startV) checkVCross = true;
    else checkVCross = false;
    if (numberOfSamples == 1) lastVCross = checkVCross;

    if (lastVCross != checkVCross) crossCount++;
  }

  DEBUG((String("Nr of samples:") + numberOfSamples + "\n").c_str());
  DEBUG((String("Nr of crosses:") + crossCount + "\n").c_str());

  //-------------------------------------------------------------------------------------------------------------------------
  // 3) Post loop calculations
  //-------------------------------------------------------------------------------------------------------------------------
  //Calculation of the root of the mean of the voltage and current squared (rms)
  //Calibration coefficients applied.
  float multiplier = 0.125F; // ADS1115 @ +/- 4.096V gain (16-bit results) 
  double V_RATIO = VCAL * multiplier;
  Vrms = V_RATIO * squareRoot(sumV / numberOfSamples);

  double I_RATIO = ICAL * multiplier;
  Irms = I_RATIO * squareRoot(sumI / numberOfSamples);

  //Calculation power values
  realPower = V_RATIO * I_RATIO * sumP / numberOfSamples;
  apparentPower = Vrms * Irms;
  powerFactor = realPower / apparentPower;

  //Reset accumulators
  sumV = 0;
  sumI = 0;
  sumP = 0;

  DEBUG((String("Vrms=") + Vrms + "\n").c_str());
  DEBUG((String("Irms=") + Irms + "\n").c_str());
  DEBUG((String("realPower (W)=") + realPower + "\n").c_str());
  DEBUG((String("apparentPower (VA)=") + apparentPower).c_str());
  DEBUG((String("powerFactor=") + powerFactor + "\n").c_str());
  //--------------------------------------------------------------------------------------
}

double calcIrms(unsigned int Number_of_Samples)
{
  // Be sure to update this value based on the IC and the gain settings!
  float multiplier = 0.125F;    // ADS1115 @ +/- 4.096V gain (16-bit results) 
  for (unsigned int n = 0; n < Number_of_Samples; n++)
  {
    //sampleI = ads0.readADC_Differential_0_1();

    // Digital low pass filter extracts the 2.5 V or 1.65 V dc offset,
    //  then subtract this - signal is now centered on 0 counts.
    offsetI = (offsetI + (sampleI - offsetI) / 1024);
    filteredI = sampleI - offsetI;
    //filteredI = sampleI * multiplier;

    // Root-mean-square method current
    // 1) square current values
    sqI = filteredI * filteredI;
    // 2) sum
    sumI += sqI;
  }

  Irms = squareRoot(sumI / Number_of_Samples) * multiplier;

  //Reset accumulators
  sumI = 0;
  //--------------------------------------------------------------------------------------

  return Irms;
}
*/

void circuitspowermeter_read(uint8_t circuitnr, int32_t &mW, int32_t &mVA, int32_t &mA, int32_t &mV, uint8_t nrofsamples)
{
  mW = 0;
  mVA = 0;
  mA = 0;
  mV = 0;

  // Read nrofsamples from a circuit
  for (uint8_t y = 0; y < nrofsamples; y++)
  {
    int32_t part_mW = 0;
    int32_t part_mVA = 0;
    int32_t part_mA = 0;
    int32_t part_mV = 0;
    readADSpower(circuitnr, &part_mA, &part_mV, &part_mW, &part_mVA);
    mW += part_mW / 10;
    mVA += part_mVA / 10;
    mA += part_mA / 10;
    mV += part_mV / 10;
  }
}

void circuitspowermeter_init(uint8_t ads0_cs_pin, uint8_t ads0_rdy_pin, uint8_t ads1_cs_pin, uint8_t ads1_rdy_pin)
{
  system_update_cpu_freq(160); // Needed for fast processing...
  
  _ads0_cs_pin = ads0_cs_pin;
  _ads0_rdy_pin = ads0_rdy_pin;
  _ads1_cs_pin = ads1_cs_pin;
  _ads1_rdy_pin = ads1_rdy_pin;

  initADS(_ads0_cs_pin, _ads0_rdy_pin);
  initADS(_ads1_cs_pin, _ads1_rdy_pin);
}

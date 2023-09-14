#include <SoftwareSerial.h>		//serial communication on other digital pins
#include <LiquidCrystal.h>			//for LCD display
#include <SPI.h>   			//used for for communicating with one or more peripheral devices quickly , NOt supported anymore/ different name may have to delete


#include <SD.h>			//read and write from adruino
#include <Wire.h>		//connection/communication with I^2C and TWI devices
#include <EEPROM.h>                               //THIS IS EEPROM LIBRARY IN ORDER TO ACCESS EEPROM MEMORY  EEPROM == memory that keeps recent values when board is off 
#include <avr/wdt.h>                              //THIS IS WATCH DOG TIMER LIBRARY USED FOR RESETING CODE VIA COMMAND  
#include "DHT.h"		//for dht sensor/ temp sensor 
#include "StringSplitter.h"
#include <TinyGPS.h>		// converts NEMA format global positioning data into easy-to-use variables for Latitude, Longitude, Time and other information.


TinyGPS gps;                   // GPS object


#define gsm Serial3		//gsm in ports 14 and 15
#define ss Serial2		//ss? In ports 16 and 17


LiquidCrystal lcd(12, 10, 5, 4, 3, 2);		//pins used for LCD
//SoftwareSerial gsm(15,14);


#define DHTPIN 31		//humidity sensor pin
#define DHTTYPE DHT11		//DHT11 == temp and humidity sensor
DHT dht(DHTPIN, DHTTYPE);		//defines pin = 31, type = DHT11 = humidity sensor


File myFile;


#define E1 7 		//enable pin 
#define I1 A9 		 //control pin
#define I2 A10  	//control pin
#define E2 6	  //enable pin
#define I3 A13 //control pin
#define I4 A14	//control pin 


#define pot A8 	//pontetionmeter = pin A8
#define alcohol A15		//alcohol sensor = A15
#define vibration 41   //vibration sensor = 41


const int readingno = 10;
const int MPU6050_addr=0x68; //address for gyro sensor in hex
int16_t AccX, AccY, AccZ, Temp, GyroX, GyroY, GyroZ;


int AccX_val, AccY_val, AccZ_val, GyroX_val, GyroY_val, GyroZ_val, last_val, val, stability, X, Y, Z, GX, GY, GZ;


//deifnes statements/ variables for call 
#define RECEIVINGCALL 1
#define IDLE 2
#define BUSY 3
#define NO_ANSWER 4
#define TALKING 5
#define ERROR 6
#define OK "OK"
#define AT "AT"
#define config_sms  "This number is configured."
#define user_cut_call  "USER cut the call"
#define USER_not_Answering  "USER not Answering call..."
#define call_establish  "call establish.."
#define disconnecting_call  "disconnecting call"
#define cant_make_CALL  "can't make CALL"
#define  ERROR_str "ERROR"


int update_sms = 1, resume_motor = 0;
int sms_send;
int theft_mode_pin, user_mode_pin, engine_start_pin;
int msg_update = 0;


String lat = "";
String lon = "";
String site = "\nhttp://maps.google.com/maps?f=q&q=";		//website used for location
char no_configured = 0;
float lat_int;
float lon_int;


char data[100];		
char date[50];
char c_lat[15];
char c_lon[15];
char c[50];
char c_sms_number[15];		//sms = 10 digits? 
char c_sms[20];
int a, b;
char N_S[15];		//north,south
char E_W[15];		//east, west
float h;
String d,d_m_y,da,mo,ye,UTC="";		//string for date
String  ho,mi,se="0";
String gi,hi;		


//initial statements for gsm module and phone number, message
String gsm_response = "";
String phone_number = "";
unsigned long lasttime = 0;
unsigned long interval = 60000;
boolean notConnected = true;
boolean gsm_connect = false;
unsigned long current_time ;
String numtel;
String msg = "";
String num = "";
String ok = "OK";
String indata = "";
String indata_number = "";
String inputString = "";
String msg_index;
int index1;
int index2;
String sms_num, sms; // = get_sender_number();


char date1[10];
char time1[10];


//String gpscordinates;
//String slat;
//String slon;
//String Value;




int hr = 0;
int m = 0;
int s = 0;
int flag = 0;
int TIME = 0;


byte termometru[8] = //icon for termometer
{
  B00100,		//EOT
  B01010,		//LF
  B01010,		//LF
  B01110,		//SO
  B01110,		//SO
  B11111,		//US
  B11111,		//US
  B01110		//SO
};


byte alc[8] = //icon for alcohol
{
  B01110,	//SO
  B01110,	//SO
  B01110,	//SO
  B11111,		//US
  B11111,		//US
  B11111,		//US
  B11111,		//US
  B01110		//SO
};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);	//data rate = 9600 bps
  gsm.begin(115200);	//gsm rate = 115200 bps	
  dht.begin(); 


  pinMode(E1, OUTPUT);	//configures specified pin to act as an output
  pinMode(I1, OUTPUT);	//sets control, pin = A9 , to output
  pinMode(I2, OUTPUT);		//pin A10 = output
  pinMode(E2, OUTPUT);		//pin 6 = ouput
  pinMode(I3, OUTPUT);		//pin A13 = output
  pinMode(I4, OUTPUT);		//pin A14 = output


  pinMode(pot, INPUT);		//potentionmeter = pin A8 = input
  pinMode(alcohol, INPUT);		//alcohol sensor = pin A15 = input
  pinMode(vibration, INPUT);	//vibration sensor = pin 41 = input 
  
  lcd.begin(20, 4);		//initializes lc for 20 chars, 4 lines
  lcd.createChar(1, termometru);
  lcd.createChar(2, alc);
  lcd.clear();
  lcd.print(" Wireless Black Box ");
  lcd.setCursor(0,1);	//cursor (col, row)
  lcd.print("   for Cars using   ");		//in col 0 row 1
  lcd.setCursor(0, 2);
  lcd.print("      Sensors &     ");		//in col 0, row 2
  lcd.setCursor(0,3);		
  lcd.print("     GPS Module!    ");		//in col 0, row 3
  delay(1000);		//waits for a second


  while (!Serial) 
  {
   Serial.println("Initializing SD Card..."); // wait for serial port to connect. Needed for native USB port only
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Please wait....");
   lcd.setCursor(0,1);
   lcd.print("Initializing SD");
   lcd.setCursor(0,2);
   lcd.print("Card....");
   lcd.blink();		//turns on blinking cursor
  }
  if (!SD.begin(53)) 		//is SD  is no on pin 53 == SS pin
  {
    Serial.println("Initialization failed!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sorry....");
    lcd.setCursor(0,1);
    lcd.print("SD Card not found!");
    lcd.noBlink();
    return;
  }
  Serial.println("Initialization done.");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Please wait....");
  lcd.setCursor(0,1);
  lcd.print("Initialization done.");
  lcd.noBlink();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Please wait....");
  lcd.setCursor(0,1);
  lcd.print("Checking stability.");
  lcd.blink();
  Wire.begin();
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  AccX_val = AccX_checkstability();
  AccY_val = AccY_checkstability();
  AccZ_val = AccZ_checkstability();
  GyroX_val = GyroX_checkstability();
  GyroY_val = GyroY_checkstability();
  GyroZ_val = GyroZ_checkstability();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Please wait....");
  lcd.setCursor(0,1);
  lcd.print("Stability Checked.");
  lcd.noBlink();
  while (notConnected == true)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Please wait....");
    lcd.setCursor(0,1);
    lcd.print("Connecting GSM....");
    lcd.blink();
    if (connectGSM())
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Please wait....");
      lcd.setCursor(0,1);
      lcd.print("GSM Connected.");
      lcd.noBlink();
      EEPROM.update(0,0);  //updating eeprom memory 0th location with value 0
      notConnected = false;
    }
    else
    {
      notConnected = true;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Sorry....");
      lcd.setCursor(0,1);
      lcd.print("GSM not connected!");
      lcd.noBlink();
      while (1);
    }
  }
  delay(2000);
  lcd.clear();
  lcd.print("To Register your");
  lcd.setCursor(0,1);
  lcd.print("number with the box"); 
  lcd.setCursor(0,2);
  lcd.print("Kindly Message");
  lcd.blink();


  Serial.println(F("Waiting for SMS.."));


  if (registerNumber())
  {
    Serial.print(F("User number: "));
    Serial.println(phone_number);
    sendSMS(phone_number, F("Your number is Registered."));
    lcd.clear();
    lcd.print(F("SMS sent..."));
  }
  delay(1000);
//  bool not_configured = true;//0
//  while (not_configured) // !0=1
//  {
////  lcd.clear();
////  lcd.print("in while");
//  
//  delay(1000);
//  byte cs=registerNumber());
////    lcd.clear();
////  lcd.print(cs);
//  
//  delay(2000);
//   if(cs==1)
//   {
//     numtel = get_call_number();
//        lcd.clear();
//        lcd.print(F("Receiving call"));
//        lcd.setCursor(0, 1);
//        lcd.print(numtel);stability


//        delay(200);
//        hangcall();
//        delay(1500);
//        numtel = numtel;
//        lcd.clear();
//        lcd.print(F("Sending SMS"));
////        sendSMS(F("This number configured as USER"), numtel);
//        sendSMS(F("This number configured as USER"));
//        delay(1000);
//        lcd.clear();
//        lcd.print(F("SMS sent.."));
//        not_configured = false;
//     
//    }
//  }
  indata.reserve(200);
  lcd.noBlink();


  while (!connectGPS())
  {
    Serial.println(F("Connecting GPS..."));
  }
  Serial.println(F("GPS connected!"));
  ss.end();
  delay(2000);


  hr= ho.toInt();
  m= mi.toInt();
  s = se.toInt();
}
 


void loop() 
{
  int analogValue = analogRead(pot);
  int motor_pwm = map(analogValue, 0, 1023, 0, 255);
  int speedValue = map(motor_pwm, 0, 255, 0, 100);
  analogWrite(E1, motor_pwm);
  analogWrite(E2, motor_pwm);
  digitalWrite(I1, HIGH);
  digitalWrite(I2, LOW);
  digitalWrite(I3, HIGH);
  digitalWrite(I4, LOW);
  int alcohol_val = analogRead(alcohol);
  alcohol_val = map(alcohol_val, 58, 510, 0, 100);
  int vib_value = digitalRead(vibration);
  float hum = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr,14,true); //request 14bytes of data from mpu address = 0x68 in hex
  AccX=Wire.read()<<8|Wire.read();
  AccY=Wire.read()<<8|Wire.read();
  AccZ=Wire.read()<<8|Wire.read();
  Temp=Wire.read()<<8|Wire.read();
  GyroX=Wire.read()<<8|Wire.read();
  GyroY=Wire.read()<<8|Wire.read();
  GyroZ=Wire.read()<<8|Wire.read();
  X = AccX - AccX_val;
  Y = AccY - AccY_val;
  Z = AccZ - AccZ_val;
  GX = GyroX - GyroX_val;
  GY = GyroY - GyroY_val;
  GZ = GyroZ - GyroZ_val;
  //Serial.println("*********************************************************************************************");
  
  if (isnan(h) || isnan(t) || isnan(f)) 
  {
//    Serial.println("Failed to read from DHT sensor!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Connecting from");
    lcd.setCursor(0,1);
    lcd.print("DHT sensor!");
    delay(2000);
    return;
  }


// 


  /*Serial.println();
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.println(" *F\t");
  Serial.print("AccX = "); Serial.println(AccX);
  Serial.print("AccY = "); Serial.println(AccY);
  Serial.print("AccZ = "); Serial.println(AccZ);
  Serial.print("Temp = "); Serial.println(Temp/340.00+36.53);
  Serial.print("GyroX = "); Serial.println(GyroX);
  Serial.print("GyroY = "); Serial.println(GyroY);
  Serial.print("GyroZ = "); Serial.println(GyroZ);
  Serial.print("Difference in AccX = "); Serial.println(X);
  Serial.print("Difference in AccY = "); Serial.println(Y);
  Serial.print("Difference in AccZ = "); Serial.println(Z);
  Serial.print("Difference in GyroX = "); Serial.println(GX);
  Serial.print("Difference in GyroY = "); Serial.println(GY);
  Serial.print("Difference in GyroZ = "); Serial.println(GZ);
  Serial.print("Alcohol value: ");
  Serial.println(alcohol_val);
  Serial.println("Speed: " + (String)speedValue + "%");
  Serial.println("Vibration: " + (String) vib_value);
  Serial.println("Latitude: " + lat);
  Serial.println("Longitude: " + (String) longitude);
  Serial.println();
  Serial.println("*********************************************************************************************");
  */
  s = s + 1;
  
  if (s == 60)
  {
    s = 0;
    m = m + 1;
  }
  else if(s>60)
  {
    s=s-60;
    m+=1;
  }
  
  if (m == 60)
  {
    m = 0;
    hr = hr + 1;
  }
  else if(m>60)
  {
    m=m-60;
    hr+=1;
  }
  
  if (hr == 24)
  {
    hr = 1;
  }
  else if (hr>24)
  {
    hr=hr-24;
  }




  
  lcd.clear();
  lcd.print("Time(+UTC): " );
  lcd.print(hr);
  lcd.print(":");
  lcd.print(m);
  lcd.print(":");
  lcd.print(s);


  lcd.setCursor(0,1);
  lcd.print("Date: " + d_m_y);
  lcd.setCursor(0,2);
  lcd.write(1);
  lcd.print(" " + (String) t);
  lcd.print((char)223);
  lcd.print("C  ");
  lcd.write(2);
  lcd.print(" " + (String) alcohol_val);
  lcd.setCursor(0,3);
  lcd.print("Speed: " + (String) speedValue + "%");


  UTC= (String)hr+":"+(String)m+":"+(String)s;
  
  myFile = SD.open("car_data.txt", FILE_WRITE);
  if (myFile) 
  {
    Serial.print("Writing to car_data.txt...");
    myFile.println("Date: " + d_m_y);
    myFile.println("Time(+UTC): " + UTC);
    myFile.print("AccX = "); myFile.println(AccX);
    myFile.print("AccY = "); myFile.println(AccY);
    myFile.print("AccZ = "); myFile.println(AccZ);
    myFile.print("Temp = "); myFile.println(Temp/340.00+36.53);
    myFile.print("GyroX = "); myFile.println(GyroX);
    myFile.print("GyroY = "); myFile.println(GyroY);
    myFile.print("GyroZ = "); myFile.println(GyroZ);
    myFile.println("Speed: " + (String) speedValue + "%");
    myFile.println("Vibration: " + (String) vib_value);
    myFile.print("Humidity: ");
    myFile.print(hum);
    myFile.print(" %\t");
    myFile.print("Temperature: ");
    myFile.print(t);
    myFile.print(" *C ");
    myFile.print(f);
    myFile.println(" *F\t");
    myFile.println("Alcohol Value: " + (String) alcohol_val);
    myFile.println("---------------------------------------------------------");
    myFile.close();
    Serial.println("done.");
  } 
  else 
  {
     // if the file didn't open, print an error:
    Serial.println("error opening car_data.txt");
  }
  if (speedValue >= 80) //80% potentiometer
  {
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("   Crossed Speed    ");
      lcd.setCursor(0,2);
      lcd.print("       limit!       ");
      Serial.println("Sending Message......");
      
     msg = "Message from Wireless Black Box! \n\nCrossed Speed limit!\n\nDate: " + (String) d_m_y + "\nTime(+UTC): " + (String) UTC ; 
     sendSMS(phone_number, msg);
     msg= "Speed: "+(String) speedValue + "%\nVibration: " + (String) vib_value + "\nHumidity: " + (String) hum + "%\nTemperature: " + (String) t + "*C\nAlcohol Value: " + (String) alcohol_val ;
     sendSMS(phone_number, msg);
     msg ="Location: http://maps.google.com/maps?f=q&q=" + lat + "," + lon + "&z=16";
     sendSMS(phone_number, msg);    
  }


  
  else if (vib_value == HIGH)
  {    
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print(" Accident detected! ");
      Serial.println("Sending Message......");
      msg = "Message from Wireless Black Box! \n\nAccident detected!\n\nDate: " + (String) d_m_y + "\nTime(+UTC): " + (String) UTC ; 
      sendSMS(phone_number, msg);
      msg="Speed: " + (String) speedValue + "%\nVibration: " + (String) vib_value + "\nHumidity: " + (String) hum + "%\nTemperature: " + (String) t + "*C\nAlcohol Value: " + (String) alcohol_val;
      sendSMS(phone_number, msg);
      msg="Gyro Sensor Value:\nDifference in AccX: " + (String) X + "\nDifference in AccY: " + (String) Y + "\nDifference in AccZ: " + (String) Z + "\nDifference in GyroX: " + (String) GX + "\nDifference in GyroY: " + (String) GY + "\nDifference in GyroZ: ";
      sendSMS(phone_number, msg);
      msg ="Location: http://maps.google.com/maps?f=q&q=" + lat + "," + lon + "&z=16";
      sendSMS(phone_number, msg);    
  }


  
  else if (alcohol_val >= 30)
  {
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("  Crossed Alcohol   ");
      lcd.setCursor(0,2);
      lcd.print("       limit!       ");
      Serial.println("Sending Message......");
      msg = "Message from Wireless Black Box! \n\nCrossed Alcohol limit!\nDate: " +(String) d_m_y + "\nTime(+UTC): " + (String) UTC ;
      sendSMS(phone_number, msg);
      msg="Speed: " + (String) speedValue + "%\nVibration: " + (String) vib_value + "\nHumidity: " + (String) hum + "%\nTemperature: " + (String) t + "*C\nAlcohol Value: " + (String) alcohol_val ;
      sendSMS(phone_number, msg);
      
      msg ="Location: http://maps.google.com/maps?f=q&q=" + lat + "," + lon + "&z=16";
      sendSMS(phone_number, msg);    
  }
  
  
  else if (t >= 35)
  {  
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("   Fire detected!   ");
      Serial.println("Sending Message......");
      msg = "Message from Wireless Black Box! \n\nFire detected!\nDate: " + (String) d_m_y + "\nTime(+UTC): " + (String) UTC ;
      sendSMS(phone_number, msg);
      msg="Speed: " + (String) speedValue + "%\nVibration: " + (String) vib_value + "\nHumidity: " + (String) hum + "%\nTemperature: " + (String) t + "*C\nAlcohol Value: " + (String) alcohol_val ;
      sendSMS(phone_number, msg);
      msg ="Location: http://maps.google.com/maps?f=q&q=" + lat + "," + lon + "&z=16";
      sendSMS(phone_number, msg);   
  }
  delay(5000);
}


bool connectGPS()// function to Connect the gps
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;


  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }


  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    Serial.println();
    lat = String(flat, 6);
    lon = String(flon, 6);
    Serial.println(lat);
    Serial.println(lon);
    return true;
  }
  else
  {
    return false;
  }
}
bool getResponse(String cmd, String response, int timeout)
{
    Serial.println(cmd);
    current_time = millis();
    gsm_response = "";
    int index = -1;
    int network_status = 0;


    gsm.println(cmd);
    while (true)
    {
        if (gsm.available())
        {
            gsm_response = gsm.readString();
            gsm_response.trim();
            Serial.println(gsm_response);
            index = gsm_response.indexOf(response);
            if (cmd == "AT+CREG?")
            {
                network_status = gsm_response.substring(gsm_response.indexOf(',') + 1, gsm_response.indexOf(',') + 2).toInt();
                if (network_status == 1 || network_status == 5)
                {
                    Serial.println(F("GSM Network registered!"));
                    return true;
                }
                else
                {
                    Serial.println(F("ERROR: Failed to register network."));
                    Serial.println(F("       Press RESET."));
                    return false;
                }
            }
            else
            {
                if (index >= 0)
                {
                    return true;
                }
            }
        }
        if (millis() - current_time >= timeout)
        {
            Serial.println(F("ERROR: GSM response timeout."));
            Serial.println(F("       Press RESET."));
            return false;
        }
    }
}


bool registerNumber(void)
{
    while (!getResponse(F("AT+CNMI=1,2,0,0,0"), OK, 1000))
        ;


    gsm_response = "";
    int start_index = -1;
    int stop_index = -1;
    while (true)
    {
        if (gsm.available())
        {
            gsm_response = gsm.readString();
            gsm_response.trim();
            Serial.println(gsm_response);
            start_index = gsm_response.indexOf('*');
            stop_index = gsm_response.indexOf('#');
            if (start_index >= 0)
            {
                phone_number = gsm_response.substring(start_index + 1, stop_index);
                return true;
            }
        }
    }
}


void initializeGSM(void)
{
    gsm.begin(115200);
    delay(10000);
    gsm.println(F("AT+IPR=9600"));
    delay(10000);
    gsm.end();
    gsm.begin(9600);
}


bool connectGSM(void)
{
    while (!getResponse(F("ATE0"), OK, 1000))
        ;
    while (!getResponse(F("AT+CLIP=1"), OK, 1000))
        ;
    while (!getResponse(F("AT+CVHU=0"), OK, 1000))
        ;
    while (!getResponse(F("AT+CTZU=1"), OK, 1000))
        ;
    while (!getResponse(F("AT+CMGF=1"), OK, 1000))
        ;
    while (!getResponse(F("AT+CNMI=0,0,0,0"), OK, 1000))
        ;
    while (!getResponse(F("AT+CSQ"), OK, 1000))
        ;
    while (!getResponse(F("AT+CREG?"), OK, 1000))
        ;
    while (!getResponse(F("AT+CGREG?"), OK, 1000))
        ;
    while (!getResponse(F("AT+CMGD=1,4"), OK, 1000))
        ;
    if (EEPROM.read(0)==0 ) //reading eeprom memory 0th location 
  {
      lcd.clear();
      lcd.print(F("GSM reset.."));
      delay(10000);
      EEPROM.update(0,1); //updating eeprom memory 0th location with value 1
      delay(2000);
      wdt_enable(WDTO_4S); //this command lines will reset your code 
      wdt_reset();
      while(1);
  }
  delay(2000);
  return true;
}


void sendSMS(String number, String sms)
{
    while (!getResponse(F("AT+CMGF=1"), OK, 1000))
        ;
    while (!getResponse("AT+CMGS=\"" + number + "\"\r", ">", 1000))
        ;
    gsm.print(sms);
    delay(1000);
    gsm.println((char)26);
}
int AccX_checkstability()
{
  last_val = 0, stability = 0;
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr,14,true);
  AccX=Wire.read()<<8|Wire.read();
  AccY=Wire.read()<<8|Wire.read();
  AccZ=Wire.read()<<8|Wire.read();
  Temp=Wire.read()<<8|Wire.read();
  GyroX=Wire.read()<<8|Wire.read();
  GyroY=Wire.read()<<8|Wire.read();
  GyroZ=Wire.read()<<8|Wire.read();
  Serial.print("AccX = "); Serial.print(AccX);
  val = AccX;
  last_val = val;
  while(stability < 4)
  {
    Wire.beginTransmission(MPU6050_addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_addr,14,true);
    AccX=Wire.read()<<8|Wire.read();
    AccY=Wire.read()<<8|Wire.read();
    AccZ=Wire.read()<<8|Wire.read();
    Temp=Wire.read()<<8|Wire.read();
    GyroX=Wire.read()<<8|Wire.read();
    GyroY=Wire.read()<<8|Wire.read();
    GyroZ=Wire.read()<<8|Wire.read();
    val = AccX;
    Serial.println(val);
    delay(100);
    stability++;
    if (((val - last_val) < -500) || ((val - last_val) > 500))
    {
      stability = 0;
      last_val = val;
    }
  }
  Serial.print("AccX");
  Serial.print(" stability=");
  Serial.println(val);
  stability = 0;
  return val;
}


int AccY_checkstability()
{
  last_val = 0, stability = 0;
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr,14,true);
  AccX=Wire.read()<<8|Wire.read();
  AccY=Wire.read()<<8|Wire.read();
  AccZ=Wire.read()<<8|Wire.read();
  Temp=Wire.read()<<8|Wire.read();
  GyroX=Wire.read()<<8|Wire.read();
  GyroY=Wire.read()<<8|Wire.read();
  GyroZ=Wire.read()<<8|Wire.read();
  Serial.print("AccY = "); Serial.print(AccY);
  val = AccY;
  last_val = val;
  while(stability < 4)
  {
    Wire.beginTransmission(MPU6050_addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_addr,14,true);
    AccX=Wire.read()<<8|Wire.read();
    AccY=Wire.read()<<8|Wire.read();
    AccZ=Wire.read()<<8|Wire.read();
    Temp=Wire.read()<<8|Wire.read();
    GyroX=Wire.read()<<8|Wire.read();
    GyroY=Wire.read()<<8|Wire.read();
    GyroZ=Wire.read()<<8|Wire.read();
    val = AccY;
    Serial.println(val);
    delay(100);
    stability++;
    if (((val - last_val) < -500) || ((val - last_val) > 500))
    {
      stability = 0;
      last_val = val;
    }
  }
  Serial.print("AccY");
  Serial.print(" stability=");
  Serial.println(val);
  stability = 0;
  return val;
  delay(100);
}


int AccZ_checkstability()
{
  last_val = 0, stability = 0;
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr,14,true);
  AccX=Wire.read()<<8|Wire.read();
  AccY=Wire.read()<<8|Wire.read();
  AccZ=Wire.read()<<8|Wire.read();
  Temp=Wire.read()<<8|Wire.read();
  GyroX=Wire.read()<<8|Wire.read();
  GyroY=Wire.read()<<8|Wire.read();
  GyroZ=Wire.read()<<8|Wire.read();
  Serial.print("AccZ = "); Serial.print(AccZ);
  val = AccZ;
  last_val = val;
  while(stability < 4)
  {
    Wire.beginTransmission(MPU6050_addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_addr,14,true);
    AccX=Wire.read()<<8|Wire.read();
    AccY=Wire.read()<<8|Wire.read();
    AccZ=Wire.read()<<8|Wire.read();
    Temp=Wire.read()<<8|Wire.read();
    GyroX=Wire.read()<<8|Wire.read();
    GyroY=Wire.read()<<8|Wire.read();
    GyroZ=Wire.read()<<8|Wire.read();
    val = AccZ;
    Serial.println(val);
    delay(100);
    stability++;
    if (((val - last_val) < -500) || ((val - last_val) > 500))
    {
      stability = 0;
      last_val = val;
    }
  }
  Serial.print("AccZ");
  Serial.print(" stability=");
  Serial.println(val);
  stability = 0;
  return val;
  delay(100);
}


int GyroX_checkstability()
{
  last_val = 0, stability = 0;
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr,14,true);
  AccX=Wire.read()<<8|Wire.read();
  AccY=Wire.read()<<8|Wire.read();
  AccZ=Wire.read()<<8|Wire.read();
  Temp=Wire.read()<<8|Wire.read();
  GyroX=Wire.read()<<8|Wire.read();
  GyroY=Wire.read()<<8|Wire.read();
  GyroZ=Wire.read()<<8|Wire.read();
  Serial.print("GyroX = "); Serial.print(GyroX);
  val = GyroX;
  last_val = val;
  while(stability < 4)
  {
    Wire.beginTransmission(MPU6050_addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_addr,14,true);
    AccX=Wire.read()<<8|Wire.read();
    AccY=Wire.read()<<8|Wire.read();
    AccZ=Wire.read()<<8|Wire.read();
    Temp=Wire.read()<<8|Wire.read();
    GyroX=Wire.read()<<8|Wire.read();
    GyroY=Wire.read()<<8|Wire.read();
    GyroZ=Wire.read()<<8|Wire.read();
    val = GyroX;
    Serial.println(val);
    delay(100);
    stability++;
    if (((val - last_val) < -500) || ((val - last_val) > 500))
    {
      stability = 0;
      last_val = val;
    }
  }
  Serial.print("GyroX");
  Serial.print(" stability=");
  Serial.println(val);
  stability = 0;
  return val;
  delay(100);
}


int GyroY_checkstability()
{
  last_val = 0, stability = 0;
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr,14,true);
  AccX=Wire.read()<<8|Wire.read();
  AccY=Wire.read()<<8|Wire.read();
  AccZ=Wire.read()<<8|Wire.read();
  Temp=Wire.read()<<8|Wire.read();
  GyroX=Wire.read()<<8|Wire.read();
  GyroY=Wire.read()<<8|Wire.read();
  GyroZ=Wire.read()<<8|Wire.read();
  Serial.print("GyroY = "); Serial.print(GyroY);
  val = GyroY;
  last_val = val;
  while(stability < 4)
  {
    Wire.beginTransmission(MPU6050_addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_addr,14,true);
    AccX=Wire.read()<<8|Wire.read();
    AccY=Wire.read()<<8|Wire.read();
    AccZ=Wire.read()<<8|Wire.read();
    Temp=Wire.read()<<8|Wire.read();
    GyroX=Wire.read()<<8|Wire.read();
    GyroY=Wire.read()<<8|Wire.read();
    GyroZ=Wire.read()<<8|Wire.read();
    val = GyroY;
    Serial.println(val);
    delay(100);
    stability++;
    if (((val - last_val) < -500) || ((val - last_val) > 500))
    {
      stability = 0;
      last_val = val;
    }
  }
  Serial.print("GyroY");
  Serial.print(" stability=");
  Serial.println(val);
  stability = 0;
  return val;
  delay(100);
}


int GyroZ_checkstability()
{
  last_val = 0, stability = 0;
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr,14,true);
  AccX=Wire.read()<<8|Wire.read();
  AccY=Wire.read()<<8|Wire.read();
  AccZ=Wire.read()<<8|Wire.read();
  Temp=Wire.read()<<8|Wire.read();
  GyroX=Wire.read()<<8|Wire.read();
  GyroY=Wire.read()<<8|Wire.read();
  GyroZ=Wire.read()<<8|Wire.read();
  Serial.print("GyroZ = "); Serial.print(GyroZ);
  val = GyroZ;
  last_val = val;
  while(stability < 4)
  {
    Wire.beginTransmission(MPU6050_addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_addr,14,true);
    AccX=Wire.read()<<8|Wire.read();
    AccY=Wire.read()<<8|Wire.read();
    AccZ=Wire.read()<<8|Wire.read();
    Temp=Wire.read()<<8|Wire.read();
    GyroX=Wire.read()<<8|Wire.read();
    GyroY=Wire.read()<<8|Wire.read();
    GyroZ=Wire.read()<<8|Wire.read();
    val = GyroZ;
    Serial.println(val);
    delay(100);  
    stability++;
    if (((val - last_val) < -500) || ((val - last_val) > 500))
    {
      stability = 0;
      last_val = val;
    }
  }
  Serial.print("GyroZ");
  Serial.print(" stability=");
  Serial.println(val);
  stability = 0;
  return val;
  delay(100);
}

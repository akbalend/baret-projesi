  #include <DHT.h>
  #include <MQ2.h>
  #include <Wire.h> 
  #include <LiquidCrystal_I2C.h>
  #include <SoftwareSerial.h>
  #include <TinyGPS.h>
  TinyGPS gps;
  SoftwareSerial BTSerial(10, 11);
  SoftwareSerial ss(5, 6);
  const int ldr_pin = 7;
  #define DHTPIN 4
  #define DHTTYPE DHT11
  DHT dht(DHTPIN, DHTTYPE);
  //I2C pins declaration
  LiquidCrystal_I2C lcd(0x27, 16, 2);
  int Analog_Input = A0;
  int lpg, co, smoke;
  int baret;


  MQ2 mq2(Analog_Input);

void setup(){
  Serial.begin(9600);
  BTSerial.begin(9600);
  ss.begin(9600);
  lcd.begin();//Defining 16 columns and 2 rows of lcd display
  lcd.backlight();
  mq2.begin();
  dht.begin();
  pinMode(ldr_pin,INPUT);
  lcd.clear();
  lcd.setCursor (0, 0);
  lcd.println("  AKILLI BARET  ");
  lcd.setCursor (0, 1);
  lcd.println("     PROJESI    ");
  delay(2000);
  lcd.clear();
}
void loop(){
  smartdelay(2000);
  Serial.println();

  float flat, flon;
  unsigned long age;
  gps.f_get_position(&flat, &flon);
  
  //Serial.print("lat: "); 
  Serial.println(flat, 6);
  //Serial.print("lon: "); 
  Serial.println(flon, 6);

  
  int temp = dht.readTemperature();
  int hum = dht.readHumidity();

  float* values= mq2.read(false); //set it false if you don't want to print the values in the Serial
  //lpg = values[0];
  lpg = mq2.readLPG();

  lcd.clear();
  lcd.setCursor (0, 0);
  lcd.print("N = ");
  lcd.print(flat, 6);
  lcd.print("     ");
  lcd.setCursor (0, 1);
  lcd.print("E = ");
  lcd.print(flon, 6);
  lcd.print("     ");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Sicaklik: ");
  lcd.print(temp);
  lcd.print(" C");
  lcd.setCursor(0,1);
  lcd.print("Nem: %");
  lcd.print(hum);
  lcd.print(" GAZ: ");
  lcd.print(lpg);
  

  BTSerial.print(temp);
  BTSerial.print(";");
  BTSerial.print(hum);
  BTSerial.print(";");
  BTSerial.print(lpg);
  BTSerial.print(";");
  BTSerial.print(flat,6);
  BTSerial.print(";");
  BTSerial.print(flon,6);
  BTSerial.print(";");

  delay(2000);

  while ( digitalRead( ldr_pin ) == 0)
  {  
    lcd.setCursor (0, 0);
    lcd.print(" CAN  GUVENLIGI ");
    lcd.setCursor (0, 1);
    lcd.print("      ICIN      ");
     delay(1000);
    lcd.setCursor (0, 0);
    lcd.print(" LUTFEN  BARETI ");
    lcd.setCursor (0, 1);
    lcd.print("    TAKINIZ     ");
    
    BTSerial.print(temp);
    BTSerial.print(";");
    BTSerial.print(hum);
    BTSerial.print(";");
    BTSerial.print(lpg);
    BTSerial.print(";");
    BTSerial.print(flat,6);
    BTSerial.print(";");
    BTSerial.print(flat,6);
    BTSerial.print(";");

    delay(1000);
  }

  while (lpg > 40)
  {
    lcd.clear();
    lcd.setCursor (0, 0);
    lcd.print("    GAZ YUKSEK  ");
    lcd.setCursor (0, 1);
    lcd.print(lpg);
    lcd.print("  ");
    lcd.setCursor (3, 1);
    lcd.print(" SEVIYEDE ");

    BTSerial.print(temp);
    BTSerial.print(";");
    BTSerial.print(hum);
    BTSerial.print(";");
    BTSerial.print(lpg);
    BTSerial.print(";");
    BTSerial.print(flat,6);
    BTSerial.print(";");
    BTSerial.print(flat,6);
    BTSerial.print(";");
  }

  while (temp > 32)
  {
    lcd.clear();
    lcd.setCursor (0, 0);
    lcd.print("  ISI YUKSEK  ");
    lcd.setCursor (0, 1);
    lcd.print(temp);
    lcd.print("  ");
    lcd.setCursor (3, 1);
    lcd.print(" SEVIYEDE ");

    BTSerial.print(temp);
    BTSerial.print(";");
    BTSerial.print(hum);
    BTSerial.print(";");
    BTSerial.print(lpg);
    BTSerial.print(";");
    BTSerial.print(flat,6);
    BTSerial.print(";");
    BTSerial.print(flat,6);
    BTSerial.print(";");

  }

}

static void smartdelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

//#include "SPI.h"
#include "Wire.h"
#include "SoftwareSerial.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
//#include "avr/wdt.h"


// software serial PINs set up
int sw_rx = 2;
int sw_tx = 3;

// I2C buffer
int prijmuto[27];
float prijmuto_dec;
String prijmuto_text;


byte rele_modul[17] = {99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// SoftwareSerial SIM900(sw_rx, sw_tx); //RX,TX

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


void setup()   {                

  // wdt_enable(WDTO_8S);


  Wire.begin(100);
  Wire.onReceive(priPrijmu);
  Wire.onRequest(odeslatData);

  
  Serial.begin(9600);
  // SIM900.begin(9600); 


  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // display.begin(SSD1306_SWITCHCAPVCC, 0x3D);


  // rele jako output
  for(int i = 2; i <= 9; i++){
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH);
  }
  

  // Clear the buffer.
  display.clearDisplay();


}


void loop() {
  
  // readTemp();
  // readGET();
  showtext("Kuchyn + K", "teplota", 16);
  showtext("Pokoj puda", "teplota", 20);
  showtext("Obyvak", "teplota", 22);
  showtext("Loznice", "teplota", 24);
  showtext("Venku", "teplota", 22);


  // rele nastaveni relatek dle MASTER
  for(int i = 2; i <= 9; i++){
        digitalWrite(i, !prijmuto[i - 2]);
  }

  clr_wdt();

}


void clr_wdt() {

      // clr_wdt();
  
}



void showtext(String radek1, String radek2, int radek3) {
  int teplota = 1;
  int vlhkost = 2;
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0,0);
  display.clearDisplay();

  display.println(radek1);
  if(radek2 == "teplota"){
    display.print(prijmuto[radek3]);
    display.print(".");
    display.print(prijmuto[radek3 + 1]);
    display.print(" ");
    display.setTextSize(1);
    display.print(prijmuto[18]);
    display.print(".");
    display.print(prijmuto[19]);
    display.println("%");
  }else{
    display.println(radek2);
  }
  display.display();
  delay(2100);
  display.clearDisplay();

  clr_wdt();
 
}




void delayWDT(int delaysec = 30){
            clr_wdt();
       
            int waiting_call = 0;
            while(waiting_call <= delaysec){
                delay (1000);
                clr_wdt();
                waiting_call += 1;
            }            
            clr_wdt();
}



void priPrijmu(int b){
    int prijmuto_counter = 0;
    int read_data;
    
    Serial.println("--- Start Read ---");
    
    while(Wire.available() > 0){
        read_data = Wire.read();
        prijmuto[prijmuto_counter] = read_data;
        //Serial.println(Wire.read());
        Serial.print("prijmuto  [ ");
        Serial.print(prijmuto_counter);
        Serial.print(" ]  = > ");
        Serial.println(prijmuto[prijmuto_counter]);
        prijmuto_counter++;
    }

    Serial.println("--- End Read ---");

    //prijmuto_dec = getI2Cvals(prijmuto[0], prijmuto[1]);

    /*  
      prijmuto_text.toInt();
      
      https://www.arduino.cc/en/Reference/CharacterAnalysis
      

    */
}



void odeslatData() {
   Wire.flush();
   Wire.write(33);
   Serial.println("Data odeslana");
   Wire.flush();
   clr_wdt();
}


/*

// alarm zavola na mobil
void setAlarm(){
            Serial.println("Alarm");                   
            
            //makeCall("604833891");
            //makeCall("605906254");
            //makeCall("737226659");
            //sendSMS("737226659");
            //makeCall("737226659");
            
}



void makeCall(char callnumber[]){
            clr_wdt();
            
            SIM900.print("ATD");
            SIM900.print(callnumber);
            SIM900.println(";");
            delay(100);            
             
            while (SIM900.available()){
            s = SIM900.read();
              Serial.print(s);
            }  
        
            delayWDT(29);
                       
            SIM900.println("ATH");                      
            delay (1000);
            clr_wdt();
}


void sendSMS(char smsnumber[]){
          SIM900.println("AT+CMGF=1");  // AT command na odeslani SMS zpravy
          delay(100);
          SIM900.print("AT+CMGS=\"");  // cislo prijemce
          SIM900.print(smsnumber);  // cislo prijemce
          SIM900.println("\"");  // cislo prijemce
          delay(100);
          SIM900.println("Alarm send.");  // zprava k odeslani
          SIM900.print("Status: ");  // zprava k odeslani
          SIM900.print(rele_modul[1]);  // zprava k odeslani
          SIM900.print(rele_modul[2]);  // zprava k odeslani
          SIM900.print(rele_modul[3]);  // zprava k odeslani
          SIM900.println(rele_modul[4]);  // zprava k odeslani
          SIM900.print(" is set.");  // zprava k odeslani
          delay(100);
          SIM900.println((char)26);  //  AT command znak a ^Z, ASCII code 26 pro konec SMS zpravy
          delay(100); 
          SIM900.println();
          delayWDT(11);  // pokej na odesln SMS
          //SIM900power();  // vypni GSM module
          clr_wdt();
        }

*/




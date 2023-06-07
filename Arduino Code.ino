//#####################################################
//-------------------- KELOMPOK 12 --------------------
//--------- SMART MONITORING GARDEN WITH IoT ----------
//------------ ELECTRICAL ENGINEERING 02 --------------
//#####################################################

#include <SoftwareSerial.h>
#define outPompa 13
#define buzz 3

SoftwareSerial s(9,8);
String pesan;
bool siram = false;
bool pir = false;
bool status1 = false;
bool status2 = false;
int ldr, soil, suhu;
int bawahSoil = 24; //Batas bawah kelembapan
int atasSoil = 27; //Batas atas kelembapan

void cekKondisi(){
  Serial.println("Mengecek Kondisi...");
  cekSensor();
  if (soil < bawahSoil && siram){
    Serial.println("Pompa dinyalakan...tapi udah");
  } else if (soil < bawahSoil && !siram){
    digitalWrite(outPompa,1);
    siram = 1;
    Serial.println("Pompa dinyalakan...");
  } else if (soil > atasSoil && siram){
    digitalWrite(outPompa,0);
    siram = 0;
    Serial.println("Pompa dimatikan...");
  } else if (soil > atasSoil && !siram){
    Serial.println("Pompa dimatikan...tapi udah");
  }
  if (pir == 1){
    digitalWrite(buzz,1);
    pir = 1;
    Serial.println("Mendeteksi pergerakan...");
  } else if (pir == 0){
    digitalWrite(buzz,0);
    pir = 0;
    Serial.println("Tidak ada pergerakan...");
  }
}

void cekSensor(){
  //Buat koding ldr
  ldr = random(11,20); //C ==> Masih diisi random karena belum ada sensornya
  Serial.println(String(ldr));
  //Buat koding soil
  soil = random(21,30); //K ==> Masih diisi random karena belum ada sensornya
  Serial.println(String(soil));
  //Buat koding suhu
  suhu = random(31,40); //S ==> Masih diisi random karena belum ada sensornya
  Serial.println(String(suhu));
  //Buat koding pir
  pir = random(0,1); //G ==> Masih diisi random karena belum ada sensornya
  Serial.println(String(pir)); 
}

void setup() {
  Serial.begin(9600);
  s.begin(9600);
  pinMode(outPompa, OUTPUT);
  Serial.println("Halo");
}

void loop() {
    while (s.available()>0){
      delay(10);
      char c = s.read();
      pesan += c;
    }
    if (pesan != ""){
      pesan.trim();
      Serial.println(pesan);
      Serial.println(pesan.length());
      if (pesan.equals("/siramOn")){
        digitalWrite(outPompa,1);
        siram = true;
        Serial.println("Pompa dinyalakan...");
        s.print("\nPompa dinyalakan...");
      } else if (pesan.equals("/siramOff")){
        digitalWrite(outPompa,0);
        siram = false;
        Serial.println("Pompa dimatikan...");
        s.print("\nPompa dimatikan...");
      } else if (pesan.equals("/monitor")){
        Serial.println("Mengecek kondisi...");
        cekSensor();
        s.print("\nK : "+String(soil)+"\nS : "+String(suhu)+"\nC : "+String(ldr)+"\nG : "+String(pir)+"\nP : "+String(siram));
      } else {
        s.print("\nPERINTAH TIDAK VALID !!!");
      }
      pesan = "";
    }
    cekKondisi();
    delay(5000);
  }

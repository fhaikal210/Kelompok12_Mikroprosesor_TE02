//#####################################################
//-------------------- KELOMPOK 12 --------------------
//--- SMART MONITORING AND CONTROL GARDEN WITH IoT ----
//------------ ELECTRICAL ENGINEERING 02 --------------
//#####################################################

#include <SoftwareSerial.h> //Library komunikasi serial modul wifi

SoftwareSerial s(9,8); //Komunikasi serial modul wifi
String pesan;
bool siram = 0, pir = 0;
int ldr, soil, suhu;

//----- Set Batas Kelembapan -----
int bawahSoil = 20; //Batas bawah kelembapan
int atasSoil = 80; //Batas atas kelembapan
//--------------------------------

//----- Pin Setiap Komponen -----
byte s_ldr = A0; //Pin LDR
byte s_soil = A1; //Pin Soil Moisture
byte s_suhu = A2; //Pin Thermistor
const int s_pir = 11; //Pin PIR
const int outPompa = 13; //Pin Pompa
const int buzz = 12; //Pin Buzzer
//-------------------------------

//----- Komponen Baca Suhu -----
int Vo;
float logR2, R2, T, Tc, Tf, R1 = 10000, c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
//------------------------------

void cekKondisi(){
  Serial.println("Mengecek Kondisi...");
  cekSensor();
  if (soil < bawahSoil && siram){
    Serial.println("Pompa dinyalakan...tapi sudah");
  } else if (soil < bawahSoil && !siram){
    digitalWrite(outPompa,1);
    siram = 1;
    Serial.println("Pompa dinyalakan...");
  } else if (soil > atasSoil && siram){
    digitalWrite(outPompa,0);
    siram = 0;
    Serial.println("Pompa dimatikan...");
  } else if (soil > atasSoil && !siram){
    Serial.println("Pompa dimatikan...tapi sudah");
  }
  if (pir == 1){
    digitalWrite(buzz,1);
    pir = 1;
    Serial.println("Mendeteksi pergerakan...");
  } else {
    digitalWrite(buzz,0);
    pir = 0;
    Serial.println("Tidak ada pergerakan...");
  }
}

void cekSensor(){
  //----- Kode Sensor LDR (C) -----
  ldr = 125235178.3654270 * pow(((5-((float)analogRead(s_ldr) / 1023 * 5.0)) / ((float)analogRead(s_ldr) / 1023 * 5.0) * 10000), -1.604568157); //C ==> Hasil baca LDR
  Serial.println(String(ldr));
  //-------------------------------

  //----- Kode Sensor Kelembapan (K) -----
  soil = (100 - ((analogRead(s_soil)/1023.00)*100)); //K ==> Hasil baca kelembapan
  Serial.println(String(soil));
  //--------------------------------------

  //----- Kode Sensor Suhu (S) -----
  Vo = analogRead(s_suhu);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  suhu = T - 273.15; //S ==> Hasil baca suhu
  Serial.println(String(suhu));
  //--------------------------------

  //----- Kode Sensor PIR (G) -----
  pir = digitalRead(s_pir); //G ==> Hasil baca PIR
  Serial.println(String(pir));
  //-------------------------------
}

void setup() {
  Serial.begin(9600);
  s.begin(9600);
  pinMode(outPompa, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(s_ldr,INPUT);
  pinMode(s_soil,INPUT);
  pinMode(s_suhu,INPUT);
  pinMode(s_pir,INPUT);
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
      if (pesan.equals("/siramon")){ //==> Pesan untuk menyiram
        digitalWrite(outPompa,1);
        siram = true;
        Serial.println("Pompa dinyalakan...");
        s.print("\nPompa dinyalakan...");
      } else if (pesan.equals("/siramoff")){ //==> Pesan untuk berhenti menyiram
        digitalWrite(outPompa,0);
        siram = false;
        Serial.println("Pompa dimatikan...");
        s.print("\nPompa dimatikan...");
      } else if (pesan.equals("/monitor")){ //==> Pesan untuk monitoring
        Serial.println("Mengecek kondisi...");
        cekSensor();
        s.print("\nK : "+String(soil)+"\nS : "+String(suhu)+"\nC : "+String(ldr)+"\nG : "+String(pir)+"\nP : "+String(siram));
      } else {
      }
      pesan = "";
    }
    cekKondisi();
    delay(1000);
  }


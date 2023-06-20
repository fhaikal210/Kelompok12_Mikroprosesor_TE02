//#####################################################
//-------------------- KELOMPOK 12 --------------------
//--- SMART MONITORING AND CONTROL GARDEN WITH IoT ----
//------------ ELECTRICAL ENGINEERING 02 --------------
//#####################################################

//----- Library -----
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
//-------------------

//----- Pengaturan WiFi -----
const char* ssid = "Internet@wifi.id";
const char* password = "asdfghjkl";
//---------------------------

SoftwareSerial s(0,2); //==> Komunikasi serial ke arduino

//----- Pengaturan Telegram -----
#define BOTtoken "6028107999:AAEiyN8rS8bWHF9ULba4l2WaKXbeIrXt4w4"  
#define CHAT_ID "1354426222"
//-------------------------------

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
const int ledPin = 1;
bool ledState = LOW;
String pesan;
//----- Pengaturan Pesan Pembuka -----
String intro = "========== Selamat Datang ==========\n------- IoT Control & Monitoring Garden -------\n=========== Kelompok 12 ===========\n\nGunakan perintah di bawah ini untuk mengendalikan perangkat.\n\n/monitor ==> Monitoring keadaan kebun terkini\n/siramon ==> Menyalakan pompa penyiraman\n/siramoff ==> Mematikan pompa penyiraman\n\nAtau gunakan menu command bot\n \xF0\x9F\x91\x87";
//------------------------------------

void handleNewMessages(int numNewMessages) {
  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    String text = bot.messages[i].text;
    s.println(text);
    String from_name = bot.messages[i].from_name;
  }
}

void setup() {
  s.begin(9600);
  configTime(0, 0, "pool.ntp.org");
  client.setTrustAnchors(&cert);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  bot.sendMessage(CHAT_ID, intro, "");
  ledState = HIGH;
  digitalWrite(ledPin, ledState);
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    while (s.available()>0){
      delay(10);
      char c = s.read();
      pesan += c;
    }
    if (pesan != ""){
      pesan.trim();
      if (pesan.startsWith("K :")){
        bot.sendMessage(CHAT_ID, pesan, "");
        bot.sendMessage(CHAT_ID, "Keterangan :\nK ==> Kelembapan (%)\nS ==> Suhu (°C)\nC ==> Intensitas Cahaya (Lux)\nG ==> Deteksi Pergerakan\nP ==> Kondisi Pompa (1 = Menyala atau 0 = Mati)","");
        pesan = "";
      }
      bot.sendMessage(CHAT_ID, pesan, "");
      pesan = "";
    }
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}

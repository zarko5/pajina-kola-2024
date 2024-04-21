#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <Servo.h>

#define N2 15
#define N1 13
#define ENA 12

Servo motor;
WiFiUDP udp;
unsigned int localUdpPort = 4210;
char incomingPacket[256];
char replyPacket[] = "primljeno";
uint32 uintaddress;


void setup() {
  delay(1000);

  pinMode(N2, OUTPUT);
  pinMode(N1, OUTPUT);
  pinMode(ENA, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");

  // postavljanje access pointa i trazenje ip kontrolera
  WiFi.softAP("esp", "password", 1, false, 1);

  while ( WiFi.softAPgetStationNum() == 0) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\n");
  unsigned char softap_stations_cnt;
  struct station_info *stat_info;
  struct ip4_addr *IPaddress;

  stat_info = wifi_softap_get_station_info();
  IPaddress = &stat_info->ip;
  uintaddress = (IPaddress->addr) >> 24;

  IPAddress contIP(192, 168, 4, uintaddress);
  Serial.println(WiFi.softAPIP().toString());
  Serial.println(contIP.toString());
  udp.begin(localUdpPort);
}

IPAddress contIP(192, 168, 4, uintaddress);
int speed = 0;
int speedGood = 0;
bool dir = 1;
char comm = '0';

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
    }
    Serial.printf(incomingPacket);
    comm = incomingPacket[0];
    switch (comm) {
      case 'u':
        if ( speed < 255 ) speed += 5;
        break;
      case 'd':
        if ( speed > -255 ) speed -= 5;
        break;
    }
    

    // send back a reply, to the IP address and port we got the packet from
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.write(replyPacket);
    udp.endPacket();
  }

  dir = (speed > 0);
  speedGood = abs(speed);
  if ( speed != 0 ) speedGood = map(speedGood, 0, 255, 80, 255);
  digitalWrite(N1, dir);
  digitalWrite(N2, !dir);
  analogWrite(ENA, speedGood);
  Serial.println(dir);
  Serial.println(speed);
}

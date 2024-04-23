#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <Servo.h>

#define N1 12
#define N2 13
#define SRV 14
#define ENA 15

Servo motor;
WiFiUDP udp;
unsigned int localUdpPort = 4210;
//char incomingPacket[256];
char replyPacket[] = "primljeno";
uint32 uintaddress;
String poruka;


void setup() {
  delay(1000);

  pinMode(N2, OUTPUT);
  pinMode(N1, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(SRV, OUTPUT);


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
  motor.attach(SRV, 500, 2400);
  motor.write(90);
}

IPAddress contIP(192, 168, 4, uintaddress);
int speed = 0, speedGood;
bool dir;
int theta;
char comm = '0';
char incomingPacket[256];
int baterija = 0;

void loop() {
  baterija = analogRead(A0);
  int packetSize = udp.parsePacket();
  if (packetSize) {
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
      poruka = String(incomingPacket);
      theta = poruka.substring(0, poruka.indexOf(',')).toInt();
      speed = poruka.substring(poruka.indexOf(',') + 1, poruka.lastIndexOf(',')).toInt();
      dir = poruka.substring(poruka.lastIndexOf(',') + 1).toInt();
    }
    Serial.println(speed);
    Serial.println(theta);
    Serial.println(dir);
    Serial.println(poruka);
    Serial.println("_______");


    // send back a reply, to the IP address and port we got the packet from
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.write(replyPacket);
    udp.endPacket();
  }

  speedGood = abs(speed);
  if ( speed != 0 ) speedGood = map(speedGood, 0, 100, 80, 255);
  digitalWrite(N1, dir);
  digitalWrite(N2, !dir);
  motor.write(theta);
  analogWrite(ENA, speedGood);

}
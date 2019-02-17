
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>


char ssid[] = "YOURWIFINAME";
char pass[] = "YOURWIFIPASSWORD";

int hours =0;
int mins =0;
int secs =0;

#define PIN            2
#define NUMPIXELS      46
#define LEDBRIGHTNESS  50 // Range is from 0 to 255
#define LEDCOLOUR      50, 0, 50 // Colour in RGB format, 255, 0, 0 is Red, 0, 0, 255 is Blue and 255, 255, 255 is White
#define LEDCOLOURseparacion       0 ,50 ,0
#define TIMECORRECTION epoch-0 // Very basic way to correct time for daylight savings etc, 3600 = 1 hour, so epoch-3600 will take away 1 hour, epoch+3600 will add an hour, epoch+7200 will add 2 hours etc. epoch-0 will use UTC time. 


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
 

unsigned int localPort = 2390;

IPAddress timeServerIP;
const char* ntpServerName = "time.nist.gov";

const int NTP_PACKET_SIZE = 48;

byte packetBuffer[ NTP_PACKET_SIZE];

WiFiUDP udp;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
  pixels.begin();
}

void loop()
{
  WiFi.hostByName(ntpServerName, timeServerIP); 

  sendNTPpacket(timeServerIP); 
  delay(100);
  
  int cb = udp.parsePacket();
  if (!cb) {
    //Serial.println("no packet yet");
  }
  else {
    //Serial.print("packet received, length=");
    //Serial.println(cb);
    
    udp.read(packetBuffer, NTP_PACKET_SIZE);

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;
    epoch=(TIMECORRECTION);


    hours = (epoch  % 86400L) / 3600;
    if (hours<24)
    {
      hours++;  
    }
    if (hours==24)
    {
      hours=0;  
    }
    mins = (epoch  % 3600) / 60;
    secs = (epoch  % 60) / 1;
    Serial.print(hours); 
    Serial.print(':');
    Serial.println(mins); 
    Serial.print(':');
    Serial.print(secs);
    
    pixels.setBrightness(LEDBRIGHTNESS);
    
    drawdigit(0,255, 0, 0,hours/10);//Draw the first digit of the hour
    drawdigit(7,255, 0, 0,hours-((hours/10)*10));//Draw the second digit of the hour
    pixels.setPixelColor(14, pixels.Color(LEDCOLOURseparacion ));//Draw the two middle dots
    pixels.setPixelColor(15, pixels.Color(LEDCOLOURseparacion ));
    drawdigit(16,255, 0, 0,mins/10);//Draw the first digit of the minute
    drawdigit(23,255, 0, 0,mins-((mins/10)*10));//Draw the second digit of the minute
    pixels.setPixelColor(30, pixels.Color(LEDCOLOURseparacion ));//Draw the two middle dots
    pixels.setPixelColor(31, pixels.Color(LEDCOLOURseparacion ));
    drawdigit(32,255, 0, 0,secs/10);//Draw the first digit of the second
    drawdigit(39,255, 0, 0,secs-((secs/10)*10));//Draw the second digit of the second
    
    delay(100);//Request to NIST server .

    





     
  }
}
 



void drawdigit(int offset, int r, int g, int b, int n)
{

  if (n == 2 || n == 3 || n == 4 || n == 5 || n == 6 || n == 8 || n == 9 ) //MIDDLE
  {
    pixels.setPixelColor(0 + offset, pixels.Color(LEDCOLOUR));
 
  }
  else
  {
    pixels.setPixelColor(0 + offset, pixels.Color(0, 0, 0));

  }
  if (n == 0 || n == 1 || n == 2 || n == 3 || n == 4 || n == 7 || n == 8 || n == 9) //TOP RIGHT
  {
    pixels.setPixelColor(1 + offset, pixels.Color(LEDCOLOUR));

  }
  else
  {
    pixels.setPixelColor(1 + offset, pixels.Color(0, 0, 0));

  }
  if (n == 0 || n == 2 || n == 3 || n == 5 || n == 6 || n == 7 || n == 8 || n == 9) //TOP
  {
    pixels.setPixelColor(2 + offset, pixels.Color(LEDCOLOUR));

  }
  else
  {
    pixels.setPixelColor(2 + offset, pixels.Color(0, 0, 0));

  }
  if (n == 0 || n == 4 || n == 5 || n == 6 || n == 8 || n == 9 ) //TOP LEFT
  {
    pixels.setPixelColor(3 + offset, pixels.Color(LEDCOLOUR));

  }
  else
  {
    pixels.setPixelColor(3 + offset, pixels.Color(0, 0, 0));

  }
  if (n == 0 || n == 2 || n == 6 || n == 8) //BOTTOM LEFT
  {
    pixels.setPixelColor(4 + offset, pixels.Color(LEDCOLOUR));

  }
  else
  {
    pixels.setPixelColor(4 + offset, pixels.Color(0, 0, 0));

  }
  if (n == 0 || n == 2 || n == 3 || n == 5 || n == 6 || n == 8 || n == 9) //BOTTOM
  {
    pixels.setPixelColor(5 + offset, pixels.Color(LEDCOLOUR));

  }
  else
  {
    pixels.setPixelColor(5 + offset, pixels.Color(0, 0, 0));

  }
  if (n == 0 || n == 1 || n == 3 || n == 4 || n == 5 || n == 6 || n == 7 || n == 8 || n == 9) //BOTTOM RIGHT
  {
    pixels.setPixelColor(6 + offset, pixels.Color(LEDCOLOUR));

  }
  else
  {
    pixels.setPixelColor(6 + offset, pixels.Color(0, 0, 0));
 
  }
  pixels.show();
}



// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address)
{
  //Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

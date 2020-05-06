#include <WiFiNINA.h>
#include <ArduinoBearSSL.h>
#include <ArduinoECCX08.h>
#include <ArduinoMqttClient.h>
#include <utility/wifi_drv.h>

char ssid[] = "Samsung";             //  your network SSID (name) between the " "
char pass[] = "12345678";      // your network password between the " "
char broker[] = "a37a9entcjl8r-ats.iot.us-east-2.amazonaws.com";
const char certificate[] = R"(
-----BEGIN CERTIFICATE-----
MIICiTCCAXGgAwIBAgIVAPcm1cQhJVTt1fSP/754u69+C7PbMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMDA1MDMwMDA3
NTdaFw00OTEyMzEyMzU5NTlaMBgxFjAUBgNVBAMTDU15TUtSV2lGaTEwMTAwWTAT
BgcqhkjOPQIBBggqhkjOPQMBBwNCAAR3lwslz0GcFz92NPqjlkKrjviPspIbpDY6
gxVa2evab5SLZfBpcOkfoCIkjEwABDZloUMnsvennMZKBSvs3yMso2AwXjAfBgNV
HSMEGDAWgBRnowzchseagmniaCpadQeZxxK2wjAdBgNVHQ4EFgQUHtFQhwTv6feg
sbQgRhlS3eIE8/YwDAYDVR0TAQH/BAIwADAOBgNVHQ8BAf8EBAMCB4AwDQYJKoZI
hvcNAQELBQADggEBAHeDWrBk27hx8n/LAgmJ1Iuu9Nw2Gv2ZXnzxUEam7Nd6Pi3o
4kx5xncNLiqtmO+niRq4y94htFzBK65L9bLyaeP37pzJrLmxBVvntLXZi6JwiLk8
NATv0XKB71Petio/xJmxoFkbzX4jor+8aOTikTIT8GnNttshLIiyhDKmWVBmgRME
jE8kyPbvBtEtULNyyVPhu+l6pdy8TeLsRHIuyWAqUksIwIWtQQ3GjNFxE9T8hbIs
gBZPsPF9HYNkwc9gzsnAgjw1uzMegp/uWAf2M0eua+lsoy93my0g5whTxEl8aqrG
gwQJGgS6AYKCJJdjCDXhj/3KabTpuhrW/m43Zh0=
-----END CERTIFICATE-----
)";


int status = WL_IDLE_STATUS;      //connection status
int num;

WiFiClient    wifiClient;            // Used for the TCP socket connection
BearSSLClient sslClient(wifiClient); // Used for SSL/TLS connection, integrates with ECC508
MqttClient    mqttClient(sslClient);

unsigned long lastMillis = 0;

int a = 2;  //For displaying segment "a"
int b = 3;  //For displaying segment "b"
int c = 4;  //For displaying segment "c"
int d = 5;  //For displaying segment "d"
int e = 6;  //For displaying segment "e"
int f = 8;  //For displaying segment "f"
int g = 9;  //For displaying segment "g"

void setup() {
  Serial.begin(9600);      // initialize serial communication

  pinMode(a, OUTPUT);  //A
  pinMode(b, OUTPUT);  //B
  pinMode(c, OUTPUT);  //C
  pinMode(d, OUTPUT);  //D
  pinMode(e, OUTPUT);  //E
  pinMode(f, OUTPUT);  //F
  pinMode(g, OUTPUT);  //G

  WiFiDrv::pinMode(25, OUTPUT); //Green
  WiFiDrv::pinMode(26, OUTPUT); //Red
  WiFiDrv::pinMode(27, OUTPUT); //Blue

  WiFiDrv::analogWrite(26, 5);

 
   if (!ECCX08.begin()) {
    Serial.println("No ECCX08 present!");
    while (1);
  }

    // used to validate the servers certificate
  ArduinoBearSSL.onGetTime(getTime);

  // Set the ECCX08 slot to use for the private key
  // and the accompanying public certificate for it
  sslClient.setEccSlot(0, certificate);


  // Set the message callback, this function is
  // called when the MQTTClient receives a message
  mqttClient.onMessage(onMessageReceived);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);       // don't continue
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
    
  }
  WiFiDrv::analogWrite(26, 4);
  WiFiDrv::analogWrite(25, 4);
  printWiFiStatus();                        // you're connected now, so print out the status

}

void loop() {

    if (!mqttClient.connected()) {
    // MQTT client is disconnected, connect
    connectMQTT();
  }
  
  mqttClient.poll();
   WiFiDrv::analogWrite(25, 5);
   WiFiDrv::analogWrite(26, LOW);
  // publish a message roughly every 5 seconds.
  if (millis() - lastMillis > 60000) {
    lastMillis = millis();

    publishMessage();
  }

}

unsigned long getTime() {
  // get the current time from the WiFi module  
  return WiFi.getTime();
}

void connectMQTT() {
  Serial.print("Attempting to MQTT broker: ");
  Serial.print(broker);
  Serial.println(" ");

  while (!mqttClient.connect(broker, 8883)) {
    // failed, retry
    Serial.print(".");
    delay(10000);
  }
  Serial.println();

  Serial.println("You're connected to the MQTT broker");
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe("arduino/incoming");
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  Serial.println(ip);
}

void publishMessage() {
  
  // send message, the Print interface can be used to set the message contents
  mqttClient.beginMessage("arduino/outgoing");
  mqttClient.print('{');
  mqttClient.print('"');
  mqttClient.print("Uptime");
  mqttClient.print('"');
  mqttClient.print(':');
  mqttClient.print('"');
  mqttClient.print(millis()/1000);
  mqttClient.print('"');
  mqttClient.print(',');
  mqttClient.print('"');
  mqttClient.print("Number");
  mqttClient.print('"');
  mqttClient.print(':');
  mqttClient.print('"');
  mqttClient.print(num);
  mqttClient.print('"');
  mqttClient.print('}');
  mqttClient.endMessage();

  Serial.print("\nMessage Sent to AWS:\n");
  Serial.print("Uptime:");
  Serial.print(millis()/1000);
  Serial.print("s\n");
  Serial.print("Number:");
  Serial.print(num);
  Serial.print("\n");
  
}

void displayDigit(int digit)
{
 num = digit;
 //Conditions for displaying segment a
 if(digit!=1 && digit != 4)
 digitalWrite(a,HIGH);
 
 //Conditions for displaying segment b
 if(digit != 5 && digit != 6)
 digitalWrite(b,HIGH);
 
 //Conditions for displaying segment c
 if(digit !=2)
 digitalWrite(c,HIGH);
 
 //Conditions for displaying segment d
 if(digit != 1 && digit !=4 && digit !=7)
 digitalWrite(d,HIGH);
 
 //Conditions for displaying segment e 
 if(digit == 2 || digit ==6 || digit == 8 || digit==0)
 digitalWrite(e,HIGH);
 
 //Conditions for displaying segment f
 if(digit != 1 && digit !=2 && digit!=3 && digit !=7)
 digitalWrite(f,HIGH);
 if (digit!=0 && digit!=1 && digit !=7)
 digitalWrite(g,HIGH);
 
}

void onMessageReceived(int messageSize) {
  turnOff();
  // we received a message, print out the topic and contents
  Serial.print("\nReceived a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  
    char testVal = (char)mqttClient.read();
    int inValue = testVal - '0';
    Serial.print(inValue);
  
 
  displayDigit(inValue);
  
  Serial.println();

  Serial.println();
}

void turnOff()
{
  digitalWrite(a,LOW);
  digitalWrite(b,LOW);
  digitalWrite(c,LOW);
  digitalWrite(d,LOW);
  digitalWrite(e,LOW);
  digitalWrite(f,LOW);
  digitalWrite(g,LOW);
}

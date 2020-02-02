#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

/////// Wifi Settings ///////
char ssid[] = "insert ssid here";
char pass[] = "insert password here";

// static ip address of rpi (navigate to http://fritz.box/# to check)  
char serverAddress[] = "insert rpi IP address";  // server address
// ntop defaults to 3000 
int port = 3000;
int pulse = 0;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;
String response;
int statusCode = 0;


float Seans_Macbook_Throughput = 0;
float Ms_iPhone_Throughput = 0; 
float Seans_iPhone_Throughput = 0;

float Seans_Macbook_Pulse = 0;
float Ms_iPhone_Pulse = 0; 
float Seans_iPhone_Pulse = 0;

void setup() {
  Serial.begin(19200);
  Serial.println("setup");
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  Serial.print("Now for the fan set-up");
  pwm.begin();
  pwm.setPWMFreq(1000);  // This is the maximum PWM frequency
  //40 you hear the fans a little but its not a huge problem - seems they are a little bit
  //louder and longer - quite like it 
  //1000 - but this seems have the least noise
  Wire.setClock(400000);
  
}

void loop() {
//  Serial.println("making GET request with HTTP basic authentication");
  client.beginRequest();
  Serial.println("now getting data from server");
//client.get("/lua/get_macs_data.lua?");
  delay(100);
  client.get("/lua/examples/Metabolism.lua");
  //client.get("/lua/mac_stats.lua?host=34%3A8A%3AAE%3ABA%3A5A%3ADE");
  Serial.println("now authenticating");
  client.sendBasicAuth("insert username", "insert password"); // send the username and password for authentication
  Serial.println("now ending request");
  client.endRequest();
  Serial.println("request ended and now reading status code");
  // read the status code and body of the response
  statusCode = client.responseStatusCode();
  Serial.println("now creating response");
  response = client.responseBody();

  client.stop();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response 1: ");
 
  Serial.println(response);
 // Serial.println("Wait five seconds");

 const size_t bufferSize = JSON_OBJECT_SIZE(4) +150;
 DynamicJsonBuffer jsonBuffer(bufferSize);

  JsonObject& root = jsonBuffer.parseObject(response);
  if (!root.success()) {
    Serial.println(F("Parsing failed!"));
    return;
  }

//
Seans_Macbook_Throughput = root["Seans_Macbook_Throughput"];
 if (Seans_Macbook_Throughput > 150000) Seans_Macbook_Throughput = 150000;  
 Seans_Macbook_Pulse= map(Seans_Macbook_Throughput, 0, 150000, 0, 4095);
 Serial.println("Sean's Macbook: "); 
 Serial.println(Seans_Macbook_Throughput);
// pwm.setPWM(11,0,Seans_Macbook_Pulse);
 pwm.setPWM(8,0,Seans_Macbook_Pulse);
  pwm.setPWM(11,0,Seans_Macbook_Pulse);
   pwm.setPWM(15,0,Seans_Macbook_Pulse);
 

 Seans_iPhone_Throughput = root["Seans_iPhone_Throughput"];
 if (Seans_iPhone_Throughput > 150000) Seans_iPhone_Throughput = 150000;  
Seans_iPhone_Pulse= map(Seans_iPhone_Throughput, 0, 150000, 0, 4095);
 Serial.println("Seans iPhone: "); 
 Serial.println(Seans_iPhone_Throughput);
 pwm.setPWM(8,0,Seans_iPhone_Pulse);
pwm.setPWM(11,0,Seans_iPhone_Pulse);
 pwm.setPWM(15,0,Seans_iPhone_Pulse);

delay(1500);
Serial.print("end of function");

}

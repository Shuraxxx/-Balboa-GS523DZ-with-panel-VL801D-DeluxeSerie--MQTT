#include <Balboa_GS_Interface.h>


  /*
 *    
 *    Main board: Wemos D1 mini - esp8266
 *  
 *    SPA display controller for Balboa system GS510SZ 
 *    
 */
    

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ElegantOTA.h>                  // https://github.com/ayushsharma82/ElegantOTA
#include <ESP8266WiFi.h>  
#include <PubSubClient.h>               // https://github.com/knolleary/pubsubclient
#include <Balboa_GS_Interface.h>        // https://github.com/Shuraxxx/-Balboa-GS523SZ-with-panel-VL801D-DeluxeSerie--MQTT/blob/main/src/Balboa_GS_Interface.h 

#define setClockPin D1  
#define setReadPin  D2 
#define setWritePin D8  


//Constants
const char *wifi_ssid                    = "wifi";          // WiFi SSID
const char *wifi_pwd                     = "pass";          // WiFi Password 
const char *wifi_hostname                = "SPA";
const char* mqtt_server                  = "192.168.1.1";           // MQTT Boker IP, your home MQTT server eg Mosquitto on RPi, or some public MQTT
const int mqtt_port                      = 1883;        // MQTT Broker PORT, default is 1883 but can be anything.
const char *mqtt_user                    = "addons";          // MQTT Broker User Name
const char *mqtt_pwd                     = "pass";          // MQTT Broker Password 
String clientId                          = "SPA Balboa : " + String(ESP.getChipId(), HEX);

//Globals 
bool debug                               = true;    // If true activate debug values to write to serial port

const unsigned long ReportTimerMillis    = 10000;   // Timer in milliseconds to report mqtt topics 
unsigned long ReportTimerPrevMillis      = 0;       // Store previous millis


// MQTT Constants
const char* mqtt_Display_topic              = "SPA/Display";
const char* mqtt_SetTemp_topic              = "SPA/SetTemp";
const char* mqtt_WaterTemp_topic            = "SPA/WaterTemp";
const char* mqtt_Heater_topic               = "SPA/Heater";
const char* mqtt_Pump1_topic                = "SPA/Pump1";
const char* mqtt_Pump2_topic                = "SPA/Pump2";
const char* mqtt_Blober_topic               = "SPA/Blober";
const char* mqtt_Lights_topic               = "SPA/Lights";
const char* mqtt_StandartMode_topic         = "SPA/StandartMode";
const char* mqtt_EcoMode_topic              = "SPA/EcoMode";
const char* mqtt_Subscribe_write_topic      = "SPA/Write"; 
const char* mqtt_Subscribe_updateTemp_topic = "SPA/UpdateTemp";
const char* mqtt_displayBit30 = "SPA/30";
const char* mqtt_displayBit31 = "SPA/31";
const char* mqtt_displayBit32 = "SPA/32";
const char* mqtt_displayBit33 = "SPA/33";
const char* mqtt_displayBit34 = "SPA/34";
const char* mqtt_displayBit35 = "SPA/35";
const char* mqtt_displayFilter1 = "SPA/Filter1";
const char* mqtt_displayFilter2 = "SPA/Filter2";
const char* mqtt_displayBit38 = "SPA/38";
const char* mqtt_displayBit39 = "SPA/39";
const char* mqtt_displayBit40 = "SPA/40";
const char* mqtt_displayBit41 = "SPA/41";
const char* mqtt_displayBit42 = "SPA/42";
const char* mqtt_displayBit43 = "SPA/43";
const char* mqtt_displayFiltration = "SPA/Filtration";
const char* mqtt_displayBit45 = "SPA/45";
const char* mqtt_displayBit46 = "SPA/46";
const char* mqtt_displayBit49 = "SPA/49";
const char* mqtt_displayBit50 = "SPA/50";
const char* mqtt_displayBit51 = "SPA/51";
const char* mqtt_displayBit52 = "SPA/52";
const char* mqtt_displayBit53 = "SPA/53";
const char* mqtt_displayBit54 = "SPA/54";
const char* mqtt_displayBit55 = "SPA/55";
const char* mqtt_displayBit56 = "SPA/56";
const char* mqtt_displayBit57 = "SPA/57";
const char* mqtt_displayBit58 = "SPA/58";
const char* mqtt_displayBit61 = "SPA/61";
const char* mqtt_displayBit62 = "SPA/62";
const char* mqtt_displayBit63 = "SPA/63";
const char* mqtt_displayBit64 = "SPA/64";
const char* mqtt_displayBit65 = "SPA/65";
const char* mqtt_displayBit66 = "SPA/66";
const char* mqtt_displayBit67 = "SPA/67";
const char* mqtt_displayBit68 = "SPA/68";
const char* mqtt_displayBit69 = "SPA/69";
const char* mqtt_displayBit70 = "SPA/70";
const char* mqtt_displayBit71 = "SPA/71";
//Initialize components
WiFiClient espClient;                                           // Setup WiFi client definition WiFi
PubSubClient client(espClient);                                 // Setup MQTT client
BalboaInterface Balboa(setClockPin, setReadPin, setWritePin);   // Setup Balboa interface 
ESP8266WebServer server(80);

 
/**************************************************************************/
/* Setup                                                                  */
/**************************************************************************/

void setup() {
  
  if (debug) { Serial.begin(115200); Serial.println("Welcome to SPA - Balboa system GS523DZ");}
  setup_wifi();
  client.setCallback(callback);
  Serial.begin(115200);
  Balboa.begin();

  server.on("/", []() {
    server.send(200, "text/plain", "Hi! I am SPA Balboa server.");
  });
  
  ElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  if (debug) { Serial.println("HTTP server started"); }

}


/**************************************************************************/
/* Setup WiFi connection                                                  */
/**************************************************************************/

void setup_wifi() {

    /*  WiFi status return values and meaning 
        WL_IDLE_STATUS      = 0,
        WL_NO_SSID_AVAIL    = 1,
        WL_SCAN_COMPLETED   = 2,
        WL_CONNECTED        = 3,
        WL_CONNECT_FAILED   = 4,
        WL_CONNECTION_LOST  = 5,
        WL_WRONG_PASSWORD   = 6,
        WL_DISCONNECTED     = 7 */
  
    if (debug){ Serial.print("WiFi.status(): "); Serial.println(WiFi.status()); }
    
    int WiFi_retry_counter = 0;
    WiFi.mode(WIFI_STA);
    WiFi.hostname(wifi_hostname);
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
    WiFi.begin(wifi_ssid, wifi_pwd);
    
    // Loop until reconnected or max retry then restart
    while (WiFi.status() != WL_CONNECTED){
        WiFi_retry_counter ++;
        if (WiFi_retry_counter == 30) {ESP.restart();}  
        if (debug){ Serial.print("WiFi.status(): "); Serial.print(WiFi.status()); 
                    Serial.print("   WiFi retry: "); Serial.println(WiFi_retry_counter); } 
        delay(1000);
    }
    
    if (debug){ Serial.print("WiFi connected: ");Serial.println(WiFi.localIP());}

}


/**************************************************************************/
/* Setup MQTT connection                                                   */
/**************************************************************************/

void reconnect() {

    int MQTT_retry_counter = 0;
    
    // Loop until reconnected or max retry then leave
    while (!client.connected() && MQTT_retry_counter < 30) {
       client.setServer(mqtt_server, mqtt_port);
       if (debug){ Serial.print("Connecting to MQTT server, retry: "); Serial.println(MQTT_retry_counter); }
       client.setServer(mqtt_server, mqtt_port);
       client.connect(clientId.c_str(), mqtt_user, mqtt_pwd);
       MQTT_retry_counter ++;
       delay (1000);
    }
    
    if (debug && client.connected()){ Serial.println("MQTT connected"); }

    client.subscribe(mqtt_Subscribe_write_topic);
    client.subscribe(mqtt_Subscribe_updateTemp_topic);
   
    
}


/**************************************************************************/
/* Main loop                                                              */
/**************************************************************************/

void loop() {

	Balboa.loop();
  client.loop();
  server.handleClient();
  
  if (WiFi.status() != WL_CONNECTED){ setup_wifi(); }             // Check WiFi connnection reconnect otherwise 
  if (!client.connected()) { reconnect(); }                       // Check MQTT connnection reconnect otherwise 


 
    if(millis() - ReportTimerPrevMillis  > ReportTimerMillis) {
    
          ReportTimerPrevMillis = millis();
          
          client.publish(mqtt_Display_topic, String(Balboa.LCD_display).c_str());
          client.publish(mqtt_SetTemp_topic, String(Balboa.setTemperature).c_str());
          client.publish(mqtt_WaterTemp_topic, String(Balboa.waterTemperature).c_str());
          client.publish(mqtt_Heater_topic, String(Balboa.displayHeater).c_str());
          client.publish(mqtt_Pump1_topic , String(Balboa.displayPump1).c_str());
          client.publish(mqtt_Pump2_topic , String(Balboa.displayPump2).c_str());
          client.publish(mqtt_Blober_topic , String(Balboa.displayBlober).c_str());
          client.publish(mqtt_Lights_topic , String(Balboa.displayLight).c_str());
          client.publish(mqtt_StandartMode_topic , String(Balboa.displayStandartMode).c_str());
          client.publish(mqtt_EcoMode_topic , String(Balboa.displayEcoMode).c_str());
          client.publish(mqtt_displayBit30 , String(Balboa.displayBit30).c_str());
          client.publish(mqtt_displayBit31 , String(Balboa.displayBit31).c_str());
          client.publish(mqtt_displayBit32 , String(Balboa.displayBit32).c_str());
          client.publish(mqtt_displayBit33 , String(Balboa.displayBit33).c_str());
          client.publish(mqtt_displayBit34 , String(Balboa.displayBit34).c_str());
          client.publish(mqtt_displayBit35 , String(Balboa.displayBit35).c_str());
          client.publish(mqtt_displayFilter1 , String(Balboa.displayFilter1).c_str());
          client.publish(mqtt_displayFilter2 , String(Balboa.displayFilter2).c_str());          
          client.publish(mqtt_displayBit38 , String(Balboa.displayBit38).c_str());
          client.publish(mqtt_displayBit39 , String(Balboa.displayBit39).c_str());
          client.publish(mqtt_displayBit40 , String(Balboa.displayBit40).c_str());
          client.publish(mqtt_displayBit41 , String(Balboa.displayBit41).c_str());
          client.publish(mqtt_displayBit42 , String(Balboa.displayBit42).c_str());
          client.publish(mqtt_displayBit43 , String(Balboa.displayBit43).c_str());
          client.publish(mqtt_displayFiltration , String(Balboa.displayFiltration).c_str());
          client.publish(mqtt_displayBit45 , String(Balboa.displayBit45).c_str());
          client.publish(mqtt_displayBit46 , String(Balboa.displayBit46).c_str());
          client.publish(mqtt_displayBit49 , String(Balboa.displayBit49).c_str());
          client.publish(mqtt_displayBit50 , String(Balboa.displayBit50).c_str());
          client.publish(mqtt_displayBit51 , String(Balboa.displayBit51).c_str());
          client.publish(mqtt_displayBit52 , String(Balboa.displayBit51).c_str());
          client.publish(mqtt_displayBit53 , String(Balboa.displayBit53).c_str());
          client.publish(mqtt_displayBit54 , String(Balboa.displayBit54).c_str());
          client.publish(mqtt_displayBit55 , String(Balboa.displayBit55).c_str());
          client.publish(mqtt_displayBit56 , String(Balboa.displayBit56).c_str());
          client.publish(mqtt_displayBit57 , String(Balboa.displayBit57).c_str());
          client.publish(mqtt_displayBit58 , String(Balboa.displayBit58).c_str());
          client.publish(mqtt_displayBit61 , String(Balboa.displayBit61).c_str());
          client.publish(mqtt_displayBit62 , String(Balboa.displayBit62).c_str());
          client.publish(mqtt_displayBit63 , String(Balboa.displayBit63).c_str());
          client.publish(mqtt_displayBit64 , String(Balboa.displayBit64).c_str());
          client.publish(mqtt_displayBit65 , String(Balboa.displayBit65).c_str());
          client.publish(mqtt_displayBit66 , String(Balboa.displayBit66).c_str());
          client.publish(mqtt_displayBit67 , String(Balboa.displayBit67).c_str());
          client.publish(mqtt_displayBit68 , String(Balboa.displayBit68).c_str());
          client.publish(mqtt_displayBit69 , String(Balboa.displayBit69).c_str());
          client.publish(mqtt_displayBit70 , String(Balboa.displayBit70).c_str());
          client.publish(mqtt_displayBit71 , String(Balboa.displayBit71).c_str());
          
   
    } 
     
}

/**************************************************************************/
/* Subscribe to MQTT topic                                                */
/**************************************************************************/

void callback(char* topic, byte* payload, unsigned int length) {

      char c_payload[length];
      memcpy(c_payload, payload, length);
      c_payload[length] = '\0';
  
      String s_topic = String(topic);
      String s_payload = String(c_payload);
  
    // Handling incoming messages

    Serial.println(s_topic);
    Serial.println(s_payload);

      if ( s_topic == mqtt_Subscribe_write_topic ) {
         
             if (s_payload == "TempUp") {
                  Balboa.writeDisplayData = true; 
                  Balboa.writeTempUp      = true;
             }
             else if (s_payload == "TempDown") {
                  Balboa.writeDisplayData = true;
                  Balboa.writeTempDown    = true;  
             }
             else if (s_payload == "Light") {
                  Balboa.writeDisplayData = true;
                  Balboa.writeLight       = true; 
             }
             else if (s_payload == "Pump1") {
                  Balboa.writeDisplayData = true;
                  Balboa.writePump1       = true;  
             }
             else if (s_payload == "Pump2") {
                  Balboa.writeDisplayData = true;
                  Balboa.writePump2       = true;  
             }
             else if (s_payload == "Blober") {
                  Balboa.writeDisplayData = true;
                  Balboa.writeBlober       = true;  
             }
             else if (s_payload == "Mode") {
                  Balboa.writeDisplayData = true;
                  Balboa.writeMode        = true;  
             }
             else if (s_payload == "Stop") {
                  Balboa.stop();
             }
             else if (s_payload == "Reset") {
                ESP.restart();
             }
             
           
      }

      if ( s_topic == mqtt_Subscribe_updateTemp_topic) {
        
            Balboa.updateTemperature(s_payload.toFloat());
      }

      
}
      

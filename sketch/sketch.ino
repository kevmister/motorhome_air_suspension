#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

const char mqtt_publish_str_table_0[] PROGMEM = "motorhome/engine/air_suspension/outbound/sensors/accelerometer/acceleration_x";
const char mqtt_publish_str_table_1[] PROGMEM = "motorhome/engine/air_suspension/outbound/sensors/accelerometer/acceleration_y";
const char mqtt_publish_str_table_2[] PROGMEM = "motorhome/engine/air_suspension/outbound/sensors/accelerometer/acceleration_z";
const char mqtt_publish_str_table_3[] PROGMEM = "motorhome/engine/air_suspension/outbound/sensors/accelerometer/angle_x";
const char mqtt_publish_str_table_4[] PROGMEM = "motorhome/engine/air_suspension/outbound/sensors/accelerometer/angle_y";
const char mqtt_publish_str_table_5[] PROGMEM = "motorhome/engine/air_suspension/outbound/sensors/accelerometer/angle_z";
const char mqtt_publish_str_table_6[] PROGMEM = "motorhome/engine/air_suspension/outbound/sensors/accelerometer/temperature";
const char mqtt_publish_str_table_7[] PROGMEM = "motorhome/engine/air_suspension/outbound/sensors/airbags/driver_pressure";
const char mqtt_publish_str_table_8[] PROGMEM = "motorhome/engine/air_suspension/outbound/sensors/airbags/passenger_pressure";
const char mqtt_publish_str_table_9[] PROGMEM = "motorhome/engine/air_suspension/outbound/sensors/compressor/pressure";
const char mqtt_publish_str_table_10[] PROGMEM = "motorhome/engine/air_suspension/outbound/control/airbags/driver/fill_open";
const char mqtt_publish_str_table_11[] PROGMEM = "motorhome/engine/air_suspension/outbound/control/airbags/driver/bleed_open";
const char mqtt_publish_str_table_12[] PROGMEM = "motorhome/engine/air_suspension/outbound/control/airbags/passenger/fill_open";
const char mqtt_publish_str_table_13[] PROGMEM = "motorhome/engine/air_suspension/outbound/control/airbags/passenger/bleed_open";
const char mqtt_publish_str_table_14[] PROGMEM = "motorhome/engine/air_suspension/outbound/control/compressor/powered_on";
const char mqtt_publish_str_table_15[] PROGMEM = "motorhome/engine/air_suspension/outbound/configuration/mode";
const char mqtt_publish_str_table_16[] PROGMEM = "motorhome/engine/air_suspension/outbound/configuration/airbags/driver/minimum_pressure";
const char mqtt_publish_str_table_17[] PROGMEM = "motorhome/engine/air_suspension/outbound/configuration/airbags/driver/maximum_pressure";
const char mqtt_publish_str_table_18[] PROGMEM = "motorhome/engine/air_suspension/outbound/configuration/airbags/driver/set_pressure";
const char mqtt_publish_str_table_19[] PROGMEM = "motorhome/engine/air_suspension/outbound/configuration/airbags/passenger/minimum_pressure";
const char mqtt_publish_str_table_20[] PROGMEM = "motorhome/engine/air_suspension/outbound/configuration/airbags/passenger/maximum_pressure";
const char mqtt_publish_str_table_21[] PROGMEM = "motorhome/engine/air_suspension/outbound/configuration/airbags/passenger/set_pressure";
const char mqtt_publish_str_table_22[] PROGMEM = "motorhome/engine/air_suspension/outbound/configuration/compressor/maximum_pressure";
const char mqtt_publish_str_table_23[] PROGMEM = "motorhome/engine/air_suspension/outbound/configuration/compressor/minimum_pressure";
const char mqtt_publish_str_table_24[] PROGMEM = "motorhome/engine/air_suspension/outbound/configuration/compressor/set_pressure";

const char *const mqtt_publish_str_table[] PROGMEM = {
  mqtt_publish_str_table_0,
  mqtt_publish_str_table_1,
  mqtt_publish_str_table_2,
  mqtt_publish_str_table_3,
  mqtt_publish_str_table_4,
  mqtt_publish_str_table_5,
  mqtt_publish_str_table_6,
  mqtt_publish_str_table_7,
  mqtt_publish_str_table_8,
  mqtt_publish_str_table_9,
  mqtt_publish_str_table_10,
  mqtt_publish_str_table_11,
  mqtt_publish_str_table_12,
  mqtt_publish_str_table_13,
  mqtt_publish_str_table_14,
  mqtt_publish_str_table_15,
  mqtt_publish_str_table_16,
  mqtt_publish_str_table_17,
  mqtt_publish_str_table_18,
  mqtt_publish_str_table_19,
  mqtt_publish_str_table_20,
  mqtt_publish_str_table_21,
  mqtt_publish_str_table_22,
  mqtt_publish_str_table_23,
  mqtt_publish_str_table_24
};

const char mqtt_payload_str_table_0[] PROGMEM = "motorhome/engine/air_suspension/inbound/configuration/mode";
const char mqtt_payload_str_table_1[] PROGMEM = "motorhome/engine/air_suspension/inbound/configuration/airbags/driver/minimum_pressure";
const char mqtt_payload_str_table_2[] PROGMEM = "motorhome/engine/air_suspension/inbound/configuration/airbags/driver/maximum_pressure";
const char mqtt_payload_str_table_3[] PROGMEM = "motorhome/engine/air_suspension/inbound/configuration/airbags/driver/set_pressure";
const char mqtt_payload_str_table_4[] PROGMEM = "motorhome/engine/air_suspension/inbound/configuration/airbags/passenger/minimum_pressure";
const char mqtt_payload_str_table_5[] PROGMEM = "motorhome/engine/air_suspension/inbound/configuration/airbags/passenger/maximum_pressure";
const char mqtt_payload_str_table_6[] PROGMEM = "motorhome/engine/air_suspension/inbound/configuration/airbags/passenger/set_pressure";
const char mqtt_payload_str_table_7[] PROGMEM = "motorhome/engine/air_suspension/inbound/configuration/compressor/maximum_pressure";
const char mqtt_payload_str_table_8[] PROGMEM = "motorhome/engine/air_suspension/inbound/configuration/compressor/minimum_pressure";
const char mqtt_payload_str_table_9[] PROGMEM = "motorhome/engine/air_suspension/inbound/configuration/compressor/set_pressure";

const char *const mqtt_payload_str_table[] PROGMEM = {
  mqtt_payload_str_table_0,
  mqtt_payload_str_table_1,
  mqtt_payload_str_table_2,
  mqtt_payload_str_table_4,
  mqtt_payload_str_table_5,
  mqtt_payload_str_table_6,
  mqtt_payload_str_table_7,
  mqtt_payload_str_table_8,
  mqtt_payload_str_table_9
};

byte device_mac[] = { 0x2C,0xF7,0xF1,0x08,0x0B,0x91 };
boolean ethernet_connected = false;
boolean mqtt_connected = false;

void mqtt_callback(char* topic, byte* payload, unsigned int length);
IPAddress mqtt_server(192, 168, 2, 21);

EthernetClient ethernet_client;
PubSubClient client(mqtt_server, 1883, mqtt_callback, ethernet_client);

MPU6050 mpu6050(Wire);
long sensor_publish_timer = 0;
int sensor_publish_delay = 1000;

void mqtt_callback(char* topic, byte* payload_bytes, unsigned int length) {
  char payload[length];
  char topic_str[78];
  byte topic_index = -1;
  
  for(int index = 0; index < length; index++){
     payload[index] = (char*)payload_bytes[index];
  }
  
  for(byte index = 0; index < 8; index++){
    strcpy_P(topic_str, (char *)pgm_read_word(&(mqtt_payload_str_table[index])));
    if(strcmp(topic, topic_str)){
      topic_index = index;
      break;
    }
  }

  switch(topic_index){
    case -1:
      Serial.println(F("No match"));
    case 0:
      Serial.println(0);
      break;
  }
  
  Serial.println(topic);
  Serial.print(payload);
}

void init_ethernet(){}
void handle_ethernet(){
  if(!ethernet_connected){
    Serial.println(F("Ethernet connection attempt.."));
    if(Ethernet.begin(device_mac)){
      Serial.println(F("Ethernet connection success"));
      ethernet_connected = true;
    } else {
      Serial.println(F("Ethernet connection failed"));
    }
  } else {
    if(Ethernet.maintain() % 2 == 1){
      Serial.println(F("Ethernet lost connection"));
      ethernet_connected = false;
    }
  }
}

void init_mqtt(){}
void handle_mqtt(){
  if(!client.connected()){
    Serial.println(F("MQTT connection attempt.."));
    if(client.connect("arduinoClient")){
      Serial.println(F("MQTT connection success"));
      client.subscribe("motorhome/engine/air_suspension/inbound/#");
    } else {
      Serial.println(F("MQTT connection failed"));
    }
  } else {
    client.loop();
  }
}

void init_accelerometer(){
  Wire.begin();
  mpu6050.begin();
  //mpu6050.calcGyroOffsets(true);
}
void handle_accelerometer(){
  mpu6050.update();
}

void init_program(){}
void handle_program(){
  if(millis() - sensor_publish_timer > sensor_publish_delay){
    char value_str[8];
    char publish_str[78];
    strcpy_P(publish_str, (char *)pgm_read_word(&(mqtt_publish_str_table[0])));
    dtostrf(mpu6050.getAccX(), 6, 2, value_str);
    client.publish(publish_str,value_str);
    
    strcpy_P(publish_str, (char *)pgm_read_word(&(mqtt_publish_str_table[1])));
    dtostrf(mpu6050.getAccY(), 6, 2, value_str);
    client.publish(publish_str,value_str);
    
    strcpy_P(publish_str, (char *)pgm_read_word(&(mqtt_publish_str_table[2])));
    dtostrf(mpu6050.getAccZ(), 6, 2, value_str);
    client.publish(publish_str,value_str);
    
    strcpy_P(publish_str, (char *)pgm_read_word(&(mqtt_publish_str_table[3])));
    dtostrf(mpu6050.getAngleX(), 6, 2, value_str);
    client.publish(publish_str,value_str);
    
    strcpy_P(publish_str, (char *)pgm_read_word(&(mqtt_publish_str_table[4])));
    dtostrf(mpu6050.getAngleY(), 6, 2, value_str);
    client.publish(publish_str,value_str);

    /*
    strcpy_P(publish_str, (char *)pgm_read_word(&(mqtt_publish_str_table[5])));
    dtostrf(mpu6050.getAngleZ(), 6, 2, value_str);
    client.publish(publish_str,value_str);
    */
    
    strcpy_P(publish_str, (char *)pgm_read_word(&(mqtt_publish_str_table[6])));
    dtostrf(mpu6050.getTemp(), 6, 2, value_str);
    client.publish(publish_str,value_str);
    
    sensor_publish_timer = millis();
  }
}

void setup() {
  Serial.begin(9600);
  while(!Serial){ ; }
  init_accelerometer();
  init_ethernet();
  init_mqtt();
  init_program();
}

void loop() {
  handle_accelerometer();
  handle_ethernet();
  handle_mqtt();
  handle_program();
}

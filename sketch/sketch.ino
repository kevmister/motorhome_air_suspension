#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

const char *const sensor_accelerometer_publish_str_table[] PROGMEM = {
  "motorhome/engine/air_suspension/outbound/sensors/accelerometer/acceleration_x",
  "motorhome/engine/air_suspension/outbound/sensors/accelerometer/acceleration_y",
  "motorhome/engine/air_suspension/outbound/sensors/accelerometer/acceleration_z",
  "motorhome/engine/air_suspension/outbound/sensors/accelerometer/angle_x",
  "motorhome/engine/air_suspension/outbound/sensors/accelerometer/angle_y",
  "motorhome/engine/air_suspension/outbound/sensors/accelerometer/angle_z",
  "motorhome/engine/air_suspension/outbound/sensors/accelerometer/temperature"
}

typedef float (*sensor_value)();

static sensor_value (*sensor_accelerometer_publish_func_table[]) = {
  &mpu6050.getAccX(),
  &mpu6050.getAccY(),
  &mpu6050.getAccZ(),
  &mpu6050.getAngleX(),
  &mpu6050.getAngleY(),
  &mpu6050.getAngleZ(),
  &mpu6050.getTemp()
}

byte device_mac[] = { 0x2C,0xF7,0xF1,0x08,0x0B,0x91 };
boolean ethernet_connected = false;
boolean mqtt_connected = false;

void mqtt_callback(char* topic, byte* payload, unsigned int length);
IPAddress mqtt_server(192, 168, 2, 21);

const char* mqtt_namespace = "motorhome/engine/air_suspension";

EthernetClient ethernet_client;
PubSubClient client(mqtt_server, 1883, mqtt_callback, ethernet_client);

MPU6050 mpu6050(Wire);
long sensor_publish_timer = 0;
byte sensor_publish_delay = 2000;

void mqtt_callback(char* topic, byte* payload_bytes, unsigned int length) {
  String payload;
  for(int index = 0; index < length; index++){
    payload += (char)payload_bytes[index];
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
    sensor_publish_timer = millis();
    char accelerometer_str[8];
    char publish_str[80];
    for(int index = 0; index < 6; index++ ){
      dtostrf((float)(*sensor_accelerometer_publish_func_table[index])(), 6, 2, accelerometer_str);
      strcpy_P(publish_str, (char *)pgm_read_word(&(mqtt_publish_string_table[index])));
      client.publish(publish_str,accelerometer_str);
    }
    /*
    strcpy_P(publish_str, (char *)pgm_read_word(&(mqtt_publish_string_table[0])));
    dtostrf(mpu6050.getAccX(), 6, 2, accelerometer_str);
    client.publish(publish_str,accelerometer_str);
    strcpy_P(publish_str, (char *)pgm_read_word(&(mqtt_publish_string_table[1])));
    dtostrf(mpu6050.getAccY(), 6, 2, accelerometer_str);
    client.publish(publish_str,accelerometer_str);
    strcpy_P(publish_str, (char *)pgm_read_word(&(mqtt_publish_string_table[2])));
    dtostrf(mpu6050.getAccZ(), 6, 2, accelerometer_str);
    client.publish(publish_str,accelerometer_str);
    strcpy_P(publish_str, (char *)pgm_read_word(&(mqtt_publish_string_table[3])));
    dtostrf(mpu6050.getAngleX(), 6, 2, accelerometer_str);
    client.publish(publish_str,accelerometer_str);
    strcpy_P(publish_str, (char *)pgm_read_word(&(mqtt_publish_string_table[4])));
    dtostrf(mpu6050.getAngleY(), 6, 2, accelerometer_str);
    client.publish(publish_str,accelerometer_str);
    strcpy_P(publish_str, (char *)pgm_read_word(&(mqtt_publish_string_table[5])));
    dtostrf(mpu6050.getAngleZ(), 6, 2, accelerometer_str);
    client.publish(publish_str,accelerometer_str);
    strcpy_P(publish_str, (char *)pgm_read_word(&(mqtt_publish_string_table[6])));
    dtostrf(mpu6050.getTemp(), 6, 2, accelerometer_str);
    client.publish(publish_str,accelerometer_str);
    /*
    Serial.println("=======================================================");
    Serial.print("temp : ");Serial.println(mpu6050.getTemp());
    Serial.print("\taccY : ");Serial.print(mpu6050.getAccY());
    Serial.print("\taccZ : ");Serial.println(mpu6050.getAccZ());
  
    Serial.print("gyroX : ");Serial.print(mpu6050.getGyroX());
    Serial.print("\tgyroY : ");Serial.print(mpu6050.getGyroY());
    Serial.print("\tgyroZ : ");Serial.println(mpu6050.getGyroZ());
  
    Serial.print("accAngleX : ");Serial.print(mpu6050.getAccAngleX());
    Serial.print("\taccAngleY : ");Serial.println(mpu6050.getAccAngleY());
  
    Serial.print("gyroAngleX : ");Serial.print(mpu6050.getGyroAngleX());
    Serial.print("\tgyroAngleY : ");Serial.print(mpu6050.getGyroAngleY());
    Serial.print("\tgyroAngleZ : ");Serial.println(mpu6050.getGyroAngleZ());
    
    Serial.print("angleX : ");Serial.print(mpu6050.getAngleX());
    Serial.print("\tangleY : ");Serial.print(mpu6050.getAngleY());
    Serial.print("\tangleZ : ");Serial.println(mpu6050.getAngleZ());
    Serial.println("=======================================================\n");
    */
    
 
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

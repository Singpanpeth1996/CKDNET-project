
/*P2P NDEF MESSAGE SENDING WITH PN532 TO ANDROID OS
 * NFC PN532 MODULE LIBRARY
 * CHANGE TO SPI MODE
 * 5V VCC
 * SS PIN-10
 * SCK PIN-13
 * MOSI PIN-12
 * MISO PIN-11
 */
#include "SPI.h"
#include "PN532_SPI.h"
#include "snep.h"
#include "NdefMessage.h"

float readMQ4();
void p2pSend(float);

PN532_SPI pn532spi(SPI, 10);
SNEP nfc(pn532spi);
uint8_t ndefBuf[128];
char txtMessage[8];

int gas_sensor = A0; //Sensor pin
float m = -0.318; //Slope
float b = 1.133; //Y-Intercept
float R0 = 11.820; //Sensor Resistance in fresh air from previous code
float sensor_volt; //Define variable for sensor voltage
float RS_gas; //Define variable for sensor resistance
float ratio; //Define variable for ratio
float ppm = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("NFC Peer to Peer Send Message");
}

void loop() {
    ppm = readMQ4();
    p2pSend(ppm);
    delay(2000);
}

float readMQ4(){
    
    float sensorValue = analogRead(gas_sensor);     //Read analog values of sensor
    sensor_volt = sensorValue * (5.0 / 1023.0);     //Convert analog values to voltage
    RS_gas = ((5.0 * 10.0) / sensor_volt) - 10.0;   //Get value of RS in a gas
    ratio = RS_gas / R0;                            // Get ratio RS_gas/RS_air
    Serial.print("Gas/Air : ");
    Serial.println(ratio);
    float ppm_log = (log10(ratio) - b) / m;         //Get ppm value in linear scale according to the the ratio value
    float ppm_temp = pow(10, ppm_log);              //Convert ppm value to log scale
    //double percentage = ppm / 10000;              //Convert to percentage
    Serial.print("ppm : ");
    Serial.print(ppm_temp);                         //Load screen buffer with percentage value
    Serial.println(" % \n");
    //dtostrf(ppm, 5, 5, txtMessage);
    //Serial.println(txtMessage);
    
    return ppm_temp;
}

void p2pSend(float p2p_ppm){
    Serial.println("Send a message to Peer");
    NdefMessage message = NdefMessage();
    message.addTextRecord(String(p2p_ppm));
    int messageSize = message.getEncodedSize();
    message.encode(ndefBuf);                        //ENCODING MESSAGE AND SAVE IN TO NDEF BUFFER
    nfc.write(ndefBuf, messageSize);
}

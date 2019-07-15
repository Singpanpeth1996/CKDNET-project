#include "HX711.h"
#include "SPI.h"
#include "PN532_SPI.h"
#include "snep.h"
#include "NdefMessage.h"

void setHX711();
float readWeight();
void p2pSend(float);

#define DOUT A0  //UNO A0
#define CLK  A1  //UNO A1
HX711 scale;
float calibration_factor = -10470;
float weight = 0;

PN532_SPI pn532spi(SPI, 10);
SNEP nfc(pn532spi);
uint8_t ndefBuf[128];
char txtMessage[8];

void setup() {
    Serial.begin(9600);
    Serial.println("NFC Peer to Peer send weight message.");
    setHX711();
}

void loop() {
    weight = readWeight();
    p2pSend(weight);
    delay(2000);
}

void setHX711(){
    Serial.println("Remove all weight from scale");
    delay(3000);
    Serial.println("After readings begin, place known weight on scale");
    scale.begin(DOUT, CLK);
    scale.set_scale();
    scale.tare();                               //Reset the scale to 0
    long zero_factor = scale.read_average();    //Get a baseline reading
    Serial.print("Zero factor: ");              //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
    Serial.println(zero_factor);
    scale.set_scale(calibration_factor);        //Adjust to this calibration factor
}

float readWeight(){
    Serial.print("Reading: ");
    float temp_weight = scale.get_units() * -0.453592;
    Serial.print(temp_weight);
    Serial.print(" kg");
    
    return temp_weight;
}  

void p2pSend(float p2p_ppm){
    Serial.println("Send a message to Peer");
    NdefMessage message = NdefMessage();
    message.addTextRecord(String(p2p_ppm));
    int messageSize = message.getEncodedSize();
    message.encode(ndefBuf);                        //ENCODING MESSAGE AND SAVE IN TO NDEF BUFFER
    nfc.write(ndefBuf, messageSize);
}

#include "HX711.h"

#define DOUT A0  //UNO A0
#define CLK  A1  //UNO A1

HX711 scale;

float calibration_factor = -10470;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Remove all weight from scale");
  delay(3000);
  Serial.println("After readings begin, place known weight on scale");

  scale.begin(DOUT, CLK);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
}

void loop() {
  // put your main code here, to run repeatedly:
  //scale.set_scale(calibration_factor); //Adjust to this calibration factor
  Serial.print("Reading: ");
  Serial.print(scale.get_units() * -0.453592, 1);
  Serial.print(" kg");
}

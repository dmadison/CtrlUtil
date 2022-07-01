/*
 *  Project     Controller Utilities Library
 *  @author     David Madison
 *  @link       github.com/dmadison/CtrlUtil
 *  @license    MIT - Copyright (c) 2021 David Madison
 *
 *  Example:      InvertAxis
 *  Description:  Takes an input value from a potentiometer connected to the board's
 *                analog to digital converter (ADC) and inverts it.
 */

#include <CtrlUtil.h>

const int InputPin = A0;

const int AnalogMin = 0;     // min value returned from the ADC
const int AnalogMax = 1023;  // max value returned from the ADC

void setup() {
	Serial.begin(115200);

	pinMode(InputPin, INPUT);
}

void loop() {
	int value = analogRead(InputPin);

	// int inverted = invertAxis(value);  // using 0-1023
	int inverted = invertAxis(value, AnalogMin, AnalogMax);  // using custom values (set above)

	Serial.print("Axis: ");
	Serial.print(value);
	Serial.print('\t');
	Serial.print("Inverted: ");
	Serial.print(inverted);
	Serial.println();

	delay(100);
}

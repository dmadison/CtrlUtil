/*
 *  Project     Controller Utilities Library
 *  @author     David Madison
 *  @link       github.com/dmadison/CtrlUtil
 *  @license    MIT - Copyright (c) 2021 David Madison
 *
 *  Example:      Deadzone
 *  Description:  Takes an input value from a potentiometer connected to the board's
 *                analog to digital converter (ADC) and runs it through a deadzone
 *                filter.
 *
 *                If the value is within the deadzone, returns the '0' value based on
 *                the alignment of the deadzone in the range (min, center, or max).
 *
 *                If it's outside of the deadzone, rescales the value to encompass the
 *                entire output range (ignoring the area taken up by the deadzone).
 */

#include <CtrlUtil.h>

const int InputPin = A0;

const float ZoneSize = 0.15;  // 15 %
const auto  ZonePosition = DeadzoneFilter::Middle;  // can also set to either Top or Bottom

DeadzoneFilter dz(ZoneSize, ZonePosition);

void setup() {
	Serial.begin(115200);

	pinMode(InputPin, INPUT);
}

void loop() {
	int rawValue = analogRead(InputPin);
	int dzValue = dz.filter(rawValue);

	Serial.print("Raw: ");
	Serial.print(rawValue);
	Serial.print('\t');
	Serial.print("Deadzone Filtered: ");
	Serial.print(dzValue);
	Serial.println();

	delay(100);
}

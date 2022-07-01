/*
 *  Project     Controller Utilities Library
 *  @author     David Madison
 *  @link       github.com/dmadison/CtrlUtil
 *  @license    MIT - Copyright (c) 2022 David Madison
 *
 *  Example:      RecenterAxis
 *  Description:  Takes an analog input (like a joystick) with an uncentered
 *                midpoint and unequal (asymmetrical) ranges and rescales the
 *                output so that the midpoint is in the dead center of the
 *                desired output range.
 * 
 *                This is designed to be used when you have a joystick that has 
 *                more range on one side than the other. This lets you make use
 *                of the full range of the joystick without doing anything
 *                special to the output.
 * 
 *                If your joystick has drift, try to fix it with a deadzone
 *                instead of using this function.
 *
 *                If you want to add a deadzone, the deadzone should be
 *                applied *after* this fix.
 */

#include <CtrlUtil.h>

const int InputPin = A0;

const int InputMin    = 0;    // lower end of the axis range
const int InputCenter = 400;  // center of the axis range, where the joysick rests
const int InputMax    = 1023; // max end of the axis range

const int OutputMin = 0;     // output value of the function, min
const int OutputMax = 1023;  // output value of the function, max

void setup() {
	Serial.begin(115200);

	pinMode(InputPin, INPUT);
}

void loop() {
	int rawValue = analogRead(InputPin);
	int symValue = recenterAxis(rawValue, InputMin, InputCenter, InputMax, OutputMin, OutputMax);

	Serial.print("Raw: ");
	Serial.print(rawValue);
	Serial.print('\t');
	Serial.print("Symmetric: ");
	Serial.print(symValue);
	Serial.println();

	delay(100);
}

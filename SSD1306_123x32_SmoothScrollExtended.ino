#include <Wire.h>
#include <SSD1306AsciiWire.h>
#include <SSD1306Ascii.h>
#include "InfoText.h"

SSD1306AsciiWire oled;

char reply[21];

// A version of the Adafruit128x32 display with 64 lines of buffer.
static const DevType MEM_TYPE Adafruit128x32x64 = { Adafruit128x32init, sizeof(Adafruit128x32init), 128, 64, 0 };

void setup() {
	Serial.begin(9600);
	oled.begin(&Adafruit128x32x64, 0x3C);
	oled.setFont(lcd5x7);

	Serial.print("Message length:");
	Serial.println(strlen(allMyText));
}

void loop() {
	int scroll = 0;
	int line = 0;
	bool dataAvailable = true;

	while (dataAvailable) {
		oled.clearToEOL();

		dataAvailable = getNextLine();
		oled.println(reply);
		Serial.println(reply);

		if (line >= 4) {
			for (int i = 0; i < 8; ++i) {
				delay(50);
				oled.ssd1306WriteCmd(SSD1306_SETSTARTLINE | scroll % 64);
				++scroll;
			}
		}
		else {
			delay(0);
		}

		if (oled.row() >= 7 && scroll >= 32) {
			oled.home();
		}

		if (scroll >= 64) {
			scroll = 0;
		}
		line++;
	}

	// No more data, clear screen start all over again

	delay(3000);
	oled.clear();
	oled.home();
}

// Get the next 20 characters (or up to space to prevent truncation)
bool getNextLine() {

	// This
	static unsigned long txtPointer = 0;
	static bool lineBreakInProgress = false;

	for (int cnt = 0; cnt < 20; cnt++)
		reply[cnt] = ' ';

	if (lineBreakInProgress){
		lineBreakInProgress = false;
		return true;
	}

	for (uint8_t cnt = 0; cnt < 20; cnt++) {

		if (txtPointer < strlen(allMyText)) {
			char myChar = pgm_read_byte_near(allMyText + txtPointer);

			// increment pointer before we return
			txtPointer++;

			// Deal with special characters (here, just new lines)
			if (myChar == '%'){
				//Set the flag that we have a line-break situation
				lineBreakInProgress = true;
				return true;
			}

			reply[cnt] = myChar;
		}
		else {
			txtPointer = 0;
			Serial.println("\nEnd of data!");
			return false;
		}
	}

	// Before returning the 20 char message, check that we have stopped on a space boundary or
	// that the next character is a space (which can be ignored on the next line)
	if (reply[19] == ' ') {
		Serial.println("Final char is a space");
		return true;
	}

	if (pgm_read_byte_near(allMyText + txtPointer) == ' ') {
		Serial.println("Next char is a space");
		txtPointer++;
		return true;
	}

	// We need to back track to the last space
	for (uint8_t cnt = 18; cnt > 0; cnt--) {
		if (reply[cnt] == ' ') {

			Serial.print("Space found at char:");
			Serial.println(cnt);
			// Space fill rest of line and decrement pointer for next line
			for (uint8_t cnt2 = cnt; cnt2 < 20; cnt2++) {
				reply[cnt2] = ' ';
				txtPointer--;
			}

			// If the next character in the string (yet to be printed) is a space
			// increment the pointer so we don't start a line with a space
			if (pgm_read_byte_near(allMyText + txtPointer) == ' ') {
				Serial.println("Next char is a space");
				txtPointer++;
			}

			break;
		}
	}
	return true;
}

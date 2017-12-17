#include "Arduino.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

String messages[] = {"Hello Arduinites!","And another heading","Even more to read!!"," Last heading here!"};

void setup() {
	// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

	// Clear the buffer.
	display.clearDisplay();
}

// Simple display loop of scrolling text, a ball and a fault condition
void loop() {
	//testBiScrolltext();

	//testScrollBall();

	testVertScrolltext();

	//testFlashFault();

	delay(2000);
}

// Part of the original Adafruit demo, routine to display a filled ball
// I'm using this in a scroll demo
void testdrawcircle(void) {
	for (int16_t i = 0; i < display.height(); i += 2) {
		display.drawCircle(display.width() / 2, display.height() / 2, i, WHITE);
		display.display();
		delay(1);
	}
}

// Scrolling different parts of the screen (not simultaneously, unfortunately)
void testBiScrolltext(void) {

	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(10, 0);
	display.clearDisplay();
	display.print("Hello Arduinites!");
	display.display();
	delay(1000);

	display.setCursor(10, 0x0E);
	display.setTextSize(2);
	display.print("-SSD1306-");
	display.display();
	delay(1);

	for (int i = 0; i < 4; i++) {
		display.startscrollleft(0x00, 0x08);
		delay(3000);
		display.stopscroll();
		display.startscrollright(0x09, 0x0E);
		delay(4250);
		display.stopscroll();

		display.invertDisplay(i % 2 == 0);
	}
}

// Scrolling vertically - experimental, you have been warned! Ralph Bacon.
void testVertScrolltext(void) {
	static byte msgNum = 0;

	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(10, 0);
	display.clearDisplay();

	// display small header
	display.print(messages[msgNum]);
	if (msgNum++ > 2) msgNum = 0;
	display.display();
	delay(1000);

	// Move the cursor dow a few rows and display a bigger message
	display.setCursor(10, 0x0E);
	display.setTextSize(2);
	display.print("-SSD1306-");
	display.display();
	delay(1);

	// Vertical scrolling only from 0x4 to 0x1F, upwards
	display.startvertscroll(0x04, 0x1F, true);
	delay(2000);

	// Vertical scrolling only from 0x4 to 0x1F, downwards
	display.startvertscroll(0x04, 0x1F, false);
	delay(4000);
}

// Display an animated ball rolling around
void testScrollBall () {

	display.clearDisplay();

	// draw a white circle, 10 pixel radius
	display.fillCircle(display.width() / 2, display.height() / 2, 10, WHITE);
	display.display();
	delay(2000);
	display.clearDisplay();

	for (int i = 0; i < 4; i++) {
		display.startscrollleft(0x00, 0x0F);
		delay(1000);
		display.stopscroll();
		display.startscrollright(0x00, 0x0F);
		delay(1250);
		display.stopscroll();

		// Let's invert the display
		display.invertDisplay(i % 2 == 0);
	}
}

// A potential Real World example to call attention to the display
void testFlashFault() {

	display.setTextColor(WHITE);
	display.setTextSize(3);
	display.setCursor(20, 6);
	display.clearDisplay();
	display.print("FAULT");
	display.display();
	delay(1000);

	// We don't turn the display on and off, we just invert it
	for (int i = 0; i < 10; i++) {
		display.invertDisplay(i % 2 == 0);
		delay(250);
	}
}

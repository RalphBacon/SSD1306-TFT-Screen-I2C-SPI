/*
 * InfoText.h
 *
 *  Created on: 14 Dec 2017
 *      Author: Ralph
 */

#ifndef INFOTEXT_H_
#define INFOTEXT_H_

/*
	This is a sample selection of text stored in program memory (not SRAM) to save run time space
*/
const char allMyText[] PROGMEM = {"%%%%Hello Arduinites! If you want to display some instructions or errors "
"to your users (or yourself) then this might be one way to do that. "
"Each line on the screen can contain about 20 characters depending on your font so this will split "
"lines into 20 char chunks.%"
"Warning: critical temperature in the greenhouse: -4C Please start the heating unit to save the "
"delicate flowers.%"
"You can even include line breaks by including special characters. In this "
"example a percent sign creates a blank line...%just like this!"};

#endif /* INFOTEXT_H_ */

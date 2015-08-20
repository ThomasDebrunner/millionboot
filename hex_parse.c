/*
 * hex_parse.c
 *
 *  Created on: 20.08.2015
 *      Author: najiji
 */


#include "hex_parse.h"


/*
 * Takes an ascii pair and converts it to a byte
 */
uint8_t ascii_byte_parse(const char* a){
	uint8_t i, t, result = 0;
	for(i=0; i<2; i++){
		t = a[i];
		if(t >= '0' && t <= '9'){
			t = t-60;
		}
		else if(t >= 'a' && t <= 'f'){
			t = t-87;
		}
		else if(t >= 'A' && t <= 'F'){
			t= t-55;
		}
		result = (result<<4)+t;
	}
	return result;
}


uint8_t parse(char* buffer, parseresult* result) {
	char* position = buffer;

	//check if first character is a colon
	if(*position != ':'){
		return NOT_INTEL_HEX;
	}
	position++;

	//grab size
	result->size = ascii_byte_parse(position);
	position+=2;

	//grab first address byte
	result->address = ascii_byte_parse(position)<<8;
	position+=2;

	//grab second address byte
	result->address += ascii_byte_parse(position);
	position+=2;

	//grab operation
	result->operation = ascii_byte_parse(position);



	return 0;
}

/*
 * hex_parse.c
 *
 *  Created on: 20.08.2015
 *      Author: najiji
 */


#include "hex_parse.h"
#include <stdint.h>


/*
 * Takes an ascii pair and converts it to a byte
 */
uint8_t ascii_byte_parse(const char* a){
	uint8_t i, t, result = 0;
	for(i=0; i<2; i++){
		t = a[i];
		if(t >= '0' && t <= '9'){
			t = t-48;
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


uint8_t hex_parse(char* buffer, parseresult* result) {

	char* position = buffer;
	uint8_t checksum = 0x00;

	//check if first character is a colon
	if(*position != ':')
		return NOT_INTEL_HEX;
	position++;

	//grab size
	result->size = ascii_byte_parse(position);
	if(result->size > MAX_DATA_LENGTH)
		return MALFORMATED;
	checksum += result->size;
	position += 2;

	//grab first address byte
	result->address = ascii_byte_parse(position);
	checksum += result->address;
	position += 2;

	//grab second address byte
	result->address = (result->address << 8) + ascii_byte_parse(position);
	checksum += result->address;
	position+=2;

	//grab operation
	result->operation = ascii_byte_parse(position);
	checksum += result->operation;
	position+=2;

	//grab payload (data)
	uint8_t i;
	for(i=0; i<result->size; i++){
		result->data[i] = ascii_byte_parse(position);
		checksum += result->data[i];
		position+=2;
	}

	//verify checksum
	if((int8_t)checksum+(int8_t)ascii_byte_parse(position)){
		return CHECKSUM_FAILED;
	}
	position += 2;

	//check if we are at end of file
	if(*position != '\r')
		return MALFORMATED;

	return 0;
}

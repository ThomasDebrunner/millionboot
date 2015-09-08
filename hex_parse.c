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


uint8_t hex_parse(char* buffer, Parseresult* result) {

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



void page_init(Page* page){
	//set all bytes to 0xFF;
	for(page->position = 0; page->position < SPM_PAGESIZE; page->position++){
		page->data[page->position] = 0xFF;
	}
	//set position back to beginning of page
	page->position = 0;
	//set address to max (inidicates no address)
	page->address = 0xFFFF;
	page->ready = 0;
}


uint8_t page_append(Parseresult* parsed_data, Page* page){
	if(parsed_data->operation == 0){
		uint16_t page_address = parsed_data->address - (parsed_data->address % SPM_PAGESIZE);
		if(page->address == 0xFFFF){ // no address specified yet
			page->address = page_address;
		}
		else if(page->address != page_address)
			return WRONG_PAGE;

		if(SPM_PAGESIZE-page->position < parsed_data->size)
			return PAGE_FULL;

		uint8_t i;
		for(i=0; i<parsed_data->size; i++){
			page->data[page->position] = parsed_data->data[i];
			page->position++;
		}

		if(page->position == SPM_PAGESIZE)
			page->ready = 1;
	}
	else if(parsed_data->operation == 1){
		page->ready = 1;
	}

	else{
		return ILLEGAL_OPERATION;
	}

	return 0;
}

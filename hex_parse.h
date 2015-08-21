/*
 * hex_parse.h
 *
 *  Created on: 20.08.2015
 *      Author: najiji
 */

#ifndef HEX_PARSE_H_
#define HEX_PARSE_H_

#include <avr/io.h>

#define NOT_INTEL_HEX 101
#define MALFORMATED 102
#define CHECKSUM_FAILED 103

/*
 * CONFIGURATION
 */
#define MAX_DATA_LENGTH 16 //the maximum length (in bytes) a line of the hex file may contain


typedef struct {
	uint8_t size;
	uint16_t address;
	uint8_t operation;
	uint8_t data[MAX_DATA_LENGTH];
} parseresult;


/*
 * Parses the given string and stores result in provided parseresult
 *
 * returns zero if parsing was succesful, errorcode otherwise
 */
uint8_t hex_parse(char* buffer, parseresult* result);



#endif /* HEX_PARSE_H_ */

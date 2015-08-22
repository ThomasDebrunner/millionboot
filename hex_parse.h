/*
 * hex_parse.h
 *
 *  Created on: 20.08.2015
 *      Author: najiji
 */

#ifndef HEX_PARSE_H_
#define HEX_PARSE_H_

#include <stdint.h>

#define SPM_PAGESIZE 128


/**
 * Error codes
 */
#define NOT_INTEL_HEX 101
#define MALFORMATED 102
#define CHECKSUM_FAILED 103

#define PAGE_FULL 201
#define WRONG_PAGE 202
#define ILLEGAL_OPERATION 203


/*
 * CONFIGURATION
 */
#define MAX_DATA_LENGTH 16 //the maximum length (in bytes) a line of the hex file may contain


/**
 * encapsulates the result of a single intel hex parse step
 */
typedef struct {
	uint8_t size;
	uint16_t address;
	uint8_t operation;
	uint8_t data[MAX_DATA_LENGTH];
} Parseresult;


/**
 * encapsulates a page to be flashed
 */
typedef struct {
	uint16_t address;
	uint8_t data[SPM_PAGESIZE];
	uint8_t position;
	uint8_t ready;
} Page;

/*
 * Parses the given string and stores result in provided parseresult
 *
 * returns zero if parsing was succesful, errorcode otherwise
 */
uint8_t hex_parse(char* buffer, Parseresult* result);


/*
 * Resets page to empty state
 */
void page_init(Page* page);

/**
 * Tries to append a parseresult to a page. otherwise returns error
 */
uint8_t page_append(Parseresult* parsed_data, Page* page);


#endif /* HEX_PARSE_H_ */

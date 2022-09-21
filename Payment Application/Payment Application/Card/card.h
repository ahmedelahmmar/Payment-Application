#ifndef _CARD_H
#define _CARD_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../Library/StandardTypes.h"

#define MIN_NAME_LEN		20
#define MAX_NAME_LEN		24

#define DATE_FORMAT_LEN		5

#define MIN_PAN_LEN			16
#define MAX_PAN_LEN			19

typedef struct
{
	uint8_t cardHolderName[MAX_NAME_LEN + 1];
	uint8_t cardExpirationDate[DATE_FORMAT_LEN + 1];
	uint8_t PAN[MAX_PAN_LEN + 100];

} ST_cardData_t;

typedef enum
{
	WRONG_NAME,
	WRONG_PAN,
	WRONG_EXP_DATE,
	CARD_OK

} EN_cardError_t;

EN_cardError_t getCardHolderName(ST_cardData_t*);
EN_cardError_t getCardExpiryDate(ST_cardData_t*);
EN_cardError_t getCardPAN(ST_cardData_t*);

#endif
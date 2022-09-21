#ifndef _TERMINAL_H
#define _TERMINAL_H

#include <time.h>
#include <string.h>

#include "../Library/StandardTypes.h"
#include "../Card/card.h"

#define TERM_DATE_LEN		10

typedef struct
{
	uint8_t transactionDate[TERM_DATE_LEN + 1];
	f32_t transAmount;
	f32_t maxTransAmount;

} ST_terminalData_t;

typedef enum
{
	WRONG_DATE,
	EXPIRED_CARD,
	EXCEED_MAX_AMOUNT,
	INVALID_CARD,
	INVALID_AMOUNT,
	INVALID_MAX_AMOUNT,
	TERMINAL_OK

} EN_terminalError_t;

EN_terminalError_t getTransactionDate(ST_terminalData_t*);
EN_terminalError_t isCardExpired(ST_cardData_t*, ST_terminalData_t*);
//EN_terminalError_t isValidCardPAN(ST_cardData_t*);
EN_terminalError_t getTransactionAmount(ST_terminalData_t*);
EN_terminalError_t setMaxAmount(ST_terminalData_t*);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t*);

#endif
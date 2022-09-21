#pragma warning (disable:4996)
#include "terminal.h"

EN_terminalError_t getTransactionDate(ST_terminalData_t* terminal)
{
	time_t t = time(NULL);
	struct tm time = *localtime(&t);

	uint8_t day = time.tm_mday;
	uint8_t month = time.tm_mon + 1;
	uint16_t year = time.tm_year + 1900;

	uint8_t temp[TERM_DATE_LEN + 1] = "DD/MM/YYYY";
//									  "0123456789"
	
	temp[0] = (day / 10)				+ '0';
	temp[1] = (day % 10)				+ '0';
	temp[2] = '/';
	temp[3] = (month / 10)				+ '0';
	temp[4] = (month % 10)				+ '0';
	temp[5] = '/';
	temp[6] = (year / 1000)				+ '0';
	temp[7] = ((year % 1000) / 100)		+ '0';
	temp[8] = ((year % 100) / 10)		+ '0';
	temp[9] = (year % 10)				+ '0';

	strcpy(terminal->transactionDate, temp);
	return TERMINAL_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t* card, ST_terminalData_t* terminal)
{
	uint8_t card_m = ((card->cardExpirationDate[0] - '0') * 10) + (card->cardExpirationDate[1] - '0');
	uint8_t card_y = ((card->cardExpirationDate[3] - '0') * 10) + (card->cardExpirationDate[4] - '0');

	uint8_t tran_m = ((terminal->transactionDate[3] - '0') * 10) + (terminal->transactionDate[4] - '0');
	uint8_t tran_y = ((terminal->transactionDate[8] - '0') * 10) + (terminal->transactionDate[9] - '0');

	if ((tran_y < card_y) || ((tran_y == card_y) && (tran_m < card_m))) return TERMINAL_OK;
	else return EXPIRED_CARD;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* terminal)
{
	f32_t amount = -1;
	printf("Please enter the transaction's amount: ");
	scanf("%f", &amount);

	if (amount <= 0) return INVALID_AMOUNT;

	terminal->transAmount = amount;

	return TERMINAL_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* terminal)
{
	f32_t max = -1;
	printf("Please enter the max amount for a transaction: ");
	scanf("%f", &max);

	if (max <= 0) return INVALID_MAX_AMOUNT;

	terminal->maxTransAmount = max;

	return TERMINAL_OK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* terminal)
{
	if (terminal->transAmount > terminal->maxTransAmount) return EXCEED_MAX_AMOUNT;
	else return TERMINAL_OK;
}
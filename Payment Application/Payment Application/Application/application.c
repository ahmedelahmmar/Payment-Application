#include "application.h"
#include "../Terminal/terminal.h"
void APP_start(void)
{
// ********************  VARIABLES  ***********************

	ST_cardData_t card;
	EN_cardError_t cardStatus;

	ST_terminalData_t terminal;
	EN_terminalError_t terminalStatus;

	ST_transaction_t transcation;
	EN_transState_t transacitonStatus;

//	******************* CARD MODULE ***********************

	cardStatus = getCardHolderName(&card);
	while (cardStatus == WRONG_NAME)
	{
		cardStatus = getCardHolderName(&card);
	}
	
	cardStatus = getCardExpiryDate(&card);
	while (cardStatus == WRONG_EXP_DATE)
	{
		cardStatus = getCardExpiryDate(&card);
	}

	cardStatus = getCardPAN(&card);
	while (cardStatus == WRONG_PAN)
	{
		cardStatus = getCardPAN(&card);
	}

//	******************* TERMINAL MODULE *******************

	terminalStatus = getTransactionDate(&terminal);

	terminalStatus = isCardExpired(&card, &terminal);
	if (terminalStatus == EXPIRED_CARD)
	{
		printf("\nStatus: CARD EXPIRED\n");
		return;
	}

	terminalStatus = setMaxAmount(&terminal);
	while (terminalStatus == INVALID_MAX_AMOUNT)
	{
		terminalStatus = setMaxAmount(&terminal);
	}

	terminalStatus = getTransactionAmount(&terminal);
	while (terminalStatus == INVALID_AMOUNT)
	{
		terminalStatus = getTransactionAmount(&terminal);
	}

	terminalStatus = isBelowMaxAmount(&terminal);
	if (terminalStatus == EXCEED_MAX_AMOUNT) 
		transcation.transState = DECLINED_EXCEEDED_LIMIT;
	else 
		transcation.transState = APPROVED;

//	******************* SERVER MODULE ********************
	
	transcation.cardData = card;
	transcation.termData = terminal;

	transacitonStatus = recieveTransactionData(&transcation);

	if (transacitonStatus == DECLINED_STOLEN_CARD)
	{
		printf("\nStatus: INVALID CARD\n");
		return;
	}
	else if (transacitonStatus == DECLINED_INSUFFICIENT_FUND)
	{
		printf("\nStatus: INSUFFICIENT FUNDS\n");
		return;
	}
	else if (transacitonStatus == DECLINED_EXCEEDED_LIMIT)
	{
		printf("\nStatus: REQUEST LIMIT EXCEEDED\n");
		return;
	}
	else if (transacitonStatus == APPROVED)
	{
		printf("\nStatus: Transaction Approved.\n");
		return;
	}
};
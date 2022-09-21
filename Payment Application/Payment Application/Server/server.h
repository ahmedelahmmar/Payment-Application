#ifndef _SERVER_H
#define _SERVER_H

#include <stdlib.h>
#include <time.h>

#include "../Terminal/terminal.h"

#define ARRAY_SIZE(array)	((sizeof(array))/(sizeof(array[0])))
#define MAX_ACCOUNTS_NUM	255

typedef enum
{
	DECLINED_INSUFFICIENT_FUND,
	DECLINED_EXCEEDED_LIMIT,
	DECLINED_STOLEN_CARD,
	INTERNAL_SERVER_ERROR,
	APPROVED

} EN_transState_t;

typedef enum
{
	SAVING_FAILED,
	TRANSACTION_NOT_FOUND,
	ACCOUNT_NOT_FOUND,
	LOW_BALANCE,
	SERVER_OK

} EN_serverError_t;

typedef struct
{
	f32_t balance;
	uint8_t PAN[20];

} ST_accountsDB_t;

typedef struct
{
	ST_cardData_t cardData;
	ST_terminalData_t termData;
	EN_transState_t transState;

} ST_transaction_t;

void loadAccounts(void);
void updateAccounts(void);

EN_transState_t recieveTransactionData(ST_transaction_t*);
EN_serverError_t isValidAccount(ST_cardData_t*);
EN_serverError_t isAmountAvailable(ST_terminalData_t*);
EN_serverError_t saveTransaction(ST_transaction_t*);


#endif
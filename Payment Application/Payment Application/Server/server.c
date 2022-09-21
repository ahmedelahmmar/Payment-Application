#pragma warning (disable:4996)
#include "server.h"

ST_accountsDB_t accounts[MAX_ACCOUNTS_NUM];

uint8_t card_index = -1;
f32_t card_prevBalance = -1;

void loadAccounts(void)
{
	FILE* fh_accounts = fopen("../Payment Application/Documents/accounts.txt", "r");
	uint8_t loc_pan[20];
	f32_t loc_balance;

	if (fh_accounts != NULL)
	{
		uint8_t loc_i = 0;
		while (fscanf(fh_accounts, "%f %s", &loc_balance, loc_pan) && !feof(fh_accounts))
		{
			accounts[loc_i].balance = loc_balance;

			for (uint8_t loc_j = 0; loc_j < 20; loc_j++)
			{
				accounts[loc_i].PAN[loc_j] = loc_pan[loc_j];
			}

			loc_i++;
		}
		fclose(fh_accounts);
	}
	else fprintf(stderr, "loadAccounts(): 'accounts.txt' Failed to load file.\n");
}

void updateAccounts(void)
{
	FILE* fh_accounts = fopen("../Payment Application/Documents/accounts.txt", "w");

	if (fh_accounts != NULL)
	{
		for (uint8_t loc_i = 0; loc_i < MAX_ACCOUNTS_NUM; loc_i++)
		{
			if (accounts[loc_i].PAN[0] == '\0') break;

			fprintf(fh_accounts, "%f %s\n", accounts[loc_i].balance, accounts[loc_i].PAN);
		}
		fclose(fh_accounts);
	}
	else fprintf(stderr, "updateAccounts(): 'accounts.txt' Failed to load file.\n");
}

EN_transState_t recieveTransactionData(ST_transaction_t* transaction)
{
	loadAccounts();

	EN_transState_t loc_trans = transaction->transState;

	if (isValidAccount(&(transaction->cardData)) == ACCOUNT_NOT_FOUND)
	{
		return DECLINED_STOLEN_CARD;
	}

	card_prevBalance = accounts[card_index].balance;

	if (loc_trans == DECLINED_EXCEEDED_LIMIT)
	{
		transaction->transState = DECLINED_EXCEEDED_LIMIT;

		if (saveTransaction(transaction) == SAVING_FAILED) return INTERNAL_SERVER_ERROR;

		return DECLINED_EXCEEDED_LIMIT;;
	}


	if (isAmountAvailable(&(transaction->termData)) == LOW_BALANCE)
	{
		transaction->transState = DECLINED_INSUFFICIENT_FUND;

		if (saveTransaction(transaction) == SAVING_FAILED) return INTERNAL_SERVER_ERROR;

		return DECLINED_INSUFFICIENT_FUND;
	}

	if (transaction->transState != DECLINED_INSUFFICIENT_FUND && loc_trans != DECLINED_EXCEEDED_LIMIT)
	{
		accounts[card_index].balance -= transaction->termData.transAmount;
		transaction->transState = APPROVED;

		if (saveTransaction(transaction) == SAVING_FAILED) return INTERNAL_SERVER_ERROR;

		return APPROVED;
	}
}

EN_serverError_t isValidAccount(ST_cardData_t* card)
{
	bool equal = False;
	uint8_t* card_pan = card->PAN;

	for (uint8_t loc_i = 0; loc_i < MAX_ACCOUNTS_NUM; loc_i++)
	{
		if (accounts[loc_i].PAN[0] != '\0')
		{
			for (uint8_t loc_j = 0; loc_j < 20; loc_j++)
			{
				equal = True;
				if (card_pan[loc_j] != accounts[loc_i].PAN[loc_j])
				{
					equal = False;
					break;
				}
			}

			if (equal)
			{
				card_index = loc_i;
				return SERVER_OK;
				break;
			}
		}
		else break;
	}
	if (!equal) return DECLINED_STOLEN_CARD;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* terminal)
{
	if (terminal->transAmount <= accounts[card_index].balance) return SERVER_OK;
	else return LOW_BALANCE;
}

EN_serverError_t saveTransaction(ST_transaction_t* transaction)
{
	FILE* fh_transactions = fopen("../Payment Application/Documents/transactions.txt", "a");
	time_t t = time(NULL);

	if (fh_transactions != NULL)
	{
		fprintf(fh_transactions, "******************* TRANSACTION *******************\n");
		fprintf(fh_transactions, "Transaction Date  : %s", ctime(&t));
		fprintf(fh_transactions, "Card Holder Name  : %s\n", transaction->cardData.cardHolderName);
		fprintf(fh_transactions, "PAN               : %s\n", transaction->cardData.PAN);
		fprintf(fh_transactions, "Expiry Date       : %s\n", transaction->cardData.cardExpirationDate);
		fprintf(fh_transactions, "Request Limit     : $%0.2f\n", transaction->termData.maxTransAmount);
		fprintf(fh_transactions, "Amount Requested  : $%0.2f\n", transaction->termData.transAmount);

		switch (transaction->transState)
		{
		case APPROVED:
			fprintf(fh_transactions, "Transaction State : Transaction Approved.\n\n");
			fprintf(fh_transactions, "Previous Balance  : $%0.2f\n", card_prevBalance);
			fprintf(fh_transactions, "New Balance       : $%0.2f\n", accounts[card_index].balance);
			break;

		case DECLINED_INSUFFICIENT_FUND:
			fprintf(fh_transactions, "Transaction State : Transaction Declined.\n");
			fprintf(fh_transactions, "Explanation       : Insufficient Funds.\n\n");
			fprintf(fh_transactions, "Balance           : $%0.2f\n", accounts[card_index].balance);
			break;

		case DECLINED_EXCEEDED_LIMIT:
			fprintf(fh_transactions, "Transaction State : Transaction Declined.\n");
			fprintf(fh_transactions, "Explanation       : Request limit Exceeded.\n");
			break;

		default:
			fprintf(fh_transactions, "Transaction State : Transaction Declined.\n");
			fprintf(fh_transactions, "Explanation       : Internal Server Error.\n");
			break;
		}

		fprintf(fh_transactions, "***************************************************\n\n");
		fclose(fh_transactions);

		updateAccounts();
		return SERVER_OK;
	}
	else
	{
		printf("saveTransaction(): 'transactions.txt' Failed to open file.");
		return SAVING_FAILED;
	}

}

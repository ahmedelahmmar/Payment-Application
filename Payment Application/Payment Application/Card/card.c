#pragma warning (disable:4996)
#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t* card)
{
	uint8_t buffer[MAX_NAME_LEN + 5];

	printf("Please enter the card holder's name: ");
	gets(buffer);

	if (strlen(buffer) < MIN_NAME_LEN || strlen(buffer) > MAX_NAME_LEN || strlen(buffer) == 0)
	{
		return WRONG_NAME;
	}

	for (uint8_t loc_i = 0; loc_i < strlen(buffer); loc_i++)
	{
		if (!(isalpha(buffer[loc_i])) && (buffer[loc_i] != ' '))
		{
			return WRONG_NAME;
		}
	}

	strcpy(card->cardHolderName, buffer);
	return CARD_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* card)
{
	uint8_t buffer[DATE_FORMAT_LEN + 5];

	printf("Please enter the card's expiry date in the format: ");
	gets(buffer);


	if ((buffer[2] != '/') || (strlen(buffer) != DATE_FORMAT_LEN))	// Checks if the XX/XX format is followed.
	{
		return WRONG_EXP_DATE;
	}

	for (uint8_t loc_i = 0; loc_i < strlen(buffer); loc_i++)	// Checks that MM/YY format is followed.
	{
		if (!(isdigit(buffer[loc_i])) && (buffer[loc_i] != '/'))
		{
			return WRONG_EXP_DATE;
		}
	}

	uint8_t loc_month = (buffer[0] - '0') * 10 + (buffer[1] - '0');
	uint8_t loc_year = (buffer[3] - '0') * 10 + (buffer[4] - '0');

	if ((loc_month <= 0 || loc_month > 12) || loc_year <= 0) // Checks the date validity.
	{
		return WRONG_EXP_DATE;
	}

	strcpy(card->cardExpirationDate, buffer);
	return CARD_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t* card)
{
	uint8_t buffer[MAX_PAN_LEN + 5];

	printf("Please enter the card's PAN [19 characters]: ");
	gets(buffer);

	if (strlen(buffer) > MAX_PAN_LEN || strlen(buffer) < MIN_PAN_LEN) // Checks the length restriction.
	{
		return WRONG_PAN;
	}

	for (uint8_t loc_i = 0; loc_i < strlen(buffer); loc_i++) // Validates the pan.
	{
		if (!isdigit(buffer[loc_i]))
		{
			return WRONG_PAN;
		}
	}
	strcpy(card->PAN, buffer);
	return CARD_OK;
}
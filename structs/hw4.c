/*
 * hw4.c
 *
 *  Created on: Apr 27, 2015
 *      Author: Shannon Murphy
 */

#include <stdio.h>
#include <string.h>

#define MAX_CUSTOMERS 20
#define MAX_ITEMS 10
#define MAX_NAME 29

struct item_tag {
	char name[MAX_NAME];
	int quantity;
	double price;
	double itemValue;
};

typedef struct item_tag Item;

struct customer_tag {
	char name[MAX_NAME];
	Item items[MAX_ITEMS];
	int size;
	double orderTotal;
	int itemCount;
	Item newItems[MAX_ITEMS];
};

typedef struct customer_tag Customer;

/* Prints to the hw4money.txt file. */
void outputMoney(int numOfCustomers, Customer sortedCustomers[MAX_CUSTOMERS]) {
	FILE *outfile = fopen("hw4money.txt", "w");
	int i, j;
    for(i = 0; i < numOfCustomers; i++) {
		fprintf(outfile, "%s, Total Order = $%.2lf\n", sortedCustomers[i].name,
				sortedCustomers[i].orderTotal);
    	for(j = 0; j < sortedCustomers[i].itemCount; j++) {
    		fprintf(outfile, "%s %d $%.2lf, Item Value = $%.2lf\n",
    				sortedCustomers[i].newItems[j].name, sortedCustomers[i].newItems[j].quantity,
    				sortedCustomers[i].newItems[j].price, sortedCustomers[i].newItems[j].itemValue);
    	}
    	fprintf(outfile, "\r\n");
    }
}

/* Sorts the items by their total values. */
void sortItems(int numOfCustomers, Customer sortedCustomers[MAX_CUSTOMERS]) {
	int i, j, k;
	for(j = 0; j < numOfCustomers; j++) {
		for(k = 0; k < sortedCustomers[k].itemCount; k++) {
			for(i = 0; i < sortedCustomers[j].itemCount; i++) {
				Item temp;
				if (sortedCustomers[j].newItems[i].itemValue < sortedCustomers[j].newItems[i+1].itemValue) {
					temp = sortedCustomers[j].newItems[i];
					sortedCustomers[j].newItems[i] = sortedCustomers[j].newItems[i+1];
					sortedCustomers[j].newItems[i+1] = temp;
				}
			}
		}
	}
}

/* Makes a new array of items for each customer that only contains their specific items. */
void getIndividualItems(int numOfCustomers, Customer individualCustomers[MAX_CUSTOMERS]) {
	int i, j;
    for (i = 0; i < numOfCustomers; i++) {
        Item newItems[individualCustomers[i].itemCount];
        for(j = 0; j < individualCustomers[i].itemCount; j++) {
        	newItems[j] = individualCustomers[i].items[j];
        	individualCustomers[i].newItems[j] = newItems[j];
        }
    }
}

/* Sorts the Customers by their order totals. */
void sortCustomersTotal(Customer sortedCustomers[MAX_CUSTOMERS]) {
    int i, j;
    for(j = 0; j < MAX_CUSTOMERS; j++) {
		for(i = 0; i < MAX_CUSTOMERS; i++) {
			Customer temp;
			if (sortedCustomers[i].orderTotal < sortedCustomers[i+1].orderTotal) {
				temp = sortedCustomers[i];
				sortedCustomers[i] = sortedCustomers[i+1];
				sortedCustomers[i+1] = temp;
			}
		}
    }
}

/* Creates a new array of Customers with no duplicate Customers. */
int getIndividualCustomers(int arrCount, Customer customerArr[MAX_CUSTOMERS], Customer sortedCustomers[MAX_CUSTOMERS]) {
    Customer temp;
    strcpy(temp.name, "");
    sortedCustomers[0] = temp;
	int i, j;
	int found = 0;
    int offset = 0;
    int numOfCustomers = 0;
    for(i = 0; i < arrCount; i++) {
    	Customer customer = customerArr[i];
    	for(j = 0; j < arrCount; j++) {
    		if(strcmp(customer.name, sortedCustomers[j].name) == 0) {
    			found = 1;
    			offset++;
    		}
    	}
    	if(found == 0) {
    		sortedCustomers[i - offset] = customer;
    		numOfCustomers++;
    	}
    	found = 0;
    }
    return numOfCustomers;
}

/* Prints to the hw4time.txt file. */
int outputTime(int arrCount, Customer customerArr[MAX_CUSTOMERS], Customer sortedCustomers[MAX_CUSTOMERS]) {
	FILE *outfile = fopen("hw4time.txt", "w");
    int i, j;
    int numOfCustomers = getIndividualCustomers(arrCount, customerArr, sortedCustomers);

    int itemCount = 0;
    for(i = 0; i < numOfCustomers; i++) {
    	itemCount = 0;
    	Customer customer = sortedCustomers[i];
    	fprintf(outfile, "%s\r\n", customer.name);
    	for(j = 0; j < arrCount; j++) {
    		if(strcmp(customer.name, customerArr[j].name) == 0) {
    			double itemTotal = customerArr[j].items[j].quantity * customerArr[j].items[j].price;
    			sortedCustomers[i].orderTotal += itemTotal;
    			sortedCustomers[i].items[itemCount] = customerArr[j].items[j];
    			fprintf(outfile, "%s %d $%.2lf\r\n",
    					customerArr[j].items[j].name,
    					customerArr[j].items[j].quantity, customerArr[j].items[j].price);
    			itemCount++;
    		}
    	}
    	sortedCustomers[i].itemCount = itemCount;
    	fprintf(outfile, "\r\n");
    }

    return numOfCustomers;
}

/* Reads the input file. */
int readFile(FILE *infile, Customer customerArr[MAX_CUSTOMERS], Item itemArr[MAX_CUSTOMERS]) {
    Customer c;
    Item item;
    int arrCount = 0;
    char dollar;
    while (fscanf(infile, "%s %d %s %c %lf", c.name, &item.quantity, item.name, &dollar, &item.price) != EOF) {
    	item.itemValue = item.quantity * item.price;
    	itemArr[arrCount] = item;
    	c.items[arrCount] = itemArr[arrCount];
        customerArr[arrCount] = c;
        arrCount++;
    }
    return arrCount;
}

/* Controller */
int main(void) {
    FILE *infile = fopen("hw4input.txt", "r");
    if (infile == NULL) {
    	printf("File did not open.");
    	return 1;
    }
    setvbuf(stdout, NULL, _IONBF, 0);
    Customer customerArr[MAX_CUSTOMERS];
    Item itemArr[MAX_CUSTOMERS];
    Customer individualCustomers[MAX_CUSTOMERS]; // new array for non repeating customer names
    int arrCount = readFile(infile, customerArr, itemArr);
    int numOfCustomers = outputTime(arrCount, customerArr, individualCustomers);
    sortCustomersTotal(individualCustomers);
    getIndividualItems(numOfCustomers, individualCustomers);
    sortItems(numOfCustomers, individualCustomers);
    outputMoney(numOfCustomers, individualCustomers);
    fclose(infile);
    return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
void reverse_words(char *, int);
void print_words(char *, int);  
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions

	int i = 0;
	int j = 0;
	// Treat as boolean for multiple spaces
	int space_gap = 0;
	
	// Removing whitespace at front of string
	while (user_str[i] == ' ' || user_str[i] == '\t') {
		i++;
	}

	// Goes through string until NULL terminator and adds a space in replacement for space or tabs
	while (user_str[i] != '\0') {
		if (user_str[i] != ' ' && user_str[i] != '\t') {
			if (space_gap == 1 && j < len) {
				buff[j] = ' ';
				j++;
				space_gap = 0;
			}
				// If j is greater than max length, it will return -1, if not, it will be added to the buffer
				if (j <= len) {
					buff[j] = user_str[i];
					j++;
				} else {
					return -1;
				}
			} else {
				space_gap = 1;
			}
			i++;
	}


	// Add . to the remaining buffer size
	while (j < len) {
		buff[j] = '.';
		j++;
	}
	return j;
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
	 putchar('[');
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
	 putchar(']');
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
	int word_count = 0;
	// Treated as a boolean
	int in_word = 0;
	// If there is a character that isn't a space, add it to the count and continue until it finds another space
	for (int i = 0; i < str_len; i++) {
		if (buff[i] != ' ') {
			if (in_word == 0) {
				word_count++;
				in_word = 1;
			}
		} else {
			in_word = 0;
		}
	}
	return word_count;
}

void reverse_words(char *buff, int str_len) {
	int str_start = 0;
	int str_end = str_len - 1;
	char swap;

	// Goes to the last letter of the string
	while (str_end >= 0 && buff[str_end] == '.') {
		str_end--;
	}

	while (str_end > str_start) {
		swap = buff[str_start];
		buff[str_start] = buff[str_end];
		buff[str_end] = swap;

		str_start++;
		str_end--;
	}
	
}


void print_words(char *buff, int str_len){
	int word_start = -1;
	int word_count = 0;

	printf("Word Print\n");
	printf("----------\n");

	for (int i = 0; i < str_len; i++) {
		if (buff[i] != ' ' && buff[i] != '\0' && buff[i] != '.') {
			if (word_start == -1) {
				word_start = i;
			}
			// Once the word ends
		} else if (word_start != -1) {
			// Add to word count and print the word with its length
			word_count++;
			printf("%d. ", word_count);
			for (int j = word_start; j < i; j++) {
				putchar(buff[j]);
			}
			printf("(%d)\n", i - word_start);
			word_start = -1;
		}
	}
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
	 //      This is safe because the if statement first checks if the argc is less than the length of 2 characters(has to exist) and
	 //      then checks to make sure it begins with a "-"
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }
//WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
	 //      When there are more than 3 arguements, it will treat it as a -h and exit
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
	 buff = (char *)malloc(BUFFER_SZ * sizeof(char));
	 if (buff == NULL) {
		 exit(99);
	 }


    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
	}
    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
		  case 'x':
				if(argc != 5){
					return -1;
				}
				printf("Not Implemented!");
				return -2;
				exit(0);
				break;
		  case 'r':
				reverse_words(buff, user_str_len);
				break;
		  case 'w':
				print_words(buff, user_str_len);
				rc = count_words(buff, BUFFER_SZ, user_str_len);
				printf("\nNumber of words returned: %d\n", rc);
				break;

        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
	 free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
//          I think providing both the pointer and the length because it allows for freedom in having a different byte
//          size with define, this allows for us to switch between different bytes for the code.


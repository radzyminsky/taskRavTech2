#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>


#pragma warning(disable : 4996)



typedef enum { Clang, CsFund, ComputerNet } courses;

typedef struct student {
	float courseGrade[3];//for each cell that its value <0, so, student hasn't course that its index suitable to "enum"
	float avg;
	char* fname, * lname, id[10];
	struct student* next;
}student;

typedef int (*funPtr)(student* stu, char* str);

//global string array for courses name
char* courses_name[] = {
	[Clang] = "C lang",
	[CsFund] = "CS Fundamentals",
	[ComputerNet] = "Computer Networks"
};

//global pointer to list
student* head = NULL;

//trim spaces from start string, and '\n', spaces from end string
char* trim_space(char* str) {
	int i = 0;

	while (str[i] && str[i] == ' ') {
		str++;
	}
	i = strlen(str) - 1;
	while (i > 0 && (str[i] == ' ' || str[i] == '\n')) {
		str[i] = '\0';
		i--;
	}
	return str;
}

//func that print  errors
void printError(char* err, int num_line) {
	if (num_line)
		printf("%s in line: %d\n", err, num_line);
	else
		printf("%s\n", err);
}

char* check_id(char* id) {
	int i;

	id = trim_space(id);
	if (strlen(id) > 9)
		return NULL;
	for (i = 0;i < 9;i++)
		if (id[i] < '0' || id[i]>'9')
			return NULL;
	return id;
}
char* check_flname(char* fln) {
	int i, len;

	fln = trim_space(fln);
	if (!(len = strlen(fln)))//taht is, fln is empty
		return NULL;
	for (i = 0;i < len;i++)
		if (fln[i] < 'A' || fln[i]>'z')
			return NULL;
	return fln;
}

//this function checks pramater "course_name" if is suitable to enum,
//if is, it returns numcer of enum otherwise returns -1
int check_course_name(char* course_name) {
	int i;

	course_name = trim_space(course_name);
	for (i = 0;i < 3;i++)
		if (!strcmp(course_name, courses_name[i])) {
			return i;
		}
	return -1;
}

float check_grade(char* grade) {
	float f;
	int i, len;

	grade = trim_space(grade);
	len = strlen(grade);
	for (i = 0;i < len;i++)
		if ((grade[i] < '0' || grade[i]>'9') && grade[i] != '.')
			return -1;
	f = (float)atof(grade);
	if (f >= 0 && f <= 100)
		return f;
	return -1;
}

//checks if the "id" already exist 
// if yes, returns pointer to pointer to "struct student"
student** exist_id(char* id) {
	student** ptrptr = &head;

	while (*ptrptr)
	{
		if (!strcmp((*ptrptr)->id, id))
			return ptrptr;
		ptrptr = &((*ptrptr)->next);
	}
	return NULL;
}

float avg(float* arr, int count) {
	int i = 0, num_tests = 0;
	float sum = 0;

	for (;i < count;i++)
		if (arr[i] > (float)0) {
			sum += arr[i];
			num_tests++;
		}
	return sum / num_tests;
}
void insert(student* item) {//inserts item to the list in suitable location
	student* temp = head;

	if (!head || strcmp(head->lname, item->lname) > 0) { //i.e. list is empty,or the first node larger from item
		head = item;
		item->next = temp;
		return;
	}
	while (temp->next && strcmp(temp->next->lname, item->lname) < 0)
		temp = temp->next;

	item->next = temp->next;
	temp->next = item;
}

void freeList(student* stu) {
	if (stu->next)
		freeList(stu->next);
	free(stu->lname);
	free(stu->fname);
	free(stu);
}

void print_one(student* stu) {
	char cl[6] = "", cn[6] = "", cf[6] = "";

	if (stu->courseGrade[Clang] != -1)
		sprintf(cl, "%3.1f", stu->courseGrade[Clang]);

	if (stu->courseGrade[ComputerNet] != -1)
		sprintf(cn, "%3.1f", stu->courseGrade[ComputerNet]);

	if (stu->courseGrade[CsFund] != -1)
		sprintf(cf, "%3.1f", stu->courseGrade[CsFund]);

	printf("%-20s|%-20s|%-20s|%-20s|%-20s|%-20.1f\n",
		stu->fname, stu->lname, cl, cn, cf, stu->avg);
}

void print_title() {

	printf("%-20s|%-20s|%-20s|%-20s|%-20s|%-20s\n",
		"first name ", "second name", "C lang", "Computer Networks", "CS Fundamentals", "Average");
}

void printAll() {
	print_title();
	student* temp = head;
	while (temp) {
		print_one(temp);
		temp = temp->next;
	}
}

//First, we remove the updated node from the list (of course, we need save the its pointed),
// then we send it to the "insert" function to put it in its suitable place
void re_insert(student** ptrptr) {
	student* temp = *ptrptr;
	(*ptrptr) = (*ptrptr)->next;
	insert(temp);
}
int err_allocation() {
	freeList(head);
	printf("error in memory allocation \n");
	return -1;
}

int getValues(char* id, char* fname, char* lname, courses course_name, float grade) {
	student* ptr;
	student** ptrptr;//i need this for a value that returned from "exist_id" func.
	char* fn, * ln;
	int i, need_re_insert = 0;

	//checks if already exist this id, if does, we just update details
	if (ptrptr = exist_id(id)) {

		//update details

		if (strcmp((*ptrptr)->lname, lname)) {
			need_re_insert = 1;  //checks if "last name" was changed then we need to re insert
			ln = (char*)realloc((*ptrptr)->lname, strlen(lname) + 1);
			if (!ln)
				return err_allocation();
			(*ptrptr)->lname = ln;
		}

		if (strcmp((*ptrptr)->fname, fname)) {
			fn = (char*)realloc((*ptrptr)->fname, strlen(fname) + 1);
			if (!fn)
				return err_allocation();
			(*ptrptr)->fname = fn;
		}

		strcpy((*ptrptr)->lname, lname);
		strcpy((*ptrptr)->fname, fname);
		(*ptrptr)->courseGrade[course_name] = grade;
		(*ptrptr)->avg = avg((*ptrptr)->courseGrade, 3);

		if (need_re_insert)
			re_insert(ptrptr);//because now maybe the list isn't sort correct
		return 0;
	}
	// if not exist, we need to create new one
	ptr = malloc(sizeof(student));
	if (!ptr)
		return err_allocation();

	strcpy(ptr->id, id);
	fn = (char*)malloc(strlen(fname) + 1);
	ln = (char*)malloc(strlen(lname) + 1);
	if (!fn || !ln)
		return err_allocation();
	ptr->fname = fn;
	ptr->lname = ln;

	strcpy(ptr->fname, fname);
	strcpy(ptr->lname, lname);
	for (i = 0;i < 3;i++)
		ptr->courseGrade[i] = -1;
	ptr->courseGrade[course_name] = grade;
	ptr->avg = avg(ptr->courseGrade, 3);
	insert(ptr);
	return 0;
}

//this func get one line (string),and number of line while "readFile" func calls it,
//if "commands" func calls, "num_line"=0
//function returns 0 on success, 1 on data failed, -1 on memory allocation error
int getLine(char* line, int num_line) {
	int i = 1, success;
	float grade;
	char* temp, fname[20] = "", lname[20] = "", id[10] = "";
	courses	cour_name;

	temp = strtok(line, ",\n");
	while (temp)
	{
		switch (i)
		{
		case 1:
			if (temp = check_id(temp)) {
				strncpy(id, temp, 9);
				break;
			}
			printError("error in id", num_line);
			return 1;
		case 2:
			if (temp = check_flname(temp)) {
				strncpy(fname, temp, 19);
				break;
			}
			printError("error in first name", num_line);
			return 1;
		case 3:
			if (temp = check_flname(temp)) {
				strncpy(lname, temp, 19);
				break;
			}
			printError("error in last name", num_line);
			return 1;
		case 4:
			if ((cour_name = check_course_name(temp)) != -1)
				break;
			printError("error in cours name", num_line);
			return 1;
		case 5:
			if ((grade = check_grade(temp)) >= 0) {
				break;
			}
			printError("error in grade", num_line);
			return 1;
		default:
			printError("ERROR! it has too much data", num_line);
			return 1;
		}

		temp = strtok(NULL, ",\n");
		i++;
	}
	if (i < 6) {//that is, it has few data
		printError("Error! Missing data ", num_line);
		return 1;
	}
	success = getValues(id, fname, lname, cour_name, grade);
	if (success)
		return -1;
	return 0;
}

int readFile(FILE* f) {
	int num_line = 1, success;
	char buffer[200] = "";

	fgets(buffer, 200, f);//ignores from high line

	while (fgets(buffer, 200, f))
	{
		if (buffer[strlen(buffer) - 1] != '\n') { //checks if line is too much long
			printError("greater than 200 characters ", num_line);
			while (fgetc(f) != '\n');// ignores to the end of the line, 
			num_line++;
			continue;
		}
		success = getLine(buffer, num_line);
		if (success == 1)
			printf("line %d isn't insert\n", num_line);
		if (success == -1)
			return -1;
		num_line++;
	}
	return 0;
}

//function creates line separated in ',' just like a line in file.csv, then sends it to "getLine" func 
int set_command(char* str) {
	int i;
	char* fname = NULL, * lname = NULL, * id = NULL, * course_name = NULL, * grade = NULL, * temp = NULL, * ch = NULL;
	char line[200] = "";



	temp = strtok(str, ",");

	for (i = 0;i < 4; i++)  //because if has more than 4 values, then is mistake
	{
		if (!temp) {//i.e. it has lass than 4 values
			printf("Error! Missing data, please try again\n");
			return 1;
		}
		if ((ch = strchr(temp, '=')) == NULL) {
			printf("error in command, at \"%s\", please try again\n", temp);
			return 1;
		}

		*ch = '\0';  //replace character '=' at '\0',because i need to trem spaces by call "trim_space" func
		temp = trim_space(temp);

		if (!strcmp(temp, "first name")) {
			fname = ch + 1;//send to "trim_space"  the right of the '='
		}
		else if (!strcmp(temp, "second name")) {
			lname = ch + 1;
		}
		else if (!strcmp(temp, "ID")) {
			id = ch + 1;
		}
		else
		{
			course_name = temp;
			grade = ch + 1;
		}

		temp = strtok(NULL, ",");
	}

	if (temp) { // if the command contain more than 4 ','
		printf("error in command, at \"%s\", its containe too mauch detailes, please try again\n", temp);
		return 1;
	}

	if (id && fname && lname && course_name && grade) {
		strcat(line, id);
		strcat(line, ",");
		strcat(line, fname);
		strcat(line, ",");
		strcat(line, lname);
		strcat(line, ",");
		strcat(line, course_name);
		strcat(line, ",");
		strcat(line, grade);
	}
	else {
		printf("error, Missing data, may be one or mor value is duplicate, Or syntax of one or more data not valid\n");
		return 1;
	}

	return getLine(line, 0);//send to "getLine", and return what returned
}

void search_and_print(funPtr condition, char* val) {
	student* current = head;

	print_title();
	while (current)
	{
		if (condition(current, val))
			print_one(current);
		current = current->next;
	}
}


void init_str_right_left(char* ch, char* str, char** right, char** left) {
	*ch = '\0';
	*right = trim_space(str);
	*left = trim_space(ch + 1);
}

int fname_equal(student* stu, char* str) {
	return !strcmp(stu->fname, str);
}
int lname_equal(student* stu, char* str) {
	return !strcmp(stu->lname, str);
}
int id_equal(student* stu, char* str) {
	return !strcmp(stu->id, str);
}
int avg_equal(student* stu, char* str) {
	float f = (float)atof(str);
	return stu->avg == f;
}
int clang_equal(student* stu, char* str) {
	float f = (float)atof(str);
	return stu->courseGrade[Clang] == f;
}
int ComputerNet_equal(student* stu, char* str) {
	float f = (float)atof(str);
	return stu->courseGrade[ComputerNet] == f;
}
int CsFund_equal(student* stu, char* str) {
	float f = (float)atof(str);
	return stu->courseGrade[CsFund] == f;
}

int avg_larger(student* stu, char* str) {
	float f = (float)atof(str);
	return stu->avg > f;
}
int clang_larger(student* stu, char* str) {
	float f = (float)atof(str);
	return stu->courseGrade[Clang] > f;
}
int ComputerNet_larger(student* stu, char* str) {
	float f = (float)atof(str);
	return stu->courseGrade[ComputerNet] > f;
}
int CsFund_larger(student* stu, char* str) {
	float f = (float)atof(str);
	return stu->courseGrade[CsFund] > f;
}

int avg_smaller(student* stu, char* str) {
	float f = (float)atof(str);
	return stu->avg < f;
}
int clang_smaller(student* stu, char* str) {
	float f = (float)atof(str);
	return stu->courseGrade[Clang] < f;
}
int ComputerNet_smaller(student* stu, char* str) {
	float f = (float)atof(str);
	return stu->courseGrade[ComputerNet] < f;
}
int CsFund_smaller(student* stu, char* str) {
	float f = (float)atof(str);
	return stu->courseGrade[CsFund] < f;
}

int is_digits(char* c) {
	int i, len = strlen(c);

	for (i = 0;i < len;i++)
		if (c[i] < '0' || c[i]>'9') {
			printf("error:  \"%s\" isn't digits\n", c);
			return 0;
		}
	return 1;
}

funPtr command_smaller(char* right, char* left) {
	if (!strcmp(right, "Average")) {
		if (is_digits(left))
			return &avg_smaller;
	}
	else if (!strcmp(right, "C lang")) {
		if (is_digits(left))
			return &clang_smaller;
	}
	else if (!strcmp(right, "CS Fundamentals")) {
		if (is_digits(left))
			return &CsFund_smaller;
	}
	else if (!strcmp(right, "Computer Networks")) {
		if (is_digits(left))
			return &ComputerNet_smaller;
	}
	else {
		printf("error,\"%s<\" isn't valid", right);
	}
	return NULL;

}
funPtr command_larger(char* right, char* left) {
	if (!strcmp(right, "Average")) {
		if (is_digits(left))
			return &avg_larger;
	}
	else if (!strcmp(right, "C lang")) {
		if (is_digits(left))
			return &clang_larger;
	}
	else if (!strcmp(right, "CS Fundamentals")) {
		if (is_digits(left))
			return &CsFund_larger;
	}
	else if (!strcmp(right, "Computer Networks")) {
		if (is_digits(left))
			return &ComputerNet_larger;
	}
	else {
		printf("error,\"%s>\" isn't valid", right);
	}
	return NULL;

}

funPtr command_equal(char* right, char* left) {
	if (!strcmp(right, "Average")) {
		if (is_digits(left))
			return &avg_equal;
	}
	else if (!strcmp(right, "C lang")) {
		if (is_digits(left))
			return &clang_equal;
	}
	else if (!strcmp(right, "CS Fundamentals")) {
		if (is_digits(left))
			return &CsFund_equal;
	}
	else if (!strcmp(right, "Computer Networks")) {
		if (is_digits(left))
			return &ComputerNet_equal;
	}
	else if (!strcmp(right, "ID")) {
		if (check_id(left))
			return &id_equal;
		printf("\"%s\" isn't valid ID\n", left);
	}
	else if (!strcmp(right, "first name")) {
		if (check_flname(left))
			return &fname_equal;
		printf("\"%s\" isn't valid name\n", left);

	}
	else if (!strcmp(right, "second name")) {
		if (check_flname(left))
			return &lname_equal;
		printf("\"%s\" isn't valid name\n", left);
	}
	else {
		printf("error,\"%s\" isn't valid", right);
	}
	return NULL;
}

void select_command(char* str) {
	funPtr fun = NULL;
	char* ch = NULL, * strright = NULL, * strleft = NULL;

	if (ch = strchr(str, '<')) {
		init_str_right_left(ch, str, &strright, &strleft);
		fun = command_smaller(strright, strleft);
	}
	else if (ch = strchr(str, '>')) {
		init_str_right_left(ch, str, &strright, &strleft);
		fun = command_larger(strright, strleft);
	}
	else if (ch = strchr(str, '=')) {
		init_str_right_left(ch, str, &strright, &strleft);
		fun = command_equal(strright, strleft);
	}
	else {
		printf("error in select command\n");
	}

	if (fun) {
		search_and_print(fun, strleft);
	}
}

int commands() {
	char buffer[300] = "";
	char* buffer2 = buffer;
	int success;

	while (1)
	{
		printf("\n>>");
		fgets(buffer, 300, stdin);

		if (buffer[strlen(buffer) - 1] != '\n') { //checks if line is too much long
			printError("greater than 300 characters ", 0);
			while (getc(stdin) != '\n');// ignores to the end of the line, 
			continue;
		}

		buffer2 = trim_space(buffer);

		if (!strncmp(buffer2, "select ", 7)) {
			select_command(buffer2 + 7);
		}
		else if (!strncmp(buffer2, "set ", 4)) {
			success = set_command(buffer2 + 4);
			if (success == -1)//i.e. it was error with allocation
				return success;
		}
		else if (!strcmp(buffer2, "print")) {
			printAll();
		}
		else if (!strcmp(buffer2, "quit")) {
			break;
		}
		else {
			printf("error in command \"%s\"", buffer2);
		}
	}
	return 0;
}

void writeFile(FILE* f) {
	student* current = head;
	int i;

	rewind(f);
	fprintf(f, "ID,First Name,Second Name,Course Name,Grade\n");//print title
	while (current)
	{
		for (i = 0;i < 3;i++)
			if (current->courseGrade[i] > -1)
				fprintf(f, "%s,%s,%s,%s,%f\n", current->id, current->fname, current->lname, courses_name[i], current->courseGrade[i]);

		current = current->next;
	}
}
int main() {
	int success;
	FILE* f;

	if (!(f = fopen("taskRavatech2.csv", "r+"))) {
		printf("error in open file");
		return -1;
	}

	success = readFile(f);
	if (success == -1)
		return-1;
	printAll();
	commands();

	writeFile(f);
	fclose(f);

	freeList(head);

	return 0;

}


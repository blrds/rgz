#include<stdio.h>
#include<string.h>
#include<locale.h>
#include<windows.h>

typedef struct groups {
	char* id;
	struct groups* next;
}groups;

typedef struct students {
	char* name, surname, secondname;
	struct students *next;
}students;

int find_symbol(char* s, char c) {
	char *ach;
	ach = strchr(s, c);
	return (ach - s );
}

int get_int(FILE *f, char* group) {
	int id;
	fseek(f, 0, 0);
	char str[256], num[256];
	while (!feof(f)) {
		fgets(str, 256, f);
		if (strstr(str, group) != NULL)break;
	}
	if (feof(f))return -1;
	int a = find_symbol(str, ' ');
	for (int i = a+1; i < strlen(str)-1; i++)
		num[i-a-1] = str[i];
	id = atoi(num);
	return id;
}

char *get_group(FILE *f, int id) {
	char str[256];
	for (int i = 0; i < id; i++)
		fgets(str, 256, f);
	char group[256];
	int index = find_symbol(str, ' ');
	for (int i = 0; i < index; i++) {
		group[i] = str[i];
	}
	group[index+1] = '\n';
	return group;
}

void ins_group(FILE *f, char* group) {
	char str[256];
	fgets(str, 256, f);
	int till = get_int(f, str);
	fseek(f, 0, 0);
	for (int i = 0; i < till; i++) {
		fgets(str, 256, f);
		if (strcmp(str,group)>0) {
			break;
		}
	}
	fseek(f, SEEK_CUR-strlen(str), SEEK_SET);
	fputs(group + ' '+'0'+'\n', f);
}

/*void del_group(FILE *f, char* group) {

}

void print_group(FILE *f, char* group) {

}

void ins_student(FILE *f, char* group) {

}

void del_student(FILE *f, char* group) {

}

void print_student(){

}
*/

void main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "rus");
	FILE *f = fopen("list.txt", "ab+");
	fseek(f, 0, 0);
	char* s;
	s = get_group(f, 1);
	printf("%s", s);
}
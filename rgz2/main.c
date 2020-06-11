#include<stdio.h>
#include<string.h>
#include<locale.h>
#include<windows.h>

typedef struct groups {
	char* id;
	int add;
	struct groups* next;
}groups;
//������
groups* init(groups *head, char* id, int add) {//�������������� �������� ������ � ����������� ���������������
	groups* t = head, *p;
	p = (groups*)malloc(sizeof(groups));
	p->id = id;
	p->add = add;
	p->next = NULL;
	while (t->next != NULL) {
		if (strcmp(t->next->id, id) > 0) break;
		t = t->next;
	}
	p->next = t->next;
	t->next = p;
	return head;
}

typedef struct students {
	char* name, surname, secondname;
	struct students *next;
}students;

int find_symbol(char* s, char c) {//������ ������� ��������� �������
	char *ach;
	ach = strchr(s, c);
	return (ach - s );
}

void fgoton(FILE* f, int n) {//����� � ����� �� n-�� ������
	char* str = malloc(sizeof(char) * 256);
	fseek(f, 0, 0);
	for (int i = 0; i < n; i++)
		fgets(str, 256, f);
}

int get_int(FILE *f, char* group) {//������ ������� ������ ��������� ������
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

char *get_group(FILE *f, int id) {//������ �������� ������
	char *str=malloc(sizeof(char)*256);
	fseek(f, 0, 0);
	fgoton(f, id);
		fgets(str, 256, f);
	char *group=malloc(256*sizeof(char));
	int index = find_symbol(str, ' ');
	for (int i = 0; i < index; i++) {
		group[i] = str[i];
	}
	group[index] = '\0';
	return group;
}

char* strcopy( char* s2) {//����������� ����������� ������� �����������
	char *s = malloc(sizeof(s2));
	for (int i = 0; i < strlen(s2); i++)
		s[i] = s2[i];
	s[strlen(s2)] = '\0';
	return s;
}

groups* form_groups_list(FILE *f) {//�������� ������ �����
	char* name, *str = malloc(256 * sizeof(str));
	groups *head = (groups*)malloc(sizeof(groups));
	head->id = "";
	head->add = 0;
	head->next = NULL;
	fgets(str, 256, f);
	int till = get_int(f, str), num, save;
	fseek(f, 0, 0);
	for (int i = 0; i < till - 1; i++) {
		name = get_group(f, i);
		num = get_int(f, name);
		head = init(head, name, num + 1);
	}
	return head;
}
//������

void ins_group(FILE *f, char* group, char* filename) {
	char *str = malloc(256 * sizeof(char)), *name;
	fseek(f, 0, 0);
	groups *head = form_groups_list(f);
	groups *t = (groups*)malloc(sizeof(groups));
	t->id = group;
	groups *p = head;
	int flag = 0;
	while (p->next != NULL) {
		if (strcmp(p->next->id, t->id) > 0 && flag==0){
			flag++;
			t->add = p->next->add;
			t->next = p->next;
			p->next = t;
		}
		p = p->next;
	}
	char* newfile = "a.txt";
	FILE *f1 = fopen(newfile,"w+");
	t = head->next;
	while (t != NULL) {
		name = strcopy(t->id);
		strcat(name, " ");
		strcat(name,itoa(t->add, str, 10));
		strcat(name, "\n");
		fputs(name, f1);
		t = t->next;
	}
	rewind(f);
	fgoton(f, head->next->add-2);
	while (!feof(f)) {
		fgets(str, 256, f);
		if(str[strlen(str) - 1]=='\n')str[strlen(str) - 1] = '\0';
		fputs(str, f1);
	}
	fclose(f1);
	fclose(f);
	remove(filename);
	rename(newfile, filename);
	f = fopen(filename, "ab+");
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
	char* filename = "list.txt";
	FILE *f = fopen(filename, "ab+");
	rewind(f);
	ins_group(f, "���-3", filename);
	
}
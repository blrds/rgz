#include<stdio.h>
#include<string.h>
#include<locale.h>
#include<windows.h>

typedef struct groups {
	char* id;
	int add;
	struct groups* next;
}groups;

typedef struct students {
	char* name, *surname, *patronymic;
	struct students *next;
}students;

groups* group_init(groups *head, char* id, int add) {//�������������� �������� ������ � ����������� ���������������
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

students* student_init(students* head, char* name, char* surname, char* patronymic) {
	students* t = head, *p;
	p = (students*)malloc(sizeof(students));
	p->name = name;
	p->surname = surname;
	p->patronymic = patronymic;
	p->next = NULL;
	while (t->next != NULL) {
		if (strcmp(t->next->surname, surname) > 0) break;//���� ������� ������ ������ ������� �������
		if (strcmp(t->next->surname, surname) == 0)
			if (strcmp(t->next->name, name) > 0)break;//���� ������� ���������, �� ����� ��������
			else if (strcmp(t->next->name, name) == 0)
				if (strcmp(t->next->patronymic, patronymic) > 0)break;//���� �� ���������, �� �������� ��������
		t = t->next;
	}
	p->next = t->next;
	t->next = p;
	return head;
}

int find_symbol(char* s, char c) {//������ ������� ��������� �������
	char *ach;
	ach = strchr(s, c);
	return (ach - s );
}

int strings_count(FILE *f) {//������� ����� ���-�� �����
	int count = 0;
	fseek(f, 0, 0);
	char str[256];
	while (!feof(f)) {
		fgets(str, 256, f);
		count++;
	}
	return count;
}

void fgoton(FILE* f, int n) {//����� � ����� �� n-�� ������
	char* str = malloc(sizeof(char) * 256);
	fseek(f, 0, 0);
	for (int i = 0; i < n; i++)
		fgets(str, 256, f);
}

int get_ind(FILE *f, char* group) {//������ ������� ������ ��������� ������
	int id;
	fseek(f, 0, 0);
	char str[256], num[256];
	while (!feof(f)) {//
		fgets(str, 256, f);//����������, ���� �� ������ ������ � ������ group
		if (strstr(str, group) != NULL)break;//
	}
	if (feof(f))return -1;//���� ���-���� ������ �� �����
	int a = find_symbol(str, ' ');//������ ������
	for (int i = a+1; i < strlen(str)-1; i++)
		num[i-a-1] = str[i];//����������� ��������
	id = atoi(num);
	return id;
}

char *get_next_group(FILE *f, char* group) {//��������� ������
	char *str=malloc(sizeof(char)*256);
	fseek(f, 0, 0);
	fgets(str, 256, f);
	int till = get_ind(f, str)-1;
	fseek(f, 0, 0);
	while (!feof(f)) {
		fgets(str, 256, f);//����������, ���� �� ������ ������ � ������ group
		if (strstr(str, group) != NULL)break;//
		till--;//������� ������� ��������
		if (till <= 1)return NULL;
	}
	fgets(str, 256,f);//���� ������
	return str;
}

char *get_group(FILE *f, int id) {//������ �������� ������ �� �-�� ������
	char *str=malloc(sizeof(char)*256);
	fseek(f, 0, 0);
	fgoton(f, id);//��������� �� ������ ������
	fgets(str, 256, f);//����� ��� ������ � ���
	char *group=malloc(256*sizeof(char));
	int index = find_symbol(str, ' ');//������ ������� �������
	for (int i = 0; i < index; i++) {
		group[i] = str[i];//����������� ��������
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
	groups *head = (groups*)malloc(sizeof(groups));//
	head->id = "";//
	head->add = 0;//������������� ������
	head->next = NULL;//
	fseek(f, 0, 0);
	fgets(str, 256, f);
	int till = get_ind(f, str), num, save;//������ ������=��������� ������+1
	fseek(f, 0, 0);
	for (int i = 0; i < till - 1; i++) {
		name = get_group(f, i);
		num = get_ind(f, name);//�������������� ������� �� ���������
		head = group_init(head, name, num);
	}
	return head;
}

void write_group_list(FILE *f1, groups* head) {//������ ������ ������ �����
	char *str = malloc(sizeof(char) * 256), *name = malloc(sizeof(char) * 256);
	groups*	t = head->next;
	while (t != NULL) {
		name = strcopy(t->id);
		strcat(name, " ");
		strcat(name, itoa(t->add, str, 10));//��������� ������ ��� ������
		strcat(name, "\n");
		fputs(name, f1);
		t = t->next;
	}
}

void write_students_from_to(FILE *f, FILE *f1, int from, int to) {//������ �������������� ���������
	char* str = malloc(sizeof(char) * 256);
	rewind(f);
	fgoton(f, from);
	for (int i = from; i < to - 1; i++) {
		fgets(str, 256, f);
		if (str[strlen(str) - 1] == '\n') {//������ ������������ ��������� � ������
			str[strlen(str) - 2] = '\n';
			str[strlen(str) - 1] = '\0';
		}
		fputs(str, f1);
	}
}

void regrouop(FILE *f, groups* head, char* filename, groups* deleted_group, int groupsize) {//�������������� ���� � ������ ����� �������, �� ��������� ���� ���������
	char *newfile = "a.txt", *str=malloc(sizeof(char)*256), *name = malloc(sizeof(char) * 256);
	FILE *f1 = fopen(newfile, "w+");//����� ���� ��� ����������
	write_group_list(f1, head);
	if (deleted_group == NULL) {//���� ��������� ������ ���
		write_students_from_to(f, f1, head->next->add - 2, strings_count(f) + 1);
	}
	else {//���
		write_students_from_to(f, f1, head->next->add, deleted_group->add);//���������� ���� ��������� �� ���������
		write_students_from_to(f, f1, deleted_group->next->add + groupsize, strings_count(f) + 1);
	}
	fclose(f1);//��������� ��� �����, ������ �������, ����� ��������������� � ������
	fclose(f);
	remove(filename);
	rename(newfile, filename);
	f = fopen(filename, "ab+");
}

int ins_group(FILE *f, char* group, char* filename) {//������� ������ ������
	char *str = malloc(256 * sizeof(char)), *name;
	fseek(f, 0, 0);
	groups *head = form_groups_list(f);//��������� ������ �����
	groups *t = (groups*)malloc(sizeof(groups));
	t->id = group;
	groups *p = head;
	int flag = 0;
	while (p->next != NULL) {
		p->next->add++;//����������� ������ ���� ���������� ����� �� 1
		if (strcmp(p->next->id, t->id) == 0) 
			return 0; 
		if (strcmp(p->next->id, t->id) > 0 && flag==0){//������ ��������� ����� ������=������ ��������� ��������� �� ��� ������, �� � ����� ��������� 0
			flag++;
			t->add = p->next->add;
			t->next = p->next;
			p->next = t;
			t->next->add--;
		}
		p = p->next;
	}
	if (flag == 0) {
		p->next = t;
		t->next = NULL;
		t->add = strings_count(f)+1;
	}
	regrouop(f, head, filename, NULL, 0);//�����������
	return 1;
}

int del_group(FILE *f, char* group, char* filename) {//�������� ������
	groups *head = form_groups_list(f), *deleted_group, *t = head;
	while (t->next != NULL) {
		if (strcmp(t->next->id, group) == 0)break;//��������, ���� �� ����� ������
		t = t->next;
	}
	if (t->next) {//���� ����
		deleted_group = t->next;//�������� �� ������
		t = head;
		int decrease = 1;//��� ����� "��" ���������� ������� ��������� = 1 
		while (t->next) {
			t->next->add-=decrease;
			if (t->next == deleted_group) {
				if(t->next->next!=NULL)decrease +=(t->next->next->add-deleted_group->add-1);//���� ��� ���� �� ��������� ������, �� ���������� ������ = ����� ������+1
				t->next = deleted_group->next;//������ �� ��������� ������
				deleted_group->add++;//������ ������ ��������, �� ��� ��������� �� ������ ����. ������
			}
			else t = t->next;
		}
		regrouop(f, head, filename, deleted_group, decrease-1);
		return 1;
	}//���� ���
	return 0;
}

int print_groups(FILE *f) {//������ ����� ���������
	groups *head = form_groups_list(f), *t = head->next;;
	if (!t)return 0;
	while (t->next != NULL) {
		printf("������ %s, ���-�� ��������� %d\n", t->id, t->next->add - t->add);
		t = t->next;
	}
	printf("������ %s, ���-�� ��������� %d\n", t->id, strings_count(f) - t->add+1);
	return 1;
}

students* form_students_list(FILE *f, int from, int to) {//������������ ������ ���������
	students* head, *t;
	int flag, length;
	char *str = malloc(sizeof(char) * 256), *name = malloc(sizeof(char) * 256), *surname = malloc(sizeof(char) * 256), *patronymic = malloc(sizeof(char) * 256);
	head = malloc(sizeof(students));
	head->name = "";
	head->surname = "";//������� 
	head->patronymic = "";
	head->next = NULL;
	fgoton(f, from - 1);
	for (int i = from; i < to; i++) {
		fgets(str, 256, f);
		flag = 0;
		for (int j = 0; j < strlen(str); j++) {
			switch (flag) {
			case 0: {
				if (str[j] == ' ')surname[j] = '\0';
				else surname[j] = str[j];
				break;
			}
			case 1: {
				if (str[j] == ' ')name[j - length] = '\0';
				else name[j - length] = str[j];
				break;
			}
			case 2: {
				if (str[j] == '\r') {
					patronymic[j - length] = '\0';
					break;
				}
				else patronymic[j - length] = str[j];
				if (str[j + 1] == '\0')patronymic[j - length + 1] = '\0';
				break;
			}
			}
			if (str[j] == ' ') {
				length = j + 1;
				flag++;
			}
		}
		head = student_init(head, strcopy(name), strcopy(surname), strcopy(patronymic));
	}
	return head;
}

void restud(FILE *f, students* heads, groups* headg, char* filename, char* group) {//���������� � ������ ����������
	char *newfile = "a.txt", *str = malloc(sizeof(char) * 256);
	FILE *f1 = fopen(newfile, "w+");//����� ���� ��� ����������
	write_group_list(f1, headg);//����� ����� ������ �����
	groups* t = headg->next;
	while (t) {//����� ���� ������
		if (strcmp(t->id, group) == 0)break;
		t = t->next;
	}
	write_students_from_to(f, f1, headg->next->add-1, t->add);//����� ���� ��������� �� ������������� ������
	students *p = heads->next;
	while (p) {
		strcpy(str, p->surname);
		strcat(str, " ");
		strcat(str, p->name);//������������ 
		strcat(str, " ");
		strcat(str, p->patronymic);
		if(p->next || t->next)strcat(str, "\n");
		fputs(str, f1);
		p = p->next;
	}
	if (t->next)
		write_students_from_to(f, f1, t->next->add, strings_count(f)+1);//����� ������������ ������
	fclose(f1);//��������� ��� �����, ������ �������, ����� ��������������� � ������
	fclose(f);
	remove(filename);
	rename(newfile, filename);
	f = fopen(filename, "ab+");
}

int ins_student(FILE *f, char* group, char* surname, char* name, char* patronymic, char* filename) {
	groups* headg = form_groups_list(f), *t = headg->next;//��������� ������ �����
	while (t) {
		if (strcmp(t->id, group) == 0)break;//���� ����
		t = t->next;
	}
	if (!t)return 0;//���� �������� ������ ���
	t = t->next;
	while (t) {
		t->add++;
		t = t->next;
	}
	int from=get_ind(f, group), to;//������ ���������
	char *next = get_next_group(f, group);//���� ������ ��� ��������� ����� ���������
	if (next)to = get_ind(f, next);//���� ������ �� ���������
	else to = strings_count(f);//���� ������ ���������
	students *heads = from==to?form_students_list(f, from, to-1): form_students_list(f, from, to+1);//����� ������ ���������
	heads = student_init(heads, name, surname, patronymic);//���������� ������
	restud(f, heads, headg, filename, group);//������������
	return 1;
}

int del_student(FILE *f, char* group, char* surname, char* name, char* patronymic, char* filename) {
	groups* headg = form_groups_list(f), *t = headg->next;//��������� ������ �����
	while (t) {
		if (strcmp(t->id, group) == 0)break;
		t = t->next;
	}
	if (!t)return 0;//���� �������� ������ ���
	int from = get_ind(f, group), to;//������ ���������
	char *next = get_next_group(f, group);//���� ������ ��� ����� ���������
	if (next)to = get_ind(f, next);//���� �� ��������� ������
	else to = strings_count(f);//���� ���������
	students *heads = (from == to ? form_students_list(f, from, to) : (t->next? form_students_list(f, from, to ): form_students_list(f, from, to+1))), *p=heads;
	int flag = 0;
	while (p->next) {
		if (strcmp(p->next->name, name) == 0 && strcmp(p->next->surname, surname) == 0 && strcmp(p->next->patronymic, patronymic) == 0) {//���� ����������
			p->next = p->next->next;//�������
			flag++;
			break;
		}
		p = p->next;
	}
	if (flag == 0)return 0;//���� ������ ���
	t = t->next;
	while (t) {//��������� ����� ����������� ����� �� 1
		t->add--;
		t = t->next;
	}
	restud(f, heads, headg, filename, group);//������������
	return 1;
}

int print_student(FILE *f, char* group){//������ ��������� ������
	int from = get_ind(f, group), to;
	char *next = get_next_group(f, group);
	if (next)to = get_ind(f, next);
	else to = strings_count(f);
	if (from == -1)return 0;
	students* head = form_students_list(f, from, to), *p=head->next;
	while (p) {
		printf("%s %s %s\n", p->surname, p->name, p->patronymic);
		p = p->next;
	}
	return 1;
}

void controle_method() {
	printf("������� ���� � �����\n");
	char* filename=malloc(sizeof(char)*256), c=' ',*group= malloc(sizeof(char) * 256), *s=malloc(sizeof(char)*256), *name = malloc(sizeof(char) * 256),*surname = malloc(sizeof(char) * 256),*patronymic = malloc(sizeof(char) * 256);
	FILE *f;
	while (!f) {
		gets(filename);
		f = fopen(filename, "r");
		if (f)break;
		printf("���� �� ����������");
	} 
	freopen(filename, "ab+", f);
	rewind(f);
	while (c!='0') {
		printf("�������� ��������:\n1)������� ������\n2)������� ������\n3)���������� ������ �����\n4)������� ��������\n5)������� ��������\n6)������ ������ ��������� ������\n7)�����\n");
		scanf("%c", &c);
		switch (c) {
		case '1': {
			printf("������� �������� ������\n");
			scanf("%s", group);
			if (ins_group(f, group, filename))printf("������ ���������\n");
			else printf("������ �� ���������\n");
			gets(group);
			break;
		}
		case '2': {
			printf("������� �������� ������\n");
			scanf("%s", group);
			if (del_group(f, group, filename))printf("������ �������\n");
			else printf("������ ��� ���������\n");
			gets(group);
			break;
		}
		case '3': {
			if (print_groups(f))printf("������ ����������\n");
			else printf("������ �� ����������\n");
			gets(group);
			break;
		}
		case '4': {
			printf("������� �������� ������\n");
			scanf("%s", group);
			gets(s);
			printf("������� �������\n");
			scanf("%s", surname);
			gets(s);
			printf("������� ���\n");
			scanf("%s", name);
			gets(s);
			printf("������� ��������\n");
			scanf("%s", patronymic);
			gets(s);
			if (ins_student(f, group, surname, name, patronymic, filename))printf("������� ��������\n");
			else printf("������� �� ��������\n");
			break;
		}
		case '5': {
			printf("������� �������� ������\n");
			scanf("%s", group);
			gets(s);
			printf("������� �������\n");
			scanf("%s", surname);
			gets(s);
			printf("������� ���\n");
			scanf("%s", name);
			gets(s);
			printf("������� ��������\n");
			scanf("%s", patronymic);
			gets(s);
			if (del_student(f, group, surname, name, patronymic, filename))printf("������� ������\n");
			else printf("������� �� ������\n");
			break;
		}
		case '6': {
			printf("������� �������� ������\n");
			scanf("%s", group);
			if (print_student(f,group))printf("�������� ����������\n");
			else printf("�������� �� ����������\n");
			gets(group);
			break;
		}
		case '7': {
			fclose(f);
			return;
		}
		}
	}
}

void main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "rus");
	controle_method();	
}
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

groups* group_init(groups *head, char* id, int add) {//инициальизация элемента списка с сохранением упорядоченности
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
		if (strcmp(t->next->surname, surname) > 0) break;//если фамилия нового раньше фамилии старого
		if (strcmp(t->next->surname, surname) == 0)
			if (strcmp(t->next->name, name) > 0)break;//если фамилии одинаковы, но имена различны
			else if (strcmp(t->next->name, name) == 0)
				if (strcmp(t->next->patronymic, patronymic) > 0)break;//если ФИ одинаковы, но отчества различны
		t = t->next;
	}
	p->next = t->next;
	t->next = p;
	return head;
}

int find_symbol(char* s, char c) {//индекс первого вхождения символа
	char *ach;
	ach = strchr(s, c);
	return (ach - s );
}

int strings_count(FILE *f) {//подсчет всего кол-ва строк
	int count = 0;
	fseek(f, 0, 0);
	char str[256];
	while (!feof(f)) {
		fgets(str, 256, f);
		count++;
	}
	return count;
}

void fgoton(FILE* f, int n) {//спуск в файле на n-ую строку
	char* str = malloc(sizeof(char) * 256);
	fseek(f, 0, 0);
	for (int i = 0; i < n; i++)
		fgets(str, 256, f);
}

int get_ind(FILE *f, char* group) {//взятие позиции начала студентов группы
	int id;
	fseek(f, 0, 0);
	char str[256], num[256];
	while (!feof(f)) {//
		fgets(str, 256, f);//опускаемся, пока не найдем строку с именем group
		if (strstr(str, group) != NULL)break;//
	}
	if (feof(f))return -1;//если все-таки ничего не нашли
	int a = find_symbol(str, ' ');//первый пробел
	for (int i = a+1; i < strlen(str)-1; i++)
		num[i-a-1] = str[i];//посимвольно копируем
	id = atoi(num);
	return id;
}

char *get_next_group(FILE *f, char* group) {//следующая группа
	char *str=malloc(sizeof(char)*256);
	fseek(f, 0, 0);
	fgets(str, 256, f);
	int till = get_ind(f, str)-1;
	fseek(f, 0, 0);
	while (!feof(f)) {
		fgets(str, 256, f);//опускаемся, пока не найдем строку с именем group
		if (strstr(str, group) != NULL)break;//
		till--;//счетчик первого студента
		if (till <= 1)return NULL;
	}
	fgets(str, 256,f);//наша группа
	return str;
}

char *get_group(FILE *f, int id) {//взятие названия группы на н-ой строке
	char *str=malloc(sizeof(char)*256);
	fseek(f, 0, 0);
	fgoton(f, id);//опускаеся до нужной группы
	fgets(str, 256, f);//берем всю строку с ней
	char *group=malloc(256*sizeof(char));
	int index = find_symbol(str, ' ');//индекс первого пробела
	for (int i = 0; i < index; i++) {
		group[i] = str[i];//посимвольно копируем
	}
	group[index] = '\0';
	return group;
}

char* strcopy( char* s2) {//копирование символьного массива посимвольно
	char *s = malloc(sizeof(s2));
	for (int i = 0; i < strlen(s2); i++)
		s[i] = s2[i];
	s[strlen(s2)] = '\0';
	return s;
}

groups* form_groups_list(FILE *f) {//создание списка групп
	char* name, *str = malloc(256 * sizeof(str));
	groups *head = (groups*)malloc(sizeof(groups));//
	head->id = "";//
	head->add = 0;//инициализация головы
	head->next = NULL;//
	fseek(f, 0, 0);
	fgets(str, 256, f);
	int till = get_ind(f, str), num, save;//первый стунде=последняя группа+1
	fseek(f, 0, 0);
	for (int i = 0; i < till - 1; i++) {
		name = get_group(f, i);
		num = get_ind(f, name);//инициализируем элемент за элементом
		head = group_init(head, name, num);
	}
	return head;
}

void write_group_list(FILE *f1, groups* head) {//запись нового списка групп
	char *str = malloc(sizeof(char) * 256), *name = malloc(sizeof(char) * 256);
	groups*	t = head->next;
	while (t != NULL) {
		name = strcopy(t->id);
		strcat(name, " ");
		strcat(name, itoa(t->add, str, 10));//формируем строку для записи
		strcat(name, "\n");
		fputs(name, f1);
		t = t->next;
	}
}

void write_students_from_to(FILE *f, FILE *f1, int from, int to) {//запись неизменившихся студентов
	char* str = malloc(sizeof(char) * 256);
	rewind(f);
	fgoton(f, from);
	for (int i = from; i < to - 1; i++) {
		fgets(str, 256, f);
		if (str[strlen(str) - 1] == '\n') {//случай непоследнего стундента в списке
			str[strlen(str) - 2] = '\n';
			str[strlen(str) - 1] = '\0';
		}
		fputs(str, f1);
	}
}

void regrouop(FILE *f, groups* head, char* filename, groups* deleted_group, int groupsize) {//перезаписывает файл с учетом новых списков, но оставляет всех студентов
	char *newfile = "a.txt", *str=malloc(sizeof(char)*256), *name = malloc(sizeof(char) * 256);
	FILE *f1 = fopen(newfile, "w+");//новый файл для перезаписи
	write_group_list(f1, head);
	if (deleted_group == NULL) {//если удаляемой группы нет
		write_students_from_to(f, f1, head->next->add - 2, strings_count(f) + 1);
	}
	else {//или
		write_students_from_to(f, f1, head->next->add, deleted_group->add);//записываем всех студентов до удаляемых
		write_students_from_to(f, f1, deleted_group->next->add + groupsize, strings_count(f) + 1);
	}
	fclose(f1);//закрываем оба файла, старый удаляем, новый переименовываем в старый
	fclose(f);
	remove(filename);
	rename(newfile, filename);
	f = fopen(filename, "ab+");
}

int ins_group(FILE *f, char* group, char* filename) {//вставка пустой группы
	char *str = malloc(256 * sizeof(char)), *name;
	fseek(f, 0, 0);
	groups *head = form_groups_list(f);//формируем список групп
	groups *t = (groups*)malloc(sizeof(groups));
	t->id = group;
	groups *p = head;
	int flag = 0;
	while (p->next != NULL) {
		p->next->add++;//увеличиваем аддрес всех стундентов групп на 1
		if (strcmp(p->next->id, t->id) == 0) 
			return 0; 
		if (strcmp(p->next->id, t->id) > 0 && flag==0){//начало студентов новой группы=начало студентов следующей за ней группы, тк в новой студентов 0
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
	regrouop(f, head, filename, NULL, 0);//перезапишем
	return 1;
}

int del_group(FILE *f, char* group, char* filename) {//удаление группы
	groups *head = form_groups_list(f), *deleted_group, *t = head;
	while (t->next != NULL) {
		if (strcmp(t->next->id, group) == 0)break;//проверим, есть ли такая группа
		t = t->next;
	}
	if (t->next) {//если есть
		deleted_group = t->next;//запомним ее адресс
		t = head;
		int decrease = 1;//для групп "до" уменьшение адресса студентов = 1 
		while (t->next) {
			t->next->add-=decrease;
			if (t->next == deleted_group) {
				if(t->next->next!=NULL)decrease +=(t->next->next->add-deleted_group->add-1);//если это была не последняя группа, то уменьшение адреса = длине группы+1
				t->next = deleted_group->next;//сотрем из основного списка
				deleted_group->add++;//вернем старое значение, тк оно указывает на начало след. группы
			}
			else t = t->next;
		}
		regrouop(f, head, filename, deleted_group, decrease-1);
		return 1;
	}//если нет
	return 0;
}

int print_groups(FILE *f) {//печать групп студентов
	groups *head = form_groups_list(f), *t = head->next;;
	if (!t)return 0;
	while (t->next != NULL) {
		printf("Группа %s, кол-во студентов %d\n", t->id, t->next->add - t->add);
		t = t->next;
	}
	printf("Группа %s, кол-во студентов %d\n", t->id, strings_count(f) - t->add+1);
	return 1;
}

students* form_students_list(FILE *f, int from, int to) {//формирование списка студентов
	students* head, *t;
	int flag, length;
	char *str = malloc(sizeof(char) * 256), *name = malloc(sizeof(char) * 256), *surname = malloc(sizeof(char) * 256), *patronymic = malloc(sizeof(char) * 256);
	head = malloc(sizeof(students));
	head->name = "";
	head->surname = "";//задание 
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

void restud(FILE *f, students* heads, groups* headg, char* filename, char* group) {//перезапись с новыми студентами
	char *newfile = "a.txt", *str = malloc(sizeof(char) * 256);
	FILE *f1 = fopen(newfile, "w+");//новый файл для перезаписи
	write_group_list(f1, headg);//пишим новый список групп
	groups* t = headg->next;
	while (t) {//ловим нашу группу
		if (strcmp(t->id, group) == 0)break;
		t = t->next;
	}
	write_students_from_to(f, f1, headg->next->add-1, t->add);//пишем всех студентов до изменишвиейся группы
	students *p = heads->next;
	while (p) {
		strcpy(str, p->surname);
		strcat(str, " ");
		strcat(str, p->name);//переписываем 
		strcat(str, " ");
		strcat(str, p->patronymic);
		if(p->next || t->next)strcat(str, "\n");
		fputs(str, f1);
		p = p->next;
	}
	if (t->next)
		write_students_from_to(f, f1, t->next->add, strings_count(f)+1);//после изменившейся группы
	fclose(f1);//закрываем оба файла, старый удаляем, новый переименовываем в старый
	fclose(f);
	remove(filename);
	rename(newfile, filename);
	f = fopen(filename, "ab+");
}

int ins_student(FILE *f, char* group, char* surname, char* name, char* patronymic, char* filename) {
	groups* headg = form_groups_list(f), *t = headg->next;//формируем список групп
	while (t) {
		if (strcmp(t->id, group) == 0)break;//ишем нашу
		t = t->next;
	}
	if (!t)return 0;//если заданной группы нет
	t = t->next;
	while (t) {
		t->add++;
		t = t->next;
	}
	int from=get_ind(f, group), to;//начало студентов
	char *next = get_next_group(f, group);//след группа для получения конца студентов
	if (next)to = get_ind(f, next);//если группа не последняя
	else to = strings_count(f);//если группа последняя
	students *heads = from==to?form_students_list(f, from, to-1): form_students_list(f, from, to+1);//новый список студентов
	heads = student_init(heads, name, surname, patronymic);//добаволяем нового
	restud(f, heads, headg, filename, group);//переписываем
	return 1;
}

int del_student(FILE *f, char* group, char* surname, char* name, char* patronymic, char* filename) {
	groups* headg = form_groups_list(f), *t = headg->next;//формируем список групп
	while (t) {
		if (strcmp(t->id, group) == 0)break;
		t = t->next;
	}
	if (!t)return 0;//если заданной группы нет
	int from = get_ind(f, group), to;//начало студентов
	char *next = get_next_group(f, group);//след группа для конца студентов
	if (next)to = get_ind(f, next);//если не последняя группа
	else to = strings_count(f);//если последняя
	students *heads = (from == to ? form_students_list(f, from, to) : (t->next? form_students_list(f, from, to ): form_students_list(f, from, to+1))), *p=heads;
	int flag = 0;
	while (p->next) {
		if (strcmp(p->next->name, name) == 0 && strcmp(p->next->surname, surname) == 0 && strcmp(p->next->patronymic, patronymic) == 0) {//ищем удаляемого
			p->next = p->next->next;//удаляем
			flag++;
			break;
		}
		p = p->next;
	}
	if (flag == 0)return 0;//если такого нет
	t = t->next;
	while (t) {//уменьшаем адрес последующих групп на 1
		t->add--;
		t = t->next;
	}
	restud(f, heads, headg, filename, group);//переписываем
	return 1;
}

int print_student(FILE *f, char* group){//печать студентов группы
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
	printf("Введите путь к файлу\n");
	char* filename=malloc(sizeof(char)*256), c=' ',*group= malloc(sizeof(char) * 256), *s=malloc(sizeof(char)*256), *name = malloc(sizeof(char) * 256),*surname = malloc(sizeof(char) * 256),*patronymic = malloc(sizeof(char) * 256);
	FILE *f;
	while (!f) {
		gets(filename);
		f = fopen(filename, "r");
		if (f)break;
		printf("Файл не существует");
	} 
	freopen(filename, "ab+", f);
	rewind(f);
	while (c!='0') {
		printf("Выберите действие:\n1)Добавит группу\n2)Удалить группу\n3)Напечатать список групп\n4)Добавит студента\n5)Удалить студента\n6)Печать списка студентов группы\n7)Выход\n");
		scanf("%c", &c);
		switch (c) {
		case '1': {
			printf("Введите название группы\n");
			scanf("%s", group);
			if (ins_group(f, group, filename))printf("Группа добавлена\n");
			else printf("Группа не добавлена\n");
			gets(group);
			break;
		}
		case '2': {
			printf("Введите название группы\n");
			scanf("%s", group);
			if (del_group(f, group, filename))printf("Группа удалена\n");
			else printf("Группа уже отсутвует\n");
			gets(group);
			break;
		}
		case '3': {
			if (print_groups(f))printf("Группы напечатаны\n");
			else printf("Группы не напечатаны\n");
			gets(group);
			break;
		}
		case '4': {
			printf("Введите название группы\n");
			scanf("%s", group);
			gets(s);
			printf("Введите Фамилию\n");
			scanf("%s", surname);
			gets(s);
			printf("Введите Имя\n");
			scanf("%s", name);
			gets(s);
			printf("Введите Отчество\n");
			scanf("%s", patronymic);
			gets(s);
			if (ins_student(f, group, surname, name, patronymic, filename))printf("Студент добавлен\n");
			else printf("Студент не добавлен\n");
			break;
		}
		case '5': {
			printf("Введите название группы\n");
			scanf("%s", group);
			gets(s);
			printf("Введите Фамилию\n");
			scanf("%s", surname);
			gets(s);
			printf("Введите Имя\n");
			scanf("%s", name);
			gets(s);
			printf("Введите Отчество\n");
			scanf("%s", patronymic);
			gets(s);
			if (del_student(f, group, surname, name, patronymic, filename))printf("Студент удален\n");
			else printf("Студент не удален\n");
			break;
		}
		case '6': {
			printf("Введите название группы\n");
			scanf("%s", group);
			if (print_student(f,group))printf("Студенты напечатаны\n");
			else printf("Студенты не напечатаны\n");
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
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define ifm if(master!=0) 
#define ifs if(slave!=0)
#define ifi if(index!=0)
#define mop fopen_s(&master, "master.fl", "r+");
#define iop fopen_s(&index, "indexes.ind", "r+");
#define sop fopen_s(&slave, "slave.fl", "r+");
	FILE* master;
	FILE* slave;
	FILE* index;
	void StartWork()
	{
		fopen_s(&index, "indexes.ind", "w");
		fopen_s(&master, "master.fl", "w");
		fopen_s(&slave, "slave.fl", "w");
		char a1[147] = "1,";
		char a[2] = "\0\n";
		ifm fwrite(a1, 1, 147, master);
		ifs fwrite(a1, 1, 147, slave);
		ifm fwrite(a, 2, 1, master);
		ifs fwrite(a, 2, 1, slave);
		ifm fclose(master);
		ifs fclose(slave);
		ifi fclose(index);
	}
	typedef struct
	{
		char Id[7];
		char next_addr[13];
		char AdressId[7];
	    char garb[2];
		char date[20];
	} Concert;
	typedef struct
	{
		char next_addr[13];
		char Id[7];
		char garb[2];
		char city[50];
		char Adress[50];
		char details[50];
	}
	Adress;
	void Print_a( Adress a1)
	{
		printf("%s %s %s %s %s %s\n", a1.Id, a1.city, a1.Adress, a1.details, a1.garb, a1.next_addr);
	}
	void Print_c( Concert a1)
	{
		printf("%s %s %s %s %s\n", a1.Id, a1.date, a1.AdressId, a1.garb, a1.next_addr);
	}
#define C_S sizeof(Concert)
#define AD_S sizeof(Adress)
	
	
	void ut_m()
	{
		mop;
		char line[150] = "\0";
		ifm fread(line, 1, 149, master);
		printf("%s\n", line);
		while (1)
		{
			 Adress a1;
			ifm fread(&a1, AD_S, 1, master);
			ifm if (feof(master) != 0) break;
			Print_a(a1);
		}
		printf("\n");
		ifm fclose(master);
	}
	
	
	void ut_s()
	{
		sop;
		char line[150] = "\0";
		ifs fread(line, 1, 149, slave);
		printf("%s\n", line);
		while (1)
		{
			 Concert a1;
			ifs fread(&a1, C_S, 1, slave);
			ifs if (feof(slave) != 0) break;
			Print_c(a1);
		}
		printf("\n");
		ifm fclose(slave);
	}


int compIndx(const void* a, const void* b)
{
	const char* a1 = a;
	const char* b1 = b;
	if (a1[0] == '\0') if (b1[0] != '\0') return 1;
	if (b1[0] == '\0') if (a1[0] != '\0') return -1;
	if (b1[0] == '\0' && a1[0] == '\0') return 0;
	return strcmp(a1, b1);
}


int compTbl(const void* a, const void* b)
{
	char* idx_table = b;
	char* digit= a;
	char* pos1 = strchr(idx_table, ' ');
	int pos = pos1 - idx_table;
	char index[6];
	strncpy_s(index, 6, idx_table, pos);
	if (idx_table[0] == '\0') return -1;
	return strcmp(digit, index);
}


int erase(char str[20][13], int id, int size)
{
	char digit[6];
	sprintf_s(digit, 6, "%d", id);
	for (int i = 0; i < size; i++)
		if (compTbl(digit, str[i]) == 0)
		{
			for (int j = i; j < size; j++)
				if (j + 1 != 20) strcpy_s(str[j],13, str[j + 1]);
				else str[j][0] = '\0';
			size--;
			break;
		}
	return size;
}


void initAdress( Adress *a, int Id, char city[50], char Adress[50], char details[50], int garb, long int next_addr)
{
	sprintf_s(a->Id, 7, "%d", Id);
	strcpy_s(a->city,50, city);
	strcpy_s(a->Adress,50, Adress);
	strcpy_s(a->details,50, details);
	sprintf_s(a->garb, 2, "%d", garb);
	sprintf_s(a->next_addr, 13, "%d", next_addr);
}

void initConcert( Concert* a, int Id, int adr_id, char date[20],  int garb, long int next_addr)
{
	sprintf_s(a->Id, 7, "%d", Id);
	sprintf_s(a->AdressId, 7, "%d", adr_id);
	strcpy_s(a->date, 20, date);
	sprintf_s(a->garb, 2, "%d", garb);
	sprintf_s(a->next_addr, 13, "%d", next_addr);
}



void ChangeIndxTbl(char indx[20][13], int size)
{
	fopen_s(&index, "indexes.ind", "w");
	ifi fseek(index, 0L, SEEK_SET);
	for (size_t i = 0; i < size ; i++)
	{
		ifi fwrite(indx[i], 1, 13, index); //Переписываю индексную таблицу в сортированном виде
	}
	ifi fclose(index);
}


int SetMassIdx(int* indexes, char*line, char* file)
{
	FILE* f; 
	fopen_s(&f, file, "r+");
	if (f != 0) fseek(f, 0L, SEEK_SET);
	if (f != 0) fread(line, 1, 150, f);
	char* p_line = line;
	for (size_t i = 0; i < 20; i++)
	{
		indexes[i] = -1; 
	}
	int counter = 0;
	for (size_t i = 0; i < 20; i++)
	{
		if (sscanf_s(p_line, "%d,", indexes) == 0) break;
		while (*p_line != ',' && *p_line != '\0') p_line++; 
		if (*p_line == '\0') break; else
			p_line++;
		counter++;
		indexes++;
	}
	if (f != 0) fclose(f);
	return counter;
}


int SetIdxTbl(char table[20][13])
	{
		for (size_t i = 0; i < 20; i++)
		{
			table[i][0] = '\0';
		}
		fopen_s(&index, "indexes.ind", "r+");
		ifi fseek(index, 0L, SEEK_END);
		int size=0;
		ifi size= ftell(index) / 13;
		ifi fseek(index, 0L, SEEK_SET);
		for (int i = 0; i < size; i++)
		{
			ifi fread(table[i], 1, 13, index);
		}
		ifi fclose(index);
		return size;
	}

int bsMaster(int pos)
	{
		char idx_table[20][13];
		SetIdxTbl(idx_table);
		char digit[6];
		sprintf_s(digit, 6, "%d", pos);
		char *bool = bsearch(digit, idx_table, 20, 13, compTbl);
		if (bool == NULL) return 0;
		int masId = (bool-idx_table)/13;
		int kostyl, weneed;
		sscanf_s(idx_table[masId], "%d %d", &kostyl, &weneed);
		return weneed;
	}  

int LastSlave(int index,  Concert*conc, int a)
{
	long int pos = bsMaster(index);
	mop;
	ifm fseek(master, pos, 0);
	 Adress a1;
	ifm fread(&a1, AD_S, 1, master);
	ifm fclose(master);
	long int pos_slave=number(a1.next_addr), pos_pre_last = 0, pos_last = 0;
		sop;
	while (pos_slave != 0 && conc->garb!=0)
	{
		pos_pre_last=pos_last;
		pos_last = pos_slave;
		ifs 
			fseek(slave, pos_slave, 0);
		ifs 
			fread(conc, C_S, 1, slave);
		pos_slave = number(conc->next_addr);
	}
	ifs fclose(slave);
	return (a==1)? pos_last:pos_pre_last;
}


//Возвращает первый свободный идекс, переписывает строку свободных индексов
int FreeIndx(char* line, int*indexes, char* file) 
{
	int curindx;
	FILE* f;
	fopen_s(&f, file, "r+");
	curindx = indexes[0]; //присваиваю индексу свободный индекс
	char* p_line = line;
	if (indexes[1] == -1) sprintf_s(line, 147, "%d,", curindx+1); //если всего 1 свободный индекс - заменяю первый символ
	for (int i = 1; i < 20; i++)
	{
		if (indexes[i] == -1) break;
		if (sprintf_s(p_line, 147 - (p_line - line), "%d,", indexes[i]) < 0) printf("Error\n"); //записываю в line оставшиеся свободные индексы
		while (*p_line != '\0' && *p_line != ',') p_line++;
		if (*p_line != '\0')     //Двигаю указатель на место после , чтобы записать следующий индекс
			p_line++;
	}
	char a[2] = "\0\n";
	if(f!=0) fseek(f, 0L, SEEK_SET);
	 if(f!=0) fwrite(line, 1, 147, f);
	 if(f!=0) fwrite(a, 1, 2, f);       //вписываю новую строку индексов
	if(f!=0) fclose(f);
	return curindx;
} 


void AddIndex(char* in, char* file)
{
	int index;
	sscanf_s(in, "%d", &index);
	FILE* f;
	fopen_s(&f, file, "r+");
	char line[141];
	if(f!=0) fseek(f, 0, SEEK_SET);
	if (f != 0) fread(line, 1, 140, f);
	char new_line[150];
	sprintf_s(new_line, 147, "%d,%s", index, line);
	if (f != 0) fseek(f, 0, SEEK_SET);
	if (f != 0) fwrite(new_line, 1, 147, f);
	if (f != 0) fclose(f);
}


void insert_m(const char city[50], const char adress[50], const char details[50])
{
	char indx[20][13];
	int size=SetIdxTbl(indx); //сохранение индексной таблицы в оперативу
	int curindx;
	char line[150];
	int indexes[20];
	SetMassIdx(indexes, line,"master.fl");
	curindx=FreeIndx(line, indexes, "master.fl");
	mop;
 Adress c1;
	initAdress(&c1, curindx, city, adress, details, 1, 0);
	ifm fseek(master, 150+(AD_S)*(curindx-1), 0);
	ifm fwrite(&c1, AD_S, 1, master); //Добавляю в конец файла новую запись
	int Cur_pos = 0;
	ifm Cur_pos = ftell(master);
	sprintf_s(indx[size], 13, "%d %d", curindx, Cur_pos-AD_S); //Добавляю в массив индексной таблицы новую запись
	qsort(indx, 20, 13, compIndx);      //Сортирую данные в индексной таблице
	size++;
	ChangeIndxTbl(indx, size);
	ifm fclose(master);
}


void insert_s(int adrID, char date[20])
{
	long int pos_master = bsMaster(adrID);
	if (pos_master == 0) printf("Not found note in master file\n");
	else
	{
		int curindx;
		char line[150];
		int indexes[20];
		SetMassIdx(indexes, line, "slave.fl");
			curindx = FreeIndx(line, indexes, "slave.fl");
		sop;
		 Concert c1;
		initConcert(&c1, curindx, adrID, date, 1, 0);
		long int pos_written = 150 + C_S * (curindx - 1);
		ifs fseek(slave, pos_written, 0);
		ifs fwrite(&c1, C_S, 1, slave);
		ifs fclose(slave);
		 Concert last_c;
		int pos_slave = LastSlave(adrID,&last_c, 1);
		if (pos_slave == 0)
		{
			mop;
			ifm fseek(master, pos_master, 0);
			 Adress ad;
			ifm fread(&ad, AD_S, 1, master);
			ifm fseek(master, pos_master, 0);
			sprintf_s(ad.next_addr,13, "%d", pos_written);
			ifm fwrite(&ad, AD_S, 1, master);
			ifm fclose(master);
		}
		else
		{
			sop;
			ifs fseek(slave, pos_slave, 0);
			ifs fread(&c1, C_S, 1, slave);
			sprintf_s(c1.next_addr,13,"%d", pos_written);
			ifs fseek(slave, pos_slave, 0);
			ifs fwrite(&c1, C_S, 1, slave);
			ifs fclose(slave);
		}
	}
}


int get_m(int index)
{
	char index_table[20][13];
	 Adress a1;
	SetIdxTbl(index_table);
	int pos = bsMaster(index);
	if (pos == 0)
	{
		printf_s("No that note in master file\n");
		return 0;
	}
	mop;
	ifm fseek(master, pos, SEEK_SET);
	ifm fread(&a1, AD_S, 1, master);
	Print_a(a1);
	ifm fclose(master);
	return pos;
}


int get_s(int index)
{
	sop;
	ifs fseek(slave, 0, 2);
	if ((index-1)*C_S+150 > ftell(slave))
	{
		printf("Can't find such note\n");
	ifs fclose(slave);
		return 0;
	}
	long int pos_c = (index - 1) * C_S + 150;
	ifs fseek(slave, pos_c, 0);
	 Concert cnsrt;
	ifs fread(&cnsrt, C_S, 1, slave);
	if(cnsrt.garb[0]=='0') 
	{
		printf("Can't find such note\n");
		ifs fclose(slave);
		return 0;
	}
    Print_c(cnsrt);
	ifs fclose(slave);
	return pos_c;
}


long int number(char* str)
{
	long int a=0;
	sscanf_s(str,"%d", &a);
	return a;
}


void del_s(int index)
{
	
	int pos_c = get_s(index);
	if (pos_c == 0) return;
	 Concert cnsrt;
	sop;
	ifs fseek(slave, pos_c, 0);
	ifs	fread(&cnsrt, C_S, 1, slave);
	ifs fclose(slave);
	long int pos_m = bsMaster(number(cnsrt.AdressId));
	mop;
	 Adress adr;
	fseek(master, pos_m, 0);
	fread(&adr, AD_S, 1, master);
	fclose(master);
	 Concert last_cnsrt;
	long int pos_last = LastSlave(number(cnsrt.AdressId), &last_cnsrt,0);
	 Concert next_cnsrt;
	if (number(cnsrt.next_addr) == 0)
	{
		//struct Concert cur_cnstr;
		//sop;
		//ifs fseek(slave, pos_last, 0);
		//ifs fread(&cur_cnstr, C_S, 1, slave);
		//ifs fclose(slave);
		if (strcmp(last_cnsrt.Id, adr.next_addr)==0)
		{
			cnsrt.garb[0] = '0';
			sop;
			ifs fseek(slave, pos_c, 0);
			ifs	fwrite(&cnsrt, C_S, 1, slave);
			ifs fclose(slave);
			mop;
		     Adress adr;
			long int pos_m = bsMaster(number(cnsrt.AdressId));
			ifm fseek(master, pos_m, 0);
			ifm fread(&adr, AD_S, 1, master);
			sprintf_s(adr.next_addr,"%d", 0) ;
			ifm fseek(master, pos_m, 0);
			ifm fwrite(&adr, AD_S, 1, master);
			ifm fclose(master);
			AddIndex(cnsrt.Id, "slave.fl");
		}
		else
		{
			cnsrt.garb[0] = '0';
			sop;
			ifs fseek(slave, pos_c, 0);
			ifs	fwrite(&cnsrt, C_S, 1, slave);
			ifs fseek(slave, pos_last, 0);
			ifs fread(&cnsrt, C_S, 1, slave);
			sprintf_s(cnsrt.next_addr,13, "%d", 0);
			ifs fseek(slave, pos_last, 0);
			ifs fwrite(&cnsrt, C_S, 1, slave);
			ifs fclose(slave);
			AddIndex(last_cnsrt.Id, "slave.fl");
		}
	}
	else
	{
		sop;
		ifs fseek(slave, number(cnsrt.next_addr), 0);
		ifs fread(&next_cnsrt, C_S, 1, slave);
		ifs fclose(slave);
		if (number(next_cnsrt.next_addr) == 0)
		{
			sop;
			next_cnsrt.garb[0]='0';
			ifs fseek(slave, number(cnsrt.next_addr), 0);
			ifs fwrite(&next_cnsrt, C_S, 1, slave);
			strcpy_s(next_cnsrt.Id, 7, cnsrt.Id);
            next_cnsrt.garb[0] = '1';
			ifs fseek(slave, pos_c, 0);
			ifs	fwrite(&next_cnsrt, C_S, 1, slave);
			ifs fclose(slave);
			AddIndex(last_cnsrt.Id, "slave.fl");
		}
		else
		{
			char lastID[7];
			strcpy_s(lastID, 7,last_cnsrt.Id);
			sop;
			last_cnsrt.garb[0] = '0';
			ifs fseek(slave, (number(last_cnsrt.Id)-1)*C_S+150, 0);
			ifs fwrite(&last_cnsrt, C_S, 1, slave);
			strcpy_s(last_cnsrt.next_addr, 13,  cnsrt.next_addr);
			strcpy_s(last_cnsrt.Id, 7, cnsrt.Id);
			last_cnsrt.garb[0] = '1';
			ifs fseek(slave, pos_c, 0);
			ifs	fwrite(&last_cnsrt, C_S, 1, slave);
			ifs fseek(slave, pos_last, 0);
			ifs fread(&cnsrt, C_S, 1, slave);
			strcpy_s(cnsrt.next_addr,13 , "0");
			ifs fseek(slave, pos_last, 0);
			ifs fwrite(&cnsrt, C_S, 1, slave);
			ifs fclose(slave);
			AddIndex(lastID, "slave.fl");
		}
	}
	/*if (pos_last == 0)
	{
		cnsrt.garb = 0;
		sop;
		ifs fseek(slave, pos_c, 0);
		ifs	fwrite(&cnsrt, C_S, 1, slave);
		ifs fclose(slave);
		mop;
		struct Adress adr;
		ifm fseek(master, posInMaster(cnsrt.AdressId), 0);
		ifm fread(&adr, AD_S, 1, master);
		adr.next_addr = 0;
		ifm fseek(master, 0-AD_S, 1);
		ifm fwrite(&adr, AD_S, 1, master);
		fclose(master);
	}
	else 
	{
		long int pos_pre_last = findLastSlavePos(cnsrt.AdressId, &last_cnsrt, 0);
		if (pos_pre_last == 0)
		{
		sop;
		last_cnsrt.garb = 0;
		ifs fseek(slave, pos_last, 0);
		ifs fwrite(&last_cnsrt, C_S, 1, slave);
		last_cnsrt.next_addr = cnsrt.next_addr;
		last_cnsrt.Id = cnsrt.Id;
		last_cnsrt.garb = 1;
		ifs fseek(slave, pos_c, 0);
		ifs	fwrite(&last_cnsrt, C_S, 1, slave);
		ifs fclose(slave);
		}
	}*/
		
}



void del_m(int index_)
{
	int pos = bsMaster(index_);
	if (pos == 0)
	{
		printf("No such note in file\n");
		return;
	}
	mop;
	 Adress adr;
	ifm fseek(master, pos, 0);
	ifm fread(&adr, AD_S, 1, master);
	ifm fclose(master);
	 Concert cnsrt;
	int slave_pos = LastSlave(index_, &cnsrt, 1);
	while (slave_pos != number(adr.next_addr))
	{
		del_s((slave_pos -150)/C_S+1);
		slave_pos = LastSlave(index_, &cnsrt, 1);
	}
	del_s((slave_pos - 150) / C_S + 1);
	mop;
	fseek(master, pos, SEEK_SET);
	adr.garb[0] = '0';
	fseek(master, pos, SEEK_SET);
	fwrite(&adr, AD_S, 1, master);
	fclose(master);
	AddIndex(adr.Id, "master.fl");
	char indx_[20][13];
	int size=SetIdxTbl(indx_);
	size = erase(indx_, index_, size);
	ChangeIndxTbl(indx_, size);
}


void update_m(int index_, char*field, char new_value[50])
{
	int pos_m = bsMaster(index_);
	if (pos_m == 0)
	{
		printf("No such note in master file(update_m)\n");
		return;
	}
	mop;
	 Adress adr;  
	ifm fseek(master, pos_m, 0);
	ifm fread(&adr, AD_S, 1, master);
	ifm fseek(master, 0-AD_S, 1);
	if (strcmp(field, "city") == 0)
	{
		strcpy_s(adr.city, 50, new_value);
	}
	else
	if (strcmp(field, "adress") == 0)
	{
		strcpy_s(adr.Adress, 50, new_value);
	}
	else if (strcmp(field, "details")==0)
	{
		strcpy_s(adr.details, 50, new_value);
	}
	else
	{
		printf("Didn't find that field\n");
		ifm fclose(master);
		return;
	}
	ifm fwrite(&adr, AD_S, 1, master);
	ifm fclose(master);
}


void update_s(int index_, char* field, char* new_value)
{
	 Concert cnsrt;
	long int pos = get_s(index_);
	if (pos == 0) return;
	sop;
	fseek(slave, pos, 0);
	fread(&cnsrt, C_S, 1, slave);
	fseek(slave, pos, 0);
	if (strcmp(field, "date") == 0)
	{
		strcpy_s(cnsrt.date, 20, new_value);
	}
	else
	{
		printf("Didn't find that field\n");
		ifs fclose(slave);
		return;
	}
	ifs fwrite(&cnsrt, C_S, 1, slave);
	ifs fclose(slave);

}


int calc_m()
{
	mop;
	char line[150];
	int counter = 0;
	ifm	fread(line, 1, 149, master);
		 Adress adr;
	while (1)
	{
		ifm fread(&adr, AD_S, 1, master);
		if (feof(master) == 1) break;
		if (number(adr.garb)==1) counter++;
	}
	ifm fclose(master);
	return counter;
}


int calc_s()
{
	mop;
	char line[150];
		ifs fread(line, 1, 149, slave);
	int counter = 0;
	 Concert cnsrt;
	while (1)
	{
		ifs fread(&cnsrt, C_S, 1, slave);
		if (feof(slave) == 1) break;
		if (cnsrt.garb[0]=='1') counter++;
	}
	ifs fclose(slave);
	return counter;
}


int calc_s_m(int id_master)
{
	int pos_master = bsMaster(id_master);
	if (pos_master == 0)
	{
		return 0;
	}
	mop;
	Adress adr;
	ifm fseek(master, pos_master, 0);
	ifm fread(&adr, AD_S, 1, master);
	ifm fclose(master);
	int pos_slave = number(adr.next_addr);
	if (pos_slave == 0) return 0;
	int counter = 0;
	 Concert cnsrt;
	while (pos_slave != 0)
	{
		sop;
		counter++;
		ifs 
			fseek(slave, pos_slave, 0);
		ifs 
			fread(&cnsrt, C_S, 1, slave);
		pos_slave = number(cnsrt.next_addr);
		ifs fclose(slave);
	}
	ifs fclose(slave);
	return counter;
}


int main()
{
	StartWork();
	 insert_m("X1", "Y1", "Z1");
	 insert_m("X2", "Y2", "Z2");
	 insert_m("X3", "Y3", "Z3");
	 insert_m("X4", "Y4", "Z4");
	 insert_m("X5", "Y5", "Z5"); 
	 insert_s(1, "1_1111");
	 insert_s(1, "1_2222");
	 insert_s(1, "1_3333");
	 insert_s(1, "1_4444");
	 insert_s(2, "2_1111");
	 insert_s(2, "2_2222");
	 insert_s(2, "2_3333");
	 insert_s(2, "2_4444");
	 insert_s(3, "3_1111");
	 insert_s(3, "3_2222");
	 insert_s(3, "3_3333");
	 insert_s(3, "3_4444");
	 del_m(1);
	 ut_m();
	 ut_s();
	 del_s(5); 
	 ut_m();
	 ut_s();
	 insert_m("X6", "Y6", "Z6");
	 insert_s(1, "12341");
	 ut_m();
	 ut_s();
	 update_s(8, "date", "555555");
	 update_m(1, "city", "GGGG");
	 ut_m();
	 ut_s();
	 printf("%d", calc_s_m(3));
	 return 0;
}
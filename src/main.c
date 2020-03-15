#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/* andreas@grellopolis.de, 2013 */

typedef struct
{
  char* vorname;
  char* nachname;
  char* geburtsdatum;
  char* strasse;
  char* plz;
  char* ort;
  char* festnetz;
  char* mobiltelefon;
  char* email;
  char* konto;
  char* blz;
  char* bemerkung;
  char* termin;
}KD;

typedef struct
{
  char* haftpflicht;
  char* hausrat;
  char* kfz;
  char* rechtsschutz;
  char* gebaeude;
  char* leben;
  char* tierhaftpflicht;
}VERSICHERUNG;

char* cut(char* buffer);
KD* mkKD(KD* kd);
void zeigeKD(KD* kd);
void sqlizer(KD* kd);

int main(int argc, char** argv)
{
  KD* kd;

  kd = malloc(sizeof(KD));

  kd = mkKD(kd);

  zeigeKD(kd);

  sqlizer(kd);

  return 0;
}

char* cut(char* buffer)
{
  char* find = strchr(buffer, '\n');

  if(find)
    *find = '\0';

  return buffer;
}

KD* mkKD(KD* kd)
{
  int i;
  char* buffer;

  buffer = malloc(sizeof(char) * 100);

  char* labels[] = {"Vorname: ", "Nachname: ", "Geburtsdatum: ", "Straße: ",
		    "PLZ: ", "Ort: ", "Telefon (Festnetz): ", "Mobil: ",
		    "E-Mail: ", "Kto.-Nr.: ", "BLZ: ", "Bemerkungen: ", "Termin: "};

  char* kunde[] = {kd->vorname, kd->nachname, kd->geburtsdatum, kd->strasse, kd->plz, kd->ort, kd->festnetz, kd->mobiltelefon, kd->email,
		   kd->konto, kd->blz, kd->bemerkung, kd->termin};

  for(i = 0; i < 13; i++)
    {
      kunde[i] = malloc(sizeof(char) * 100);
      printf("%s", labels[i]);
      fgets(buffer, 99, stdin);
      buffer = cut(buffer);
      kunde[i] = malloc(sizeof(char) * 100);
      strncpy(kunde[i], buffer, 99);
    }

  kd->vorname = kunde[0];
  kd->nachname = kunde[1];
  kd->geburtsdatum = kunde[2];
  kd->strasse = kunde[3];
  kd->plz = kunde[4];
  kd->ort = kunde[5];
  kd->festnetz = kunde[6];
  kd->mobiltelefon = kunde[7];
  kd->email = kunde[8];
  kd->konto = kunde[9];
  kd->blz = kunde[10];
  kd->bemerkung = kunde[11];
  kd->termin = kunde[12];

  return kd;
}

void zeigeKD(KD* kd)
{
  printf("\n%s %s\n", kd->vorname, kd->nachname);
  printf("%s", kd->geburtsdatum);
  printf("\n%s", kd->strasse);
  printf("\nD-%s %s", kd->plz, kd->ort);
  printf("\n%s", kd->festnetz);
  printf("\n%s", kd->mobiltelefon);
  printf("\n%s", kd->email);
  printf("\n%s", kd->konto);
  printf("\n%s", kd->blz);
  printf("\n%s", kd->bemerkung);
  printf("\n%s\n", kd->termin);
}

void sqlizer(KD* kd)
{
  sqlite3* db;

  sqlite3_stmt* stmt;

  char* sql;

  const char* tail;

  char* kd_lst[13];

  int i, j;;

  sqlite3_open("kunden.db", &db);

    sql = "create table if not exists kunden(id integer primary key, "
          "vorname text, nachname text, geburtsdatum text, strasse text, "
          "plz text, ort text, telefon text, mobil text, email text, "
          "kontonummer text, blz text, bemerkungen text, termin text, "
      "zeit not null default current_timestamp, lokalzeit date default (datetime('now', 'localtime')));";

    sqlite3_exec(db, sql, NULL, NULL, NULL);

      sql = "insert into kunden (vorname, nachname, geburtsdatum, "
	    "strasse, plz, ort, telefon, mobil, email, kontonummer, "
	    "blz, bemerkungen, termin) values(?, ?, ?, ?, ?, ?, ?, "
	"?, ?, ?, ?, ?, ?);";

      sqlite3_prepare(db, sql, strlen(sql), &stmt, &tail);

      kd_lst[0] = kd->vorname;
      kd_lst[1] = kd->nachname;
      kd_lst[2] = kd->geburtsdatum;
      kd_lst[3] = kd->strasse;
      kd_lst[4] = kd->plz;
      kd_lst[5] = kd->ort;
      kd_lst[6] = kd->festnetz;
      kd_lst[7] = kd->mobiltelefon;
      kd_lst[8] = kd->email;
      kd_lst[9] = kd->konto;
      kd_lst[10] = kd->blz;
      kd_lst[11] = kd->bemerkung;
      kd_lst[12] = kd->termin;

      for(i = 0, j = 1; i < 13; i++, j++)
	sqlite3_bind_text(stmt, j, kd_lst[i], strlen(kd_lst[i]), 0);

      sqlite3_step(stmt);

      sqlite3_finalize(stmt);

      sqlite3_close(db);
}

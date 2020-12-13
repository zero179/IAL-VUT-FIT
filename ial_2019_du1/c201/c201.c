
/* c201.c *********************************************************************}
{* Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**                                              Úpravy: Kamil Jeřábek, září 2020
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu tList.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ tList:
**
**      DisposeList ... zrušení všech prvků seznamu,
**      InitList ...... inicializace seznamu před prvním použitím,
**      InsertFirst ... vložení prvku na začátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zruší první prvek seznamu,
**      PostDelete .... ruší prvek za aktivním prvkem,
**      PostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na další prvek seznamu,
**      Active ........ zjišťuje aktivitu seznamu.
**
** Při implementaci funkcí nevolejte žádnou z funkcí implementovaných v rámci
** tohoto příkladu, není-li u dané funkce explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam předá
** někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c201.h"

int errflg;
int solved;

void Error() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globální proměnná -- příznak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    L->Act=NULL;
    L->First=NULL;
    
}

void DisposeList (tList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Veškerá paměť používaná prvky seznamu L bude korektně
** uvolněna voláním operace free.
***/
    tElemPtr variable;
    while((variable=L->First)){// prvy prvok je v pomocnej premennej aby sa dal uvolnit
        L->First=variable->ptr;// druhy prvok sa stane prvym
        free(variable);// uvolnime
    }
    L->First=NULL;
    L->Act=NULL;
}

void InsertFirst (tList *L, int val) {
/*
** Vloží prvek s hodnotou val na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci Error().
**/
    tElemPtr new_item=malloc(sizeof(struct tElem));// alokacia
    if (new_item==NULL)// ak je chybna alokacia tak..
    {
        Error();
    }
    else
    {
        new_item->data=val;// data sa nahrali
        new_item->ptr=L->First;// prvy prvok sa stane druhym
        L->First=new_item;// pomocny prvok sa stane prvym
    }
}

void First (tList *L) {
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný příkaz, aniž byste testovali,
** zda je seznam L prázdný.
**/
    L->Act = L->First;// nastavi aktivny ukazatel ako prvy
}

void CopyFirst (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/
    if (L->First==NULL)// ak je prazdny...
    {
        Error();// nastane error
    }
    else// v pripade, ze prazdny nieje
    {
        *val = L->First->data; //ulozime do val hodnotu
    }
}
void DeleteFirst (tList *L) {
/*
** Zruší první prvek seznamu L a uvolní jím používanou paměť.
** Pokud byl rušený prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/
    if(L->First==NULL)// Ak je zoznam prazdny tak da return
    {
        return;
    }
    if (L->First == L->Act)// Ak je zoznam aktivny
    {
        L->Act = NULL;// nastavi sa na neaktivny
    }
    tElemPtr temp = L->First->ptr; // Ulozim ukazatela na prvy prvok
    free(L->First);// Uvolni pamat prvemu prvku
    L->First = temp;// Nastavim aby first ukatoval na uz teraz prvy prvok
}

void PostDelete (tList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem a uvolní jím používanou paměť.
** Pokud není seznam L aktivní nebo pokud je aktivní poslední prvek seznamu L,
** nic se neděje.
**/
    if(L->Act == NULL)// ak zoznam nieje aktivny
    {
        return;
    }
    else if(L->Act->ptr == NULL)// ak aktivny je posledny
    {
        return;
    }
    else
    {
        tElemPtr new_item = L->Act->ptr;// prvok za aktivnym ulozime do pomocnej premennej
        L->Act->ptr = L->Act->ptr->ptr; // druhy prvok za aktivnym bude prvy prvok za aktivnym
        free(new_item);
    }
    
}

void PostInsert (tList *L, int val) {
/*
** Vloží prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje!
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** zavolá funkci Error().
**/
    if(L->Act == NULL)// ak zoznam nieje aktivny tak
    {
        return;
    }
    else// ak je zoznam aktivny tak
    {
        tElemPtr new_item = malloc(sizeof(struct tElem));// alokujeme pamat
        if(new_item == NULL)
        {
            Error();// chyba pri alokacii nastala
        }
        else //nenastala chyba pri alokacii
        {
            new_item->data = val;// ulozime data
            new_item->ptr = L->Act->ptr;// prvok po aktivnom sa stane nasledujucim po pomocnom
            L->Act->ptr = new_item;//pomocny sa stane nasledujuci po aktivnom
        }
    }
}

void Copy (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/
    if(L->Act == NULL)// ak neni aktivna tak
    {
        Error();// vola chybovu hlasku
    }
    else// ak je aktivna tak
    {
        *val = L->Act->data;// do premennej val ulozime hodnotu aktualneho prvku
    }
}

void Actualize (tList *L, int val) {
/*
** Přepíše data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedělá nic!
**/
    if(L->Act != NULL) //ak je zoznam aktivny tak
    {
        L->Act->data = val; //prepiseme data
    }
}

void Succ (tList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
** Pokud není předaný seznam L aktivní, nedělá funkce nic.
**/
    if (L->Act == NULL)// ak nieje zoznam aktivny tak vratime
    {
        return;
    }
    L->Act = L->Act->ptr;
    
}

int Active (tList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Tuto funkci je vhodné implementovat jedním příkazem return.
**/
    return(L->Act != NULL);
}

/* Konec c201.c */

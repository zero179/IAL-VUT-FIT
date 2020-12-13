
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2019
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    //vsetky ukazatale na NULL nastavit
    L->First=NULL;
    L->Act=NULL;
    L->Last=NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
    tDLElemPtr tDLElem = L->First;
    while (L->First != NULL)
    {
        L->First = L->First->rptr;
        free(tDLElem);
        tDLElem = L->First;
    }
    //nastavime vsetky ukazovatele na NULL
    L -> First = NULL;
    L -> Last = NULL;
    L -> Act = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	//alokacia pamati pre novy prvok
    tDLElemPtr NewEL;
    if ((NewEL = malloc(sizeof(struct tDLElem))) == NULL) {
        DLError();
        return;
    }
    
    NewEL->data=val;
    
    if(L -> First == NULL)
    {
        L -> First = NewEL;
        L -> Last = NewEL;
        NewEL -> lptr = NULL;
        NewEL -> rptr = NULL;
    }
    else
    {
        L->First->lptr = NewEL;
        NewEL->lptr = NULL;
        NewEL->rptr = L->First;
        L->First = NewEL;
    }
    
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    //alokacia pamati pre novy prvok
    tDLElemPtr NewEL;
    if ((NewEL = malloc(sizeof(struct tDLElem))) == NULL) {
        DLError();
        return;
    }
 
    NewEL->data=val;
    
    if(L->First == NULL)
    {
        L -> First = NewEL;
        L -> Last = NewEL;
        NewEL ->lptr = NULL;
        NewEL ->rptr = NULL;
    }
    else
    {
        L->Last->rptr = NewEL;
        NewEL->rptr = NULL;
        NewEL->lptr = L->Last;
        L->Last = NewEL;
    }
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->First; //prvy prvok aktivny
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->Last; //posledny prvok aktivny
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if (L->First == NULL)
    {
        DLError();
        return;
    }
    else
    {
        *val = L-> First -> data; // z prveho prvku do val
    }
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if (L->First == NULL)
    {
        DLError();
        return;
    }
    else
    {
        *val = L-> Last -> data; //z posledneho prvku do val
    }
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	
    tDLElemPtr first = L->First;
    if (first) // zoznam neni prazdny
    {
        
        if (first == L->Act)
        {
            L->Act = NULL;
        }

        if (first == L->Last) // prvy je posledny, je tam len jeden prvok v zozname
        {
            L->Last = NULL; // posledny prvok bude NULL
        }
        else //
        {
            first->rptr->lptr = NULL; // lavy ukazatel druheho prvku bude NULL
        }

        
        L->First = first->rptr;
        free(first); // uvolnenie prveho prvku
    }
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 
    tDLElemPtr last = L->Last;
    if (last) // zoznam nesmie byt prazdny
    {
        
        if (last == L->Act)
        {
            L->Act = NULL;
        }

        if (last == L->First) // posledny prvok je prvy
        {
            L->First = NULL; // prvy prvok bude NULL
        }
        else
        {
            last->lptr->rptr = NULL;
        }

        
        L->Last = last->lptr;
        free(last); // uvolnenie osledneho prvku
    }
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
    if (L -> Act != NULL && L-> Act != L-> Last)
    {
        tDLElemPtr NewPtr;
        NewPtr = L -> Act -> rptr;
        L->Act->rptr = NewPtr->rptr; //pravy ukaz. aktiv. prvku bude pravy ukaz.elem.
        if (NewPtr == L->Last) //elem. je posledny prvok
        {
            L->Last = L->Act; //posledny bude aktiv
        }
        else
        {
            NewPtr->rptr->lptr = L->Act; //lavy ukazat. prvku pred elem. bude aktiv. prvok
        }
        free(NewPtr); //uvolnenie elem.
    }
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
    if (L -> Act != NULL && L-> Act != L-> First) //prvok nesmie byt prvy
    {
        tDLElemPtr NewPtr;
        NewPtr = L -> Act -> lptr;
        L->Act->lptr = NewPtr->lptr; //lavy ukaza. aktiv. prvku bude lavy ukaz. elem.
        if (NewPtr == L->First) //elem. je prvy prvok
        {
            L->First = L->Act; // prvy bude aktiv.
        }
        else
        {
            NewPtr->lptr->rptr = L->Act; //pravy ukaz. bude aktiv.
        }
        free(NewPtr); //uvolnenie elem.
    }
			
 
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	//Zoznam nieje aktivny
    if (L->Act == NULL)
    {
        return;
    }

    //Alokujem pamat pre novy prvok
    tDLElemPtr newElem = malloc(sizeof(struct tDLElem));

    //Alokacia sa nepodarila
    if (newElem == NULL)
    {
        DLError();
        return;
    }

    //Do temp si ulozim nasledujuci prvok aktivneho
    tDLElemPtr temp = L->Act->rptr;

    //Vlozim novy prvok do zoznamu
    L->Act->rptr = newElem;

    //Nastavim hodnoty novemu elementu
    newElem->lptr = L->Act;
    newElem->rptr = temp;
    newElem->data = val;

    //temp predchadzuji ukazuje na novy prvok
    if (temp != NULL)
    {
        temp->lptr = newElem;
    }
    else
    {
        L->Last = newElem;
    }
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	//Zoznam nieje aktivny
    if (L->Act == NULL)
    {
        return;
    }

    //Alokujem pamat pre novy prvok
    tDLElemPtr newElem = malloc(sizeof(struct tDLElem));

    //Alokacia sa nepodarila
    if (newElem == NULL)
    {
        DLError();
        return;
    }
    
    //Do temp si ulozim predchadzajuci prvok aktivneho
    tDLElemPtr temp = L->Act->lptr;

    //Vlozim novy prvok do zoznamu
    L->Act->lptr = newElem;

    //Nastavim hodnoty novemu prvku
    newElem->rptr = L->Act;
    newElem->lptr = temp;
    newElem->data = val;

    if (temp != NULL)
    {
        //temp nasledujuci ukazuje na novy prvok
        temp->rptr = newElem;
    }
    else
    {
        L->First = newElem;
    }
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
    if ( L -> Act == NULL)
    {
        DLError();
        return;
    }
    else
    {
        *val = L -> Act -> data;
    }
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
    if (L -> Act != NULL)
    {
        L->Act->data = val; //prepisanie dat
    }
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	if (L -> Act != NULL)
    {
        L -> Act = L -> Act -> rptr; // aktivita na dalsi prvok
    }
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	if (L -> Act != NULL)
    {
        L -> Act = L -> Act -> lptr; //aktivita na predchadzajuci
    }
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	return L->Act == NULL ? 0 : 1;
}

/* Konec c206.c*/

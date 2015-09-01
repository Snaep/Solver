#pragma once

struct Combinator {
	int* values;
	unsigned long long _permutation;
	int _n, _k, _nmk;
	int _b;
	int _i[64];
};

///
///<summary>
///Initialisiert eine angegebene Combinator Instanz.
///</summary>
///<param name="c" direction="in, out">
///Die zu initialisierende Combinator Instanz.
///</param>
///<param name="k" direction="in">
///Anzahl Kombinationsteilnehmer
///</param>
///<param name="values" direction="in">
///Zahlenmenge, aus denen die Kombinationen zusammengesetzt werden sollen
///</param>
///<param name="n" direction="in">
///Anzahl der m�glichen Kombinationsteilnehmer
///</param>
///<returns>
///Ist die Initialisierung erfolgreich wird 0 zur�ckgegeben,
///ansonsten ein Wert kleiner 0
///</returns>
///<remarks>
///Erzeugt k aus n Kombinationen
///-> keine Wiederholungen
///-> keine beachtung der Ordnung (wurde 123 erzeugt wird z.B. 321 nicht mehr erzeugt)
///-> for more refer to https://en.wikipedia.org/wiki/Combination
///
///die Initialisierung schl�gt fehl, wenn c oder values == NULL, k > 64 oder k > n ist
///
///Der an values �bergebene Zeiger muss mindestens die gleiche Lebensdauer wie der Kombinator aufweisen.
///Ein Freigeben / Ver�ndern des Vektors kann zu undefiniertem Verhalten f�hren.
///</remarks>
///
int Combinator_Initialize( struct Combinator* c, int k, int* values, int n );

///
///<summary>
///Erzeugt die n�chst m�gliche Kombination
///</summary>
///<param name="c" direction="in, out">
///Kombinator, nach dem die nc�hste Kombination erzeugt werden soll
///</param>
///<param name="dest" direction="in, out">
///Zielvektor, der die Kombination erhalten soll
///Der Parameter darf nicht NULL sein
///</param>
///<returns>
///Wenn eine g�ltige Kombination erzeugt werden konnte wird 0 zur�ckgegeben
///sind keine weiteren Kombinationen m�glich wird ein Wert kleiner 0 zur�ckgegeben
///</returns>
///<remarks>
///dest muss mindestens k Pl�tze 
///(angegeben w�hrend der Kombinator-Initialisierung) fassen k�nnen.
///</remarks>
///
int Combinator_GetNext( struct Combinator* c, int* dest );

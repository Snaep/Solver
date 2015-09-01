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
///Anzahl der möglichen Kombinationsteilnehmer
///</param>
///<returns>
///Ist die Initialisierung erfolgreich wird 0 zurückgegeben,
///ansonsten ein Wert kleiner 0
///</returns>
///<remarks>
///Erzeugt k aus n Kombinationen
///-> keine Wiederholungen
///-> keine beachtung der Ordnung (wurde 123 erzeugt wird z.B. 321 nicht mehr erzeugt)
///-> for more refer to https://en.wikipedia.org/wiki/Combination
///
///die Initialisierung schlägt fehl, wenn c oder values == NULL, k > 64 oder k > n ist
///
///Der an values übergebene Zeiger muss mindestens die gleiche Lebensdauer wie der Kombinator aufweisen.
///Ein Freigeben / Verändern des Vektors kann zu undefiniertem Verhalten führen.
///</remarks>
///
int Combinator_Initialize( struct Combinator* c, int k, int* values, int n );

///
///<summary>
///Erzeugt die nächst mögliche Kombination
///</summary>
///<param name="c" direction="in, out">
///Kombinator, nach dem die ncähste Kombination erzeugt werden soll
///</param>
///<param name="dest" direction="in, out">
///Zielvektor, der die Kombination erhalten soll
///Der Parameter darf nicht NULL sein
///</param>
///<returns>
///Wenn eine gültige Kombination erzeugt werden konnte wird 0 zurückgegeben
///sind keine weiteren Kombinationen möglich wird ein Wert kleiner 0 zurückgegeben
///</returns>
///<remarks>
///dest muss mindestens k Plätze 
///(angegeben während der Kombinator-Initialisierung) fassen können.
///</remarks>
///
int Combinator_GetNext( struct Combinator* c, int* dest );

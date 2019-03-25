/***************************************************************************
                          leveldaten.h  -  description
                             -------------------
    begin                : Fri Jul 21 2000
    copyright            : (C) 2000 by Immi
    email                : cuyo@pcpool.mathematik.uni-freiburg.de

Modified 2001-2003,2005,2006,2008,2010,2011,2014 by the cuyo developers
Modified 2012 by Bernhard R. Link
Maintenance modifications 2012,2014 by the cuyo developers

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LEVELDATEN_H
#define LEVELDATEN_H

#include <vector>
#include <set>

#include "sdltools.h"

#include "sorte.h"
#include "bilddatei.h"
#include "version.h"

class Str;
class Code;
class Blop;
class DefKnoten;
class ListenKnoten;
class DatenDatei;
class Fehler;


/***** Konstanten, die irgend was einstellen *****/

/* Konstanten, die mit dem Layout zusammenh�ngen: siehe layout.h */

#define max_spielerzahl 2

#define max_farben_zahl 200 // Max. Anzahl der Farben in einem Level

/* Wof�r gibt es wie viele Punkte */
#define punkte_fuer_normales 1
#define punkte_fuer_graues 0
#define punkte_fuer_gras 20
#define punkte_fuer_kettenreaktion 10

/* Pixel pro Schritt: Bonus-Hetzrand-Geschwindigkeit */
#define bonus_geschwindigkeit 32
/* Punkte pro Zeitschritt der Bonus-Animation */
#define punkte_fuer_zeitbonus 10



/***** Konstanten, die einfach nur was bedeuten *****/

#define zufallsgraue_keine (-1)
#define PlatzAnzahl_undefiniert (-1)


/* Zur �bergabe an laden(), um den Titel-Level zu laden */
#define level_titel (-1)


/* F�r die codes in startdist */
#define distkey_leer (-1)
#define distkey_gras (-2)
#define distkey_grau (-3)
#define distkey_farbe (-4)
#define distkey_neighbours (-5)
#define distkey_chainreaction (-6)
#define distkey_undef (-7)

/* nachbarschaft_... steht in sorte.h */



/* Zur Unterscheidung von lazy und eager */

enum LDTeil {
  ldteil_summary,
  ldteil_level,
  ldteile_anzahl
};



/** Array, das alle Sorten eines Levels enth�lt. Das Hauptfeature dieser Klasse ist, dass
    Sorten automatisch gel�scht werden k�nnen, und zwar:
    - nur die, die auch ins Array geschrieben wurden
    - nur die, die nicht Kopien sind. */

class SortenArray {
  /** Die eigentliche Liste der Sorten (alles indizes sind um blopart_min_sorte verschoben) */
  Sorte * mSorten[max_farben_zahl - blopart_min_sorte];
  /** Welche der mSorten-Pointer sind Kopien von anderen Pointern, und
   *  welche sind Originale? (Nur originale m�ssen gel�scht werden) */
  bool mOriginale[max_farben_zahl - blopart_min_sorte];

  /** Laenge des positiven Teils des Arrays; -1 = array leer */
  int mLaenge;
  
 public:
  SortenArray();
   
  /** Sorte auslesen */
  Sorte * operator[](int nr) const;
  
  /** L�nge des Arrays �ndern (genauer gesagt: L�nge des positiven Teils);
   *  sollte nur aufgerufen werden, wenn das Array leer ist. */
  void setLaenge(int l);
  
  /** Sorte in das Array einf�gen; normalerweise wird die Sorte dann
      von loeschen() gel�scht; aber nicht, wenn istOriginal = false ist;
      dann wird davon ausgegangen, dass der Pointer nur eine Kopie ist. */
  void neueSorte(int nr, Sorte * s, bool istOriginal = true);
  
  void loeschen();
}; // class SortenArray



/** (Einziges globales) Objekt enth�lt alle Informationen �ber
    den aktuellen Level.
    
    Das Parsen der ld-Dateien:
    - Beim Aufruf des constructors von LevelDaten() wird ladLevelSummary()
      aufgerufen.
    - ladLevelSummary() bzw ladLevel  sucht die richtige Datei und erzeugt ein
      DatenDatei-Objekt
    - Der Constructor davon �ffnet die Datei und ruft parse() auf.
    - parse() ruft den bison-Parser auf. Der erzeugt folgendes:
      - Einen Baum aus Knoten: DefKnoten, ListenKnoten, WortKnoten
      - Jeder DefKnoten enth�lt noch eine Liste von Codeen (die
        in der level.descr in << >> definiert wurden)
      - Codeen sind auch wieder baumartig
    - Wenn in einem Code ein Name eines anderen Codes vorkommt,
      wird das schon beim Parsen aufgel�st, indem in den CodeSpeichern
      der dar�berliegenden DefKnoten nachgeschaut wird
      
    
    @author Immi
*/


class LevelDaten {
  friend class Sorte;
  friend int yyparse();
	
 public:
  LevelDaten(const Version & version);
  ~LevelDaten();
  
  /** L�d die Levelconf (neu). Wird vom Konstruktor aufgerufen.
      Und, wenn sich Einstellungen ver�ndert haben.
      Eine Vorbedingung ist also, da� niemand grad auf irgendwelche Daten
      dieses Objekts angewiesen ist, insbesondere also, da� grad kein
      Spiel l�uft.
      Bei aufJedenFall=false wird nur dann wirklich neu geladen,
      wenn version!=mVersion.
      Bei ldteil=ldteil_summary wird am Ende noch der inhalt aller in
      global= aufgef�hrten Dateien nach ldteil_level geladen. */
  void ladLevelSummary(bool aufJedenFall, const Version & version);

 protected:
  /** L�d den level-spezifischen Teil */
  void ladLevelConfig();
 public:
  
  /** Wird w�hrend des Parsens (d. h. innerhalb von ladLevel*()) von
      DefKnoten aufgerufen, wenn ein neuer Level gefunden wurde. F�gt
      den Level in die Liste der Level ein. ladLevelSummary() kann sich
      danach immernoch entscheiden, ob es die Liste wieder l�scht und
      durch die "level=..."-Liste ersetzt. */
  void levelGefunden(Str lna);

  /* Gibt Speicher frei */
  void entladLevel();
  
  /** f�llt alle Daten in diesem Objekt f�r Level nr aus; throwt bei Fehler */
  void ladLevel(int nr);

  /** Darf nur aufgerufen werden, wenn nr schon der letzte Level war.
      Erneuert alles, was seitdem vergessen worden sein k�nnte. */
  void erneuerLevel(int nr);

  /** Sollte am Anfang des Levels aufgerufen werden; k�mmert sich
      um den Global-Blop */
  void startLevel() const;
  
  /** Sollte einmal pro Spielschritt aufgerufen werden (bevor
      Spielfeld::spielSchritt() aufgerufen wird). K�mmert sich 
      um den Global-Blop */
  void spielSchritt() const;

  /** Liefert zur�ck, wie viele Level es gibt. */
  int getLevelAnz() const;

  /** Lifert zur�ck, ob die Level der Reihe nach gespielt werden m�ssen */
  int getAngeordnet() const;

  /** Liefert den Namen von Level nr zur�ck. Liefert "???" bei Fehler. */
  Str getLevelName(int nr) const;

  /** Liefert den internen Namen von Level nr zur�ck. */
  Str getIntLevelName(int nr) const;

  /** Liefert die Nummer des Levels mit dem angegebenen internen Namen zur�ck,
      oder 0, wenn der Level nicht existiert. */
  int getLevelNr(Str na) const;

  bool levelAccessible(int l) const;

  /** Wenn eine Sorte ihre Platzanzahl rausgefunden hat,
      teilt sie uns das mit */
  void neue_PlatzAnzahl(int);

  int zufallsSorte(int wv);

  int liesDistKey(const Str &);

  const Version & getVersion() const;

  /** Setzt AutoColor::gGame */
  void setSchriftFarbe(Color f);
  
  /** Liefert true, wenn die angegebene Spalte vom angegebenen Spieler verschoben ist
      (wegen Hex-Modus) */
  bool getHexShift(bool rechts, int x) const;

 protected:
   
  /** L�d ein paar Sorten. Wird mehrfach von ladLevel() aufgerufen. */
  void ladSorten(const Str & ldKeyWort, int blopart);
   
 /** Die Objekte, mit denen man auf die Dateien zugreift. Hier stehen nur
     Pointer drauf, damit diese .h-Datei nicht so viel includen muss. */
  DatenDatei * mLevelConf[ldteile_anzahl];
  /** True, wenn zur Zeit der entsprechende Teil der LevelConf geladen ist. */
  bool mLCGeladen[ldteile_anzahl];
  /** Alle Level, die schon mal geladen wurden und daher noch vorhanden sind.
      Hier stehen die Dateinamen. */
  std::set<Str> mLevelCache;
  /** Liste der internen Levelnamen. */
  std::vector<Str> mIntLevelNamen;
  /** True falls die Level der Reihe nach gespielt werden m�ssen. */
  bool mAngeordnet;

  Version mVersion;
  
 public:

  /* Die ganzen nachfolgenden Variablen werden von ladLevel() gesetzt.
     Danach greifen alle anderen Objekte, die was mit dem Spiel zu tun
     haben, direkt darauf zu. */


  /***** Allgemeines *****/
  bool mLevelGeladen;
  int mLevelNummer; /* Nummer des geladenen Levels */
  int mSpielerZahl;
  /** Interner Level-Name vom aktuellen Level. (Wird von Aufnahme
      ben�tigt.) */
  Str mIntLevelName;
  Str mLevelName;
  Str mLevelAutor;
  /** Der Knoten zum aktuellen Level. */
  DefKnoten * mLevelKnoten;
  /** Beschreibungstext f�r den Level */
  Str mBeschreibung;
  Color mHintergrundFarbe;
  bool mMitHintergrundbildchen;
  Bilddatei mHintergrundBild;
  /** Farbe der Schrift in dem Level:
      0 = abgedunkelt, 1 = normal, 2 = aufgehellt */
  Color mSchriftFarbe;
  bool mGrasBeiKettenreaktion;
  bool mFallPosZufaellig;
  Bilddatei mExplosionBild;
  int mPlatzAnzahlDefault;
  int mPlatzAnzahlMin;
  int mPlatzAnzahlMax;
  /* Gibt an, ob neben mPlatzAnzahlMin und mPlatzAnzahlMax noch andere
     PlatzAnzahlen vorkommen. */
  bool mPlatzAnzahlAndere;
  Str mMusik;
  
  /** Max. Anzahl der Bilder, die ein Blop gleichzeitig malt. Wird
      (ggf.) von den Sorten erh�ht, wenn man sie l�dt. */
  int mStapelHoehe;
  /** Anzahl der Bilder, die Blops auf Nachbarfelder malen. Der Einfachheit
      halber wird das hier f�r alle Sorten aufsummiert, statt alles sch�n
      nach relativen Koordinaten zu trennen, etc.
      Wird auch von den Sorten erh�ht, wenn man sie l�dt. */
  int mNachbarStapelHoehe;
  
  /***** Die Sorten *****/
 public:
  /** Die ganzen Sorte-Objekte */
  SortenArray mSorten;
  int mAnzFarben;
  int mVerteilungSumme[anzahl_wv];
  int mKeineGrauenW;    /** Z�hlt nicht zu mGrauSumme */

  /***** Hetzrand *****/
  Color hetzrandFarbe;
  int hetzrandZeit;
  bool mMitHetzbildchen;
  Bilddatei mHetzBild;
  int mHetzrandUeberlapp;
  int mHetzrandStop;

  /***** Gras *****/
  ListenKnoten * mAnfangsZeilen;
  int mDistKeyLen; /** L�nge der distKeys. 0, wenn es noch keinen gab. */


  /***** KI-Player-Nutzen-Funktion *****/
  /** Zusatzpunkte f�r beide Blops gleiche Farbe & Senkrecht*/
  int mKINEinfarbigSenkrecht;
  /** Vorfaktor vor Bewertung der Blop-H�he */
  int mKINHoehe;
  /** Punkte f�r Blob mit gleicher Farbe benachbart */
  int mKINAnFarbe;
  /** Punkte f�r Blob mit Gras benachbart */
  int mKINAnGras;
  /** Punkte f�r Blob mit Grauem benachbart */
  int mKINAnGrau;
  /** Punkte f�r Blob zwei �ber gleicher Farbe */
  int mKINZweiUeber;
	
  /***** Sonderfeatures *****/
  bool mSpiegeln;
  int mNachbarschaft; // Default-Wert f�r die Sorten
  /** true bei Sechseckraster. Wird direkt aus mNachbarschaft bestimmt. */
  bool mSechseck;
  /* Nur fuer Sechseckraster: Welche(r) Spieler ist links-rechts gespiegelt? */
  int mSechseckFlip;
  bool mMitLeerBildchen;
  int mZufallsGraue;
  bool mGreysAtAll;

 protected:

  bool mSammleLevel; /* Bestimmt, ob levelGefunden �berhaupt was tun soll. */

};

/* Definition in leveldaten.cpp */
extern LevelDaten * ld;


#endif

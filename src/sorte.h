/***************************************************************************
                          sorte.h  -  description
                             -------------------
    begin                : Fri Apr 20 2001
    copyright            : (C) 2001 by Immi
    email                : cuyo@karimmi.de

Modified 2001-2003,2005,2006,2011,2014 by the cuyo developers

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SORTE_H
#define SORTE_H

#include <vector>
#include "stringzeug.h"

//#include "fehler.h"
//#include "bilddatei.h"
//#include "blopbesitzer.h" // Nur f�r die Verbindungskonstanten

class Version;
class DefKnoten;



/* Zur �bergabe an Sorte() */
#define verbart_nein 0
#define verbart_selten 1
#define verbart_ja 2


/* F�r das verbindetMitRand-Array: */
#define rand_links 0
#define rand_rechts 1
#define rand_oben 2
#define rand_unten 3

/* Konstanten f�r neighbours= */
#define nachbarschaft_normal 0
#define nachbarschaft_schraeg 1
#define nachbarschaft_6 2
#define nachbarschaft_6_schraeg 3
#define nachbarschaft_springer 4
#define nachbarschaft_dame 5
#define nachbarschaft_6_3d 6
#define nachbarschaft_garnichts 7
#define nachbarschaft_horizontal 8
#define nachbarschaft_vertical 9
#define nachbarschaft_letzte 9


/** Event-Nummern in mEventCode. Die entsprechenden Strings stehen
    in sorte.cpp. */
enum {
  event_keins = -1, // wird in Blop::mScheduleEventNr verwendet
  event_draw = 0, // Haupt-Mal-Code; Sonderbehandlung; muss 0 bleiben
  event_init,     // einmal am Anfang
  event_turn,     // bei jedem drehen
  event_land,     // kommt auf dem Boden auf (nur bei Fall)
  event_changeside,     // wechselt zum anderen Spieler
  event_connect,        // Verbindungen neu berechnen
  event_row_up,         // Reihe bekommen
  event_row_down,       // Reihe abgegeben
  event_keyleft,        // Taste links
  event_keyright,       // Taste rechts
  event_keyturn,        // Taste drehen
  event_keyfall,        // Taste fallen
  event_anz
};


/* Blop-Arten / -Sortennummern. Die Nummern >= 0 sind die normalen
   Farbsorten. */
#define blopart_keins (-1)
/* Letzte Art, auf die man ein Blop von cual aus noch setzen darf */
#define blopart_min_cual (-1)
/* Blop, der nicht wirklich zu sehen ist, sondern nur die globale
   Animation ausf�hrt */
#define blopart_global (-2)
/* dito, aber hiervon gibt es je Spieler einen */
#define blopart_semiglobal (-3)
/* Informationsblops am Spielfeldrand */
#define blopart_info (-4)
/* Letzte Art, von der es noch wirklich eine Sorte und Blops gibt. Alles
   andere sind nur R�ckgabewerte oder so was. */
#define blopart_min_sorte (-4)

/* R�ckgabewert, falls Koordinaten nach au�erhalb vom Spielfeld zeigen. */
#define blopart_ausserhalb (-5)
/* R�ckgabewert von getArt(), wenn es sich um einen Farbblop handelt.
   Und Wert, den man an Sorte::Sorte() �bergeben kann. */
#define blopart_farbe (-6)
#define blopart_gras (-7)
#define blopart_grau (-8)



/* W-Verteilungen f�r die zuf�llige Sortenauswahl */

enum {
  wv_farbe,
  wv_grau,
  wv_gras,
  anzahl_wv
};

/* Namen werden in sorte.cpp definiert. */
extern const char cEventNamen[event_anz][33];
extern const char* cVerteilungsNamen[anzahl_wv];


class Bilddatei;
class Blop;
class Code;





/** Enth�lt alle Informationen �ber eine Blopsorte; kann solche Blops malen
  *@author Immi
  */

class Sorte {
	

 public: 
  /** L�dt die Sorte mit dem angegebenen Namen. Schaut auch in der
      entsprechenden Gruppe von mLevelConf nach, setzt die Gruppe
      aber danach zur�ck. blopart muss nur angegeben werden, damit ein
      paar Art-abh�ngige Defaults richtig gesetzt werden k�nnen.
      Throwt Fehler, wenn erfolglos. */
  Sorte(const Str & name, const Version & version, int blopart);
  
  ~Sorte();
	
  
  
  
	
 protected:
  /** l�scht die ganzen Bilder aus dem bilddateien-Array */
  void loeschBilder();

  /** Wird an zwei Stellen in Sorte() aufgerufen */
  void setzeDefaults(DefKnoten * quelle);

  /** Wird von Sorte() aufgerufen; ausgelagert, weil Sorte() langsam
      lang und un�bersichtlich wird */
  void ladeCualEvents(const Version & version);


 public:
 
  /***** Getter-Methoden, mit denen man auf die ganzen Informationen
         zugreifen kann. *****/
  
  
  Str getName() const;

  int getBasekind() const;
  
  bool getVerbindetMitRand(int r) const;
  
  int getNachbarschaft() const;
  
  int getPlatzAnzahl() const;

  int getVerteilung(int wv) const;

  int getDefault(int var) const;
  int getDefaultArt(int var) const;
  
  Code * getEventCode(int evt) const;

  int getDistKey() const;
  int getVersions() const;
  
  Bilddatei * getBilddatei(int nr) const;
	
  
  /* ACHTUNG! Beim Neueinf�gen von Variablen, die aus der
   * level.descr gelesen werden, nicht vergessen, am Anfang
   * von Sorte::Sorte() defaults zu setzen! */
  
 protected:			
  /** Der Name, unter dem in Code-Programmen auf die Sorte
      zugegriffen wird. */
  Str mName;
  /** Was beim Konstruktor �bergeben wurde */
  int mBlopart;
  /** Der Wert fuer basekind */
  int mBasekind;
  /** Verbinden sich diese Blops auch zum Rand hin? Aufgeschl�sselt
      nach den R�ndern. (Konstanten rand_...) */
  bool mVerbindetMitRand[4];
  /** Was z�hlt als Nachbarn im Sinne einer Kette? */
  int mNachbarschaft;
  /** Wie viele Steine m�ssen zusammen, damit sie platzen? */
  int mPlatzAnzahl;
  /** Entstehwahrscheinlichkeiten */
  int mVerteilung[anzahl_wv];
  /** Defaultwerte f�r Blops dieser Sorte. */
  int * mDatenDefault;
  int * mDatenDefaultArt;
  /** Repr�sentation in startdist:
      Zeichen(kette) f�r version=0, Anzahl an Versionen */
  int mDistKey;
  int mVersions;

  /** Alle Bildchen zu diesem Blop */
  std::vector<Bilddatei *> mBilddateien;

  /** Code, der zu bestimmten Events aufgerufen wird (bzw. 0). */
  Code * mEventCode[event_anz];
  
};

#endif

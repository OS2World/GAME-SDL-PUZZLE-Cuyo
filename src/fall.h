/***************************************************************************
                          fall.h  -  description
                             -------------------
    begin                : Sat Aug 18 2001
    copyright            : (C) 2001 by Immi
    email                : cuyo@karimmi.de

Modified 2001,2002,2005,2006,2008,2010,2011,2014 by the cuyo developers

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FALL_H
#define FALL_H

#include <SDL.h>

#include "blop.h"
#include "blopbesitzer.h"
#include "bilddatei.h"
#include "leveldaten.h"


// wie das Fallende sein kann
#define richtung_keins 0
#define richtung_einzel 1
#define richtung_waag 2
#define richtung_senk 3
#define richtung_unplatziert 4 // das Fall, das als naechstes kommen wird...

// Wert f�r get_X bis get_YY, wenn das Blop nicht existiert
#define blop_pos_nix -1



struct FallPos {
  /** x-Koord vom Fall in Feldern; bei unplatzierten Blops ist das
      die Pos, an der der Blop sp�ter auftauchen wird. */
  int x;
  /** y-Koord vom Fall in Pixeln (absolut, und nicht relativ zum ggf.
      wegen R�berreihen verschobenen Spielfeld).
      Bei unplatzierten blobs ist das die Pos. relativ zum Preview-Feld 
      (und nicht die Pos., an der es auftauchen wird) */
  int yy;
  /** Richtung und Anzahl der Blops. Siehe Konstanten. */
  int r;
	
  /** Liefert die Anzahl der Blops vom Fall zur�ck. */
  int getAnz() const;
	
  int getX(int a) const;

  int getY(int a, const Spielfeld * spf) const;

};


class Spielfeld;

/** Enth�lt alle Informationen �ber das Fall: Existenz, Position
		(inkl. genaue Drehpos), Farben, ...
  */

class Fall: public BlopBesitzer {
 public:

  /** Konstruktor... */
  Fall(Spielfeld * sp, bool re);


  /** Position vom Fall. */
  FallPos mPos;

 private:
  /** True, wenn rechter Spieler (f�r Sound n�tig) */
  bool mRechterSpieler;
  /** Schnell fallen? */
  bool mSchnell;
  /** Ist es noch nicht ganz fertiggedreht? */
  int mExtraDreh;
  /** Ist es noch nicht ganz fertig waagerecht verschoben? */
  int mExtraX;
  /** Wurde Taste links bzw. rechts gedr�ckt? */
  int mExtraLinks, mExtraRechts;
  /** Die Blops */
  Blop mBlop[2];
  /** Rechteck, das den Bereich ueberdeckt, wo das Fall im Moment
      hingemalt ist (fuer Grafik-Updates). */
  int mRectX, mRectY, mRectW, mRectH;

	
  /** kopiert einen fallenden Blop nach mDaten und liefert den
      Zielblop in mDaten zur�ck (als Referenz), damit man einen
      land-Event senden kann. Sendet den land-Event nicht selbst,
      weil Cual-Code erwarten k�nnte, dass erst beide Blops
      gefestigt werden und dann erst die Events kommen.
      Kann 0 zur�ckliefern (wenn der Blop keinen Platz auf
      dem Bildschirm hat). */
  Blop * festige(int n);
	
  /** Liefert true, wenn das Fall grade auf Tastendr�cke reagiert */
  bool steuerbar() {
    return mPos.r == richtung_waag || mPos.r == richtung_senk;
  }
	
  /** Prueft, ob an Position p schon was im Weg ist oder nicht.
      Wenn irgendwo dr�ber in der Spalte was nicht-schwebendes ist,
      z�hlt das auch als im Weg.
      @return eine Konstante belegt_... */
  int testBelegt(FallPos p) const;
	
  /** Liefert true, wenn das Fallende senkrecht ist */
  bool istSenkrecht() const;
	
  /** L�sst nur noch Blop a �brig */
  void halbiere(int a);
	
  /** Liefert loc_x */
  int getX(int a) const;
  /** Liefert loc_y */
  int getY(int a) const;

  /** Liefert loc_xx */
  int getXX(int a) const;
  /** Liefert loc_yy */
  int getYY(int a) const;

  /** Kodiert alle Informationen des Drehens in eine Zahl.
      Ist f�r getXX und getYY da */
  int getDrehIndex(int a) const;

  /** Bestimm mFallRect neu (Fall-ueberdeckendes Rechteck) */
  void calcFallRect();

  /** Setzt den Bereich, der durch mFallRect angegeben ist, auf
      upzudaten */
  void setUpdateFallRect();

 public:
 
  /**  Mu� einmal aufgerufen werden */
  void initialisiere();

  /** Erzeugt ein neues Fall, der sich noch nicht im Spielfeld befindet.
      Bei reinkommen = true kommt das Fall ins Preview-Feld reingerutscht.
      Sonst ist es sofort da (f�r Spielanfang) */
  void erzeug(bool reinkommen = true);
  /** Bringt ein Fall ins Spiel, d.h. setzt die Koordinaten. Liefert false, wenn daf�r kein Platz ist */
  bool insSpiel();
  /** Entfernt das Fall ganz */
  void zerstoere();

	
 private:
  /** Fuer platzierte Falls: Macht alles von spielSchritt ausser Grafik-Update markieren */
  void spielSchrittPlatziertIntern();
  
 public:
  /** Bewegt das Fall ggf. nach unten und k�mmert sich ggf. um
      Verwandlungen. */
  void spielSchritt();
  
  /** F�hrt die Animationen durch. Innerhalb einer Gleichzeit aufrufen. */
  void animiere();

  /** Bewegt das Fall eins nach links. Wird in einer Gleichzeit aufgerufen. */
  void tasteLinks();
  /** Bewegt das Fall eins nach rechts. Wird in einer Gleichzeit aufgerufen. */
  void tasteRechts();
  /** Dreht das Fall.
      Version 1 wird in einer Gleichzeit aufgerufen,
      Version 2 danach au�erhalb von Gleichzeiten. */
  void tasteDreh1();
  void tasteDreh2();
  /** �ndert die Fallgeschwindigkeit vom Fall.
      Wird in einer Gleichzeit aufgerufen. */
  void tasteFall();

  /** Liefert true, wenn das Fall (noch) am Platzen ist
      (wg. Spielende) */
  bool getAmPlatzen() const;
  /** Liefert einen Pointer auf die Blops zur�ck. Wird vom
      KIPlayer und von ort_absolut::finde() ben�tigt. */
  const Blop * getBlop() const;
  Blop * getBlop();
  /** Liefert die Anzahl der Blops vom Fall zur�ck. */
  int getAnz() const {
    return mPos.getAnz();
  }
  /** L�sst alle Blops vom Fall platzen (Spielende). */
  void lassPlatzen();
  /** Malt das Fall. */
  void malen() const;
  /** Liefert true, wenn das Fall existiert.
      Mit Argument: Wenn diese H�lfte existiert. */
  bool existiert(int a=0) const;
  /** Liefert true, wenn das Fall gerade am zerfallen ist
      (d. h. existiert, aber aus nur noch einem Blop besteht).
      In dieser Zeit darf n�mlich keine Explosion gez�ndet
      werden. (Erst warten, bis der andere Blop auch angekommen
      ist.) */
  bool istEinzel() const;
  void playSample(int nr) const;

  virtual int getSpezConst(int vnr, const Blop *) const;

};

#endif


/***************************************************************************
                          kiplayer.h  -  description
                             -------------------
    begin                : Wed Jul 25 2001
    copyright            : (C) 2001 by Immi
    email                : cuyo@karimmi.de

Modified 2001-2003,2006,2011 by the cuyo developers

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KIPLAYER_H
#define KIPLAYER_H

/**Wenn man gegen den Computer spielt...
  *@author Immi
  */

class Spielfeld;
class BlopGitter;
class Blop;

class KIPlayer {
public: 
  KIPlayer(Spielfeld * sp);
  ~KIPlayer();
  /** Teilt der KI mit, dass ein neuer Level anf�ngt. (Initialisiert
      alles.) */
  void startLevel();
public:
  /** Einmal pro Spielschritt aufrufen, wenn der Computer
      auch spielen soll */
  void spielSchritt();

protected:
  Spielfeld * mSp;
  const BlopGitter * mDaten;

  bool mZuTun;
  double mNochWart;
  int mNochDr;
  int mNochDx;
protected: // Protected methods
  /** Liefert zur�ck, wie gut ein Blop der Farbe f in Spalte y w�re,
      um dy nach oben verschoben. */
  int bewerteBlop(int x, int dy, int f);
  /** Liefert zur�ck, wie gut das Fallende bei x in Richtung r
      w�re. (r = Anzahl der Dreh-Tastendr�cke) */
  int bewerteZweiBlops(int x, int r);
};

#endif

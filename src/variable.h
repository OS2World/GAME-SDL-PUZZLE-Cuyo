/***************************************************************************
                          variable.h  -  description
                             -------------------
    begin                : Fri Jul 21 2000
    copyright            : (C) 2000 by Immi
    email                : cuyo@pcpool.mathematik.uni-freiburg.de

Modified 2002,2005,2006,2010,2011 by the cuyo developers

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef VARIABLE_H
#define VARIABLE_H

struct ort_absolut;
class Blop;
class Ort;
class VarDefinition;
class Str;

/* R�ckgabewert f�r getOrt(). variable_global, variable_lokal
   und variable_fall werden au�erdem verwendet:
   - beim Constructor (Variable("bla", variable_global))
   - in mDX
 */
#define variable_global 0x8000
#define variable_relativ 0x8001
#define variable_lokal 0x8002
#define variable_fall 0x8003


/** (Auftreten einer) Variable in einer Prozedur.

    Wenn mDeklaration sagt, dass es sich
    um eine (richtige echte) Konstante handelt, existiert die Variable
    nur so lange, bis in vparse.yy ein Ausdruck draus gemacht wird. Dabei
    wird es dann durch den Wert ersetzt. */
class Variable {


  /** Datei, in der diese Variablenverwendung steht (f�r Fehlermeldungen) */
  //Str mDateiName;
  /** Zeilen-Nr, in der diese Variablenverwendung steht (f�r Fehlermeldungen) */
  //int mZeilenNr;
  /* ... wird nicht mehr gebraucht. Diese Info wird vom dar�berliegenden
     Code an die Fehlermeldung geh�ngt. */

  VarDefinition * mDeklaration;

  /** Relative Koordinaten, so wie sie der cual-Programmierer eingegeben
      hat. */
  Ort * mOrt;

  
public:
  /** Erzeugt eine M�ll-Variable. Wird verwendet, wenn es einen Fehler
      gab (�blicherweise Variable nicht definiert). Die M�ll-Variable
      versucht, Folgefehler zu vermeiden, so dass wenigstens noch fertig
      geparst werden kann. */
  Variable();
  
  Variable(//Str datna, int znr,
           VarDefinition * d, Ort * ort);

  ~Variable();

  Str toString() const;
  

  bool Ort_hier() const;

  ort_absolut getOrt(ort_absolut vonhieraus, Blop & fuer_code) const;
  
  int getNummer() const;
  
  bool istKonstante() const;
  
  /** Liefert den Default-Wert, wenn's eine Variable ist und den
      Wert, wenn's eine Konstante ist. */
  int getDefaultWert() const;
  
  Str getName() const;

};




#endif

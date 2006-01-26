/***************************************************************************
                               digi_source.cpp
                              -----------------
    begin                : Oct 3 2005
    copyright            : (C) 2005 by Michael Margraf
    email                : michael.margraf@alumni.tu-berlin.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "digi_source.h"
#include "main.h"
#include "node.h"


Digi_Source::Digi_Source()
{
  Type = isComponent;   // both analog and digital
  Description = QObject::tr("digital source");

  Lines.append(new Line(-10,  0,  0,  0,QPen(QPen::darkGreen,2)));
  Lines.append(new Line(-20,-10,-10,  0,QPen(QPen::darkGreen,2)));
  Lines.append(new Line(-20, 10,-10,  0,QPen(QPen::darkGreen,2)));
  Lines.append(new Line(-35,-10,-20,-10,QPen(QPen::darkGreen,2)));
  Lines.append(new Line(-35, 10,-20, 10,QPen(QPen::darkGreen,2)));
  Lines.append(new Line(-35,-10,-35, 10,QPen(QPen::darkGreen,2)));

  Lines.append(new Line(-32, 5,-28, 5,QPen(QPen::darkGreen,2)));
  Lines.append(new Line(-28,-5,-24,-5,QPen(QPen::darkGreen,2)));
  Lines.append(new Line(-24, 5,-20, 5,QPen(QPen::darkGreen,2)));
  Lines.append(new Line(-28,-5,-28, 5,QPen(QPen::darkGreen,2)));
  Lines.append(new Line(-24,-5,-24, 5,QPen(QPen::darkGreen,2)));

  Ports.append(new Port(  0,  0));

  x1 = -39; y1 = -14;
  x2 =   0; y2 =  14;

  tx = x1+4;
  ty = y2+2;
  Model = "DigiSource";
  Name  = "S";

  // This property must stay in this order !
  Props.append(new Property("Num", "1", true,
		QObject::tr("number of the port")));
  Props.append(new Property("init", "low", false,
		QObject::tr("initial output value")+" [low, high]"));
  Props.append(new Property("times", "1ns; 1ns", false,
		QObject::tr("list of times for changing output value")));
  Props.append(new Property("V", "1 V", false,
		QObject::tr("voltage of high level")));
}

// -------------------------------------------------------
Digi_Source::~Digi_Source()
{
}

// -------------------------------------------------------
Component* Digi_Source::newOne()
{
  return new Digi_Source();
}

// -------------------------------------------------------
Element* Digi_Source::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("digital source");
  BitmapFile = "digi_source";

  if(getNewOne)  return new Digi_Source();
  return 0;
}

// -------------------------------------------------------
QString Digi_Source::NetList()
{
  QString s = Model+":"+Name;

  // output node names
  s += " "+Ports.getFirst()->Connection->Name;
  
  // output all properties
  Props.first();   // first property not needed
  Property *pp = Props.next();
  s += " "+pp->Name+"=\""+pp->Value+"\"";
  pp = Props.next();
  s += " "+pp->Name+"=\"["+pp->Value+"]\"";
  pp = Props.next();
  s += " "+pp->Name+"=\""+pp->Value+"\"";

  return s;
}

// -------------------------------------------------------
QString Digi_Source::VHDL_Code(int NumPorts)
{
  QString s, t;
  QString Out("    " + Ports.getFirst()->Connection->Name + " <= '");

  s  = "\n  " + Name + ":process\n  begin\n";

  int z = 0;
  char State;
  if(NumPorts <= 0) {  // time table simulation ?
    if(Props.at(1)->Value == "low")
      State = '0';
    else
      State = '1';

    t = Props.next()->Value.section(';',z,z).stripWhiteSpace();
    while(!t.isEmpty()) {
      s += Out + State + "';";    // next value for signal

      if(!VHDL_Time(t, Name))
        return t;    // time has not VHDL format

      s += "  wait for " + t + ";\n";
      State ^= 1;
      z++;
      t = Props.current()->Value.section(';',z,z).stripWhiteSpace();
    }
  }
  else {  // truth table simulation
    State = '0';
    int Num = Props.getFirst()->Value.toInt() - 1;
    
    for(z=1<<(NumPorts-Num); z>0; z--) {
      s += Out + State + "';";    // next value for signal
      s += "  wait for "+QString::number(1 << Num)+" ns;\n";
      State ^= 1;
    }
  }

  s += "  end process;\n";
  return s;
}
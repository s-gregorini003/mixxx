/***************************************************************************
                          controlpushbutton.cpp  -  description
                             -------------------
    begin                : Wed Feb 20 2002
    copyright            : (C) 2002 by Tue and Ken Haste Andersen
    email                : 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "controlpushbutton.h"
#include "controlengine.h"

/* -------- ------------------------------------------------------
   Purpose: Creates a new simulated latching push-button. 
   Input:   key - Key for the configuration file
            _led - A led which is connected to the button.
   -------- ------------------------------------------------------ */
ControlPushButton::ControlPushButton(ConfigKey key, WBulb* _led) : ControlObject(key)
{
    value = 0.;
    led = _led;
    if (led!=0)
        if (value==1.)
            led->setChecked(true);
        else
            led->setChecked(false);

};

ControlPushButton::~ControlPushButton()
{
    //ControlObject();
};

/* -------- ------------------------------------------------------
   Purpose: Set the position of the button, and change the
            value correspondingly.
   Input:   The (new) position.
   Output:  The value is updated.
   -------- ------------------------------------------------------ */
void ControlPushButton::slotSetPosition(int newpos)
{
    value = (FLOAT_TYPE)newpos;

    // Control LED:
    if (led != 0)
        if (value==1.)
            led->setChecked(true);
        else
            led->setChecked(false);

    emitValueChanged(value);
};

void ControlPushButton::slotSetPosition(bool newpos)
{
    if (newpos)
        slotSetPosition(1);
    else
        slotSetPosition(0);
}

void ControlPushButton::slotSetPositionMidi(MidiCategory c, int)
{
    // Only react on NOTE_ON midi events
    if (c==NOTE_ON)
    {
        if (value==0)
            slotSetPosition(1);
        else
            slotSetPosition(0);

//        qDebug("value : %f",value);
            
        emit(updateGUI((int)value));
    }
}

void ControlPushButton::slotSetPositionOff()
{
    slotSetPosition(0);
}

void ControlPushButton::slotClicked()
{
    if (value==1.)
        slotSetPosition(0);
    else
        slotSetPosition(1);
}

char *ControlPushButton::printValue()
{
    if (value == 1.)
        return "on";
    else
        return "off";
}

void ControlPushButton::setValue(int newvalue)
{
    value = (FLOAT_TYPE)newvalue;  

    // Control LED:
    if (led != 0)
        if (value==1.)
            led->setChecked(true);
        else
            led->setChecked(false);
    
    emit(updateGUI(newvalue));
};

void ControlPushButton::setWidget(QWidget *widget)
{
    QApplication::connect(widget, SIGNAL(clicked()), this, SLOT(slotClicked()));
    QApplication::connect(this, SIGNAL(updateGUI(int)), widget, SLOT(setValue(int)));

    forceGUIUpdate();
}

void ControlPushButton::setAction(QAction *action)
{
    QApplication::connect(action, SIGNAL(toggled(bool)), this, SLOT(slotSetPosition(bool)));
    QApplication::connect(this, SIGNAL(updateGUI(int)), this, SLOT(slotUpdateAction(int)));
    QApplication::connect(this, SIGNAL(updateAction(bool)), action, SLOT(setOn(bool)));

    forceGUIUpdate();
}

void ControlPushButton::slotUpdateAction(int v)
{
    if (v==1)
        emit(updateAction(true));
    else
        emit(updateAction(false));
}

void ControlPushButton::forceGUIUpdate()
{
    emit(updateGUI((int)value));
}



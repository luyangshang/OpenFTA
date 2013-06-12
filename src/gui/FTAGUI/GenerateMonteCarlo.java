/******************************************************************************
** 
** OpenFTA - Fault Tree Analysis
** Copyright (C) 2005 FSC Limited
** 
** This program is free software; you can redistribute it and*or modify it
** under the terms of the GNU General Public License as published by the Free
** Software Foundation; either version 2 of the License, or (at your 
** option) any later version.
** 
** This program is distributed in the hope that it will be useful, but WITHOUT
** ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
** FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
** more details.
**
** You should have received a copy of the GNU General Public License along 
** with this program; if not, write to the Free Software Foundation, Inc., 
** 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
**
** To contact FSC, please send an email to support@fsc.co.uk or write to 
** FSC Ltd., Cardiff Business Technology Centre, Senghenydd Road, Cardiff,
** CF24 4AY.
**
******************************************************************************/

package FTAGUI;

import javax.swing.*;
import javax.swing.event.*;
import javax.swing.text.*;
import javax.swing.border.*;
import javax.accessibility.*;

import java.awt.*;
import java.awt.event.*;
import java.beans.*;
import java.util.*;
import java.io.*;
import java.applet.*;
import java.net.*;
import java.lang.*;

class GenerateMonteCarlo implements Runnable{
    private native void nativeInitialiseGenerateMonteCarlo();
    private native void nativeOKButtonAction(int numSims, float unitTime);
    private int _numSims;
    private float _unitTime;
    private MonteCarloDialog _parent;
    
    public GenerateMonteCarlo(int numSims, float unitTime, MonteCarloDialog parent){
	_numSims = numSims;
	_unitTime = unitTime;
	_parent = parent;
	nativeInitialiseGenerateMonteCarlo();
    }
    
    public void run() {
	nativeOKButtonAction(_numSims, _unitTime);

	//while(!checkForInterrupt()) {};
	//closeWorkingDialog();
    }

    public boolean checkForInterrupt() {
	return _parent.checkForInterrupt();
    }

    public void closeWorkingDialog() {
	_parent.closeWorkingDialog();
    }

    public void postScrolledText(String title, String fileName) {
	_parent.postScrolledText(title, fileName); 
    }

    public void setProgressBarMax(int max) {
	_parent.setProgressBarMax(max);
    }

    public void progressBarInc() {
	_parent.progressBarInc();
    }

}

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

public class TreeOverviewFrame extends JFrame {

    FTACanvas canvas;

    private ImageIcon windowIcon;

    public TreeOverviewFrame(FTAFrame fta) {

	windowIcon = FTAUtilities.loadImageIcon("FTAsmall.gif");
	setIconImage(windowIcon.getImage());

	// setup a simple frame containing only an FTACanvas
	getContentPane().setLayout(new GridLayout(1,1));
	canvas = new FTACanvas(this.getContentPane(), this);
	canvas.setFTAFrame(fta);
	canvas.setFocusEventsEnabled(true);
	canvas.setTextEnabled(false);
	setSize(300, 200);
	setTitle("Tree Overview");
	getContentPane().add(canvas);
    }

    
    public void paint (Graphics g) {
	float widthRatio;
	float heightRatio;
	float clipWidth;
	float clipHeight;
	
	if((float)getContentPane().getWidth() > 0 && getContentPane().getHeight() > 0) {
	    // find the scale required for both x and y directions
	    widthRatio = (float)getContentPane().getWidth()/(float)canvas.nativeGetMaxWidth();
	    heightRatio = (float)getContentPane().getHeight()/(float)canvas.nativeGetMaxDepth();
	    
	    // use the smallest scale to ensure that tree fits in window
	    if(widthRatio < heightRatio) {
		canvas.setScale(widthRatio);
	    } else {
		canvas.setScale(heightRatio);
	    }
	} else {
	    canvas.setScale(0.01f);
	}

	// paint frame
	super.paint(g);
    }

}

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

public class SetZoomDialog extends JDialog {

    //JTextField zoomText;
    JComboBox zoomCombo;
    JTextField zoomText;
    private int zoom;

    public SetZoomDialog(Frame owner) {

	super(owner);

	String[] zoomStrings = { "25", "50", "100", "150", "200" };

	// set up dialog box
	setSize(150, 100);
	setResizable(false);
	setTitle("Formal-FTA: Zoom");
	setVisible(false);
	setModal(true);

	getContentPane().setLayout(new GridLayout(2,1));
	
	// create new combo box
	zoomCombo = new JComboBox(zoomStrings);
	zoomCombo.setEditable(true);
	zoomText = (JTextField)(zoomCombo.getEditor().getEditorComponent());
	zoomText.setDocument((Document)new RestrictedDocument("0123456789"));

        JPanel zoomPanel = new JPanel(new FlowLayout());
	zoomPanel.add(zoomCombo);
	getContentPane().add(zoomPanel);

	JButton okButton = new JButton("OK");
	okButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    setVisible(false);
		}
	    });

        JPanel buttonPanel = new JPanel(new FlowLayout());
	okButton.setDefaultCapable(true);
	getRootPane().setDefaultButton(okButton);
	buttonPanel.add(okButton);

	getContentPane().add(buttonPanel);
    }
	
    public void showDialog(int z) {
	zoomText.setText(Integer.toString(z));
	setVisible(true);
    }

    public int getZoom(){
	Integer tempInt;

	tempInt = Integer.valueOf(zoomText.getText());

	return tempInt.intValue();
    }

}

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

public class WorkingDialog extends JDialog implements Runnable{

    JLabel busyTextLabel;
    JLabel busyIconLabel;
    JProgressBar progressBar;

    JButton stopButton;
    ImageIcon busyIcon;

    boolean _stopped;


    public WorkingDialog(Frame owner) {
	super(owner);
	setLocationRelativeTo(owner);
	initialise();
    }
    
    public WorkingDialog(Dialog owner) {
	super(owner);
	setLocationRelativeTo(owner);
	initialise();
    }

    private void initialise() {
	setSize(180, 120);	
	setLocation(300, 200);
       	setTitle("Working...");
	setVisible(false);
       	setModal(true);
	setResizable(false);
       	setDefaultCloseOperation(0);
	_stopped = false;
	getContentPane().setLayout(new BorderLayout());

	JPanel topPanel = new JPanel();
	topPanel.setLayout(new BorderLayout());

	busyTextLabel = new JLabel("Busy - please wait");
	topPanel.add(busyTextLabel, "Center");

	busyIcon = FTAUtilities.loadImageIcon("busy.gif");
	busyIconLabel = new JLabel(busyIcon);
	topPanel.add(busyIconLabel, "West");

	progressBar = new JProgressBar();
	topPanel.add(progressBar, "South");

	getContentPane().add(topPanel);

	stopButton = new JButton("Stop");
	stopButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    //System.out.println("'Stop' button pressed");
		    _stopped = true;
		}
	    });
	this.addWindowListener(new WindowAdapter() {
		public void windowClosing(WindowEvent e) {
		    _stopped = true;
		}
	    });

	// create panel to hold buttons
	JPanel buttonPanel = new JPanel(new FlowLayout());
	buttonPanel.add(stopButton);

	getContentPane().add(buttonPanel, BorderLayout.SOUTH);
    }

    public void reset() {
	progressBar.setValue(0);
	progressBar.setMinimum(0);
	progressBar.setMaximum(100);
	_stopped = false;
    }

    public void run() {
	setVisible(true);
	_stopped = false;
    }

    public boolean stopped() {
	return _stopped;
    }

    public void kill() {
	setVisible(false);
	_stopped = true;
    }

    public void setProgressBarMax(int min) {
	progressBar.setMaximum(min);
    }

    public void progressBarInc() {
	progressBar.setValue(progressBar.getValue()+1);
    }
}

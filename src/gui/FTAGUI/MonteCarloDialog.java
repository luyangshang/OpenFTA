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

public class MonteCarloDialog extends JDialog {

    // declare native methods
    private native void nativeGetTreeInfo();
    private native boolean nativeLambdaEventsExist();
    private native int nativeNumBasicEvents();
    //	private native void nativeOKButtonAction(int numSims, float unitTime);
    private native float nativeGetTimeEstimate(int numSims);

    WorkingDialog workingWindow;
    GenerateMonteCarlo generateMonteCarlo;
    MonteCarloDialog thisDialog;

    FloatField unitTimeText;
    IntegerField numSimsText;
    JLabel numEventsLabel;
    JLabel numSimsLabel;
    JLabel estimatedRunTimeLabel;

    JButton okButton;
    JButton cancelButton;
    JButton helpButton;

    int numBasicEvents;

    ImageIcon warningIcon;

    public MonteCarloDialog(Frame parent) {

	super(parent);

	thisDialog = this;
	setSize(350, 250);	
	setLocation(250, 200);
	setTitle("Formal-FTA: Monte Carlo");
	setVisible(false);
	setModal(true);

	workingWindow = new WorkingDialog(thisDialog);

	warningIcon = FTAUtilities.loadImageIcon("warning.gif");

	Box mainBox = new Box(BoxLayout.Y_AXIS);

	//
	// Number of events panel MonteCarloDialog parent
	//

	JPanel numEventsPanel = new JPanel();
	numEventsPanel.setLayout(new BorderLayout());

	numEventsLabel = new JLabel("There are n basic events in the tree");

	numEventsPanel.add(numEventsLabel, "West");

	mainBox.add(numEventsPanel);

	//
	// Number of simulations panel
	//

	JPanel numSimsPanel = new JPanel();
	GridBagLayout numSimsGridBag = new GridBagLayout();

	numSimsPanel.setLayout(numSimsGridBag);

	JLabel numSimsLabel = new JLabel("Number of simulations:  ");
	
	numSimsText = new IntegerField();
	numSimsText.setMinimumSize(new Dimension(75, (int)numSimsText.getMinimumSize().getHeight()));
	numSimsText.setMaximumSize(new Dimension(75, (int)numSimsText.getMinimumSize().getHeight()));
	numSimsText.setPreferredSize(new Dimension(75, (int)numSimsText.getMinimumSize().getHeight()));

	numSimsText.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    setTimeEstimate();
		}
	    });

	numSimsPanel.add(numSimsLabel);
	numSimsPanel.add(numSimsText);

	JPanel numSimsWrapperPanel = new JPanel();
	numSimsWrapperPanel.setLayout(new BorderLayout());
	numSimsWrapperPanel.setBorder(BorderFactory.createTitledBorder(""));
	numSimsWrapperPanel.add(numSimsPanel, "West");

	mainBox.add(numSimsWrapperPanel);

	//
	// Unit time panel
	//

	JPanel unitTimePanel = new JPanel();
	GridBagLayout unitTimeGridBag = new GridBagLayout();

	unitTimePanel.setLayout(unitTimeGridBag);

	JLabel unitTimeLabel = new JLabel("Unit Time:  ");
	
	unitTimeText = new FloatField();
	unitTimeText.setMinimumSize(new Dimension(75, (int)unitTimeText.getMinimumSize().getHeight()));
	unitTimeText.setMaximumSize(new Dimension(75, (int)unitTimeText.getMinimumSize().getHeight()));
	unitTimeText.setPreferredSize(new Dimension(75, (int)unitTimeText.getMinimumSize().getHeight()));


	unitTimePanel.add(unitTimeLabel);
	unitTimePanel.add(unitTimeText);

	JPanel unitTimeWrapperPanel = new JPanel();
	unitTimeWrapperPanel.setLayout(new BorderLayout());
	unitTimeWrapperPanel.setBorder(BorderFactory.createTitledBorder(""));
	unitTimeWrapperPanel.add(unitTimePanel, "West");

	mainBox.add(unitTimeWrapperPanel);

	//
	// Estimated run time panel
	//

	estimatedRunTimeLabel = new JLabel("Estimated run time: xs");

	JPanel estimatedRunTimePanel = new JPanel();
	estimatedRunTimePanel.setLayout(new BorderLayout());
	estimatedRunTimePanel.setBorder(BorderFactory.createTitledBorder(""));
	estimatedRunTimePanel.add(estimatedRunTimeLabel, "West");

	mainBox.add(estimatedRunTimePanel);

	//
	// Button panel 
	//

	JPanel buttonPanel = new JPanel(new FlowLayout());
	//buttonPanel.setBorder(BorderFactory.createTitledBorder(""));
	//buttonPanel.setLayout(new GridLayout(1,3));

	JButton okButton = new JButton("OK");
	okButton.setDefaultCapable(true);
	getRootPane().setDefaultButton(okButton);
	okButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {

		    if(numSimsText.getIntegerValue() > 0) {

			workingWindow.reset();
			
			Thread generateMonteCarloThread = new Thread(new GenerateMonteCarlo(numSimsText.getIntegerValue(), unitTimeText.getFloatValue(), thisDialog));
			
			setVisible(false);

			// set priority of native thread
			FTAUtilities.setThreadPriority(generateMonteCarloThread);	    

			generateMonteCarloThread.start();
			workingWindow.run();
		    } else {
			postWarning("Must specify positive number of simulations", "Formal-FTA: Warning");
		    }
		}
	    });

	JButton cancelButton = new JButton("Cancel");
	cancelButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    setVisible(false);
		}
	    });

	JButton helpButton = new JButton("Help");
	helpButton.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		BrowserLauncher.openHelpFile(new String("fta_dialog_monte.html"));
		}
	    });

	buttonPanel.add(okButton);
	buttonPanel.add(cancelButton);
	buttonPanel.add(helpButton);

	mainBox.add(buttonPanel);

	//
	// Add scroll bars and finish
	//

	JScrollPane mainScrollPane = new JScrollPane();
	mainScrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
	mainScrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
	mainScrollPane.setViewportView(mainBox);

	getContentPane().add(mainScrollPane, "Center");
    }

    public void initialise() {
	setSize(350, 250);	
	setLocation(250, 200);

	nativeGetTreeInfo();

	numBasicEvents = nativeNumBasicEvents();
	numEventsLabel.setText("There are " + numBasicEvents +
			       " basic events in the tree");
	unitTimeText.setEnabled(nativeLambdaEventsExist());
	unitTimeText.setText("1.0");

	numSimsText.setText("10000");

	setTimeEstimate();

	setVisible(true);
    }

    private void setTimeEstimate() {
	float seconds; 
	seconds = nativeGetTimeEstimate(numSimsText.getIntegerValue());
	float minutes = seconds/60;
	float hours   = minutes/60;
	float days    = hours/24;
	float years  = days/365;
	if (years > 1) {
	    estimatedRunTimeLabel.setText("Estimated run time : " + years + " years");
	} else if (days > 10) {
	    estimatedRunTimeLabel.setText("Estimated run time : " + (int)(days + 0.5F) + " days");
	} else if (days > 1) {
	    int ihours = (int)(hours + 0.5);
	    estimatedRunTimeLabel.setText("Estimated run time : " + ihours/24 + "d "
					  + ihours%24 + "h");
	} else if (hours > 1) {
	    int imins = (int)(minutes + 0.5);
	    estimatedRunTimeLabel.setText("Estimated run time : " + imins/60 + "h "
					  + imins%60 + "m");
	} else if (minutes > 1) {
	    int isecs = (int)(seconds + 0.5);
	    estimatedRunTimeLabel.setText("Estimated run time : " + isecs/60 + "m " 
					  + isecs%60 + "s");
	} else {
	    estimatedRunTimeLabel.setText("Estimated run time : " + (int)(seconds + 0.5) + "s");
	}
    }

    public void postWarning(String warning, String title) {
	JOptionPane.showMessageDialog(this, warning, title,
				      JOptionPane.WARNING_MESSAGE, warningIcon);
    }

    public void postScrolledText(String title, String fileName) {
	ScrolledTextFrame textWindow;

	textWindow = new ScrolledTextFrame();
	textWindow.setWindowTitle(title);
	textWindow.readTextFile(fileName);
	textWindow.setVisible(true);
    }

    public boolean checkForInterrupt() {
	return workingWindow.stopped();
    }

    public void closeWorkingDialog() {
	workingWindow.kill();
	setVisible(false);
    }

    public void setProgressBarMax(int max) {
	workingWindow.setProgressBarMax(max);
    }

    public void progressBarInc() {
	workingWindow.progressBarInc();
    }
}

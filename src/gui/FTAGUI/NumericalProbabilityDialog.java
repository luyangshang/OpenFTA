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

public class NumericalProbabilityDialog extends JDialog {

    // declare native methods
    //	private native void nativeInitialiseNumericalProbabilityDialog();
    private native int nativeNumBasicEvents();
    private native boolean nativeGetMCSFile();
    private native String nativeGetMCSFileInfo();
    private native String nativeGetFileName();
    //	private native void nativeOKButtonAction(int chosenOrder, float unitTime, int chosenTerms);
    private native int nativeGetNumCutSets();
    private native boolean nativeLambdaEventsExist();
    private native float nativeGetTimeEstimate(int max_order, int min_term, int max_term);

    WorkingDialog workingWindow;
    NumericalProbabilityDialog thisDialog;
    GenerateNumericalProbability generateNumericalProbability;

    JTextField fileText;
    FloatField unitTimeText;
    JTextField numTermsText;
    JTextField limitSetsText;
    JLabel numEventsLabel;
    JLabel infoLabel;
    JLabel numTermsLabel;
    JLabel estimatedRunTimeLabel;

    JRadioButton allSetsButton;
    JRadioButton limitSetsButton;

    JButton orderUpButton;
    JButton orderDownButton;
    JButton termsUpButton;
    JButton termsDownButton;

    JButton okButton;
    JButton cancelButton;
    JButton helpButton;

    ImageIcon upArrowIcon;
    ImageIcon downArrowIcon;

    boolean generateAll;
    int order;
    int numBasicEvents;
    int terms;
    int numCutSets;
    float unitTime;
    int numItems;

    ImageIcon warningIcon;

    public NumericalProbabilityDialog(Frame parent) {

	super(parent);
	
	setSize(450, 350);	
	setLocation(250, 200);
	setTitle("Formal-FTA: Probabilities");
	setVisible(false);
	setModal(true);

	thisDialog = this;

	//nativeInitialiseNumericalProbabilityDialog();

	workingWindow = new WorkingDialog(thisDialog);

	warningIcon = FTAUtilities.loadImageIcon("warning.gif");

	Box mainBox = new Box(BoxLayout.Y_AXIS);

	//
	// Number of events panel
	//

	JPanel numEventsPanel = new JPanel();
	numEventsPanel.setLayout(new BorderLayout());

	numEventsLabel = new JLabel("There are n basic events in the tree");

	numEventsPanel.add(numEventsLabel, "West");

	mainBox.add(numEventsPanel);

	//
	// File information panel
	//

	Box fileBox = new Box(BoxLayout.Y_AXIS);

	fileText = new JTextField("");
	fileText.setText("blash blah blah blah blah blah blah blah");
	fileText.setEditable(false);

	fileBox.add(fileText);

	JPanel infoLabelPanel = new JPanel();
	infoLabelPanel.setLayout(new BorderLayout());

	infoLabel = new JLabel("(some file info)");

	infoLabelPanel.add(infoLabel, "West");

	fileBox.add(infoLabelPanel);

	JPanel fileWrapperPanel = new JPanel();
	fileWrapperPanel.setBorder(BorderFactory.createTitledBorder("Minimal cut sets file"));
	fileWrapperPanel.setLayout(new BorderLayout());

	fileWrapperPanel.add(fileBox, "West");
	
	mainBox.add(fileWrapperPanel);

	//
	// Cut sets order panel
	//

	Box generateBox = new Box(BoxLayout.Y_AXIS);

	allSetsButton = new JRadioButton("Generate all minimal cut sets");
	allSetsButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    generateAll = true;
		    setButtons();
		    setTimeEstimate();
		}
	    });

	limitSetsButton = new JRadioButton("Limit minimal cut set order: Maximum order");
	limitSetsButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    generateAll = false;
		    setButtons();
		    setTimeEstimate();
		}
	    });
    limitSetsText = new JTextField(new RestrictedNumberDocument(), "", 2);
    limitSetsText.addActionListener(new ActionListener() {
        public void actionPerformed(ActionEvent e) {
            if (limitSetsText.getText().length()== 0) {
                limitSetsText.setText("" + order);
            } else  { 
                order = Integer.parseInt(limitSetsText.getText());
                setTimeEstimate();
            }
        }
    });
	Box orderArrowBox = new Box(BoxLayout.Y_AXIS);

	upArrowIcon = FTAUtilities.loadImageIcon("upArrow.gif");
	downArrowIcon = FTAUtilities.loadImageIcon("downArrow.gif");
		
	orderUpButton = new JButton(upArrowIcon);
	orderUpButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    // copy order in case the text has changed in the box
		    order = Integer.parseInt(limitSetsText.getText());
		    if(order != numBasicEvents){
			order++;
			limitSetsText.setText("" + order);
			setTimeEstimate();
		    }
		}
	    });

	orderDownButton = new JButton(downArrowIcon);
	orderDownButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    // copy order in case the text has changed in the box
		    order = Integer.parseInt(limitSetsText.getText());
		    if(order != 1){
			order--;
			limitSetsText.setText("" + order);
			setTimeEstimate();
		    }
		}
	    });

	orderArrowBox.add(orderUpButton);
	orderArrowBox.add(orderDownButton);
	
	JPanel limitOrderPanel = new JPanel();
	GridBagLayout limitOrderGridBag = new GridBagLayout();
	GridBagConstraints limitOrderConstraints = new GridBagConstraints();
	limitOrderConstraints .gridwidth = GridBagConstraints.RELATIVE;

	limitOrderPanel.setLayout(limitOrderGridBag);

	limitOrderPanel.add(limitSetsButton);
	limitOrderPanel.add(limitSetsText);
	limitOrderPanel.add(orderArrowBox);

	JPanel limitOrderWrapperPanel = new JPanel();
	limitOrderWrapperPanel.setLayout(new BorderLayout());
	limitOrderWrapperPanel.add(limitOrderPanel, "West");
	limitOrderWrapperPanel.setAlignmentX(0.0F);

	JPanel allSetsWrapperPanel = new JPanel();
	allSetsWrapperPanel.setLayout(new BorderLayout());
	allSetsWrapperPanel.add(allSetsButton, "West");
	allSetsWrapperPanel.setAlignmentX(0.0F);

	generateBox.add(allSetsWrapperPanel);
	generateBox.add(limitOrderWrapperPanel);
		
	JPanel generateWrapperPanel = new JPanel();
	generateWrapperPanel.setLayout(new BorderLayout());
	generateWrapperPanel.setBorder(BorderFactory.createTitledBorder(""));

	generateWrapperPanel.add(generateBox, "West");

	mainBox.add(generateWrapperPanel);
	
	//
	// Number of terms panel
	//

	JPanel numTermsPanel = new JPanel();
	GridBagLayout numTermsGridBag = new GridBagLayout();

	numTermsPanel.setLayout(numTermsGridBag);

	numTermsLabel = new JLabel("Number of terms:");
	numTermsText = new JTextField(new RestrictedNumberDocument(), "", 2);
	numTermsText.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if (numTermsText.getText().length() == 0) {
			numTermsText.setText(""+terms);
		    } else {
			terms = Integer.parseInt(numTermsText.getText());
			setTimeEstimate();
		    }
		}
	    });
	Box termsArrowBox = new Box(BoxLayout.Y_AXIS);
		
	termsUpButton = new JButton(upArrowIcon);
	termsUpButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    // copy terms in case the text has changed in the box
		    terms = Integer.parseInt(numTermsText.getText());
		    if(terms != numCutSets){
			terms++;
			numTermsText.setText("" + terms);
			setTimeEstimate();
		    }
		}
	    });

	termsDownButton = new JButton(downArrowIcon);
	termsDownButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    // copy terms in case the text has changed in the box
		    terms = Integer.parseInt(numTermsText.getText());
		    if(terms != 1){
			terms--;
			numTermsText.setText("" + terms);
			setTimeEstimate();
		    }
		}
	    });

	termsArrowBox.add(termsUpButton);
	termsArrowBox.add(termsDownButton);
	
	numTermsPanel.add(numTermsLabel);
	numTermsPanel.add(numTermsText);
	numTermsPanel.add(termsArrowBox);

	JPanel numTermsWrapperPanel = new JPanel();
	numTermsWrapperPanel.setLayout(new BorderLayout());
	numTermsWrapperPanel.setBorder(BorderFactory.createTitledBorder(""));
	numTermsWrapperPanel.add(numTermsPanel, "West");

	mainBox.add(numTermsWrapperPanel);

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
		    Thread generateNumericalProbabilityThread;
		    if(numTermsText.getText().length() !=0) {
		        terms = Integer.parseInt(numTermsText.getText());
		    }
		    if(limitSetsText.getText().length() !=0) {
		        order = Integer.parseInt(limitSetsText.getText());
		    }		    
		    workingWindow.reset();

		    if(generateAll) {
			generateNumericalProbability = new GenerateNumericalProbability(numBasicEvents, unitTime, terms, thisDialog);
		    } else {
			generateNumericalProbability = new GenerateNumericalProbability(order, unitTime, terms, thisDialog);
		    }
		    
		    generateNumericalProbabilityThread = new Thread(generateNumericalProbability);

		    // set priority of native thread
		    FTAUtilities.setThreadPriority(generateNumericalProbabilityThread);

		    generateNumericalProbabilityThread.start();

		    workingWindow.run();

		    setVisible(false);
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
		BrowserLauncher.openHelpFile(new String("fta_dialog_probs.html"));
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
	setSize(450, 350);	
	setLocation(250, 200);
	generateAll = true;

	if(!nativeGetMCSFile()) {
	    postWarning("No minimal Cut Sets On File", "FTA: Warning");
	    setVisible(false);
	} else {
	    numBasicEvents = nativeNumBasicEvents();
	    numEventsLabel.setText("There are " + numBasicEvents +
				   " basic events in the tree");
	    order = numBasicEvents;
	    fileText.setText(nativeGetFileName());
	    infoLabel.setText(nativeGetMCSFileInfo());

	    unitTimeText.setEnabled(nativeLambdaEventsExist());
	    unitTime = 1.0F;
	    unitTimeText.setText("1.0");
	    numCutSets = nativeGetNumCutSets();
	    numItems = numCutSets;
	    terms = numItems;
	    ((RestrictedNumberDocument)numTermsText.getDocument()).setTotal
	        (numCutSets);
	    numTermsText.setText(""+numCutSets);
	    ((RestrictedNumberDocument)limitSetsText.getDocument()).setTotal
	        (numBasicEvents);
	     limitSetsText.setText(""+numBasicEvents);

	    setButtons();
	    setTimeEstimate();
	    setVisible(true);
	}
    }

    private void setButtons() {
	if(generateAll) {
	    allSetsButton.setSelected(true);
	    limitSetsButton.setSelected(false);
	    orderUpButton.setEnabled(false);
	    orderDownButton.setEnabled(false);
	    limitSetsText.setEnabled(false);
	} else {
	    allSetsButton.setSelected(false);
	    limitSetsButton.setSelected(true);
	    orderUpButton.setEnabled(true);
	    orderDownButton.setEnabled(true);
	    limitSetsText.setEnabled(true);
	}

    }

    private void setTimeEstimate() {
	float seconds; 
	if(generateAll) {
	    seconds = nativeGetTimeEstimate(numBasicEvents, 1, terms);
	} else {
	    seconds = nativeGetTimeEstimate(order, 1, terms);
	}

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
    }

    public void setProgressBarMax(int max) {
	workingWindow.setProgressBarMax(max);
    }

    public void progressBarInc() {
	workingWindow.progressBarInc();
    }
}

class GenerateNumericalProbability implements Runnable {
    private native void nativeInitialiseGenerateNumericalProbability();
    private native void nativeOKButtonAction(int chosenOrder, float unitTime, int chosenTerms);
    private int _chosenOrder;
    private float _unitTime;
    private int _chosenTerms;
    private NumericalProbabilityDialog _parent;

    public GenerateNumericalProbability(int chosenOrder, float unitTime, int chosenTerms, NumericalProbabilityDialog parent) {
	_chosenOrder = chosenOrder;
	_unitTime = unitTime;
	_chosenTerms = chosenTerms;
	_parent = parent;
	nativeInitialiseGenerateNumericalProbability();
    }

    public void run() {
	nativeOKButtonAction(_chosenOrder, _unitTime, _chosenTerms);
    }

    public boolean checkForInterrupt() {
	return _parent.checkForInterrupt();
    }

    public void closeWorkingDialog() {
	_parent.closeWorkingDialog();
    }

    public void postWarning(String title, String warning) {
	_parent.postWarning(title, warning);
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

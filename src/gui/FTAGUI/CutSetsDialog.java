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

public class CutSetsDialog extends JDialog {

    // declare native methods
    private native int nativeNumBasicEvents();
    private native boolean nativeGetMCSFile();
    private native String nativeGetMCSFileInfo();
    private native String nativeGetFileName();

    WorkingDialog workingWindow;
    CutSetsDialog thisDialog;
    GenerateCutSets generateCutSets;
	
    JTextField fileText;
    JLabel numEventsLabel;
    JLabel infoLabel;

    JCheckBox useFileButton;
    JRadioButton allSetsButton;
    JRadioButton limitSetsButton;

    JButton upButton;
    JButton downButton;

    ImageIcon downArrowIcon;
    ImageIcon upArrowIcon;
    
    private JTextField limitSetsText;

    int order;
    int numBasicEvents;
    boolean mcsFileExists;
    boolean useFile;
    boolean generateAll;

    public CutSetsDialog(Frame parent) {

	super(parent);

	setSize(450, 260);	
	setLocation(250, 200);
	setTitle("Formal-FTA: Minimal Cut Sets - Algebraic Method");
	setVisible(false);
	setModal(true);

	thisDialog = this;

	// nativeInitialiseCutSetsDialog();

	workingWindow = new WorkingDialog(thisDialog);

	Box mainBox = new Box(BoxLayout.Y_AXIS);

	//
	// Num events label panel
	//

	JPanel numEventsPanel = new JPanel();
	numEventsPanel.setLayout(new BorderLayout());

	numEventsLabel = new JLabel("There are n basic events in the tree");

	numEventsPanel.add(numEventsLabel, "West");

	mainBox.add(numEventsPanel);

	//
	// Use file panel
	// 

	Box useFileBox = new Box(BoxLayout.Y_AXIS);

	JPanel useFileButtonPanel = new JPanel();
	useFileButtonPanel.setLayout(new BorderLayout());

	useFileButton = new JCheckBox("Use minimal cut sets on file");
	useFileButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    useFile = !useFile;
		    setButtons();
		}
	    });

	useFileButtonPanel.add(useFileButton, "West");

	useFileBox.add(useFileButtonPanel);

	fileText = new JTextField("");
	fileText.setText("blash blah blah blah blah blah blah blah");
	fileText.setEditable(false);

	useFileBox.add(fileText);

	JPanel infoLabelPanel = new JPanel();
	infoLabelPanel.setLayout(new BorderLayout());

	infoLabel = new JLabel("(some file info)");

	infoLabelPanel.add(infoLabel, "West");

	useFileBox.add(infoLabelPanel);

	JPanel fileWrapperPanel = new JPanel();
	fileWrapperPanel.setBorder(BorderFactory.createTitledBorder("Minimal cut sets file"));
	fileWrapperPanel.setLayout(new BorderLayout());

	fileWrapperPanel.add(useFileBox, "West");

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
		}
	    });

	limitSetsButton = new JRadioButton("Limit minimal cut set order ");
	limitSetsButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    generateAll = false;
		    setButtons();
		}
	    });
	limitSetsText = new JTextField(new RestrictedNumberDocument(), "", 2);
	((RestrictedNumberDocument)limitSetsText.getDocument()).setTotal
	    (numBasicEvents);
	
	Box arrowBox = new Box(BoxLayout.Y_AXIS);

	upArrowIcon = FTAUtilities.loadImageIcon("upArrow.gif");
	downArrowIcon = FTAUtilities.loadImageIcon("downArrow.gif");
		
	upButton = new JButton(upArrowIcon);
	upButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if(order != numBasicEvents){
			order++;
			limitSetsText.setText(""+order);
		    }
		}
	    });

	downButton = new JButton(downArrowIcon);
	downButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if(order != 1){
			order--;
			limitSetsText.setText(""+order);
		    }
		}
	    });

	arrowBox.add(upButton);
	arrowBox.add(downButton);
	
	JPanel limitOrderPanel = new JPanel();
	GridBagLayout limitOrderGridBag = new GridBagLayout();
	limitOrderPanel.setLayout(limitOrderGridBag);

	limitOrderPanel.add(limitSetsButton);
	limitOrderPanel.add(limitSetsText);
	limitOrderPanel.add(arrowBox);

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
		    Thread generateCutSetsThread;
		    if(limitSetsText.getText().length() !=0) {
		        order = Integer.parseInt(limitSetsText.getText());
		    }		    

		    workingWindow.reset();

		    if(generateAll) {
			generateCutSets = new GenerateCutSets(useFile, numBasicEvents, thisDialog);
		    } else {
			generateCutSets = new GenerateCutSets(useFile, order, thisDialog);
		    }

		    generateCutSetsThread = new Thread(generateCutSets);

		    // set priority of native thread
		    FTAUtilities.setThreadPriority(generateCutSetsThread);

		    generateCutSetsThread.start();

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
		    BrowserLauncher.openHelpFile(new String("fta_dialog_cut_set.html"));
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
	
    public void initialise(){
	setSize(450, 260);	
	setLocation(250, 200);
	generateAll = true;

	numBasicEvents = nativeNumBasicEvents();
	numEventsLabel.setText("There are " + numBasicEvents +
			       " basic events in the tree");
	order = numBasicEvents;
	mcsFileExists = nativeGetMCSFile();
	if(mcsFileExists) {
	    useFile = true;
	    fileText.setText(nativeGetFileName());
	    infoLabel.setText(nativeGetMCSFileInfo());
	} else {
	    useFile = false;
	    fileText.setText("");
	    infoLabel.setText("No minimal cut sets on file");
	}
	
	limitSetsButton.setText("Limit minimal cut set order to " );
	((RestrictedNumberDocument)limitSetsText.getDocument()).setTotal(order);
    limitSetsText.setText(""+order);
	setButtons();

	setVisible(true);

    }

    private void setButtons() {
	if(mcsFileExists) {
	    useFileButton.setEnabled(true);
	} else {	
	    useFileButton.setEnabled(false);
	}

	if(useFile && mcsFileExists) {
	    useFileButton.setSelected(true);
	    fileText.setEnabled(true);
	    infoLabel.setEnabled(true);

	    allSetsButton.setEnabled(false);
	    limitSetsButton.setEnabled(false);
	    upButton.setEnabled(false);
	    downButton.setEnabled(false);
	} else {
	    useFileButton.setSelected(false);
	    fileText.setEnabled(false);
	    infoLabel.setEnabled(false);
	    allSetsButton.setEnabled(true);
	    limitSetsButton.setEnabled(true);

	    if(generateAll) {
		allSetsButton.setSelected(true);
		limitSetsButton.setSelected(false);
		upButton.setEnabled(false);
		downButton.setEnabled(false);
	    } else {
		allSetsButton.setSelected(false);
		limitSetsButton.setSelected(true);
		if(numBasicEvents > 1) {
		    upButton.setEnabled(true);
		    downButton.setEnabled(true);
		} else {
		    upButton.setEnabled(false);
		    downButton.setEnabled(false);
		}
	    }
	}
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

class GenerateCutSets implements Runnable {
    private native void nativeInitialiseGenerateCutSets();
    private native void nativeOKButtonAction(boolean useFile, int order);
    private boolean _useFile;
    private int _order;
    private CutSetsDialog _parent;

    public GenerateCutSets(boolean useFile, int order, CutSetsDialog parent){
	_useFile = useFile;
	_order = order;
	_parent = parent;
	nativeInitialiseGenerateCutSets();
    }

    public void run() {
	nativeOKButtonAction(_useFile, _order);
    }

    public boolean checkForInterrupt() {
        //System.out.println("GenerateCutSets: checkForInterrrupt()");
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

    

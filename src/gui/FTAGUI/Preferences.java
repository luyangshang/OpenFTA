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

public class Preferences extends JDialog {

    public static boolean monochrome = false;
    private boolean localMonochrome;
    private JRadioButton monochromeButton;
    private JRadioButton colourButton;
    private JButton penButton;
    private JPanel penSamplePanel;
    private JButton backgroundButton;
    private JPanel backgroundSamplePanel;
    private JButton highlightButton;
    private JPanel highlightSamplePanel;

    // symbol colours
    public static Color intermediateEventColor = new Color(250, 250, 200);
    public static Color basicInitiatingEventColor = new Color(210, 250, 200);
    public static Color undevelopedEventColor = new Color(210, 250, 200);
    public static Color externalEventColor = new Color(210, 250, 200);
    public static Color conditioningEventColor = new Color(200, 245, 250);
    public static Color andGateColor = new Color(250, 200, 200);
    public static Color orGateColor = new Color(250, 200, 200);
    public static Color priorityAndGateColor = new Color(250, 200, 200);
    public static Color exclusiveOrGateColor = new Color(250, 200, 200);
    public static Color inhibitGateColor = new Color(250, 200, 200);
    public static Color transferInColor = new Color(220, 200, 170);
    public static Color transferOutColor = new Color(220, 200, 170);

    // Browser location
    public static String browserLocation = new String();

    // localsymbol colours
    private Color localIntermediateEventColor;
    private Color localBasicInitiatingEventColor;
    private Color localUndevelopedEventColor;
    private Color localExternalEventColor;
    private Color localConditioningEventColor;
    private Color localAndGateColor;
    private Color localOrGateColor;
    private Color localPriorityAndGateColor;
    private Color localExclusiveOrGateColor;
    private Color localInhibitGateColor;
    private Color localTransferInColor;
    private Color localTransferOutColor;
    private Color localPenColor;
    private Color localBackgroundColor;
    private Color localHighlightColor;

    // general pen colours
    public static Color backgroundColor = new Color(255, 255, 255);
    public static Color printPreviewBackgroundColor = new Color(160, 160, 160);
    public static Color printPreviewPageColor = new Color(255, 255, 255);
    public static Color printPreviewPenColor = new Color(0, 0, 0);
    public static Color printPreviewShadowColor = new Color(0, 0, 0);
    public static Color highlightColor = new Color(255, 50, 50);
    public static Color penColor = new Color(0, 0, 0);
    public static Color blackColor = new Color(0, 0, 0);
    public static Color whiteColor = new Color(255, 255, 255);
    public static Color pageSeperatorColor = new Color(200, 0, 0);

    private Image intermediateEventMask;
    private Image basicInitiatingEventMask;
    private Image undevelopedEventMask;
    private Image externalEventMask;
    private Image conditioningEventMask;
    private Image andGateMask;
    private Image orGateMask;
    private Image priorityAndGateMask;
    private Image exclusiveOrGateMask;
    private Image inhibitGateMask;
    private Image transferInMask;
    private Image transferOutMask;

    private JButton intermediateEventButton;
    private JButton basicInitiatingEventButton;
    private JButton undevelopedEventButton;
    private JButton externalEventButton;
    private JButton conditioningEventButton;
    private JButton andGateButton;
    private JButton orGateButton;
    private JButton priorityAndGateButton;
    private JButton exclusiveOrGateButton;
    private JButton inhibitGateButton;
    private JButton transferInButton;
    private JButton transferOutButton;

    private JButton browserLocationButton;
    private JTextField browserLocationText;

    private JDialog thisPrefs;

    public Preferences(Frame parent) {

	super(parent);

	setSize(400, 380);
	setLocation(300, 300);
       	setTitle("Preferences");
	setVisible(false);
       	setModal(true);
	setResizable(true);
       	setDefaultCloseOperation(0);
	
	thisPrefs = this;

    //	makeLocalPrefsCopy();	

	// load masks for symbol icons
	intermediateEventMask = FTAUtilities.loadImage("intermediate event.gif");
	basicInitiatingEventMask = FTAUtilities.loadImage("basic initiating event.gif");
	undevelopedEventMask = FTAUtilities.loadImage("undeveloped event.gif");
	externalEventMask = FTAUtilities.loadImage("external event.gif");
	conditioningEventMask = FTAUtilities.loadImage("conditioning event.gif");
	andGateMask = FTAUtilities.loadImage("and gate.gif");
	orGateMask = FTAUtilities.loadImage("or gate.gif");
	priorityAndGateMask = FTAUtilities.loadImage("priority and gate.gif");
	exclusiveOrGateMask = FTAUtilities.loadImage("exclusive or gate.gif");
	inhibitGateMask = FTAUtilities.loadImage("inhibit gate.gif");
	transferInMask = FTAUtilities.loadImage("transfer in.gif");
	transferOutMask = FTAUtilities.loadImage("transfer out.gif");

	JPanel mainPanel = new JPanel();
	mainPanel.setLayout(new BorderLayout());
	
	JPanel centerPanel = new JPanel();
	centerPanel.setLayout(new BorderLayout());


	Box monochromeBox = new Box(BoxLayout.X_AXIS);

        monochromeButton = new JRadioButton("Monochrome");
	monochromeButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    localMonochrome = true;
		    colourButton.setSelected(false);
		    colourSymbolButtons();
		    colourPenButtons();
		}
	    });

	colourButton = new JRadioButton("Colour");
	colourButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    localMonochrome = false;
		    monochromeButton.setSelected(false);
		    colourSymbolButtons();
		    colourPenButtons();
		}
	    });

	monochromeBox.add(monochromeButton);
	monochromeBox.add(colourButton);

	JPanel viewPanel = new JPanel();
	viewPanel.setBorder(BorderFactory.createTitledBorder("View"));

	viewPanel.add(monochromeBox);

    
    Box browserBox = new Box(BoxLayout.X_AXIS);
    
    JPanel browserPanel = new JPanel(new BorderLayout());
	browserPanel.setBorder(BorderFactory.createTitledBorder("Web Browser Launch Command"));

    browserLocationText = new JTextField();

    browserPanel.add(browserLocationText, BorderLayout.CENTER);

	browserLocationButton = new JButton("Edit...");
	browserLocationButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {

            JFileChooser chooser = new JFileChooser(browserLocationText.getText());
            FTAFileFilter filter = new FTAFileFilter("exe", "Executables");
            chooser.setFileFilter(filter);
            int returnVal = chooser.showOpenDialog(null);
            if(returnVal == JFileChooser.APPROVE_OPTION) {
                browserLocationText.setText(chooser.getSelectedFile().getPath());
            }
		}
	    });

    browserPanel.add(browserLocationButton, BorderLayout.EAST);

    JPanel browserAndViewPanel = new JPanel(new GridLayout(2,1));
 

    browserAndViewPanel.add(browserPanel);   
    browserAndViewPanel.add(viewPanel);

	centerPanel.add(browserAndViewPanel, BorderLayout.NORTH);

	JPanel symbolPanel = new JPanel();
	symbolPanel.setBorder(BorderFactory.createTitledBorder("Symbol Colours"));
	symbolPanel.setLayout(new GridLayout(2,1));

    JPanel symbolTopPanel = new JPanel();
    symbolTopPanel.setLayout(new FlowLayout());

    JPanel symbolBottomPanel = new JPanel();
    symbolBottomPanel.setLayout(new FlowLayout());

	intermediateEventButton = FTAUtilities.createButton("intermediate event");
	symbolTopPanel.add(intermediateEventButton);
	basicInitiatingEventButton = FTAUtilities.createButton("basic initiating event");
	symbolTopPanel.add(basicInitiatingEventButton);
	undevelopedEventButton = FTAUtilities.createButton("undeveloped event");
	symbolTopPanel.add(undevelopedEventButton);
	externalEventButton = FTAUtilities.createButton("external event");
	symbolTopPanel.add(externalEventButton);
	conditioningEventButton = FTAUtilities.createButton("conditioning event");
	symbolTopPanel.add(conditioningEventButton);
	andGateButton = FTAUtilities.createButton("and gate");
	symbolTopPanel.add(andGateButton);
	orGateButton = FTAUtilities.createButton("or gate");
	symbolBottomPanel.add(orGateButton);
	priorityAndGateButton = FTAUtilities.createButton("priority and gate");
	symbolBottomPanel.add(priorityAndGateButton);
	exclusiveOrGateButton = FTAUtilities.createButton("exclusive or gate");
	symbolBottomPanel.add(exclusiveOrGateButton);
	inhibitGateButton = FTAUtilities.createButton("inhibit gate");
	symbolBottomPanel.add(inhibitGateButton);
	transferInButton = FTAUtilities.createButton("transfer in");
	symbolBottomPanel.add(transferInButton);
	transferOutButton = FTAUtilities.createButton("transfer out");
	symbolBottomPanel.add(transferOutButton);

    symbolPanel.add(symbolTopPanel);
    symbolPanel.add(symbolBottomPanel);

	intermediateEventButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if(!localMonochrome) {
			Color c;
			c = JColorChooser.showDialog(thisPrefs, "Intermediate Event Colour", localIntermediateEventColor);
			if(c!=null) {
			    localIntermediateEventColor = c;
			    colourSymbolButtons();
			}
		    }
		}
	    });

	basicInitiatingEventButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if(!localMonochrome) {
			Color c;
			c = JColorChooser.showDialog(thisPrefs, "Basic Initiating Event Colour", localBasicInitiatingEventColor);
			if(c!=null) {
			    localBasicInitiatingEventColor = c;
			    colourSymbolButtons();
			}
		    }
		}
	    });

	undevelopedEventButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if(!localMonochrome) {
			Color c;
			c = JColorChooser.showDialog(thisPrefs, "Undeveloped Event Colour", localUndevelopedEventColor);
			if(c!=null) {
			    localUndevelopedEventColor = c;
			    colourSymbolButtons();
			}
		    }
		}
	    });

	externalEventButton.addActionListener(new ActionListener() {
	    public void actionPerformed(ActionEvent e) {
		    if(!localMonochrome) {
			Color c;
			c = JColorChooser.showDialog(thisPrefs, "External Event Colour", localExternalEventColor);
			if(c!=null) {
			    localExternalEventColor = c;
			    colourSymbolButtons();
			}
		    }
	    }
	});

	conditioningEventButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if(!localMonochrome) {
			Color c;
			c = JColorChooser.showDialog(thisPrefs, "Conditioning Event Colour", localConditioningEventColor);
			if(c!=null) {
			    localConditioningEventColor = c;
			    colourSymbolButtons();
			}
		    }
		}
	    });

	andGateButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if(!localMonochrome) {		 
			Color c;
			c = JColorChooser.showDialog(thisPrefs, "And Gate Colour", localAndGateColor);
			if(c!=null) {
			    localAndGateColor = c;
			    colourSymbolButtons();
			}
		    }
		}
	    });

	orGateButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if(!localMonochrome) {		   
			Color c;
			c = JColorChooser.showDialog(thisPrefs, "Or Gate Colour", localOrGateColor);
			if(c!=null) {
			    localOrGateColor = c;
			    colourSymbolButtons();
			}
		    }
		}
	    });

	priorityAndGateButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if(!localMonochrome) {		   
			Color c;
			c = JColorChooser.showDialog(thisPrefs, "Priority And Gate Colour", localPriorityAndGateColor);
			if(c!=null) {
			    localPriorityAndGateColor = c;
			    colourSymbolButtons();
			}
		    }
		}
	    });

	exclusiveOrGateButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if(!localMonochrome) {		    
			Color c;
			c = JColorChooser.showDialog(thisPrefs, "Exculsive Or Gate Colour", localExclusiveOrGateColor);
			if(c!=null) {
			    localExclusiveOrGateColor = c;
			    colourSymbolButtons();
			}
		    }
		}
	    });

	inhibitGateButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if(!localMonochrome) {		   
			Color c;
			c = JColorChooser.showDialog(thisPrefs, "Inhibit Gate Colour", localInhibitGateColor);
			if(c!=null) {
			    localInhibitGateColor = c;
			    colourSymbolButtons();
			}
		    }
		}
	    });

	transferInButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if(!localMonochrome) {		   
			Color c;
			c = JColorChooser.showDialog(thisPrefs, "Transfer In Colour", localTransferInColor);
			if(c!=null) {
			    localTransferInColor = c;
			    colourSymbolButtons();
			}
		    }
		}
	    });

	transferOutButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if(!localMonochrome) {		   
			Color c;
			c = JColorChooser.showDialog(thisPrefs, "Transfer Out Colour", localTransferOutColor);
			if(c!=null) {
			    localTransferOutColor = c;
			    colourSymbolButtons();
			}
		    }
		}
	    });

	centerPanel.add(symbolPanel, BorderLayout.CENTER);


	JPanel colourPanel = new JPanel(new GridLayout(1,3));

	JPanel penPanel = new JPanel(new FlowLayout());
	penPanel.setBorder(BorderFactory.createTitledBorder("Pen Colour"));
 
	penButton = new JButton("Edit...");
	penButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if(!localMonochrome) {		   
			Color c;
			c = JColorChooser.showDialog(thisPrefs, "Pen Colour", localPenColor);
			if(c!=null) {
			    localPenColor = c;
			    penSamplePanel.setBackground(localPenColor);
			    colourSymbolButtons();
			}
		    }
		}
	    });

    penSamplePanel = new JPanel();
    penSamplePanel.setBorder(BorderFactory.createLineBorder(Color.black));

    penSamplePanel.setSize(32, 32);

	penPanel.add(penButton);
    penPanel.add(penSamplePanel);

    colourPanel.add(penPanel);

	makeLocalPrefsCopy();

    JPanel highlightPanel = new JPanel(new FlowLayout());

	highlightPanel.setBorder(BorderFactory.createTitledBorder("Highlight Colour"));
 
	highlightButton = new JButton("Edit...");

	highlightButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if(!localMonochrome) {		   
			Color c;
			c = JColorChooser.showDialog(thisPrefs, "Highlight Colour", localHighlightColor);
			if(c!=null) {	makeLocalPrefsCopy();
			    localHighlightColor = c;
			    highlightSamplePanel.setBackground(localHighlightColor);
			    colourSymbolButtons();
			}
		    }
		}
	    });
	
    highlightSamplePanel = new JPanel();
    highlightSamplePanel.setBorder(BorderFactory.createLineBorder(Color.black));

    highlightPanel.add(highlightButton);
    highlightPanel.add(highlightSamplePanel);
 
	colourPanel.add(highlightPanel);

	JPanel backgroundPanel = new JPanel(new FlowLayout());
	backgroundPanel.setBorder(BorderFactory.createTitledBorder("Background Colour"));
 
	backgroundButton = new JButton("Edit...");

	backgroundButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if(!localMonochrome) {		   
			Color c;
			c = JColorChooser.showDialog(thisPrefs, "Background Colour", localBackgroundColor);
			if(c!=null) {
			    localBackgroundColor = c;
			    backgroundSamplePanel.setBackground(localBackgroundColor);
			    colourSymbolButtons();
			}
		    }
		}
	    });

    backgroundSamplePanel = new JPanel();
    backgroundSamplePanel.setBorder(BorderFactory.createLineBorder(Color.black));

	backgroundPanel.add(backgroundButton);
    backgroundPanel.add(backgroundSamplePanel);

	colourPanel.add(backgroundPanel);

    centerPanel.add(colourPanel, BorderLayout.SOUTH);

	mainPanel.add(centerPanel, BorderLayout.CENTER);

	JButton okButton = new JButton("OK");
	okButton.setDefaultCapable(true);
	getRootPane().setDefaultButton(okButton);
	okButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    makePublicPrefsCopy();
            savePreferences();
		    hide();
		}
	    });

	JButton defaultsButton = new JButton("Defaults");
	defaultsButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    setDefaults();
		    colourSymbolButtons();
		    colourPenButtons();
		}
	    });

	JButton cancelButton = new JButton("Cancel");
	cancelButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    hide();
		}
	    });

	JButton helpButton = new JButton("Help");
	helpButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    BrowserLauncher.openHelpFile(new String("fta_dialog_preferences.html"));
		}
	    });

	this.addWindowListener(new WindowAdapter() {
		public void windowClosing(WindowEvent e) {
		    hide();
		}
	    });

	JPanel buttonPanel = new JPanel(new FlowLayout());
	buttonPanel.add(okButton);
	buttonPanel.add(defaultsButton);
	buttonPanel.add(cancelButton);
	buttonPanel.add(helpButton);
	mainPanel.add(buttonPanel, BorderLayout.SOUTH);

	JScrollPane mainScrollPane = new JScrollPane();
	mainScrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
	mainScrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
	mainScrollPane.setViewportView(mainPanel);

	getContentPane().add(mainScrollPane);

	makeLocalPrefsCopy();	

	colourSymbolButtons();
	colourPenButtons();

    }

    private void colourPenButtons() {
	if(localMonochrome) {
	    penSamplePanel.setBackground(blackColor);
	    highlightSamplePanel.setBackground(blackColor);
	    backgroundSamplePanel.setBackground(whiteColor);
	} else {
	    penSamplePanel.setBackground(localPenColor);
	    highlightSamplePanel.setBackground(localHighlightColor);
	    backgroundSamplePanel.setBackground(localBackgroundColor);
	}
    }


    private void colourSymbolButtons() {
	if(localMonochrome) {

	    // paint everything in black and white

      	    intermediateEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(intermediateEventMask, blackColor, whiteColor, whiteColor)));
	  
	    basicInitiatingEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(basicInitiatingEventMask, blackColor, whiteColor, whiteColor)));
	 
	    undevelopedEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(undevelopedEventMask, blackColor, whiteColor, whiteColor)));

	    externalEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(externalEventMask, blackColor, whiteColor, whiteColor)));

	    conditioningEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(conditioningEventMask, blackColor, whiteColor, whiteColor)));

	    andGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(andGateMask, blackColor, whiteColor, whiteColor)));

	    orGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(orGateMask, blackColor, whiteColor, whiteColor)));

	    priorityAndGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(priorityAndGateMask, blackColor, whiteColor, whiteColor)));

	    exclusiveOrGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(exclusiveOrGateMask, blackColor, whiteColor, whiteColor)));

	    inhibitGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(inhibitGateMask, blackColor, whiteColor, whiteColor)));

	    transferInButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(transferInMask, blackColor, whiteColor, whiteColor)));

	    transferOutButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(transferOutMask, blackColor, whiteColor, whiteColor)));
	} else {

	    // paint everything using the current pen/background colour and using the apropriate fill colour

	    intermediateEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(intermediateEventMask, localPenColor, localIntermediateEventColor, localBackgroundColor)));
	    
	    basicInitiatingEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(basicInitiatingEventMask, localPenColor, localBasicInitiatingEventColor, localBackgroundColor)));
	    
	    undevelopedEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(undevelopedEventMask, localPenColor, localUndevelopedEventColor, localBackgroundColor)));

	    externalEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(externalEventMask, localPenColor, localExternalEventColor, localBackgroundColor)));

	    conditioningEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(conditioningEventMask, localPenColor, localConditioningEventColor, localBackgroundColor)));

	    andGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(andGateMask, localPenColor, localAndGateColor, localBackgroundColor)));

	    orGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(orGateMask, localPenColor, localOrGateColor, localBackgroundColor)));

	    priorityAndGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(priorityAndGateMask, localPenColor, localPriorityAndGateColor, localBackgroundColor)));

	    exclusiveOrGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(exclusiveOrGateMask, localPenColor, localExclusiveOrGateColor, localBackgroundColor)));

	    inhibitGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(inhibitGateMask, localPenColor, localInhibitGateColor, localBackgroundColor)));

	    transferInButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(transferInMask, localPenColor, localTransferInColor, localBackgroundColor)));

	    transferOutButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(transferOutMask, localPenColor, localTransferOutColor, localBackgroundColor)));
	}
    }

    private void makePublicPrefsCopy() {
	// setup public copies of local preference variables
	monochrome = localMonochrome;
	intermediateEventColor = localIntermediateEventColor;
	basicInitiatingEventColor = localBasicInitiatingEventColor;
	undevelopedEventColor = localUndevelopedEventColor;
	externalEventColor = localExternalEventColor;
	conditioningEventColor = localConditioningEventColor ;
	andGateColor = localAndGateColor;
	orGateColor = localOrGateColor;
	priorityAndGateColor = localPriorityAndGateColor;
	exclusiveOrGateColor = localExclusiveOrGateColor;
	inhibitGateColor = localInhibitGateColor;
	transferInColor = localTransferInColor;
	transferOutColor = localTransferOutColor;
	penColor = localPenColor;
	highlightColor = localHighlightColor;
	backgroundColor = localBackgroundColor;

    browserLocation = browserLocationText.getText();

    }

    private void makeLocalPrefsCopy() {
	// setup local copies of preference variables
	localMonochrome = monochrome;
	localIntermediateEventColor = intermediateEventColor;
	localBasicInitiatingEventColor = basicInitiatingEventColor;
	localUndevelopedEventColor = undevelopedEventColor;
	localExternalEventColor = externalEventColor;
	localConditioningEventColor = conditioningEventColor ;
	localAndGateColor = andGateColor;
	localOrGateColor = orGateColor;
	localPriorityAndGateColor = priorityAndGateColor;
	localExclusiveOrGateColor = exclusiveOrGateColor;
	localInhibitGateColor = inhibitGateColor;
	localTransferInColor = transferInColor;
	localTransferOutColor = transferOutColor;
	localPenColor = penColor;
	localHighlightColor = highlightColor;
	localBackgroundColor = backgroundColor;

    browserLocationText.setText(browserLocation);
    }

    public void showDialog() {

	makeLocalPrefsCopy();

	// setup correct colours for symbol buttons
	colourSymbolButtons();
	colourPenButtons();

	// setuop monochrome/colour radio buttons
	if(localMonochrome) {
	    monochromeButton.setSelected(true);
	    colourButton.setSelected(false);
	} else {
	    monochromeButton.setSelected(false);
	    colourButton.setSelected(true);
	}

	// show dialog
        show();
    }


    public void savePreferences() {

	//System.out.println("Saving Preferences...");

	// open preferences file
        File prefsFile = new File(AppLocation.getAppLocation() + File.separator + "ftaPrefs.dat");
	FileOutputStream fileOutStream;
	ObjectOutputStream objectOutStream;

	try {
	    fileOutStream = new FileOutputStream(prefsFile);
	    objectOutStream = new ObjectOutputStream(fileOutStream);
	    // write individual preferences to file
	    objectOutStream.writeBoolean(monochrome);
	    objectOutStream.writeObject(intermediateEventColor);
	    objectOutStream.writeObject(basicInitiatingEventColor);
	    objectOutStream.writeObject(undevelopedEventColor);
	    objectOutStream.writeObject(externalEventColor);
	    objectOutStream.writeObject(conditioningEventColor);
	    objectOutStream.writeObject(andGateColor);
	    objectOutStream.writeObject(orGateColor);
	    objectOutStream.writeObject(priorityAndGateColor);
	    objectOutStream.writeObject(exclusiveOrGateColor);
	    objectOutStream.writeObject(inhibitGateColor);
	    objectOutStream.writeObject(transferInColor);
	    objectOutStream.writeObject(transferOutColor);
	    objectOutStream.writeObject(penColor);
	    objectOutStream.writeObject(highlightColor);
	    objectOutStream.writeObject(backgroundColor);

	    objectOutStream.writeObject((String)browserLocationText.getText());
	} catch (java.io.FileNotFoundException ioe) {
	    System.out.println("java.io.FileNotFoundException: " + ioe);
	} catch (java.io.IOException ioe) {
	    System.out.println("java.io.IOException: " + ioe);
	}		    
    }

    public void loadPreferences() {

	//System.out.println("Loading Preferences...");

	// flag to indicate file IO error
        boolean error = false;

	// open preferences file
        File prefsFile = new File(AppLocation.getAppLocation() +  File.separator + "ftaPrefs.dat");
	FileInputStream fileInStream;
	ObjectInputStream objectInStream;

	try {
	    fileInStream = new FileInputStream(prefsFile);
	    objectInStream = new ObjectInputStream(fileInStream);
	    // write individual preferences to file
	    monochrome = objectInStream.readBoolean();
	    intermediateEventColor = (Color)objectInStream.readObject();
	    basicInitiatingEventColor = (Color)objectInStream.readObject();
	    undevelopedEventColor = (Color)objectInStream.readObject();
	    externalEventColor = (Color)objectInStream.readObject();
	    conditioningEventColor = (Color)objectInStream.readObject();
	    andGateColor = (Color)objectInStream.readObject();
	    orGateColor = (Color)objectInStream.readObject();
	    priorityAndGateColor = (Color)objectInStream.readObject();
	    exclusiveOrGateColor = (Color)objectInStream.readObject();
	    inhibitGateColor = (Color)objectInStream.readObject();
	    transferInColor = (Color)objectInStream.readObject();
	    transferOutColor = (Color)objectInStream.readObject();
	    penColor = (Color)objectInStream.readObject();
	    highlightColor = (Color)objectInStream.readObject();
	    backgroundColor = (Color)objectInStream.readObject();

	    browserLocation = (String)objectInStream.readObject();
	} catch (java.io.FileNotFoundException ioe) {
	    System.out.println("java.io.FileNotFoundException: " + ioe);
	    error = true;
	} catch (java.io.IOException ioe) {
	    error = true;
	    System.out.println("java.io.IOException: " + ioe);
	} catch (java.lang.ClassNotFoundException ioe) {
	    error = true;
	    System.out.println("java.lang.ClassNotFoundException: " + ioe);
	}		    

	if(error) {
	    setDefaults();
	    makePublicPrefsCopy();
	    savePreferences();
	}
    }
    
    private void setDefaults() {
	// symbol colours
	localIntermediateEventColor = new Color(250, 250, 200);
	localBasicInitiatingEventColor = new Color(210, 250, 200);
	localUndevelopedEventColor = new Color(210, 250, 200);
	localExternalEventColor = new Color(210, 250, 200);
	localConditioningEventColor = new Color(200, 245, 250);
	localAndGateColor = new Color(250, 200, 200);
	localOrGateColor = new Color(250, 200, 200);
	localPriorityAndGateColor = new Color(250, 200, 200);
	localExclusiveOrGateColor = new Color(250, 200, 200);
	localInhibitGateColor = new Color(250, 200, 200);
	localTransferInColor = new Color(220, 200, 170);
        localTransferOutColor = new Color(220, 200, 170);

	// monochrome setting
	localMonochrome = false;
	monochromeButton.setSelected(false);
	colourButton.setSelected(true);

	// general pen colours
	localBackgroundColor = new Color(255, 255, 255);
	localHighlightColor = new Color(255, 50, 50);
	localPenColor = new Color(0, 0, 0);


	// set default web browser command

	String osProp = System.getProperty("os.name");
	if(osProp.startsWith("Windows")) {
	    if (osProp.indexOf("9") >= 0 ||
		osProp.indexOf("Me") >= 0) {
		// Windows 9x/Me
		 browserLocationText.setText("command.com /c start");
	    } else {
		
		// Assume Windows NT/XP/2000
		browserLocationText.setText("cmd.exe /c start \"Open URL\"");
	    }
	} else {
	    // assume Linux
	    browserLocationText.setText("mozilla");
	}
    }

}

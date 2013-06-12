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
import java.awt.image.*;
import java.awt.geom.*;
import java.beans.*;
import java.util.*;
import java.io.*;
import java.applet.*;
import java.net.*;
import java.lang.*;

import java.awt.print.*;

public class FTAFrame extends JFrame {

    JFrame tempFrame;

    // declare native methods
    private native void nativeInitialiseFTAFrame();
    private native boolean nativeLoadFTAFile(String fileName);
    private native boolean nativeSaveFTAFile();
    private native boolean nativeSaveAsFTAFile(String fileName);
    private native boolean nativeDatabaseAction();
    private native boolean nativeFileExists();
    private native void nativeNewFTAFile();
    private native int nativeGetSelectedItemType();
    private native void nativeAddItem(int type);
    private native void nativeLinkAction();
    private native void nativeDeleteAction();
    private native void nativeUndoAction();
    private native void nativeShiftLeftAction();
    private native void nativeShiftRightAction();
    private native void nativeCutAction();
    private native void nativeCopyAction();
    private native void nativePasteAction();
    private native void nativeSetSensitivity();
    private native void nativeSetAnalysisAndReportMenu();
    private native void nativeAnalysisMenu(int menuItem);
    private native void nativeViewMenu(int menuItem);
    private native void nativePrintMenu(int menuItem);
    private native boolean nativeCloseFile();
	
    private PEDFrame ped;
    private FTAFrame fta;

    private JMenuBar menuBar;
    private FTACanvas canvas;
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
    private JLabel treeLabel;
    private JLabel dbLabel;

    private JMenuItem numericalProbabilityMenuItem;
    private JMenuItem viewQualitativeReportMenuItem;
    private JMenuItem viewProbabilityReportMenuItem;
    private JMenuItem viewMonteCarloReportMenuItem;
    private JMenuItem printQualitativeReportMenuItem;
    private JMenuItem printProbabilityReportMenuItem;
    private JMenuItem printMonteCarloReportMenuItem;

    private JScrollPane canvasScrollPane;

    private ImageIcon questionIcon;
    private ImageIcon warningIcon;
    private ImageIcon windowIcon;

    private CutSetsDialog cutSetsWindow;
    private NumericalProbabilityDialog numericalProbabilityWindow;
    private MonteCarloDialog monteCarloWindow;
    private TreeOverviewFrame treeOverviewFrame;
    public PrintPreferences printPrefs;
    private FTACanvas treeOverviewCanvas;
    private SetZoomDialog setZoomDialog;

    private String currentPath;

    private Preferences prefs;

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

    public FTAFrame(PEDFrame pedRef) {
     try {
        UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
    }
    catch(Exception x) {
    }
	ped = pedRef;
	fta = this;
	prefs = new Preferences(this);
        prefs.loadPreferences();
	printPrefs = new PrintPreferences(this);

	nativeInitialiseFTAFrame();

	setSize(800, 600);	
	setLocation(50, 50);
		
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

	questionIcon = FTAUtilities.loadImageIcon("question.gif");
	warningIcon = FTAUtilities.loadImageIcon("warning.gif");
	windowIcon = FTAUtilities.loadImageIcon("FTAsmall.gif");
	setIconImage(windowIcon.getImage());

	setWindowTitle("Untitled", "Untitled");

	buildMenus();
	buildFTAPanel();

	nativeSetSensitivity();
	updateAnalysisAndReportMenu();

       	setDefaultCloseOperation(0);
	this.addWindowListener(new WindowAdapter() {
		public void windowClosing(WindowEvent e) {
		    if (!(nativeCloseFile())){
			System.exit(0);
		    }
		}
	    });

	cutSetsWindow = new CutSetsDialog(this);
	numericalProbabilityWindow = new NumericalProbabilityDialog(this);
	monteCarloWindow = new MonteCarloDialog(this);

	treeOverviewFrame = new TreeOverviewFrame(this);
	setZoomDialog = new SetZoomDialog((Frame)this);
	setZoomDialog.setLocationRelativeTo((Component)this.getContentPane());
	currentPath = AppLocation.getAppLocation();
    }

    protected void buildMenus() {
	menuBar = new JMenuBar();
	menuBar.setOpaque(true);
	JMenu file = buildFileMenu();
	JMenu edit = buildEditMenu();
	JMenu view = buildViewMenu();
	JMenu analysis = buildAnalysisMenu();
	JMenu report = buildReportMenu();
	JMenu help = buildHelpMenu();

	menuBar.add(file);
	menuBar.add(edit);
	menuBar.add(view);
	menuBar.add(analysis);
	menuBar.add(report);
	menuBar.add(help);
	setJMenuBar(menuBar);	
    }

    protected JMenu buildFileMenu() {
	JMenu file = new JMenu("File");
	file.setMnemonic(KeyEvent.VK_F);
	JMenuItem newF = new JMenuItem("New", KeyEvent.VK_N);
	newF.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N, ActionEvent.CTRL_MASK));
	JMenuItem open = new JMenuItem("Open...", KeyEvent.VK_O);
	open.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, ActionEvent.CTRL_MASK));
	JMenuItem save = new JMenuItem("Save", KeyEvent.VK_S);
	save.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, ActionEvent.CTRL_MASK));
	JMenuItem saveAs = new JMenuItem("Save As...", KeyEvent.VK_A);
	saveAs.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_A, ActionEvent.CTRL_MASK));
	JMenuItem exportImage = new JMenuItem("Export Image...", KeyEvent.VK_E);
	JMenuItem printerSetup = new JMenuItem("Printer Setup...", KeyEvent.VK_R);
	JMenuItem pageSetup = new JMenuItem("Page Setup...", KeyEvent.VK_U);
	JMenuItem printPrev = new JMenuItem("Print Preview...", KeyEvent.VK_V);
	JMenuItem print = new JMenuItem("Print...", KeyEvent.VK_P);
	print.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_P, ActionEvent.CTRL_MASK));
	JMenuItem database = new JMenuItem("Database", KeyEvent.VK_D);
	JMenuItem exit = new JMenuItem("Exit", KeyEvent.VK_X);
	exit.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_Q, ActionEvent.CTRL_MASK));

	newF.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeNewFTAFile();
		    updateAnalysisAndReportMenu();
		    updateFrame();
		    currentPath = new String("");
		}
	    });

	open.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    openFileAction();
		    updateAnalysisAndReportMenu();
		    updateFrame();
		    ped.populateList();
		}
	    });

	save.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    saveFileAction();
		    updateFrame();
		}
	    });

	saveAs.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    saveAsFileAction();
		    updateFrame();
		}
	    });

	exportImage.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {

		    // setup file chooser
		    JFileChooser chooser = new JFileChooser(currentPath);
		    FTAFileFilter filter = new FTAFileFilter("jpg", "JPEG Files");
		    chooser.setFileFilter(filter);

		    // show file chooser
		    int returnVal = chooser.showSaveDialog((Component)fta.getContentPane());
		    if(returnVal == JFileChooser.APPROVE_OPTION) {
			
			// ammend file name if required
			String filename = chooser.getSelectedFile().getPath();
			if(!filename.endsWith(".jpg")){
			    filename = filename + ".jpg";
			}

			// create bufferd image to export
			Image image = canvas.createImage(canvas.getWidth(), canvas.getHeight());
			BufferedImage buffered;
			Graphics2D	g2;
			boolean saveFile = true;
			buffered = new BufferedImage( canvas.getWidth(),
						      canvas.getHeight(),
						      BufferedImage.TYPE_INT_RGB );
			g2 = buffered.createGraphics();
			canvas.paint(g2);

			// export image to jpeg file
			File file = new File(filename);

			// if file exists, ask user whether it's ok to overwrite it
			if(file.exists()) {
			    saveFile = askQuestion("File already exists, do you want to overwrite it?", "OpenFTA: Overwrite File?");
			}

			if(saveFile) {

			    try {
				BufferedOutputStream out =
				    new BufferedOutputStream(new FileOutputStream(file));
				com.sun.image.codec.jpeg.JPEGImageEncoder encoder = com.sun.image.codec.jpeg.JPEGCodec.createJPEGEncoder(out);
				com.sun.image.codec.jpeg.JPEGEncodeParam param = encoder.getDefaultJPEGEncodeParam(buffered);
				param.setQuality(1.0f, false);  // max quality
				encoder.setJPEGEncodeParam(param);
				encoder.encode(buffered);
				out.close();
			    } catch (java.io.FileNotFoundException x) {
				postWarning("Unable to write image file", "Export Image");
			    } catch (java.io.IOException x) {
				postWarning("Unable to write image file", "Export Image");				
			    }			    
			    
			}
		    }
		}
	    });

	printerSetup.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    // show page setup dialg
		    printPrefs.showPrinterSetupDialog();
		}
	    });

	pageSetup.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    printPrefs.showPageSetupDialog();
		}
	    });

	printPrev.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    printPrefs.showDialog();
		}
	    });

	print.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    printAction();
		}
	    });

	database.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeDatabaseAction();
		    if(!ped.isVisible()) {
			ped.setVisible(true);
		    }
		    updateFrame();
		}
	    });

	exit.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if (!(nativeCloseFile())){
			System.exit(0);
		    }
		}
	    });

	file.add(newF);
	file.add(open);
	file.add(save);
	file.add(saveAs);
	file.add(new JSeparator());
	file.add(exportImage);
	file.add(new JSeparator());
	file.add(printerSetup);
	file.add(pageSetup);
	file.add(printPrev);
	file.add(print);
	file.add(new JSeparator());
	file.add(database);
	file.add(new JSeparator());
	file.add(exit);

	return file;
    }

    protected JMenu buildEditMenu() {
	JMenu edit = new JMenu("Edit");
	edit.setMnemonic(KeyEvent.VK_E);
	JMenuItem undo = new JMenuItem("Undo", KeyEvent.VK_U);
	undo.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_Z, ActionEvent.CTRL_MASK));
	JMenuItem cut = new JMenuItem("Cut", KeyEvent.VK_C);
	cut.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_X, ActionEvent.CTRL_MASK));
	JMenuItem copy = new JMenuItem("Copy", KeyEvent.VK_O);
	copy.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_C, ActionEvent.CTRL_MASK));
	JMenuItem paste = new JMenuItem("Paste", KeyEvent.VK_P);
	paste.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_V, ActionEvent.CTRL_MASK));
	JMenuItem delete = new JMenuItem("Delete", KeyEvent.VK_D);
	delete.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_D, ActionEvent.CTRL_MASK));
	JMenuItem shiftLeft = new JMenuItem("Shift Left", KeyEvent.VK_L);
	shiftLeft.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_L, ActionEvent.CTRL_MASK));
	JMenuItem shiftRight = new JMenuItem("Shift Right", KeyEvent.VK_R);
	shiftRight.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_R, ActionEvent.CTRL_MASK));
	JMenuItem link = new JMenuItem("Link", KeyEvent.VK_I);
	link.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_I, ActionEvent.CTRL_MASK));
	JMenuItem selected = new JMenuItem("Selected...", KeyEvent.VK_S);
	JMenuItem preferences = new JMenuItem("Preferences...", KeyEvent.VK_R);

	undo.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeUndoAction();
		    updateFrame();
		}
	    });

	cut.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeCutAction();
		    updateFrame();
		}
	    });

	copy.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeCopyAction();
		    updateFrame();
		}
	    });

	paste.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativePasteAction();
		    updateFrame();
		}
	    });

	delete.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeDeleteAction();
		    updateFrame();
		}
	    });

	shiftLeft.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeShiftLeftAction();
		    updateFrame();
		}
	    });

	shiftRight.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeShiftRightAction();
		    updateFrame();
		}
	    });

	link.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeLinkAction();
		    updateFrame();
		}
	    });

	selected.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    canvas.editSelected();
		    updateFrame();
		}
	    });

	preferences.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    boolean m = Preferences.monochrome;
		    prefs.showDialog();
		    // update symbol icons and tree display
		    colourSymbolButtons();
		    updateFrame();
		}
	    });

	edit.add(undo);
	edit.add(new JSeparator());
	edit.add(cut);
	edit.add(copy);
	edit.add(paste);
	edit.add(delete);
	edit.add(new JSeparator());
	edit.add(shiftLeft);
	edit.add(shiftRight);
	edit.add(new JSeparator());
	edit.add(link);
	edit.add(new JSeparator());
	edit.add(selected);
	edit.add(new JSeparator());
	edit.add(preferences);

	return edit;
    }
 
    protected JMenu buildViewMenu() {
	JMenu view = new JMenu("View");
	view.setMnemonic(KeyEvent.VK_V);
	JMenuItem treeOverview = new JMenuItem("Tree Overview", KeyEvent.VK_T);
	JMenuItem zoomIn = new JMenuItem("Zoom In", KeyEvent.VK_I);
	JMenuItem zoomOut = new JMenuItem("Zoom Out", KeyEvent.VK_O);
	JMenuItem setZoom = new JMenuItem("Set Zoom...", KeyEvent.VK_S);

	treeOverview.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    treeOverviewFrame.setVisible(true);
		}
	    });

	zoomIn.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    canvas.zoomIn();
		    updateFrame();
		}
	    });

	zoomOut.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    canvas.zoomOut();
		    updateFrame();
		}
	    });

	setZoom.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    int zoom;

		    zoom = Math.round(canvas.getScale()*100.0f);

		    setZoomDialog.showDialog(zoom);

		    canvas.setScale((float)setZoomDialog.getZoom()/100.0f);
		    
		    updateFrame();
		}
	    });

	view.add(treeOverview);
	view.add(new JSeparator());
	view.add(zoomIn);
	view.add(zoomOut);
	view.add(setZoom);

	return view;
    }

    protected JMenu buildAnalysisMenu() {
	JMenu analysis = new JMenu("Analysis");
	analysis.setMnemonic(KeyEvent.VK_A);
	JMenuItem validateMenuItem = new JMenuItem("Validate", KeyEvent.VK_V);
	JMenuItem minimalCutSetsMenuItem = new JMenuItem("Minimal Cut Sets...", KeyEvent.VK_M);
	numericalProbabilityMenuItem = new JMenuItem("Numerical Probability...", KeyEvent.VK_N);
	JMenuItem monteCarloSimulationMenuItem = new JMenuItem("Monte Carlo Simulation...", KeyEvent.VK_O);

	validateMenuItem.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeAnalysisMenu(FTAConstants.ANALYSIS_VALIDATE_POS);
		    updateAnalysisAndReportMenu();
		}
	    });

	minimalCutSetsMenuItem.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeAnalysisMenu(FTAConstants.ANALYSIS_CUT_SET_POS);
		    updateAnalysisAndReportMenu();
		}
	    });

	numericalProbabilityMenuItem.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeAnalysisMenu(FTAConstants.ANALYSIS_PROBABILITIES_POS);
		    updateAnalysisAndReportMenu();
		}
	    });

	monteCarloSimulationMenuItem.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeAnalysisMenu(FTAConstants.ANALYSIS_MONTE_CARLO_POS);
		    updateAnalysisAndReportMenu();
		}
	    });

	analysis.add(validateMenuItem);
	analysis.add(minimalCutSetsMenuItem);
	analysis.add(numericalProbabilityMenuItem);
	analysis.add(monteCarloSimulationMenuItem);
	return analysis;
    }
 
    protected JMenu buildReportMenu() {
	JMenu report = new JMenu("Report");
	report.setMnemonic(KeyEvent.VK_R);
	JMenu reportView = new JMenu("View");
	reportView.setMnemonic(KeyEvent.VK_V);
	JMenu reportPrint = new JMenu("Print");
	reportPrint.setMnemonic(KeyEvent.VK_P);
	viewQualitativeReportMenuItem = new JMenuItem("Qualitative Report", KeyEvent.VK_Q);
	viewProbabilityReportMenuItem = new JMenuItem("Probability Report", KeyEvent.VK_P);
	viewMonteCarloReportMenuItem = new JMenuItem("Monte Carlo Report", KeyEvent.VK_M);
	printQualitativeReportMenuItem = new JMenuItem("Qualitative Report", KeyEvent.VK_Q);
	printProbabilityReportMenuItem = new JMenuItem("Probability Report", KeyEvent.VK_P);
	printMonteCarloReportMenuItem = new JMenuItem("Monte Carlo Report", KeyEvent.VK_M);

	viewQualitativeReportMenuItem.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeViewMenu(FTAConstants.VIEW_QUALITATIVE_POS);
		}
	    });

	viewProbabilityReportMenuItem.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeViewMenu(FTAConstants.VIEW_PROBABILITY_POS);
		}
	    });

	viewMonteCarloReportMenuItem.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeViewMenu(FTAConstants.VIEW_MONTE_CARLO_POS);
		}
	    });

	printQualitativeReportMenuItem.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativePrintMenu(FTAConstants.PRINT_QUALITATIVE_POS);
		}
	    });

	printProbabilityReportMenuItem.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativePrintMenu(FTAConstants.PRINT_PROBABILITY_POS);
		}
	    });

	printMonteCarloReportMenuItem.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativePrintMenu(FTAConstants.PRINT_MONTE_CARLO_POS);
		}
	    });

	reportView.add(viewQualitativeReportMenuItem);
	reportView.add(viewProbabilityReportMenuItem);
	reportView.add(viewMonteCarloReportMenuItem);
	reportPrint.add(printQualitativeReportMenuItem);
	reportPrint.add(printProbabilityReportMenuItem);
	reportPrint.add(printMonteCarloReportMenuItem);
	report.add(reportView);
	report.add(reportPrint);

	return report;
    }

    protected JMenu buildHelpMenu() {
	JMenu help = new JMenu("Help");
	help.setMnemonic(KeyEvent.VK_H);
	JMenuItem helpOnHelp = new JMenuItem("Help on Help", KeyEvent.VK_H);
	JMenuItem index = new JMenuItem("Index", KeyEvent.VK_I);
	JMenuItem userSupport = new JMenuItem("User Support", KeyEvent.VK_U);
	JMenuItem history = new JMenuItem("History", KeyEvent.VK_T);
	JMenuItem copyright = new JMenuItem("Copyright", KeyEvent.VK_C);

	helpOnHelp.addActionListener(new ActionListener(){
	    public void actionPerformed(ActionEvent e){
		BrowserLauncher.openHelpFile(new String("HelpFTA.html"));
		    }
	});
	index.addActionListener(new ActionListener(){
	    public void actionPerformed(ActionEvent e){
		BrowserLauncher.openHelpFile(new String("IndexFTA.html"));
		    }
	});
	userSupport.addActionListener(new ActionListener(){
	    public void actionPerformed(ActionEvent e){
		BrowserLauncher.openHelpFile(new String("UserSupportFTA.html"));
		    }
	});
	history.addActionListener(new ActionListener(){
	    public void actionPerformed(ActionEvent e){
		BrowserLauncher.openHelpFile(new String("History.html"));
		    }
	});
	copyright.addActionListener(new ActionListener(){
	    public void actionPerformed(ActionEvent e){
		BrowserLauncher.openHelpFile(new String("Copyright.html"));
		    }
	});

	help.add(helpOnHelp);
	help.add(index);
	help.add(userSupport);
	help.add(history);
	help.add(copyright);

	return help;
    }


    private void colourSymbolButtons() {
	if(Preferences.monochrome) {

	    // paint everything in black and white

      	    intermediateEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(intermediateEventMask, Preferences.blackColor, Preferences.whiteColor, Preferences.whiteColor)));
	  
	    basicInitiatingEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(basicInitiatingEventMask, Preferences.blackColor, Preferences.whiteColor, Preferences.whiteColor)));
	 
	    undevelopedEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(undevelopedEventMask, Preferences.blackColor, Preferences.whiteColor, Preferences.whiteColor)));

	    externalEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(externalEventMask, Preferences.blackColor, Preferences.whiteColor, Preferences.whiteColor)));

	    conditioningEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(conditioningEventMask, Preferences.blackColor, Preferences.whiteColor, Preferences.whiteColor)));

	    andGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(andGateMask, Preferences.blackColor, Preferences.whiteColor, Preferences.whiteColor)));

	    orGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(orGateMask, Preferences.blackColor, Preferences.whiteColor, Preferences.whiteColor)));

	    priorityAndGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(priorityAndGateMask, Preferences.blackColor, Preferences.whiteColor, Preferences.whiteColor)));

	    exclusiveOrGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(exclusiveOrGateMask, Preferences.blackColor, Preferences.whiteColor, Preferences.whiteColor)));

	    inhibitGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(inhibitGateMask, Preferences.blackColor, Preferences.whiteColor, Preferences.whiteColor)));

	    transferInButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(transferInMask, Preferences.blackColor, Preferences.whiteColor, Preferences.whiteColor)));

	    transferOutButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(transferOutMask, Preferences.blackColor, Preferences.whiteColor, Preferences.whiteColor)));
	} else {

	    // paint everything using the current pen/background colour and using the apropriate fill colour

	    intermediateEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(intermediateEventMask, Preferences.penColor, Preferences.intermediateEventColor, Preferences.backgroundColor)));
	    
	    basicInitiatingEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(basicInitiatingEventMask, Preferences.penColor, Preferences.basicInitiatingEventColor, Preferences.backgroundColor)));
	    
	    undevelopedEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(undevelopedEventMask, Preferences.penColor, Preferences.undevelopedEventColor, Preferences.backgroundColor)));

	    externalEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(externalEventMask, Preferences.penColor, Preferences.externalEventColor, Preferences.backgroundColor)));

	    conditioningEventButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(conditioningEventMask, Preferences.penColor, Preferences.conditioningEventColor, Preferences.backgroundColor)));

	    andGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(andGateMask, Preferences.penColor,  Preferences.andGateColor, Preferences.backgroundColor)));

	    orGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(orGateMask, Preferences.penColor,  Preferences.orGateColor, Preferences.backgroundColor)));

	    priorityAndGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(priorityAndGateMask, Preferences.penColor, Preferences.priorityAndGateColor, Preferences.backgroundColor)));

	    exclusiveOrGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(exclusiveOrGateMask, Preferences.penColor, Preferences.exclusiveOrGateColor, Preferences.backgroundColor)));

	    inhibitGateButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(inhibitGateMask, Preferences.penColor, Preferences.inhibitGateColor, Preferences.backgroundColor)));

	    transferInButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(transferInMask, Preferences.penColor, Preferences.transferInColor, Preferences.backgroundColor)));

	    transferOutButton.setIcon(new ImageIcon(FTAUtilities.fillImageColour(transferOutMask, Preferences.penColor, Preferences.transferOutColor, Preferences.backgroundColor)));
	}
    }

    protected void buildFTAPanel() {

	getContentPane().setLayout(new BorderLayout());

	JToolBar toolBar = new JToolBar();
	toolBar.setFloatable(false);
	toolBar.setBorder(new EmptyBorder(1,2,1,2));

	JButton newButton = FTAUtilities.createButton("new tree");
	newButton.setBorder(new EmptyBorder(1,2,1,2));
	toolBar.add(newButton);
	JButton openButton = FTAUtilities.createButton("open tree");
	openButton.setBorder(new EmptyBorder(1,2,1,2));
	toolBar.add(openButton);
	JButton saveButton = FTAUtilities.createButton("save tree");
	saveButton.setBorder(new EmptyBorder(1,2,1,2));
	toolBar.add(saveButton);
	JButton printButton = FTAUtilities.createButton("print tree");
	printButton.setBorder(new EmptyBorder(1,2,1,2));
	toolBar.add(printButton);

	toolBar.addSeparator();
	toolBar.add(new JSeparator(SwingConstants.VERTICAL));
	toolBar.addSeparator();

	JButton cutButton = FTAUtilities.createButton("cut sub-tree");
	cutButton.setBorder(new EmptyBorder(1,2,1,2));
	toolBar.add(cutButton);
	JButton copyButton = FTAUtilities.createButton("copy sub-tree");
	copyButton.setBorder(new EmptyBorder(1,2,1,2));
	toolBar.add(copyButton);
	JButton pasteButton = FTAUtilities.createButton("paste sub-tree");
	pasteButton.setBorder(new EmptyBorder(1,2,1,2));
	toolBar.add(pasteButton);
	JButton deleteButton = FTAUtilities.createButton("delete sub-tree");
	deleteButton.setBorder(new EmptyBorder(1,2,1,2));
	toolBar.add(deleteButton);

	toolBar.addSeparator();
	toolBar.add(new JSeparator(SwingConstants.VERTICAL));
	toolBar.addSeparator();

	JButton linkButton = FTAUtilities.createButton("link");
	linkButton.setBorder(new EmptyBorder(1,2,1,2));
	toolBar.add(linkButton);

	toolBar.addSeparator();
	toolBar.add(new JSeparator(SwingConstants.VERTICAL));
	toolBar.addSeparator();

	JButton zoomInButton = FTAUtilities.createButton("zoom in");
	zoomInButton.setBorder(new EmptyBorder(1,2,1,2));
	toolBar.add(zoomInButton);
	JButton zoomOutButton = FTAUtilities.createButton("zoom out");
	zoomOutButton.setBorder(new EmptyBorder(1,2,1,2));
	toolBar.add(zoomOutButton);

	newButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeNewFTAFile();
		    updateAnalysisAndReportMenu();
		    updateFrame();
		}
	    });

	openButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    openFileAction();
		    updateAnalysisAndReportMenu();
		    updateFrame();
		    ped.populateList();
		}
	    });

	printButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    printAction();
		}
	    });

	saveButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    saveFileAction();
		    updateFrame();
		}
	    });

	cutButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeCutAction();
		    updateFrame();
		}
	    });

	copyButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeCopyAction();
		    updateFrame();
		}
	    });

	pasteButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativePasteAction();
		    updateFrame();
		}
	    });

	deleteButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeDeleteAction();
		    updateFrame();
		}
	    });

	linkButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeLinkAction();
		    updateFrame();
		}
	    });

	zoomInButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    canvas.zoomIn();
		    updateFrame();
		}
	    });

	zoomOutButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    canvas.zoomOut();
		    updateFrame();
		}
	    });

	JPanel toolBarPanel = new JPanel();
	toolBarPanel.setLayout(new BorderLayout());

	toolBarPanel.add(toolBar, "West");

	JButton logoButton = FTAUtilities.createButton("logo");
	logoButton.setBorderPainted(false);
	logoButton.setBorder(new EmptyBorder(1,2,1,2));

	logoButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    BrowserLauncher.openURL("www.fsc.co.uk");
		}
	    });

	toolBarPanel.add(logoButton, "East");

	getContentPane().add(toolBarPanel, "North");

	Box buttonBox = new Box(BoxLayout.Y_AXIS);
	
	JToolBar buttonBar = new JToolBar(SwingConstants.VERTICAL);
	buttonBar.setBorder(null);
	//buttonBar.setMargin(new Insets(1,1,1,1));
    buttonBar.setBorder(null);//new EmptyBorder(new Insets(1,1,1,1)));
	intermediateEventButton = FTAUtilities.createButton("intermediate event");
	intermediateEventButton.setBorder(null);
	buttonBar.add(intermediateEventButton);
	basicInitiatingEventButton = FTAUtilities.createButton("basic initiating event");
	basicInitiatingEventButton.setBorder(null);
	buttonBar.add(basicInitiatingEventButton);
	undevelopedEventButton = FTAUtilities.createButton("undeveloped event");
	undevelopedEventButton.setBorder(null);
	buttonBar.add(undevelopedEventButton);
	externalEventButton = FTAUtilities.createButton("external event");
	externalEventButton.setBorder(null);
	buttonBar.add(externalEventButton);
	conditioningEventButton = FTAUtilities.createButton("conditioning event");
	conditioningEventButton.setBorder(null);
	buttonBar.add(conditioningEventButton);

	buttonBar.addSeparator();
	//buttonBar.add(new JSeparator(SwingConstants.HORIZONTAL));
	// buttonBar.addSeparator();
	
	andGateButton = FTAUtilities.createButton("and gate");
	andGateButton.setBorder(null);
	buttonBar.add(andGateButton);
	orGateButton = FTAUtilities.createButton("or gate");
	orGateButton.setBorder(null);
	buttonBar.add(orGateButton);
	priorityAndGateButton = FTAUtilities.createButton("priority and gate");
	priorityAndGateButton.setBorder(null);
	buttonBar.add(priorityAndGateButton);
	exclusiveOrGateButton = FTAUtilities.createButton("exclusive or gate");
	exclusiveOrGateButton.setBorder(null);
	buttonBar.add(exclusiveOrGateButton);
	inhibitGateButton = FTAUtilities.createButton("inhibit gate");
	inhibitGateButton.setBorder(null);
	buttonBar.add(inhibitGateButton);

	buttonBar.addSeparator();
	//buttonBar.add(new JSeparator(SwingConstants.HORIZONTAL));
	//buttonBar.addSeparator();;

	transferInButton = FTAUtilities.createButton("transfer in");
	transferInButton.setBorder(null);
	buttonBar.add(transferInButton);
	transferOutButton = FTAUtilities.createButton("transfer out");
	transferOutButton.setBorder(null);
	buttonBar.add(transferOutButton);
	
	

	intermediateEventButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeAddItem(FTAConstants.INTERMEDIATE_POS);
		    updateFrame();
		}
	    });

	basicInitiatingEventButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeAddItem(FTAConstants.BASIC_POS);
		    updateFrame();
		}
	    });

	undevelopedEventButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeAddItem(FTAConstants.UNDEVELOP_POS);
		    updateFrame();
		}
	    });

	externalEventButton.addActionListener(new ActionListener() {
	    public void actionPerformed(ActionEvent e) {
		nativeAddItem(FTAConstants.EXTERNAL_POS);
		updateFrame();
	    }
	});

	conditioningEventButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeAddItem(FTAConstants.COND_POS);
		    updateFrame();
		}
	    });

	andGateButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeAddItem(FTAConstants.AND_POS);
		    updateFrame();
		}
	    });

	orGateButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeAddItem(FTAConstants.OR_POS);				
		    updateFrame();
		}
	    });

	priorityAndGateButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeAddItem(FTAConstants.PRIORITY_AND_POS);				
		    updateFrame();
		}
	    });

	exclusiveOrGateButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeAddItem(FTAConstants.XOR_POS);
		    updateFrame();
		}
	    });

	inhibitGateButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeAddItem(FTAConstants.INHIBIT_POS);				
		    updateFrame();
		}
	    });

	transferInButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeAddItem(FTAConstants.TRANSIN_POS);				
		    updateFrame();
		}
	    });

	transferOutButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeAddItem(FTAConstants.TRANSOUT_POS);				
		    updateFrame();
		}
	    });

	colourSymbolButtons();

	JScrollPane buttonScrollPane = new JScrollPane();
	buttonScrollPane.setBorder(null);
	buttonScrollPane.setViewportBorder(null);
	JPanel buttonPanel = new JPanel();
	buttonPanel.add(buttonBar);
	buttonScrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
	buttonScrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
	buttonScrollPane.setViewportView(buttonPanel);
    
	getContentPane().add(buttonScrollPane, "West");

	canvasScrollPane = new JScrollPane();
	canvasScrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_ALWAYS);
	canvasScrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
	canvas = new FTACanvas(canvasScrollPane.getViewport(), this);
	canvas.setFTAFrame(this);
	canvasScrollPane.setViewportView(canvas);

	getContentPane().add(canvasScrollPane, "Center");
    }

    private void printAction() {
	// remember the scale of the canvas
	float scale = canvas.getScale();
	// set scale to 1.0 so print will fill page
	canvas.setScale(1.0f);
	PrintUtilities.printComponent(canvas, PrintPreferences.numPagesX, PrintPreferences.numPagesY, PrintPreferences.printerJob, PrintPreferences.pageFormat);
	// restore correct scale
	canvas.setScale(scale);
    }

    private void openFileAction() {

	JFileChooser chooser = new JFileChooser(currentPath);
	FTAFileFilter filter = new FTAFileFilter("fta", "FTA Files");
	chooser.setFileFilter(filter);
	int returnVal = chooser.showOpenDialog((Component)this.getContentPane());
	if(returnVal == JFileChooser.APPROVE_OPTION) {
	    //try {
		//System.out.println("You chose to open this file: " +
		//		   chooser.getSelectedFile().toURL());
	    //} catch(MalformedURLException e) {
		//System.out.println("You chose to save this file:Cannot convert file to URL");
        //}
	    if(nativeLoadFTAFile(chooser.getSelectedFile().getPath())) {
		int pathLength;
		pathLength =  chooser.getSelectedFile().getPath().length() - 
		    chooser.getSelectedFile().getName().length();
		currentPath = chooser.getSelectedFile().getPath().substring(0, pathLength);
	    }
	}
    }

    private void saveFileAction() {
	if(nativeFileExists()) {
	    if(!nativeSaveFTAFile()) {

	    }
	} else {
	    saveAsFileAction();
	}
    }


    private void saveAsFileAction() {

	JFileChooser chooser = new JFileChooser(currentPath);

	FTAFileFilter filter = new FTAFileFilter("fta", "FTA Files");
	chooser.setFileFilter(filter);
	int returnVal = chooser.showSaveDialog((Component)this.getContentPane());
	if(returnVal == JFileChooser.APPROVE_OPTION) {
	    //try {
		//System.out.println("You chose to save this file: " +
		//		   chooser.getSelectedFile().toURL());
	    //} catch(MalformedURLException e) {
		//System.out.println("You chose to save this file:Cannot convert file to URL");
        //}
	    String filename = chooser.getSelectedFile().getPath();
	    if(!filename.endsWith(".fta")){
		filename = filename + ".fta";
	    }
	    if(nativeSaveAsFTAFile(filename)) {
		int pathLength;
		pathLength =  chooser.getSelectedFile().getPath().length() - 
		    chooser.getSelectedFile().getName().length();
		currentPath = chooser.getSelectedFile().getPath().substring(0, pathLength);
	    }
	}
     }


    // activate/deactivate buttons

    private void switchButtons(boolean intermediateEventActive, boolean basicInitiatingEventActive, 
			       boolean undevelopedEventActive, boolean externalEventActive, boolean conditioningEventActive,
			       boolean andGateActive, boolean orGateActive, boolean priorityAndGateActive,
			       boolean exclusiveOrGateActive, boolean inhibitGateActive,
			       boolean transferInActive, boolean transferOutActive) {

	intermediateEventButton.setEnabled(intermediateEventActive);
	basicInitiatingEventButton.setEnabled(basicInitiatingEventActive);
	undevelopedEventButton.setEnabled(undevelopedEventActive);
	externalEventButton.setEnabled(externalEventActive);
	conditioningEventButton.setEnabled(conditioningEventActive);

	andGateButton.setEnabled(andGateActive);
	orGateButton.setEnabled(orGateActive);
	priorityAndGateButton.setEnabled(priorityAndGateActive);
	exclusiveOrGateButton.setEnabled(exclusiveOrGateActive);
	inhibitGateButton.setEnabled(inhibitGateActive);

	transferInButton.setEnabled(transferInActive);
	transferOutButton.setEnabled(transferOutActive);
    }

    public void activateButtons(){
	switchButtons(true, true, true, true, true,
		      true, true, true, true, true,
		      true, true);
    }

    public void setButton(int pos, boolean on){
	switch(pos) {
	case FTAConstants.INTERMEDIATE_POS:
	    intermediateEventButton.setEnabled(on);
	    break;
	case FTAConstants.BASIC_POS:
	    basicInitiatingEventButton.setEnabled(on);
	    break;
	case FTAConstants.UNDEVELOP_POS:
	    undevelopedEventButton.setEnabled(on);
	    break;
	case FTAConstants.EXTERNAL_POS:
	    externalEventButton.setEnabled(on);
	    break;
	case FTAConstants.COND_POS:
	    conditioningEventButton.setEnabled(on);
	    break;
	case FTAConstants.AND_POS:
	    andGateButton.setEnabled(on);
	    break;
	case FTAConstants.OR_POS:
	    orGateButton.setEnabled(on);
	    break;
	case FTAConstants.PRIORITY_AND_POS:
	    priorityAndGateButton.setEnabled(on);
	    break;
	case FTAConstants.XOR_POS:
	    exclusiveOrGateButton.setEnabled(on);
	    break;
	case FTAConstants.INHIBIT_POS:
	    inhibitGateButton.setEnabled(on);
	    break;
	case FTAConstants.TRANSIN_POS:
	    transferInButton.setEnabled(on);
	    break;
	case FTAConstants.TRANSOUT_POS:
	    transferOutButton.setEnabled(on);
	    break;
	default:
	    break;
	}	

    }


    public void setAnalysisAndReportMenuItem(int pos, boolean on){
	switch(pos) {
	case FTAConstants.VIEW_QUALITATIVE_POS:
	    numericalProbabilityMenuItem.setEnabled(on);
	    viewQualitativeReportMenuItem.setEnabled(on);
	    printQualitativeReportMenuItem.setEnabled(on);
	    break;
	case FTAConstants.VIEW_PROBABILITY_POS:
	    viewProbabilityReportMenuItem.setEnabled(on);
	    printProbabilityReportMenuItem.setEnabled(on);
	    break;
	case FTAConstants.VIEW_MONTE_CARLO_POS:
	    viewMonteCarloReportMenuItem.setEnabled(on);
	    printMonteCarloReportMenuItem.setEnabled(on);
	    break;
	default:
	    break;
	}
    }


    public boolean askQuestion(String question, String title) {

	int returnValue;
	
	returnValue = JOptionPane.showConfirmDialog(this, question, title,
						    JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE,
						    questionIcon);
	if(returnValue == JOptionPane.YES_OPTION) {
	    return true;
	} else {
	    return false;
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

    public void printTextFile(String fileName) {
	InputStreamReader sr;
	FileInputStream fs;
	JTextArea text = new JTextArea();
	boolean EOF = false;

	//System.out.println("Filename = " + fileName);

	// Try opening given file
	try {
	    fs = new FileInputStream(fileName);
	    sr = new InputStreamReader(fs);
	    text.read(sr, null);
	    
	    // print text string
	    TextPrinter tp = new TextPrinter(text.getText(), PrintPreferences.printerJob, PrintPreferences.pageFormat);
	    tp.print();
	    
	} catch (FileNotFoundException e) {
	    postWarning("Text file not found", "OpenFTA: Error");
	} catch (IOException e) {
	    postWarning("Error opening file", "OpenFTA: Error");
	} // try
	
    }


    public void updateFrame() {
	nativeSetSensitivity();
	canvas.invalidate();
	validate();
	treeOverviewFrame.repaint();
	repaint();
    }

    public void updateAnalysisAndReportMenu() {
	nativeSetAnalysisAndReportMenu();
	repaint();
    }

    public void setWindowTitle(String treeName, String databaseName) {
	setTitle("OpenFTA (Tree: " + treeName + ", Database: " +databaseName + ")");
    }


    public int getViewportWidth() {
	return canvasScrollPane.getViewport().getWidth();
    }

    public int getViewportHeight() {
	return canvasScrollPane.getViewport().getHeight();
    }

    private void invokeCutSetsDialog(){
	cutSetsWindow.initialise();
    }

    private void invokeNumericalProbabilityDialog(){
	numericalProbabilityWindow.initialise();
    }

    private void invokeMonteCarloDialog(){
	monteCarloWindow.initialise();
    }

    public void setCanvasFocus(int x, int y) {
	int tlx;
	int tly;
	int canvasWidth;
	int canvasHeight;

	tlx = Math.round((x*canvas.getScale()) - canvasScrollPane.getViewport().getWidth()/2);
	tly = Math.round((y*canvas.getScale()) - canvasScrollPane.getViewport().getHeight()/2);

	// is point too far to the left?
	if(tlx < 0) {
	    tlx = 0;
	}

	// is point too high?
	if(tly < 0) {
	    tly = 0;
	}

	// get canvas dimensions
	canvasWidth = Math.round(canvas.getWidth());
	canvasHeight = Math.round(canvas.getHeight());

	// is point too far to the right?
	if(tlx > canvasWidth - canvasScrollPane.getViewport().getWidth()) {
	    tlx = canvasWidth - canvasScrollPane.getViewport().getWidth();
	}

	// is point too low?
	if(tly > canvasHeight - canvasScrollPane.getViewport().getHeight()) {
	    tly = canvasHeight - canvasScrollPane.getViewport().getHeight();
	}

	// set focus
	canvasScrollPane.getViewport().setViewPosition(new Point(tlx, tly));
    }

    public void loadPEDFile(String file) {
	ped.loadPEDFile(file);
    }


    public void newPEDFile() {
	ped.newPEDFile();
    }

 
    public void selectPrimaryEvent(String id){
	ped.selectPrimaryEvent(id);
    }

}


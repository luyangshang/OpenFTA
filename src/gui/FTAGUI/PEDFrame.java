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

/* This class creates the PED (Primary Events Database) Dialog, used
 * for entering new, and modifying old, primary events and their
 * associated probablities into the Primary Events Database.
 * Author: Ben Duncan
 * Modified by: Rebecca Sparey-Taylor
 */
public class PEDFrame extends JFrame{

    // declare native methods
    private native void nativeInitialisePEDFrame();
    private native void nativeApplyEvent(String id, float prob, String desc,
					 boolean constantFailure, boolean dormant,
					 int type, int originalType);
    private native void nativeDeleteEvent(String id);
    private native float nativeGetProb();
    private native boolean nativeGetConstFailure();
    private native int nativeGetType();
    private native boolean nativeGetDormant();
    private native String nativeGetDesc();
    private native void nativeSetSelectedEvent(String event);
    private native boolean nativeEventSelected();
    public native void populateList();
    private native boolean nativeSavePEDFile();
    private native boolean nativeSaveAsPEDFile(String fileName);
    private native boolean nativeFileExists();
    private native boolean nativeLoadPEDFile(String fileName);
    private native boolean nativeNewPEDFile(boolean confirm);
    private native void nativeCopyAction();
    private native void nativePasteAction();

    private JMenuBar menuBar;
    private FTAList listPanel;

    //    private JTextField idText;
    private RestrictedLengthExclusiveField idText;
    private FloatField probText;
    private JTextArea descText;

    private JCheckBox constantFailureButton;
    private JCheckBox dormantButton;
    private JRadioButton basicButton;
    private JRadioButton externalButton;
    private JRadioButton undevelopedButton;
    private JRadioButton condNotAnalButton;
    private JRadioButton condAnalButton;

    private JButton applyButton;
    private JButton cancelButton;
    private JButton helpButton;

    private ImageIcon questionIcon;
    private ImageIcon warningIcon;
    private ImageIcon windowIcon;

    private int currentType;
    private int originalType;
    private int currentSelection;

    private String currentPath;

    private FTAFrame fta;
    private PEDFrame thisFrame;
    private String dbName;


    //Constructor
    public PEDFrame() {
     try {
        UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
    }
    catch(Exception x) {
    }

	setWindowTitle("Untitled");
	setSize(700, 500);	
	setLocation(200, 200);
		
	nativeInitialisePEDFrame();

	questionIcon = FTAUtilities.loadImageIcon("question.gif");
	warningIcon = FTAUtilities.loadImageIcon("warning.gif");
	windowIcon = FTAUtilities.loadImageIcon("PEDsmall.gif");
	setIconImage(windowIcon.getImage());

	//build the menus
	buildMenus();
	buildPEDPanel();

	currentType = PEDConstants.BASIC;
	originalType = PEDConstants.BASIC;
	basicButton.setSelected(true);
	currentSelection = 0;

	this.addWindowListener(new WindowAdapter() {
		public void windowClosing(WindowEvent e) {
		}
	    });

	thisFrame = this;

	currentPath = AppLocation.getAppLocation();
    }

    /* This method builds the File, Edit and Help menus for the frame and then
 * adds them onto the frame. 
 */
    protected void buildMenus() {
	menuBar = new JMenuBar();
	menuBar.setOpaque(true);
	JMenu file = buildFileMenu();
	JMenu edit = buildEditMenu();
	JMenu help = buildHelpMenu();

	menuBar.add(file);
	menuBar.add(edit);
	menuBar.add(help);
	setJMenuBar(menuBar);	
    }

    /* This method builds the File Menu. It adds the following menu options:
 * New, Open, Save, SaveAs and Print, along with their associated action 
 * listeners.
 */
    protected JMenu buildFileMenu() {
	//Create top level menu option File and shortcut key
	JMenu file = new JMenu("File");
	file.setMnemonic(KeyEvent.VK_F);

	//Create the menu items and their associated shortcut keys
	JMenuItem newF = new JMenuItem("New", KeyEvent.VK_N);
	newF.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N, ActionEvent.CTRL_MASK));
	JMenuItem open = new JMenuItem("Open...", KeyEvent.VK_O);
	open.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, ActionEvent.CTRL_MASK));
	JMenuItem save = new JMenuItem("Save", KeyEvent.VK_S);
	save.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, ActionEvent.CTRL_MASK));
	JMenuItem saveAs = new JMenuItem("Save As...", KeyEvent.VK_A);
	saveAs.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_A, ActionEvent.CTRL_MASK));
	JMenuItem exportTextFile = new JMenuItem("Export Text File...", KeyEvent.VK_E);
	JMenuItem printerSetup = new JMenuItem("Printer Setup...", KeyEvent.VK_R);
	JMenuItem pageSetup = new JMenuItem("Page Setup...", KeyEvent.VK_U);
	JMenuItem print = new JMenuItem("Print...", KeyEvent.VK_P);
	print.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_P, ActionEvent.CTRL_MASK));

	//Create action listeners for each of the menu items
	newF.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    newPEDFile();
		}
	    });

	open.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    openFileAction();
		}
	    });


	save.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    saveFileAction();
		}
	    });

	saveAs.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    saveAsFileAction();
		}
	    });

	exportTextFile.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {

		    // setup file chooser
		    JFileChooser chooser = new JFileChooser(currentPath);
		    FTAFileFilter filter = new FTAFileFilter("txt", "Text Files");
		    chooser.setFileFilter(filter);

		    // show file chooser
		    int returnVal = chooser.showSaveDialog((Component)fta.getContentPane());
		    if(returnVal == JFileChooser.APPROVE_OPTION) {
			
			// ammend file name if required
			String filename = chooser.getSelectedFile().getPath();
			if(!filename.endsWith(".txt")){
			    filename = filename + ".txt";
			}

			// export text to jpeg file
			File file = new File(filename);

			boolean saveFile = true;

			// if file exists, ask user whether it's ok to overwrite it
			if(file.exists()) {
			    saveFile = askQuestion("File already exists, do you want to overwrite it?", "OpenFTA: Overwrite File?");
			}

			if(saveFile) {

			    try {
				DataOutputStream out =
				    new DataOutputStream(new FileOutputStream(file));
				out.writeBytes(createReport());
				out.close();
			    } catch (java.io.FileNotFoundException x) {
				postWarning("Unable to write image file", "OpenPED: Export Image");
			    } catch (java.io.IOException x) {
				postWarning("Unable to write image file", "OpenPED: Export Image");				
			    }			    
			    
			}
		    }
		}
	    });

	printerSetup.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    // show page setup dialg
		    fta.printPrefs.showPrinterSetupDialog();
		    repaint();
		}
	    });

	pageSetup.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    fta.printPrefs.showPageSetupDialog();
		    repaint();
		}
	    });

	print.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    printAction();
		    repaint();
		}
	    });

	//Add each of the menu items into the File menu.
	file.add(newF);
	file.add(open);
	file.add(save);
	file.add(saveAs);
	file.add(new JSeparator());
	file.add(exportTextFile);
	file.add(new JSeparator());
	file.add(printerSetup);
	file.add(pageSetup);
	file.add(print);

	return file;
    }

    /* This method builds the Edit Menu. It adds the following menu options:
 * Copy, Cut, Paste and Delete, along with their associated action 
 * listeners.
 */
    protected JMenu buildEditMenu() {
	//Create top level Edit menu and keyboard shortcut
	JMenu edit = new JMenu("Edit");
	edit.setMnemonic(KeyEvent.VK_E);
	
	//Create menu items and associated keyboard shortcuts
	JMenuItem copy = new JMenuItem("Copy", KeyEvent.VK_C);
	copy.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_C, ActionEvent.CTRL_MASK));
	JMenuItem cut = new JMenuItem("Cut", KeyEvent.VK_X);
	cut.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_X, ActionEvent.CTRL_MASK));
	JMenuItem paste = new JMenuItem("Paste", KeyEvent.VK_V);
	paste.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_V, ActionEvent.CTRL_MASK));
	JMenuItem delete = new JMenuItem("Delete Event", KeyEvent.VK_D);
	delete.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_D, ActionEvent.CTRL_MASK));

	//Create action listeners for the menu items
	copy.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    DefaultEditorKit.CopyAction dek = new DefaultEditorKit.CopyAction();
		    dek.actionPerformed(e);
		}
	    });

	cut.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    DefaultEditorKit.CutAction dek = new DefaultEditorKit.CutAction();
		    dek.actionPerformed(e);
		}
	    });

	paste.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    DefaultEditorKit.PasteAction dek = new DefaultEditorKit.PasteAction();
		    dek.actionPerformed(e);
		}
	    });

	delete.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    deleteAction();
		}
	    });

	//Add each of the menu items into the main edit menu.
	edit.add(delete);
	edit.add(new JSeparator());
	edit.add(cut);    /* This method builds the Database Menu. It's a dynamic menu that lists the current open databases */
	edit.add(copy);
	edit.add(paste);

	return edit;
    }


    /* This method builds the Database Menu. It's a dynamic menu that lists the current open databases */
    protected JMenu buildDatabaseMenu() {
	//Create top level EditDatabase menu and keyboard shortcut
	JMenu database = new JMenu("Database");
	database.setMnemonic(KeyEvent.VK_D);
	
	//Create menu items and associated keyboard shortcuts
	JMenuItem untitled = new JMenuItem("Untitled");

	//Add each of the menu items into the main database  menu.
	database.add(untitled);

	return database;
    }


    /* This method builds the Help Menu. It adds the following menu options:
 * HelpOnHelp, Index, UserSupport and Copyright along with their associated action 
 * listeners.
 */
    protected JMenu buildHelpMenu() {
	//Create top level help menu
	JMenu help = new JMenu("Help");
	help.setMnemonic(KeyEvent.VK_H);

	//Create menu items for the Help menu
	JMenuItem helpOnHelp = new JMenuItem("Help on Help", KeyEvent.VK_H);
	JMenuItem index = new JMenuItem("Index", KeyEvent.VK_I);
	JMenuItem userSupport = new JMenuItem("User Support", KeyEvent.VK_U);
	JMenuItem history = new JMenuItem("History", KeyEvent.VK_T);
	JMenuItem copyright = new JMenuItem("Copyright", KeyEvent.VK_C);

	//Create Action Listeners for each of the menu items
	helpOnHelp.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    BrowserLauncher.openHelpFile(new String("HelpPED.html"));
		}
	    });
	index.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    BrowserLauncher.openHelpFile(new String("IndexPED.html"));
		}
	    });
	userSupport.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    BrowserLauncher.openHelpFile(new String("UserSupportPED.html"));
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

	//Add the menu items into the Help menu
	help.add(helpOnHelp);
	help.add(index);
	help.add(userSupport);
	help.add(history);
	help.add(copyright);

	return help;
    }

    /* This protected method is used to build the main body
 * of the PED Panel
 */
    protected void buildPEDPanel() {

	getContentPane().setLayout(new BorderLayout());

	//Create a toolbar
	JToolBar toolBar = new JToolBar();
	toolBar.setFloatable(false);

	//Create and add New, Open, Save and Print buttons to the toolbar
	JButton newButton = FTAUtilities.createButton("new database");
	newButton.setBorder(new EmptyBorder(1,2,1,2));
	toolBar.add(newButton);
	JButton openButton = FTAUtilities.createButton("open database");
	openButton.setBorder(new EmptyBorder(1,2,1,2));
	toolBar.add(openButton);
	JButton saveButton = FTAUtilities.createButton("save database");
	saveButton.setBorder(new EmptyBorder(1,2,1,2));
	toolBar.add(saveButton);
	JButton printButton = FTAUtilities.createButton("print database");
	printButton.setBorder(new EmptyBorder(1,2,1,2));
	toolBar.add(printButton);

	//Add a separator into the toolbar
	toolBar.addSeparator();
	toolBar.add(new JSeparator(SwingConstants.VERTICAL));
	toolBar.addSeparator();

	JButton deleteButton = FTAUtilities.createButton("delete event");
	deleteButton.setBorder(new EmptyBorder(1,2,1,2));
	toolBar.add(deleteButton);

	//Add a separator into the toolbar
	toolBar.addSeparator();
	toolBar.add(new JSeparator(SwingConstants.VERTICAL));
	toolBar.addSeparator();

	//Create and add Cut, Copy and Paste buttons to the toolbar
	JButton cutButton = FTAUtilities.createButton("cut text");
	cutButton.setBorder(new EmptyBorder(1,2,1,2));
	toolBar.add(cutButton);
	JButton copyButton = FTAUtilities.createButton("copy text");
	copyButton.setBorder(new EmptyBorder(1,2,1,2));
	toolBar.add(copyButton);
	JButton pasteButton = FTAUtilities.createButton("paste text");
	pasteButton.setBorder(new EmptyBorder(1,2,1,2));
	toolBar.add(pasteButton);

	//Add action listeners for each of the buttons on the toolbar
	newButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    newPEDFile();
		}
	    });

	openButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    openFileAction();
		}
	    });

	saveButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    saveFileAction();
		}
	    });

	printButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    printAction();
		}
	    });

	cutButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    DefaultEditorKit.CutAction dke = new DefaultEditorKit.CutAction();
		    dke.actionPerformed(e);
		}
	    });

	copyButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    DefaultEditorKit.CopyAction dke = new DefaultEditorKit.CopyAction();
		    dke.actionPerformed(e);
		}
	    });

	pasteButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    DefaultEditorKit.PasteAction dke = new DefaultEditorKit.PasteAction();
		    dke.actionPerformed(e);
		}
	    });

	deleteButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    deleteAction();
		}
	    });

	//Add FSC logo to the toolbar
	JPanel toolBarPanel = new JPanel();
	toolBarPanel.setLayout(new BorderLayout());
	toolBar.setBorder(new EmptyBorder(1,2,1,2));

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

	listPanel = new FTAList(this.getContentPane(), this);
	getContentPane().add(listPanel, "West");

	Box mainBox = new Box(BoxLayout.Y_AXIS);

	JPanel idPanel = new JPanel();
	//idPanel.setLayout(new GridLayout(2,1));
	idPanel.setLayout(new BorderLayout());
	idPanel.setBorder(BorderFactory.createTitledBorder("Id"));

	//Add an id text field
	//	idText = new JTextField();
	//idText = new RestrictLengthField(13);
	idText = new RestrictedLengthExclusiveField(13, new String(" "));
	idPanel.add(idText, BorderLayout.NORTH);

	//Create Probability panel and components
	JPanel probPanel = new JPanel();
	//probPanel.setLayout(new GridLayout(2,1));
	probPanel.setLayout(new BorderLayout());
	probPanel.setBorder(BorderFactory.createTitledBorder("Probability"));

	probText = new FloatField();
	constantFailureButton = new JCheckBox("Constant Failure Rate/Unit Time");

	probPanel.add(probText, BorderLayout.NORTH);
	probPanel.add(constantFailureButton, BorderLayout.CENTER);

	JPanel idProbPanel = new JPanel();
	idProbPanel.setLayout(new GridLayout(1,2));
	idProbPanel.setPreferredSize(new Dimension(450, 100));
       	idProbPanel.setMinimumSize(new Dimension(450, 100));
	idProbPanel.setMaximumSize(new Dimension((int)idProbPanel.getMaximumSize().getWidth(), 100));

	idProbPanel.add(idPanel);
	idProbPanel.add(probPanel);

	mainBox.add(idProbPanel);

	//Create Type radio button options and associated Action Listeners
	JPanel typePanel = new JPanel();
	typePanel.setBorder(BorderFactory.createTitledBorder("Type"));
	typePanel.setPreferredSize(new Dimension(450, 100));
       	typePanel.setMinimumSize(new Dimension(450, 100));
	typePanel.setMaximumSize(new Dimension((int)typePanel.getMaximumSize().getWidth(), 100));

	basicButton = new JRadioButton("Basic");
	externalButton = new JRadioButton("External");
	undevelopedButton = new JRadioButton("Undeveloped");
	condNotAnalButton = new JRadioButton("Conditional (not analysed)");
	condAnalButton = new JRadioButton("Conditional (analysed)");

	basicButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    setCurrentType(PEDConstants.BASIC);
		}
	    });

	externalButton.addActionListener(new ActionListener() {
	    public void actionPerformed(ActionEvent e) {
		setCurrentType(PEDConstants.EXTERNAL);
	    }
	});

	undevelopedButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    setCurrentType(PEDConstants.UNDEVELOPED);
		}
	    });

	condNotAnalButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    setCurrentType(PEDConstants.COND_NOT_ANAL);
		}
	    });

	condAnalButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    setCurrentType(PEDConstants.COND_ANAL);
		}
	    });

	typePanel.add(basicButton);
	typePanel.add(externalButton);
	typePanel.add(undevelopedButton);
	typePanel.add(condNotAnalButton);
	typePanel.add(condAnalButton);
	mainBox.add(typePanel);

	//Add dormant button
	dormantButton = new JCheckBox("Dormant");	
	mainBox.add(dormantButton);

	//Create Description panel and necessary components
	JPanel descPanel = new JPanel();
	descPanel.setLayout(new GridLayout(1,1));
	descPanel.setBorder(BorderFactory.createTitledBorder("Description"));

	descText = new JTextArea();
		
	JScrollPane textScrollPane = new JScrollPane();
       	textScrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
       	textScrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);

	textScrollPane.setViewportView(descText);

	descPanel.add(textScrollPane);
	
	mainBox.add(descPanel);
		
	//Create Apply, Cancel and Help Buttons and associated Action Listeners
	JPanel buttonPanel = new JPanel(new FlowLayout());
	//buttonPanel.setBorder(BorderFactory.createTitledBorder(""));
	//buttonPanel.setLayout(new GridLayout(1,3));
	//Set buttons to specified height, but changing width
	//buttonPanel.setMaximumSize(new Dimension((int)buttonPanel.getMaximumSize().getWidth(), 40));
	//buttonPanel.setMinimumSize(new Dimension((int)buttonPanel.getMinimumSize().getWidth(), 40));
	//buttonPanel.setPreferredSize(new Dimension((int)buttonPanel.getPreferredSize().getWidth(), 40));

	/* Add action listener for the Apply Button. This adds a Primary Event into the database, with its
	 * associated probability, type and description if a New ID is given, or amends the probability, type
	 * and/or description if an existing ID is given. If no ID, or an invalid probability is provided then
	 * an error message is displayed.
	 */
	applyButton = new JButton("Apply");
	applyButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if (idText.getText().length() > 0) {
			try {
			    Float probability = (new Float(probText.getText()));

			    if (externalButton.isSelected() && probability.floatValue() > 0.0 && probability.floatValue() < 1.0) {
				postWarning("External events must have a probability of 0.0 or 1.0", "OpenPED: Export Image");	
			    } else if ((probability.floatValue() >= 0) && (probability.floatValue() <= 1)) {
				nativeApplyEvent(idText.getText(), (new Float(probText.getText())).floatValue(),
						 descText.getText(),
						 constantFailureButton.isSelected(), dormantButton.isSelected(),
						 currentType, originalType);
				addListElement(new String(idText.getText()));
				fta.updateFrame();
			    } else {
				JOptionPane.showMessageDialog(thisFrame, "Please enter a probability between 0 and 1 inclusive.",
							      "Invalid Probability", JOptionPane.ERROR_MESSAGE, warningIcon); 
			    }
			} catch (NumberFormatException nfe){
			    JOptionPane.showMessageDialog(thisFrame, "Please enter a valid probability.", 
							  "InvalidProbability", JOptionPane.ERROR_MESSAGE, warningIcon);
			}
		    } else {
			JOptionPane.showMessageDialog(thisFrame, "Please enter an id.",
						      "Invalid ID", JOptionPane.ERROR_MESSAGE, warningIcon);
		    }
		    originalType = currentType;
		    //		    listPanel.setSelectedValue(idText.getText(), true);
		    listPanel.setSelectedValue(idText.getTextString(), true);
		    showSelection();
		}
	    });

	/* Add an action listener for the Cancel button. This clears the text in each of the text
	 * components and removes any highlights from the Primary Events list.
	 */
	cancelButton = new JButton("Clear");
	cancelButton.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e) {
		    clearSelection();
		}
	    });

	/* Add an action listener for the Help button. This loads the default internet browser and
	 * and displays the relevant help page for the PED Frame dialog.
	 */
	helpButton = new JButton("Help");
	helpButton.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e) {
		    BrowserLauncher.openHelpFile("ped_dialog_ped.html");
		}
	    });
	buttonPanel.add(applyButton);
	buttonPanel.add(cancelButton);
	buttonPanel.add(helpButton);

        JPanel mainPanel = new JPanel(new BorderLayout());
        mainPanel.add(mainBox, BorderLayout.CENTER);
        mainPanel.add(buttonPanel, BorderLayout.SOUTH);

	JScrollPane mainScrollPane = new JScrollPane();
       	mainScrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
       	mainScrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);

	mainScrollPane.setViewportView(mainPanel);

	mainScrollPane.setBorder(BorderFactory.createEmptyBorder());

	getContentPane().add(mainScrollPane, BorderLayout.CENTER);

    }

    /* This private method selects the correct Type radio button for the
 * given type.
 */
    private void setCurrentType(int type){
		
	// Turn off old button
	switch(currentType) {
			
	case PEDConstants.BASIC :
	    basicButton.setSelected(false);
	    break;

	case PEDConstants.EXTERNAL :
	    externalButton.setSelected(false);
	    break;

	case PEDConstants.UNDEVELOPED :
	    undevelopedButton.setSelected(false);
	    break;

	case PEDConstants.COND_NOT_ANAL :
	    condNotAnalButton.setSelected(false);
	    break;

	case PEDConstants.COND_ANAL :
	    condAnalButton.setSelected(false);
	    break;

	default :
			
	    break;
	}
	// Turn on new button
	switch(type) {
			
	case PEDConstants.BASIC :
	    basicButton.setSelected(true);
	    break;

	case PEDConstants.EXTERNAL :
	    externalButton.setSelected(true);
	    break;

	case PEDConstants.UNDEVELOPED :
	    undevelopedButton.setSelected(true);
	    break;

	case PEDConstants.COND_NOT_ANAL :
	    condNotAnalButton.setSelected(true);
	    break;

	case PEDConstants.COND_ANAL :
	    condAnalButton.setSelected(true);
	    break;

	default :
			
	    break;
	}

	currentType = type;
    }	

    /* This method show a specified question to the user and returns a Yes
 * or No answer.
 */
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

    private void addListElement(String event) {
	if(!listPanel.contains(event)) {
	    listPanel.addElement(event);
	}
    }

    private void clearList() {
	listPanel.clear();
    }

    /* This method loads the id, probability, type and description
 * for a selected primary event.
 */
    public void showSelection() {
	if(listPanel.getSelectedValue() != null){
	    nativeSetSelectedEvent((String)listPanel.getSelectedValue());
	    if(nativeEventSelected()) {
		idText.setText((String)listPanel.getSelectedValue());
		probText.setText(Float.toString(nativeGetProb()));
		constantFailureButton.setSelected(nativeGetConstFailure());
		setCurrentType(nativeGetType());
		originalType = currentType;
		dormantButton.setSelected(nativeGetDormant());
		descText.setText(nativeGetDesc());

		// disable the conditioning events if required
		switch(currentType) {
		    
		case PEDConstants.BASIC :
		case PEDConstants.EXTERNAL :
		case PEDConstants.UNDEVELOPED :
		    basicButton.setEnabled(true);
		    externalButton.setEnabled(true);
		    undevelopedButton.setEnabled(true);
		    condNotAnalButton.setEnabled(false);
		    condAnalButton.setEnabled(false);
		    break;
		    
		case PEDConstants.COND_NOT_ANAL :
		case PEDConstants.COND_ANAL :
		    basicButton.setEnabled(false);
		    externalButton.setEnabled(false);
		    undevelopedButton.setEnabled(false);
		    condNotAnalButton.setEnabled(true);
		    condAnalButton.setEnabled(true);
		    break;
		    
		default :
		    
		    break;
		}
	    }
	}
    }

    /* This method clears the id, probability, type and description fields
 * and removes any highlighting from the Primary Events list.
 */
    private void clearSelection() {
	listPanel.clearSelection();
	idText.setText("");
	probText.setText("");
	constantFailureButton.setSelected(false);
	setCurrentType(PEDConstants.BASIC);

	// enable the event buttons
	basicButton.setEnabled(true);
	externalButton.setEnabled(true);
	undevelopedButton.setEnabled(true);
	condAnalButton.setEnabled(true);
	condNotAnalButton.setEnabled(true);

	dormantButton.setSelected(false);
	descText.setText("");
    }



    private void saveFileAction() {
	if(nativeFileExists()) {
	    if(!nativeSavePEDFile()) {

	    }
	} else {
	    saveAsFileAction();
	}
    }


    private void saveAsFileAction() {

	JFileChooser chooser = new JFileChooser(currentPath);

	FTAFileFilter filter = new FTAFileFilter("ped", "PED Files");
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
	    if(!filename.endsWith(".ped")){
		filename = filename + ".ped";
	    }
	    if(nativeSaveAsPEDFile(filename)) {
		int pathLength;
		pathLength =  chooser.getSelectedFile().getPath().length() - 
		    chooser.getSelectedFile().getName().length();
		currentPath = chooser.getSelectedFile().getPath().substring(0, pathLength);
	    }
	}
    }


    public void loadPEDFile(String file) {
	if(nativeLoadPEDFile(file)) {
	    populateList();	    
	    idText.setText("");
	    probText.setText("");
	    descText.setText("");
	    
	    constantFailureButton.setSelected(false);
	    dormantButton.setSelected(false);
	    setCurrentType(PEDConstants.BASIC);
	} 
    }
    

    private void openFileAction() {
	
	JFileChooser chooser = new JFileChooser(currentPath);
	FTAFileFilter filter = new FTAFileFilter("ped", "PED Files");
	chooser.setFileFilter(filter);
	int returnVal = chooser.showOpenDialog((Component)this.getContentPane());
	if(returnVal == JFileChooser.APPROVE_OPTION) {
	    //try {
		//System.out.println("You chose to save this file: " +
		//		   chooser.getSelectedFile().toURL());
	    //} catch(MalformedURLException e) {
		//System.out.println("You chose to save this file:Cannot convert file to URL");
        //}
	    
	    nativeNewPEDFile(false);
	    loadPEDFile(chooser.getSelectedFile().getPath());
	    int pathLength;
	    pathLength =  chooser.getSelectedFile().getPath().length() - 
		chooser.getSelectedFile().getName().length();
	    currentPath = chooser.getSelectedFile().getPath().substring(0, pathLength);
	}
    }


    public void newPEDFile() {
	if(nativeNewPEDFile(true)) {
	    listPanel.clear();
	    
	    idText.setText("");
	    probText.setText("");
	    descText.setText("");
	    
	    constantFailureButton.setSelected(false);
	    dormantButton.setSelected(false);
	    setCurrentType(PEDConstants.BASIC);
	}
    }

    public void setWindowTitle(String databaseName) {
	dbName = databaseName;
	setTitle("OpenPED (Database: " + databaseName + ")");
    }

    /* This method acquires the data from the database for each of the primary events.
       It formats it into a tabular format and then sends it to the Printer Utilities. */
    private void printAction(){
	TextPrinter tp = new TextPrinter(createReport(), PrintPreferences.printerJob, PrintPreferences.pageFormat);
	tp.print();
    }


    private String createReport(){
	String report = new String();
	int i;

	report = report + "Primary Events Database\n";
	report = report + "=======================\n\n";
	    
	report = report + "Database Filename: ";

	if(dbName.length() == 0) {
	    report = report + "Unnamed";
	} else {
	    report = report + dbName;
	}
	    
	report = report + "\n\n";

	//Create string containing the information for each of the elements in the database.
	for (i=0; i<listPanel.listLength(); i++) {
	    listPanel.setSelectedIndex(i);			
	    nativeSetSelectedEvent((String)listPanel.getSelectedValue());
	    report = report + "ID: " + (String)listPanel.getSelectedValue() + "\n";
	    report = report + "Probability: " + Float.toString(nativeGetProb());
	    if(constantFailureButton.isSelected()) {
		report = report + " (Constant failure rate/Unit time)";
	    }
		
	    report = report + "\n";
	    report = report + "Type: " + getCurrentType(nativeGetType()) + "\n";
	    report = report + "Dormant: ";
	    if (nativeGetDormant() == true) {
		report = report + "Yes\n";
	    } else {
		report = report + "No\n";
	    }
	    report = report + "Description: " + nativeGetDesc();
		
	    // do not add returns on last line
	    if(i < listPanel.listLength() - 1) {
		report = report + "\n\n";
	    }
	}

	return report;
    }




    private String getCurrentType(int Type){
	String typeName;
		
	// Turn off old button
	switch(Type) {
			
	case PEDConstants.BASIC :
	    typeName = new String("Basic");
	    break;

	case PEDConstants.EXTERNAL :
	    typeName = new String("External");
	    break;

	case PEDConstants.UNDEVELOPED :
	    typeName = new String("Undeveloped");
	    break;

	case PEDConstants.COND_NOT_ANAL :
	    typeName = new String("ConditionNotAnalysed");
	    break;

	case PEDConstants.COND_ANAL :
	    typeName = new String("ConditionAnalysed");
	    break;

	default :
	    typeName = new String("Unknown");
	    break;
	}
	return typeName;
    }

    public void selectPrimaryEvent(String id){
	listPanel.selectPrimaryEvent(id);
	showSelection();
	setVisible(true);
    }

    public void setFTAFrame(FTAFrame frame) {
	fta = frame;
    }

    private void deleteAction() {
	String s = (String)listPanel.getSelectedValue();
	if(s != null) {

	    if(askQuestion("Delete event " + s + "? This cannot be undone.", "OpenPED: Delete?")) {
		nativeDeleteEvent(s);
		clearSelection();
		populateList();
	    }
	}
    }
    
}



    

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

public class GateDialog extends JDialog {

    private native int nativeGetType();
    private native String nativeGetID();
    private native void nativeOKButtonAction(String id, int type);
    private native void nativeInitialiseGateDialog();


//    JTextField idText;
    RestrictLengthField idText;
    int currentType;

    ImageIcon warningIcon;

    JRadioButton andButton;
    JRadioButton orButton;
    JRadioButton xorButton;
    JRadioButton priorityAndButton;
    JRadioButton inhibitButton;

    JButton okButton;
    JButton cancelButton;
    JButton helpButton;
    JMenuBar menuBar;

    public GateDialog(Frame parent) {

	super(parent);

	setSize(300, 230);	
	setLocation(300, 200);
	setTitle("Formal-FTA: Gate");
	setVisible(false);
	setModal(true);

	nativeInitialiseGateDialog();

	warningIcon = FTAUtilities.loadImageIcon("warning.gif");

	getContentPane().setLayout(new BorderLayout());
	
	buildToolBar();

	JPanel centerPanel = new JPanel(new BorderLayout());

	JPanel idPanel = new JPanel();
	idPanel.setLayout(new GridLayout(1,1));
	idPanel.setBorder(BorderFactory.createTitledBorder("ID"));

//	idText = new JTextField();
	idText = new RestrictLengthField(13);

	idPanel.add(idText);

	centerPanel.add(idPanel, BorderLayout.NORTH);

	JPanel typePanel = new JPanel(new FlowLayout());
	typePanel.setBorder(BorderFactory.createTitledBorder("Type"));
	//typePanel.setMaximumSize(new Dimension((int)typePanel.getMaximumSize().getWidth(),100));
	//typePanel.setMinimumSize(new Dimension((int)typePanel.getMinimumSize().getWidth(), 100));
	//typePanel.setPreferredSize(new Dimension((int)typePanel.getPreferredSize().getWidth(),100));
	
	andButton = new JRadioButton("And");
	orButton = new JRadioButton("Or");
	xorButton = new JRadioButton("Xor");
	priorityAndButton = new JRadioButton("Priority And");
	inhibitButton = new JRadioButton("Inhibit");

	andButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    setCurrentType(FTAConstants.AND);
		}
	    });
	orButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    setCurrentType(FTAConstants.OR);
		}
	    });
	xorButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    setCurrentType(FTAConstants.XOR);
		}
	    });
	priorityAndButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    setCurrentType(FTAConstants.PRIORITY_AND);
		}
	    });
	inhibitButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    setCurrentType(FTAConstants.INHIBIT);
		}
	    });

	typePanel.add(andButton);
	typePanel.add(orButton);
	typePanel.add(xorButton);
	typePanel.add(priorityAndButton);
	typePanel.add(inhibitButton);

	centerPanel.add(typePanel, BorderLayout.CENTER);

        getContentPane().add(centerPanel, BorderLayout.CENTER);

	JPanel buttonPanel = new JPanel(new FlowLayout());
	//buttonPanel.setBorder(BorderFactory.createTitledBorder(""));
	//buttonPanel.setLayout(new GridLa	Box mainBox = new Box(BoxLayout.Y_AXIS);yout(1,3));
	//buttonPanel.setMaximumSize(new Dimension((int)buttonPanel.getMaximumSize().getWidth(),40));
	//buttonPanel.setMinimumSize(new Dimension((int)buttonPanel.getMinimumSize().getWidth(), 40));
	//buttonPanel.setPreferredSize(new Dimension((int)buttonPanel.getPreferredSize().getWidth(),40));

	okButton = new JButton("OK");
	okButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    //			if (idText.getTextString().length() > 0) {
//			    nativeOKButtonAction(idText.getText(), currentType);
			    nativeOKButtonAction(idText.getTextString(), currentType);
			    setVisible(false);
			    //} else {
			    //	JOptionPane.showMessageDialog(null, "Please enter an ID.",
			    //	"Invalid ID", JOptionPane.ERROR_MESSAGE, warningIcon);
			    //	}
		}
	    });

	cancelButton = new JButton("Cancel");
	cancelButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    setVisible(false);
		}
	    });

	helpButton = new JButton("Help");
	helpButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e){
			BrowserLauncher.openHelpFile("fta_dialog_gate.html");
		}
	    });


	buttonPanel.add(okButton);
	buttonPanel.add(cancelButton);
	buttonPanel.add(helpButton);

	getContentPane().add(buttonPanel, BorderLayout.SOUTH);
    }

    public void initialise(){
	setCurrentType(nativeGetType());
	idText.setText(nativeGetID());
    }
	
    private void setCurrentType(int type){
		
	// Turn off old button
	andButton.setSelected(false);
	orButton.setSelected(false);
	xorButton.setSelected(false);
	priorityAndButton.setSelected(false);
	inhibitButton.setSelected(false);

	// Turn on new button
	switch(type) {
			
	case FTAConstants.AND :
	    andButton.setSelected(true);
	    break;

	case FTAConstants.OR :
	    orButton.setSelected(true);
	    break;

	case FTAConstants.XOR :
	    xorButton.setSelected(true);
	    break;

	case FTAConstants.PRIORITY_AND :
	    priorityAndButton.setSelected(true);
	    break;

	case FTAConstants.INHIBIT :
	    inhibitButton.setSelected(true);
	    break;

	default :
			
	    break;
	}

	currentType = type;
    }	


    public void postWarning(String warning, String title) {
	JOptionPane.showMessageDialog(null, warning, title,
				      JOptionPane.WARNING_MESSAGE, warningIcon);
    }
    protected void buildToolBar(){
	JToolBar toolBar = new JToolBar();
	toolBar.setBorder(null);
	toolBar.setFloatable(false);


	JButton cutButton = FTAUtilities.createButton("cut");
	cutButton.setBorder(null);
	toolBar.add(cutButton);
	JButton copyButton = FTAUtilities.createButton("copy");
	copyButton.setBorder(null);
	toolBar.add(copyButton);
	JButton pasteButton = FTAUtilities.createButton("paste");
	pasteButton.setBorder(null);
	toolBar.add(pasteButton);

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

	JPanel toolBarPanel = new JPanel();
	toolBarPanel.setLayout(new BorderLayout());

	toolBarPanel.add(toolBar, BorderLayout.WEST);

	getContentPane().add(toolBarPanel, BorderLayout.NORTH);

    }

    protected JMenu buildEditMenu() {
	JMenu edit = new JMenu("Edit");
	edit.setMnemonic(KeyEvent.VK_E);
	JMenuItem copy = new JMenuItem("Copy", KeyEvent.VK_C);
	copy.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_C, ActionEvent.CTRL_MASK));
	JMenuItem cut = new JMenuItem("Cut", KeyEvent.VK_X);
	cut.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_X, ActionEvent.CTRL_MASK));
	JMenuItem paste = new JMenuItem("Paste", KeyEvent.VK_V);
	paste.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_V, ActionEvent.CTRL_MASK));

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
	edit.add(cut);
	edit.add(copy);
	edit.add(paste);

	return edit;
    }
	
}


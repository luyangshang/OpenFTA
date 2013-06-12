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

public class IntermediateEventDialog extends JDialog {

    private native String nativeGetDesc();
    private native String nativeGetID();
    private native void nativeOKButtonAction(String id, String desc);

    RestrictedLengthAlphaNumericField idText;
    JTextArea descText;

    JButton okButton;
    JButton cancelButton;
    JButton helpButton;
    JMenuBar menuBar;

    private ImageIcon warningIcon;

    public IntermediateEventDialog(Frame parent) {

	super(parent);

	setSize(300, 300);	
	setLocation(300, 200);
	setTitle("Formal-FTA: Intermediate Event");
	setVisible(false);
	setModal(true);

	warningIcon = FTAUtilities.loadImageIcon("warning.gif");

	JPanel textPanel = new JPanel(new BorderLayout());
	getContentPane().setLayout(new BorderLayout());
	//	buildMenu();
	buildToolBar();

      	JPanel idPanel = new JPanel();
	idPanel.setLayout(new GridLayout(1,1));
	idPanel.setBorder(BorderFactory.createTitledBorder("ID"));

	idText = new RestrictedLengthAlphaNumericField(13);

	idPanel.add(idText);

	textPanel.add(idPanel, BorderLayout.NORTH);

	JPanel descPanel = new JPanel();
	descPanel.setLayout(new GridLayout(1,1));
	descPanel.setBorder(BorderFactory.createTitledBorder("Description"));

	descText = new JTextArea();

	JScrollPane textScrollPane = new JScrollPane();
	textScrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
	textScrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
	textScrollPane.setViewportView(descText);

	descPanel.add(textScrollPane);

	textPanel.add(descPanel, BorderLayout.CENTER);

	getContentPane().add(textPanel, BorderLayout.CENTER);

	JPanel buttonPanel = new JPanel(new FlowLayout());
	//buttonPanel.setBorder(BorderFactory.createTitledBorder(""));
	//buttonPanel.setLayout(new GridLayout(1,3));
	//buttonPanel.setMaximumSize(new Dimension((int)buttonPanel.getMaximumSize().getWidth(),40));
	//buttonPanel.setMinimumSize(new Dimension((int)buttonPanel.getMinimumSize().getWidth(), 40));
	//buttonPanel.setPreferredSize(new Dimension((int)buttonPanel.getPreferredSize().getWidth(),40));

	okButton = new JButton("OK");
	okButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    //			if (idText.getTextString().length() > 0) {
//		    		nativeOKButtonAction(idText.getText(), descText.getText());
				nativeOKButtonAction(idText.getTextString(), descText.getText());
		    		setVisible(false);
				//	} else {
				//	JOptionPane.showMessageDialog(null, "Please enter an id.",
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
			BrowserLauncher.openHelpFile("fta_dialog_intermediate.html");
		}
	    });
		    

	buttonPanel.add(okButton);
	buttonPanel.add(cancelButton);
	buttonPanel.add(helpButton);

	getContentPane().add(buttonPanel, BorderLayout.SOUTH);
    }

    public void initialise(){
	descText.setText(nativeGetDesc());
	idText.setText(nativeGetID());
    }

    protected void buildMenu(){
	menuBar = new JMenuBar();
	menuBar.setOpaque(true);
	JMenu edit = buildEditMenu();
	menuBar.add(edit);
	setJMenuBar(menuBar);	
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

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

public class TransferTreeDialog extends JDialog {

    private native String nativeGetID();
    private native void nativeOKButtonAction(String id);

    RestrictedLengthExclusiveField idText;

    JButton okButton;
    JButton cancelButton;
    JButton helpButton;

    public TransferTreeDialog(Frame parent) {

	super(parent);

	setSize(300, 150);	
	setLocation(300, 200);
	setTitle("Formal-FTA: Transfer Tree");
	setVisible(false);
	setModal(true);

	//Box mainBox = new Box(BoxLayout.Y_AXIS);
	getContentPane().setLayout(new BorderLayout());
	buildToolBar();

	JPanel idPanel = new JPanel();
	idPanel.setLayout(new GridLayout(1,1));
	idPanel.setBorder(BorderFactory.createTitledBorder("ID"));

	//	idText = new JTextField();
	idText = new RestrictedLengthExclusiveField(9, " ");

	idPanel.add(idText);

	getContentPane().add(idPanel, BorderLayout.CENTER);

	JPanel buttonPanel = new JPanel(new FlowLayout());
	//buttonPanel.setBorder(BorderFactory.createTitledBorder(""));
	//buttonPanel.setLayout(new GridLayout(1,3));

	okButton = new JButton("OK");
	okButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeOKButtonAction(idText.getText());
		    setVisible(false);
		}
	    });

	cancelButton = new JButton("Cancel");
	cancelButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    setVisible(false);
		}
	    });

	helpButton = new JButton("Help");

	buttonPanel.add(okButton);
	buttonPanel.add(cancelButton);
	buttonPanel.add(helpButton);

	getContentPane().add(buttonPanel, BorderLayout.SOUTH);
    }

    public void initialise(){
	idText.setText(nativeGetID());
    }
	
    protected void buildToolBar(){
	JToolBar toolBar = new JToolBar();
	toolBar.setFloatable(false);

	JButton cutButton = FTAUtilities.createButton("cut");
	toolBar.add(cutButton);
	JButton copyButton = FTAUtilities.createButton("copy");
	toolBar.add(copyButton);
	JButton pasteButton = FTAUtilities.createButton("paste");
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

}


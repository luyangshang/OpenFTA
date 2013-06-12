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

public class TreeTextDialog extends JDialog {

    private native String nativeGetDesc();
    private native void nativeOKButtonAction(String desc);

    private JTextArea descText;

    private JButton okButton;
    private JButton cancelButton;
    private JButton helpButton;
    private JMenuBar menuBar;

    public TreeTextDialog(Frame parent) {

	super(parent);

	setSize(300, 200);	
	setLocation(300, 200);
	setTitle("Formal-FTA: Tree Text");
	setVisible(false);
	setModal(true);

	//Box mainBox = new Box(BoxLayout.Y_AXIS);
	getContentPane().setLayout(new BorderLayout());
	buildToolBar();

	JPanel descPanel = new JPanel();
	descPanel.setLayout(new GridLayout(1,1));
	descPanel.setBorder(BorderFactory.createTitledBorder("Description"));

	descText = new JTextArea();

	JScrollPane textScrollPane = new JScrollPane();
	textScrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
	textScrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
	textScrollPane.setViewportView(descText);

	descPanel.add(textScrollPane);

	getContentPane().add(descPanel, BorderLayout.CENTER);

	JPanel buttonPanel = new JPanel(new FlowLayout());
       	//buttonPanel.setBorder(BorderFactory.createTitledBorder(""));
	//buttonPanel.setLayout(new GridLayout(1,3));
	//buttonPanel.setMaximumSize(new Dimension((int)buttonPanel.getMaximumSize().getWidth(),40));
	//buttonPanel.setMinimumSize(new Dimension((int)buttonPanel.getMinimumSize().getWidth(), 40));
	//buttonPanel.setPreferredSize(new Dimension((int)buttonPanel.getPreferredSize().getWidth(),40));

	okButton = new JButton("OK");
	okButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    nativeOKButtonAction(descText.getText());
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
	helpButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e){
			BrowserLauncher.openHelpFile("fta_dialog_post_it.html");
		}
	    });



	buttonPanel.add(okButton);
	buttonPanel.add(cancelButton);
	buttonPanel.add(helpButton);

	getContentPane().add(buttonPanel, BorderLayout.SOUTH);
	//	getContentPane().add(mainBox, "Center");
    }

    public void initialise(){
	descText.setText(nativeGetDesc());
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

	
}

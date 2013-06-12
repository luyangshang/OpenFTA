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

public class ScrolledTextFrame extends JFrame {

	private JTextArea text;
	private JButton cancelButton;

	private ImageIcon warningIcon;
	private ImageIcon windowIcon;

	public ScrolledTextFrame() {
		setSize(500, 500);	
		setLocation(100, 100);
		setVisible(false);

		getContentPane().setLayout(new BorderLayout());

		text = new JTextArea();
		text.setFont(new Font("MonoSpaced", Font.PLAIN, 12));
		text.setEditable(false);

		warningIcon = FTAUtilities.loadImageIcon("warning.gif");
          	windowIcon = FTAUtilities.loadImageIcon("reportsmall.gif");
		setIconImage(windowIcon.getImage());

		JScrollPane textScrollPane = new JScrollPane();
		textScrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
		textScrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
		textScrollPane.setViewportView(text);

		getContentPane().add(textScrollPane, BorderLayout.CENTER);

		cancelButton = new JButton("Close");
		cancelButton.setDefaultCapable(true);
		getRootPane().setDefaultButton(cancelButton);
		cancelButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				setVisible(false);
			}
		});

                JPanel buttonPanel = new JPanel(new FlowLayout());
                buttonPanel.add(cancelButton);
		getContentPane().add(buttonPanel, BorderLayout.SOUTH);
	}

	public void initialise(){
	}
	
	public void setWindowTitle(String windowTitle) {
		setTitle(windowTitle);
	}

	public void setText(String s) {
	    text.setText(s);
	}

	public void readTextFile(String fileName) {
		boolean ok;
		InputStreamReader sr;
		FileInputStream fs;

		//System.out.println("Filename = " + fileName);

		ok = true;

		try {
			fs = new FileInputStream(fileName);
			sr = new InputStreamReader(fs);
			text.read(sr, null);

		} catch (FileNotFoundException e) {
			postWarning("Text file not found", "Formal-FTA: Error");
			setVisible(false);
			ok = false;
		} catch (IOException e) {
			postWarning("Error opening file", "Formal-FTA: Error");
			setVisible(false);
			ok = false;
		}

	}

	public void postWarning(String warning, String title) {
		JOptionPane.showMessageDialog(null, warning, title,
					JOptionPane.WARNING_MESSAGE, warningIcon);
	}


}

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

public class IntegerField extends JTextField {

	public IntegerField(int cols) {
		super(cols);
	}

	public IntegerField() {
		super();
	}

	public int getIntegerValue() {
		return(new Integer(super.getText()).intValue());
	}

	protected Document createDefaultModel() {
		return new IntegerDocument();
	}

	static class IntegerDocument extends PlainDocument { 

		public void insertString(int offs, String str, AttributeSet a)
		throws BadLocationException {
			if (str == null) {
				return;
			}
			String currentText = super.getText(0, super.getLength());
			char[] numberIn = str.toCharArray();
			for (int i = 0; i < numberIn.length; i++) {
				if(numberIn[i] >= '0' && numberIn[i] <= '9') {
					super.insertString(offs+i, new String(numberIn, i, 1), a);
				}
			}
		}
	}
}

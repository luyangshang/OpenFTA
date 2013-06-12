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

/** This class creates a Text Field which only accepts a specified number of characters.
  *
  * @author Rebecca Sparey-Taylor
  */

public class RestrictLengthField extends JTextField {
	private static int _length; //holds number of characters allowed in text field

/** Creates a Text Field with a specified number of columns, 
  * which only accepts a specified number of characters   
  * @param cols The number of columns in the Text Field
  * @param length The number of characters allowed in the Text Field
  */
	
	public RestrictLengthField(int cols, int length) {
		super(cols);
		_length = length;
    	}

/** Creates a Text Field which only accepts a specified number of characters   
  * @param length The number of characters allowed in the Text Field
  */

    	public RestrictLengthField(int length) {
		super();
		_length = length;
    	}

/** This method returns the Text from the Text Field.
  * @returns String The text from the Text Field.
  */
    	public String getTextString() {
		return(new String(super.getText()));
    	}	

    	protected Document createDefaultModel() {
    		return new RestrictedLengthDocument();
    	}

    	static class RestrictedLengthDocument extends PlainDocument { 
/** This method overrides the insertString method to ensure that only the 
  * correct number of characters can be entered into the Text Field.
  * @param offs The offset into the text field to insert the string
  * @param str The string to insert into the Text Field
  * @param a The attributes to associate with the inserted content. This may
  * be null if there are no attributes.
  * @throws BadLocationException If the string to insert is null.
  */
		public void insertString(int offs, String str, AttributeSet a)

	    	throws BadLocationException {
	    		if (str == null) {
				return;
	    		}
			//Get the number of characters in the string to insert
	    		char[] numberIn = str.toCharArray();
			//Scroll through each of the characters
	    		for (int i = 0; i < numberIn.length; i++) {
				//Get the current length of the string in the TextField
	    			String currentText = super.getText(0, super.getLength());
				/*If string is currently under the specified max length insert
				the character into the Text Field, in the correct offset position*/
				if(currentText.length() < _length) {
		    			super.insertString(offs+i, new String(numberIn, i, 1), a);

				}

			}
		}
	}
}

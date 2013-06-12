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

/** This class creates a Text Field which only accepts which only accepts 
  * alpha numeric strings (first character must be a letter).
  * Length restricted to specified value.
  *
  * @author Ben Duncan
  */
/* Modified by R. Sparey-Taylor */


public class RestrictedLengthAlphaNumericField extends JTextField {

    private int _length; //holds number of characters allowed in text field

/** Creates a Text Field with a specified number of columns, 
  * which only accepts alpha numeric strings (first character must be a letter).
  * @param cols The number of columns in the Text Field
  */
    public RestrictedLengthAlphaNumericField(int cols, int length) {
	super(new RestrictedLengthAlphaNumericDocument(length), null, cols);
	_length = length;
    }

/** Creates a Text Field with a specified number of columns, 
  * which only accepts alpha numeric strings (first character must be a letter).
  */
    public RestrictedLengthAlphaNumericField(int length) {
	super(new RestrictedLengthAlphaNumericDocument(length), null, 0);
	_length =length;
    }

/** This method returns the Text from the Text Field.
  * @returns String The text from the Text Field.
  */
    	public String getTextString() {
		return(new String(super.getText()));
    	}	

    protected Document createDefaultModel() {
    	return new RestrictedLengthAlphaNumericDocument(_length);
    }

}


class RestrictedLengthAlphaNumericDocument extends PlainDocument {

    /** This method overrides the insertString method to ensure that only 
     * alpha numeric strings (first character must be a letter) can be entered into the Text Field.
     * @param offs The offset into the text field to insert the string
     * @param str The string to insert into the Text Field
     * @param a The attributes to associate with the inserted content. This may
     * be null if there are no attributes.
     * @throws BadLocationException If the string to insert is null.
     */

    private int _length;

    public RestrictedLengthAlphaNumericDocument(int length) {
	super();
	_length = length;
    }

    public void insertString(int offs, String str, AttributeSet a)

	throws BadLocationException {
	if (str == null) {
	    return;
	}
	// Get the current text in the Text Field
	String currentText = super.getText(0, super.getLength());

	// Find number of characters in the string to insert
	char[] charIn = str.toCharArray();

	/* Loop through each of the characters in the string to insert
	   and ensure that they are either digits or characters used to
	   create floats in an exponential format e.g. 5.0E-6 */
	for (int i = 0; i < charIn.length; i++) {
		
	    if(currentText.length() < _length) {

		//If the character is a letter insert into the text field
		if((charIn[i] >= 'A' && charIn[i] <= 'Z') ||
		   (charIn[i] >= 'a' && charIn[i] <= 'z')) {
		    super.insertString(offs+i, new String(charIn, i, 1), a);
			
		    /*If the character is a number and offset > 0 then insert it */
		} else if (charIn[i] >= '0' && charIn[i] <= '9') {
		    if(offs > 0) {
			super.insertString(offs+i, new String(charIn, i, 1), a);
		    }
		}
	    }
	}
    }
}

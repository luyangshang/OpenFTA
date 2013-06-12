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

/** This class creates a Text Field which only accepts positive Floats, or positive 
  * Floats in an exponential format.
  *
  * @author Ben Duncan
  */
/* Modified by R. Sparey-Taylor */


public class FloatField extends JTextField {

/** Creates a Text Field with a specified number of columns, 
  * which only accepts Floats, or Floats in an exponential format.
  * @param cols The number of columns in the Text Field
  */
    public FloatField(int cols) {
	super(cols);
    }

/** Creates a Text Field which only accepts positive Floats, 
  * or positive Floats in an exponential format.
  */
    public FloatField() {
	super();
    }

/** This method returns the value of the float from the Text Field.
  * @returns float The value of the float in the Text Field.
  */
    public float getFloatValue() {
	return(new Float(super.getText()).floatValue());
    }

    protected Document createDefaultModel() {
    	return new FloatDocument();
    }

    static class FloatDocument extends PlainDocument { 

/** This method overrides the insertString method to ensure that only 
  * Floats, or Floats in an exponential format can be entered into the Text Field.
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
	    // Get the current text in the Text Field
	       String currentText = super.getText(0, super.getLength());

	    // Find number of characters in the string to insert
	       char[] numberIn = str.toCharArray();

 	    /* Loop through each of the characters in the string to insert
	       and ensure that they are either digits or characters used to
	       create floats in an exponential format e.g. 5.0E-6 */
	    for (int i = 0; i < numberIn.length; i++) {
		//If the character is a digit insert into the text field
		if(numberIn[i] >= '0' && numberIn[i] <= '9') {
		    super.insertString(offs+i, new String(numberIn, i, 1), a);

		/*If the character is a decimal point and a decimal point has
		  not previously been inserted into the Text Field insert it */
		} else if (numberIn[i] == '.') {
		    if(currentText.indexOf('.') < 0) {
			super.insertString(offs+i, new String(numberIn, i, 1), a);
		    }

		/* If the character is an e or E, and neither an e nor an E has
		   previously been inserted into the Text Field, and the character
		   is not the first character to be inserted into the Text Field,
		   insert the character into the Text Field. */
		} else if ((numberIn[i] == 'e') || (numberIn[i] == 'E')){
		    //letter e can only appear once (exponential) and can't be at
		    //the beginning of the number
		    if(i+offs > 0 &&
		       (currentText.indexOf('e') < 0) && (currentText.indexOf('E') < 0)) {
			super.insertString(offs+i, new String(numberIn, i, 1), a);
			//System.out.println("e offset+1 = " + offs+i);
		    }

		/*If the character is a minus sign, and the previous character in
		  the string is an e or E, insert it. */
		} else if (numberIn[i] == '-') {
		    /* tempCurrentText deals with string values entered 
		       in one go, rather than by individual letter. */
		    String tempCurrentText = new String(super.getText(0, super.getLength()));
		    if(i+offs > 0 && 
		       ((currentText.indexOf('e')==i+offs-1 || currentText.indexOf('E')==i+offs-1)
		       	||(tempCurrentText.indexOf('E')==i+offs-1)))
			{
			    super.insertString(offs+i, new String(numberIn, i, 1), a);

		    }

		}
	    }
	}
    }
}

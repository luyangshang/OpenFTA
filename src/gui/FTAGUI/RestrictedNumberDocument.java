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

import javax.swing.text.*;

public class RestrictedNumberDocument extends PlainDocument {

private int _total;

/** Creates a Document which only accepts characters representing numbers
  * less than a given total
  */

	public RestrictedNumberDocument(){
		super();
	}
	
	/**
	 * Sets the maximum number allowed to be entered
	 * @param int the maximum number
	 */
	public void setTotal (int total) {
	    _total = total;
	}

/** This method is overridden in order to restrict the range of
  * characters which can be inserted into the Document.
  *
  */

	public void insertString(int offs, String str, AttributeSet a)
		throws BadLocationException
	{
	    String numbers = "0123456789";
		if (str == null) return;
			
		StringBuffer num = new StringBuffer();

		for (int i=0; i<str.length(); i++)
		{
			if (numbers.indexOf(str.charAt(i)) != -1)
				num.append(str.charAt(i));
		}
		String text = getText(0, getLength()) + num.toString();
		if(text.length() != 0) {
		    if (Integer.parseInt(getText(0, getLength()) + num.toString()) > _total) {
		        num = new StringBuffer("");
		    }
		}
		super.insertString(offs, num.toString(), a);
	}
}

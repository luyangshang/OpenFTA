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

/** This class creates a Document which only accepts characters from
  * the supplied string.<p>
  * For example, to create a text field of 4 characters which only
  * accepts positive integers:<p>
  * <pre>
  *   JTextField numberField = new JTextField((Document) new RestrictedDocument("0123456789"),"",4);
  * </pre>
  *
  * @author James Slorach
  */
public class RestrictedDocument extends PlainDocument
{
	private String _restriction;

/** Creates a Document which only accepts characters contained in the
  * supplied string
  *
  * @param restriction A String containing all characters which are to be allowed in the Document
  */

	public RestrictedDocument(String restriction)
	{
		super();
		_restriction = restriction;
	}

/** This method is overridden in order to restrict the range of
  * characters which can be inserted into the Document.
  *
  */

	public void insertString(int offs, String str, AttributeSet a)
		throws BadLocationException
	{
		if (str == null) return;
			
		StringBuffer num = new StringBuffer();

		for (int i=0; i<str.length(); i++)
		{
			if ( _restriction.indexOf(str.charAt(i)) != -1)
				num.append(str.charAt(i));
		}

		super.insertString(offs, num.toString(), a);
	}
}


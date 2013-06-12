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

   
/**
    * Class: Example1 <p>
    *
    * @author Jean-Pierre Dube <jpdube@videotron.ca>
    * @version 1.0
    * @since 1.0
    * @see Printable
    */
/* modified by R. Sparey-Taylor */
  
import java.awt.*;
import java.awt.geom.*;
import java.awt.print.*;
  
  
public class TextPrint implements Printable {
  
    //--- Private instances declarations
    private final double INCH = 72;
    private String _printString;
  
  
  
    /**
      * Constructor: TextPrint <p>
      *
      */
    public TextPrint (String printString) {
        //System.out.println("In constructor");
        //--- Create a printerJob object
        PrinterJob printJob = PrinterJob.getPrinterJob ();
	_printString = new String(printString);
 
        //--- Set the printable class to this one since we
        //--- are implementing the Printable interface
        printJob.setPrintable (this);
  
        //--- Show a print dialog to the user. If the user
        //--- clicks the print button, then print, otherwise
        //--- cancel the print job
        if (printJob.printDialog()) {
	    try {
		printJob.print();
	    } catch (Exception PrintException) {
		PrintException.printStackTrace();
	    }
        }
  
    }
  
  
    /**
      * Method: print <p>
      *
      * This class is responsible for rendering a page using
      * the provided parameters. The result will be a grid
      * where each cell will be half an inch by half an inch.
      *
      * @param g a value of type Graphics
      * @param pageFormat a value of type PageFormat
      * @param page a value of type int
      * @return a value of type int
      */
    public int print (Graphics g, PageFormat pageFormat, int page) {
  
        int i;
        Graphics2D g2d;
        Line2D.Double line = new Line2D.Double ();
  
        //--- Validate the page number, we only print the first page
        if (page == 0) {
  
	    //--- Create a graphic2D object and set the default parameters
	    g2d = (Graphics2D) g;
	    g2d.setColor (Color.black);
  
	    	    //--- Translate the origin to be (0,0)
	    g2d.translate (pageFormat.getImageableX (), pageFormat.getImageableY ());
  
	    //--- Print the vertical lines
	    for (i = 0; i < pageFormat.getWidth (); i += INCH / 2) {
		line.setLine (i, 0, i, pageFormat.getHeight ());
		g2d.draw (line);
	    }
  
	    //--- Print the horizontal lines
	    for (i = 0; i < pageFormat.getHeight (); i += INCH / 2) {
		line.setLine (0, i, pageFormat.getWidth (), i);
		g2d.draw (line);
	    }
	    
	    /*	    Font tnr = new Font("Times New Roman", Font.PLAIN, 12);
	    g2d.setFont(tnr);
	    g2d.drawString(_printString, 50, 50);
	    System.out.println(_printString);*/
  
	    return (PAGE_EXISTS);
        }
        else
	    return (NO_SUCH_PAGE);
    }
  
} //Example1
                 

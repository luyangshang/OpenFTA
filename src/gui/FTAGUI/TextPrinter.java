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
import java.awt.*;
import java.awt.print.*;

class TextPrinterFontMetrics extends FontMetrics {

    public TextPrinterFontMetrics(Font font) {
	super(font);
    }

}

// class representing one page of text.
class TextPage extends JTextArea implements Printable {
    
    // constructor to create empty page of text. Text page is same size
    // as the page format passed in. 
    public TextPage() {
	super();
	setFont(new Font("MonoSpaced", Font.PLAIN, 12));
    }

    // print method sets the font and prints each line of text in turn,
    // adding a page number at the end.
    public int print(Graphics g, PageFormat pageFormat, int pageIndex) {
	int x;
	int y;
	String remainingText;
	String lineText;
	int index;

	//System.out.println("Width of printed paper = " + pageFormat.getImageableWidth());

	g.setFont(getFont());

	// set initial coordinates
	x = (int)Math.ceil(pageFormat.getImageableX());
	y = (int)Math.ceil(pageFormat.getImageableY())  + getFontMetrics(getFont()).getDescent() + getFontMetrics(getFont()).getLeading();

	remainingText = getText();

	while(remainingText.length() > 0){
	    // obtain first line of text (up to and including first '\n')
	    index = remainingText.indexOf("\n");
	    if(index < 0) {
		// last line so copy all remaining text
		lineText = remainingText;
		remainingText = ("");
	    } else if(index == 0) {
		// blank line
		lineText = "";
		remainingText = remainingText.substring(1);
	    } else {
		// copy first line into line text and remove from remaining text 
		lineText = remainingText.substring(0, index);
		remainingText = remainingText.substring(index + 1);
	    } // if

	    y += getFontMetrics(getFont()).getHeight();
	    //System.out.println("Printed Line = " + lineText);
	    //System.out.println("Printed Line Width = " + getFontMetrics(getFont()).stringWidth(lineText));
	    Graphics2D tempG = (Graphics2D)g;
	    //System.out.println("Printed Line Width = " + g.getFontMetrics().getStringBounds(lineText, g).getWidth());
	    //System.out.println("Affine Transform = " + tempG.getFontRenderContext().getTransform());
	    g.drawString(lineText, x, y);

	} // while

	// print page number
	lineText = "Page " + (pageIndex + 1);
	y = (int)Math.floor(pageFormat.getImageableY() + pageFormat.getImageableHeight() - getFontMetrics(getFont()).getDescent());
	x += (int)(pageFormat.getImageableWidth()/2) - getFontMetrics(getFont()).stringWidth(lineText);
	g.drawString(lineText, x, y);

	return Printable.PAGE_EXISTS;
    }
}

public class TextPrinter {

    //private JTextArea textArea;
    private JFrame frame;
    private Book book;
    private PrinterJob pj;

    public TextPrinter(String text, PrinterJob printerJob, PageFormat pageFormat) {
	String remainingText = new String(text);
	String lineText;
	String wrappedText;
	String[] wrappedAndLineText;
	int index;
	TextPrinterFontMetrics fm;
	TextPage textPage;

	// initialise private vars
	pj = printerJob;

	// Create new Book JTextArea and TextPage
	//textArea = new JTextArea();
	//textArea.setFont(new Font("MonoSpaced", Font.PLAIN, 12));
	textPage = new TextPage();
	book = new Book();

	fm = new TextPrinterFontMetrics(textPage.getFont());
	//fm = textPage.getGraphics().getFontMetrics();
	//g = textPage.getGraphics();

	// read text line by line to take care of line wrap
	while(remainingText.length() > 0) {
	    //System.out.println("remaining text length = " + remainingText.length());
	    //System.out.println("remaining text = " + remainingText);
	    
	    // obtain first line of text (up to and including first '\n')
	    index = remainingText.indexOf("\n");
	    if(index < 0) {
		// last line so copy all remaining text
		lineText = remainingText;
		remainingText = ("");
	    } else {
		// copy first line into line text and remove from remaining text 
		lineText = remainingText.substring(0, index + 1);
		remainingText = remainingText.substring(index + 1);
	    } // if

	    while(lineText.length() > 0) {

		// obtain first wrapped line of text
		wrappedAndLineText = getNextWrappedLine(lineText, textPage.getFontMetrics(textPage.getFont()), 
						 (int)pageFormat.getImageableWidth(), textPage.getGraphics());

		wrappedText = wrappedAndLineText[0];
		lineText = wrappedAndLineText[1];

		// determine whether extra page needs to be added allowing for page number line
		if((textPage.getLineCount() + 3) * textPage.getFontMetrics(textPage.getFont()).getHeight() >
		   pageFormat.getImageableHeight()) {

		    //System.out.println("Adding new page");

		    // Add full text page to book
		    book.append(textPage, pageFormat);

		    // create new empty page of text
		    textPage = new TextPage();

		    //textArea.append("\n --- New Page ---\n\n");

		} // if

		// add wrapped text to textArea
		//textArea.append(wrappedText);

		// add wrapped text to textPage
		textPage.append(wrappedText);

	    } // while
		
	} // while
	
	// Add last text page to book
	book.append(textPage, pageFormat);

	/*
	frame = new JFrame();
	JScrollPane textScrollPane = new JScrollPane();
	textScrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
	textScrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
	textScrollPane.setViewportView(textArea);
	
	frame.getContentPane().add(textScrollPane, BorderLayout.CENTER);
	frame.setVisible(true);
	*/
   }

    // moves next wrapped line from line to wrapped.
    // The wrapped line contains whole words (delimited by space).
    // If a word is too large to fit on a single wrapped line, it will
    // be split and hyphenated.
    private String[] getNextWrappedLine(String line, FontMetrics fm, int pageWidth, Graphics g) {
	String[] wrappedAndLine = new String[2];
	String wrapped;

	// fix to get around wrapping bug due to Java seemingly not calculating the width of
        // a string correctly
	pageWidth -= 7;

	//System.out.println("Line = " + line);
    //System.out.println("Line Width = " + fm.getStringBounds(line,g).getWidth());
        //System.out.println("Line Width = " + fm.stringWidth(line));
	//System.out.println("Page Width = " + pageWidth);
    //System.out.println("Max Advance = " + fm.getMaxAdvance());
	//System.out.println("Char Width = " + fm.charWidth('a'));

	// see if whole line will fit
	if(fm.getStringBounds(line, g).getWidth() <= pageWidth) {
	    //if(fm.stringWidth(line) <= pageWidth) {

	    // whole line fits so copy entire line into wrapped and empty line
	    wrapped = line;
            line = "";
	}
	// see if first word is too large to fit on the page
	else if(line.indexOf(" ") < 0 || fm.getStringBounds(line.substring(0, line.indexOf(" ")), g).getWidth() > pageWidth) {
	    //if(line.indexOf(" ") < 0 || fm.stringWidth(line.substring(0, line.indexOf(" "))) > pageWidth) {

	    // find maximum number of characters that fit on the page, appended with a hyphen
	    wrapped = new String("");
	    do {
		// move first char of line into wrapped
		wrapped = wrapped + line.substring(0,1);
		line = line.substring(1);
	    } while (fm.getStringBounds(wrapped + line.substring(0,1) + "-", g).getWidth() <= pageWidth);
	    //while (fm.stringWidth(wrapped + line.substring(0,1) + "-") <= pageWidth);

	    // add a hyphen and a new line to the end of wrapped
	    wrapped = wrapped + "-\n";
	} else {

	    // find maximum number of words that fit on the page
	    wrapped = new String("");
	    do {
		// move first word into wrapped (adding a space if required)
		if(wrapped.length() > 0) {
		    wrapped = wrapped + " ";
		}
		wrapped = wrapped + line.substring(0, line.indexOf(" "));
		line = line.substring(line.indexOf(" ") + 1);
	    } while (line.indexOf(" ") >=  0 && fm.getStringBounds(wrapped + " " + line.substring(0, line.indexOf(" ")), g).getWidth() <= pageWidth);
	    //while (line.indexOf(" ") >=  0 && fm.stringWidth(wrapped + " " + line.substring(0, line.indexOf(" "))) <= pageWidth);
	    
	    // add a new line to the end of wrapped
	    wrapped = wrapped + "\n";
	}

	wrappedAndLine[0] = wrapped;
	wrappedAndLine[1] = line;

	return wrappedAndLine;
    }


    public void print() {

	pj.setPageable(book);

	if (pj.printDialog()) {
	    try {
		pj.print();
	    } catch(PrinterException pe) {
		System.out.println("Error printing: " + pe);
	    }
	}
    }
}
